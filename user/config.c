
#define __CONFIG_H_INC__

#include "config.h"

#include "sst25vf.h"
#include "crc.h"
#include "string.h"
//#include "ds1302.h"

// // 第一行默认语 "欢迎乘坐舟山快速公交"
// const uint8_t const_word1[] = {
// 	0xBB,0xB6,0xD3,0xAD,0xB3,0xCB,0xD7,0xF8,0xD6,0xDB,0xC9,0xBD,0xBF,0xEC,0xCB,0xD9,0xB9,0xAB,0xBD,0xBB,
// };

// // 第二行默认语 "欢迎乘坐舟山快速公交"
// const uint8_t const_word2[] = {
// 	0xBB,0xB6,0xD3,0xAD,0xB3,0xCB,0xD7,0xF8,0xD6,0xDB,0xC9,0xBD,0xBF,0xEC,0xCB,0xD9,0xB9,0xAB,0xBD,0xBB,
// };

// // 第三行默认语 "Welcome to Zhoushan Bus Rapid Transit"
// const uint8_t const_word3[] = {
// 	0x57,0x65,0x6C,0x63,0x6F,0x6D,0x65,0x20,0x74,0x6F,0x20,0x5A,0x68,
// 	0x6F,0x75,0x73,0x68,0x61,0x6E,0x20,0x42,0x75,0x73,0x20,0x52,0x61,
// 	0x70,0x69,0x64,0x20,0x54,0x72,0x61,0x6E,0x73,0x69,0x74,
// };

const uint8_t const_word[ScreenLength] = {
	//0xBB, 0xB6, 0xD3, 0xAD, 0xB9, 0xE2, 0xC1, 0xD9,//欢迎光临
	0xD7, 0xA2, 0xD2, 0xE2, 0xB0, 0xB2, 0xC8, 0xAB,//注意安全
};


//
void Data_Init(void)
{
	u8  i;
	u16 x;
	u8 *p,*pp;
	
	time_index = 0;
	time_upindex = 0;
	
	CanRxIndex = 0;
	CanDoIndexRx = 0;
	
	CanZiIndex = 0;
	CanDoIndexZi = 0;
	
	CanUpIndex = 0;
	CanDoIndexUp = 0;

	timeout_index = 0;
	timeout_index_p = 0;
	timeout_flag = 1;
	timeout_onece_text = 0;
	timeout_doing_color = Red;
	
	for(x = 0; x < sizeof(RxBuffer); x++)
	{
		RxBuffer[x] = 0;
	}
	
	// 扫描
	LED_Index = 0;								// LED运行指示灯
	LineIndex = 0;								// 扫描第几行
	ClearDisplay();								
	DisplayIndex = 0;							// 处理缓存区选择

	PictureIndex = 0;
	Light = 7;
	ID    = 0;
	// 串口
	ZhuanYi = 0;								// 1 下一个要转义 0 不用转义
	Updata = 0;									// 有文本内容更新 0 头 1 文本 2 升级
	RxIndex = 0;								// 接收USART计数
	HeadIndex = 0;
	NowIndex = 0;
	ReceiveUSART1Data = 0;						// 接收一个字节
	ReceiveUSART1True = FALSE;					// 判断是否有接收字节
	if_display_none = 0;
	load_diplay();
	
	if_change_clor = 0;
	
	// 清除协议文本
	for (i = 0 ; i < 15; i++) {
		agreement_data.text[i].data[0] = 0xff;
	}
}


void load_diplay(void)
{
	u8  i,len;
	u8 date_temp[100];
	u8  data_xor;
	u16 x;
	u8 *p,*pp;
	u8 CMD;
	u8 temp8,ret;
	u8 temp[8] = {0,0,0,0,0,0,0,0};
	
display_done = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}// 	if(DisplayIndex)  p = DisplayBuf0, pp = DisplayBuf1;
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}// 	else              p = DisplayBuf1, pp = DisplayBuf0;
	
	currnet_index = DEFAULT;

	currnet_data[0].flag = Invalid;
	currnet_data[1].flag = Invalid;
	currnet_data[2].flag = Invalid;
	currnet_data[3].flag = Invalid;
	display_list[0] = 0;
	display_list[1] = 0;
	display_list[2] = 0;
	display_list[3] = 0;
	display_list_index = 0;

	for (i = 0; i < 4; i++) {
		currnet_data[i].id = i;
		currnet_data[i].display_time = 15;
		currnet_data[i].change_time = 3;
		currnet_data[i].color = Yellow;
		currnet_data[i].style = 9;//DISPLAY_MOVE;//
		currnet_data[i].display_count = 0;
		currnet_data[i].length = 0;//sizeof(const_word);
	}

	diplay_data.id = 0;
	diplay_data.display_time = currnet_data[0].display_time;
	diplay_data.change_time = currnet_data[0].change_time;
	diplay_data.color = currnet_data[0].color;
	diplay_data.style = currnet_data[0].style;//DISPLAY_MOVE;//
	diplay_data.upstyle = 3;
	diplay_data.if_screen_off = 1;
	diplay_data.display_count = currnet_data[0].display_count;
	diplay_data.length = 0;//sizeof(const_word);
	for (i = 0; i < sizeof(const_word); i++) {
		//diplay_data.text[i] = const_word[i];
		//currnet_data[0].text[i] = const_word[i];
	}
	
	Bemove        = 0;
	HaveDisplayNum = 0;							// 上次移动后扫描多少次
	MoveWord_p = &currnet_data[0].text[0];

	updata_flag = 0;
	update_agreement = 0;
	text_count = 0;
	
	data_flag = 0;
	
	display_color = Yellow;
	
	time_index = 0;
	time_upindex = 0;
	time_sec = 0;
	time_upsec = 0;
	upturn_index = 0;
	
	return;
	
	//ClrNSS();
	//Read_Data(PromptSartAddr1);
	//for(i = 0; i < 8; i++){
	//	temp[i]=Get_Byte();	
	//}
	//SetNSS();
	//Can_Send_Msg(temp, 8);
	
	// 查看文本
	CMD = 0;
	if((0x7E == Read_Byte(PromptSartAddr1)))
	{
		len = Read_Byte(PromptSartAddr1 + 1);
		
		ClrNSS();
		Read_Data(PromptSartAddr1 + 2);
		data_xor = 0;
		for( i = 0; i < len + 1; i++) {
			date_temp[i] = Get_Byte();
			data_xor ^= date_temp[i];
		}
		SetNSS();
		// 校准通过
		if(!data_xor){
			diplay_data.length = len;
			for (i = 0; i < len; i++) {
				diplay_data.text[i] = date_temp[i];
				currnet_data[0].text[i] = date_temp[i];
			}
			CMD = 1;
		}
		
		
	}

	// 查看控制信息
	CMD = 0;
	if((0x7E == Read_Byte(PromptSartAddr2)))
	{
		
		ClrNSS();
		data_xor = 0;
		for( i = 0; i < 5; i++)
			data_xor ^= Get_Byte();
		SetNSS();
		// 校准通过
		if(!data_xor){
			CMD = 1;
			diplay_data.display_time = Read_Byte(PromptSartAddr2 + 1);
			diplay_data.color = Read_Byte(PromptSartAddr2 + 2);
			diplay_data.style = Read_Byte(PromptSartAddr2 + 3);
			diplay_data.change_time = Read_Byte(PromptSartAddr2 + 4);

			currnet_data[0].display_time = diplay_data.display_time;
			currnet_data[0].color = diplay_data.color;
			currnet_data[0].style = diplay_data.style;
			currnet_data[0].change_time = diplay_data.change_time;
		}
		
	}
	
	
	if (DISPLAY_STATIC == diplay_data.style) {
		for (x = 0; x < DisplayBufMaxLength; x++)
			upturn_buf[x] = 0;
		
		//uptext_index = 8;
		upturn_index = 0;
		
		if(diplay_data.length > ScreenLength)
			temp8 = ScreenLength;
		else
			temp8 = diplay_data.length;
		
		CheckFile(&upturn_buf[0], &diplay_data.text[0], temp8);
		
		uptext_index = temp8 - ret;
		
		for (x = 0; x < DisplayBufMaxLength; x++)
			*(p + x) = 0;
		
		DisplayIndex = (DisplayIndex + 1) & 1;
		for (x = 0; x < DisplayBufMaxLength; x++)
			*(pp + x) = 0;
	} else {
		MoveIndex     = 0;							// 移动计数，满8位加载下一个文字
		Move_LoadGB   = 0;							// 当前移动字符是GB2312(1)或ASCII(0)
		MovetextNum   = 0;							// 移动第几个字节
	}// 移动
	
}


/********************************************************************************
 *								加载显示信息										*
 ********************************************************************************
*/
u8 CheckFile(u8 *buf_p, u8 *text_p, u8 len)
{
	u8  i,h,l,*p;
	u16 x;
	u8 j,line, temp;
	s16 z;

	p = buf_p;
	x = 0;
	
	if (len < ScreenLength) {
		x = ((ScreenLength - len) >> 1)  << 7;
	}

	for ( i = 0; i < len; i++) {
		h = *(text_p + i);
		
		if (h < 0xA1) {
			if (i >= ScreenLength)
				return(len - i + 1);
			LoadChar(p + x, (u16)h);
			x += 128;
		}
		else
		{
			if (i >= (ScreenLength - 1))
				return(len - i);
			l  = *(text_p + i + 1);
			LoadChar(p + x, ((u16)h << 8) | l );
			x += 256;
			i++;
		}
	}
	
	// 修正居中
	if ((len < ScreenLength) && ((ScreenLength - len)%2)) {
		p = buf_p;
		for (z = DisplayBufMaxLength - 8; z >= 128; z -= 8){
			*(p + z + 4) = *(p + z);
			*(p + z + 5) = *(p + z + 1);
			*(p + z + 6) = *(p + z + 2);
			*(p + z + 7) = *(p + z + 3);
			*(p + z)     = *(p + z - 124);
			*(p + z + 1) = *(p + z - 123);
			*(p + z + 2) = *(p + z - 122);
			*(p + z + 3) = *(p + z - 121);
		}
		for (; z >= 0; z -= 8){
			*(p + z + 4) = *(p + z);
			*(p + z + 5) = *(p + z + 1);
			*(p + z + 6) = *(p + z + 2);
			*(p + z + 7) = *(p + z + 3);
			*(p + z)     = 0;
			*(p + z + 1) = 0;
			*(p + z + 2) = 0;
			*(p + z + 3) = 0;
		}

	}
	
	return 0;

	

	
	
	
}

//
/********************************************************************************
 *								加载图片信息										*
 ********************************************************************************
*/
#if 0
void LoadPicture(uint8_t Index)
{
	uint8_t  *p,*pp;
	uint32_t addr;
	uint16_t Length;
	uint16_t x;
	uint8_t  XOR, i,j,m,n;
	uint8_t  table[11];
	#if 0
	switch(Index)
	{
		case 0: m = 0x01; n = 0; break;
		case 1: m = 0x02; n = 0; break;
		case 2: m = 0x03; n = 0; break;
		case 3: m = 0x01; n = 2; break;
		case 4: m = 0x02; n = 1; break;
		
	}
	if(DisplayIndex) {p = &DisplayBuf0[0][0]; pp = &DisplayBuf1[0][0];}
	else             {p = &DisplayBuf1[0][0]; pp = &DisplayBuf0[0][0];}
	
	for(x = 0; x < MaxLine * 32; x++) *p++ = m;
	for(x = 0; x < MaxLine * 32; x++) *p++ = m;
	for(x = 0; x < MaxLine * 32; x++) *p++ = n;

	DisplayIndex = (DisplayIndex + 1) & 1;
	#endif
	//
	
	addr = PictureStartAddr + Index * 0x4000;
	if(DisplayIndex) {p = &DisplayBuf0[0][0]; pp = &DisplayBuf1[0][0];}
	else             {p = &DisplayBuf1[0][0]; pp = &DisplayBuf0[0][0];}
	ClrNSS();
	Read_Data(addr);
	for(i = 0; i < 11; i++)
	{
		table[i] = Get_Byte();
	}
	SetNSS();
	//
	if( (0x55  == table[0]) && (0xFF == table[1])  && (0x55 == table[2]) &&
		(0x53  == table[3]) && (0x49 == table[4])  && (0x4E == table[5]) &&
		((ID   == table[6]) || (0xFF == table[6])) && (0xA1 == table[7]) &&
		(Index == table[10] - 1) )
// 	if( (0x55 == Read_Byte(addr))     && (0xFF == Read_Byte(addr + 1))  && (0x55 == Read_Byte(addr + 2)) &&
// 		(0x53 == Read_Byte(addr + 3)) && (0x49 == Read_Byte(addr + 4))  && (0x4E == Read_Byte(addr + 5)) &&
// 		((ID  == Read_Byte(addr + 6)) || (0xFF == Read_Byte(addr + 6))) && (0xA1 == Read_Byte(addr + 7)) &&
// 		(Index == Read_Byte(addr + 10) - 1) )
	{
		Length = ((uint16_t)table[8] << 8) + table[9] - 1;
		
		XOR = 0;
		ClrNSS();
		Read_Data(addr);
		for(x = 0; x < 12 + Length; x++)
		{
			//XOR ^= Read_Byte(addr + x);
			XOR ^= Get_Byte();
		}
		SetNSS();
		if(XOR) return;
		
		//ClearDisplay();
		ClrNSS();
		Read_Data(addr + 11);
		for(x = 0; x < Length; x++)
		{
			//*(p + x) = Read_Byte(addr + x + 11);
			*(p + x) = Get_Byte();
		}
		SetNSS();
		DisplayIndex = (DisplayIndex + 1) & 1;
		//for(x = 0; x < sizeof(DisplayBuf0); x++) *(pp + x) = *(p + x);
	}
}
#endif
/************************************************************************************
 *                                      加载文字										*
 *********************************************************8**************************/
void LoadChar(uint8_t *p, uint16_t dat)
{
    uint8_t i;
	u8 a,b;
	u16 x,y;
	
    ClrNSS();
    if(dat == 0xA9A5)
	{
		for(i = 0; i < 32; i++) {
			//*(p + i)  = 0x00;//0xFF;//
			*(p + i*8)     = 0;
			*(p + i*8 + 1) = 0;
			*(p + i*8 + 2) = 0;
			*(p + i*8 + 3) = 0;
			*(p + i*8 + 4) = 0;
			*(p + i*8 + 5) = 0;
			*(p + i*8 + 6) = 0;
			*(p + i*8 + 7) = 0;
		}
		//*(p + 7)      = 0xff;//0x00;//
		//*(p + 8)      = 0xff;//0x00;//
		//*(p + 7 + 16) = 0xfe;//0x01;//
		//*(p + 8 + 16) = 0xfe;//0x01;//
		
		x = 56;
		y = 184;
		for (i = 0; i < 16; i++) {
			*(p + x + i)     = 0;
			*(p + y + i)     = 0;
		}		
	}
	else if(dat >= 0xB000)
    {
			Read_Data(((uint32_t)(((dat >> 8u) - 0xB0) * 94u + ((dat & 0xFF) - 0xA1)) << 5) + 0x1CAF00u);
		//Read_Data(((uint32_t)(((dat >> 8u) - 0xB0) * 94u + ((dat & 0xFF) - 0xA1)) << 5) + 0xAF00u);
			for(i = 0; i < 16; i++)
			{
				//*(p + i)      = ~Get_Byte();
				//*(p + i + 16) = ~Get_Byte();
				a = Get_Byte();
				b = Get_Byte();
				*(p + i*8)     = (a & (0x80)) >> 7;
				*(p + i*8 + 1) = (a & (0x40)) >> 6;
				*(p + i*8 + 2) = (a & (0x20)) >> 5;
				*(p + i*8 + 3) = (a & (0x10)) >> 4;
				*(p + i*8 + 4) = (a & (0x08)) >> 3;
				*(p + i*8 + 5) = (a & (0x04)) >> 2;
				*(p + i*8 + 6) = (a & (0x02)) >> 1;
				*(p + i*8 + 7) = (a & (0x01));


				*(p + i*8 + 128) = (b & (0x80)) >> 7;
				*(p + i*8 + 129) = (b & (0x40)) >> 6;
				*(p + i*8 + 130) = (b & (0x20)) >> 5;
				*(p + i*8 + 131) = (b & (0x10)) >> 4;
				*(p + i*8 + 132) = (b & (0x08)) >> 3;
				*(p + i*8 + 133) = (b & (0x04)) >> 2;
				*(p + i*8 + 134) = (b & (0x02)) >> 1;
				*(p + i*8 + 135) = (b & (0x01));
			}
    }
    else if(dat >= 0xA100)
    {
        Read_Data(((uint32_t)(((dat >> 8u) - 0xA1) * 94u + ((dat & 0xFF) - 0xA1)) << 5) + 0x1C8000u);
		//Read_Data(((uint32_t)(((dat >> 8u) - 0xA1) * 94u + ((dat & 0xFF) - 0xA1)) << 5) + 0x8000u);
			for(i = 0; i < 16; i++)
			{
				//a = ~Get_Byte();
				//b = ~Get_Byte();
				a = Get_Byte();
    		b = Get_Byte();

				*(p + i*8)     = (a & (0x80)) >> 7;
				*(p + i*8 + 1) = (a & (0x40)) >> 6;
				*(p + i*8 + 2) = (a & (0x20)) >> 5;
				*(p + i*8 + 3) = (a & (0x10)) >> 4;
				*(p + i*8 + 4) = (a & (0x08)) >> 3;
				*(p + i*8 + 5) = (a & (0x04)) >> 2;
				*(p + i*8 + 6) = (a & (0x02)) >> 1;
				*(p + i*8 + 7) = (a & (0x01));

				*(p + i*8 + 128) = (b & (0x80)) >> 7;
				*(p + i*8 + 129) = (b & (0x40)) >> 6;
				*(p + i*8 + 130) = (b & (0x20)) >> 5;
				*(p + i*8 + 131) = (b & (0x10)) >> 4;
				*(p + i*8 + 132) = (b & (0x08)) >> 3;
				*(p + i*8 + 133) = (b & (0x04)) >> 2;
				*(p + i*8 + 134) = (b & (0x02)) >> 1;
				*(p + i*8 + 135) = (b & (0x01));
        }
    }
    else
    {
       Read_Data(((uint32_t)(dat & 127) << 4) + 0x1C0000);
		//Read_Data(((uint32_t)(dat & 127) << 4));
			for(i = 0; i < 16; i++)
			{
				//a = ~Get_Byte();
				a = Get_Byte();
				
				*(p + i*8)     = (a & (0x80)) >> 7;
				*(p + i*8 + 1) = (a & (0x40)) >> 6;
				*(p + i*8 + 2) = (a & (0x20)) >> 5;
				*(p + i*8 + 3) = (a & (0x10)) >> 4;
				*(p + i*8 + 4) = (a & (0x08)) >> 3;
				*(p + i*8 + 5) = (a & (0x04)) >> 2;
				*(p + i*8 + 6) = (a & (0x02)) >> 1;
				*(p + i*8 + 7) = (a & (0x01));
    	}
    }
    SetNSS();
}
//
/********************  清屏  *****************************/
void ClearScreen(void)
{
	uint8_t i;

    // 清屏 //
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	
    GPIOB->BSRR = ((uint16_t)0xFF << 8);
	
    for(i = 0; i < ScreenLength; i++)
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_12);
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();
    }
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
	//
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}
//
void delay_ms(uint8_t num)
{
	uint8_t i;
	uint16_t j;
	
	for(i = 0; i < num; i++)
	{
		j = 12000;
		while(j--);
	}
}

void delay_us(uint8_t num)
{
	uint8_t i;
	uint8_t j;
	
	for(i = 0; i < num; i++)
	{
		j = 10;
		while(j--);
	}
}



//
void ClearDisplay(void)
{
	uint16_t i;
	
	for(i = 0; i < DisplayBufMaxLength; i++)
	{
			DisplayBuf0[i] = 0;
			DisplayBuf1[i] = 0;
		
	}
}
//
void ReadID(void)
{
	uint8_t tmp_table[12],i,crc;
	
	//
	ClrNSS();
	Read_Data(IDAddr);
	for(i = 0; i < 12; i++)
	{
		tmp_table[i] = Get_Byte();
	}
	SetNSS();  
	
	if( 0x55 == tmp_table[0] && 0xff == tmp_table[1] && 0x55 == tmp_table[2] && 0x53 == tmp_table[3] &&
		0x49 == tmp_table[4] && 0x4E == tmp_table[5] && 0xa4 == tmp_table[7] ) 
	{
		crc = 0;
		for(i = 0; i < 12; i++ )
		{
			crc ^= tmp_table[i];
		}
		if(!crc)
		{
			ID = tmp_table[10];
		}
	}
}
//
void ReadLight(void)
{
	uint8_t tmp_table[12],i,crc;
	
	//
	ClrNSS();
	Read_Data(LightAddr);
	for(i = 0; i < 12; i++)
	{
		tmp_table[i] = Get_Byte();
	}
	SetNSS();  
	
	if( 0x55 == tmp_table[0] && 0xff == tmp_table[1] && 0x55 == tmp_table[2] && 0x53 == tmp_table[3] &&
		0x49 == tmp_table[4] && 0x4E == tmp_table[5] && 0xa3 == tmp_table[7] ) 
	{
		crc = 0;
		for(i = 0; i < 12; i++ )
		{
			crc ^= tmp_table[i];
		}
		if(!crc)
		{
			Light = tmp_table[10];
		}
	}
}
//
/********************************************************************************
 *								初始化											*
 ********************************************************************************
*/
void init_system(void)
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		// 开启AFIO时钟
	AFIO->MAPR      |= 2 << 24;									// 关闭JTAG,启用SWD
	
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);			// 关闭DMA1时钟
	DMA1_Channel5->CCR   &= ~1;
	EXTI->PR  = 0x000FFFFFu;									// 清除中断标志
    EXTI->EMR = 0;												// 关闭事件
    EXTI->IMR = 0;												// 关闭中断
	SysTick->CTRL = 0;
	TIM3->CR1  &= ~(1u << 3);									// 设置工作模式
	__disable_irq();
	
	// 定时器TIM3初始化  显示时间5秒
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		// 时钟使能
	TIM_TimeBaseStructure.TIM_Period = SysDisplaySpeed;			// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 719;					// 设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				// 根据指定的参数初始化TIMx的时间基数单位
	
	// 定时器TIM2初始化  扫描显示
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		// 时钟使能
	TIM_TimeBaseStructure.TIM_Period = SysDisplaySpeed;//9;//			// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					// 设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				// 根据指定的参数初始化TIMx的时间基数单位
	
	// 定时器TIM4初始化  打铃定时
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		// 时钟使能
// 	TIM_TimeBaseStructure.TIM_Period = 9999;
// 	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );					// 使能指定的TIM2中断,允许更新中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );					// 使能指定的TIM3中断,允许更新中断
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );
	
	// 外设时钟 USART1 时钟开启，IO端口ABC时钟开启
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);		
	
	USART_InitStructure.USART_BaudRate 				= 9600;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;				//0
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;					//0
	USART_InitStructure.USART_Parity 				= USART_Parity_No;					//0
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None | USART_DMAReq_Rx;	// 0 | 0x0040
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	//4 | 8
	
	USART_Init(USART1, &USART_InitStructure);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			// 使能 接收中断
	
	// 使能 USART1
	USART_Cmd(USART1, ENABLE);
	while(!(USART1->SR & 0x40));
	
// 	USART1->BRR  = 0x1D4C;											 // 波特率 9600bps
// 	USART1->CR1  = (1 << 13) | (1 << 3) | (1 << 2);		             // 使能USART1,使能接收和发送
// 	USART1->CR3 |= (1 << 6);								         // 使能DMA接收

    // 配置 USART1 Tx (PA.09) //复用推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 USART1 Tx (PA.10) //上拉输入
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA1_Channel5->CCR   &= 0xFFFFFFFE;							// DMA 关闭
	DMA1_Channel5->CNDTR  = UARTMaxLength;       				// DMA 长度       				
	DMA1_Channel5->CPAR   = (uint32_t)(&USART1->DR);			// 通道x外设地址
	DMA1_Channel5->CMAR   = (uint32_t)(&RxBuffer[0]);			// 通道x存储器地址
	DMA1_Channel5->CCR    = (1 << 7) | (1 << 5);				// 执行存储器地址增量操作、执行循环操作
	DMA1_Channel5->CCR   |= 1;									// DMA 开启

	// 配置 显示行ABCD (PC.9、8、7、6) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 配置 数据红色R12 (PB 10 13) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   

	// 配置 数据绿色G12 (PB 11 14) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	// 配置 数据蓝色B12 (PB 12 15) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 配置595  OE、CLK、LAT (PA PC4 PC5) 推挽输出
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// RS485     PC12 推挽输出
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	// LED  PA1 推挽输出
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	LEDOFF();
	
	// 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);             // 使能外设时钟
    SPI1->CR1  &= (~(1 << 6)) & 0xFFFF;                              // 禁止SPI设备
    SPI1->CR1   = (3 << 8) | (0 << 3) | (1 << 2);                    // 启用软件从设备管理 波特率fPCLK/2 
	SPI1->CR1  |= 1 << 6;                                            // 开启SPI设备
	
	// 配置 SPI1 NSS (PA.4) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	SetNSS();
	
	// 配置 SPI1 CLK (PA.5) 复用推挽
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置 SPI1 MISO (PA.6) 输入
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置 SPI1 MOSI (PA.7) 复用推挽
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	
	NVIC->ICER[7  / 32]	|= 1u << (7  % 32);                          // 禁止EXTI1中断
	NVIC->ICER[8  / 32]	|= 1u << (8  % 32);                          // 禁止EXTI2中断
	// USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//TIM2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);								//初始化NVIC寄存器
	
	//
	//TIM3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);								//初始化NVIC寄存器
	
	//TIM4
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);								//初始化NVIC寄存器
	
	// 看门狗
	//IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);     //①使能对寄存器 I 写操作 IWDG->KR = 0x5555;
	//IWDG_SetPrescaler(4);            //②设置 IWDG 预分频值:设置 IWDG 预分频值 IWDG->PR = 4;
	//IWDG_SetReload(625);               //②设置 IWDG 重装载值
	//IWDG_ReloadCounter();   //③按照 IWDG 重装载寄存器的值重装载 IWDG 计数器
	//IWDG_Enable();         //④使能 IWDG
	
	OffDisplay();	//关显示
	Init_SST25VF();						//初始化Flash
	
	
	
	RS485_R();							//485接收
	ClearScreen();						//清屏一行
	//Data_Init();
	
	ReadID();
	ReadLight();
	
	TIM_Cmd(TIM3, DISABLE);
//	TIM_Cmd(TIM4, DISABLE);
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
	
	__enable_irq();						//使能所有中断 
}
//
//
