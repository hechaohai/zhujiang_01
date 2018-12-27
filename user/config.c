
#define __CONFIG_H_INC__

#include "config.h"

#include "sst25vf.h"
#include "crc.h"
#include "string.h"
//#include "ds1302.h"

// // ��һ��Ĭ���� "��ӭ������ɽ���ٹ���"
// const uint8_t const_word1[] = {
// 	0xBB,0xB6,0xD3,0xAD,0xB3,0xCB,0xD7,0xF8,0xD6,0xDB,0xC9,0xBD,0xBF,0xEC,0xCB,0xD9,0xB9,0xAB,0xBD,0xBB,
// };

// // �ڶ���Ĭ���� "��ӭ������ɽ���ٹ���"
// const uint8_t const_word2[] = {
// 	0xBB,0xB6,0xD3,0xAD,0xB3,0xCB,0xD7,0xF8,0xD6,0xDB,0xC9,0xBD,0xBF,0xEC,0xCB,0xD9,0xB9,0xAB,0xBD,0xBB,
// };

// // ������Ĭ���� "Welcome to Zhoushan Bus Rapid Transit"
// const uint8_t const_word3[] = {
// 	0x57,0x65,0x6C,0x63,0x6F,0x6D,0x65,0x20,0x74,0x6F,0x20,0x5A,0x68,
// 	0x6F,0x75,0x73,0x68,0x61,0x6E,0x20,0x42,0x75,0x73,0x20,0x52,0x61,
// 	0x70,0x69,0x64,0x20,0x54,0x72,0x61,0x6E,0x73,0x69,0x74,
// };

const uint8_t const_word[12] = {
	//0xBB, 0xB6, 0xD3, 0xAD, 0xB9, 0xE2, 0xC1, 0xD9,//��ӭ����
	//0xD7, 0xA2, 0xD2, 0xE2, 0xB0, 0xB2, 0xC8, 0xAB,//ע�ⰲȫ
	0x20,0x20,0x20,0x56,0x37,0x2E,0x30,0x2E,0x30,0x20,0x20,0x20,// V7.0.0
	//0x36,0xBB, 0xB6, 0xD3, 0xAD, 0xB9, 0xE2, 0xC1, 0xD9,// V7.0.0
};

const uint8_t const_word1[ScreenLength] = {
	//0xBB, 0xB6, 0xD3, 0xAD, 0xB9, 0xE2, 0xC1, 0xD9,//��ӭ����
	0x20,0x20, 0xD7, 0xA2, 0xD2, 0xE2, 0xB0, 0xB2, 0xC8, 0xAB, 0x20,0x20,//ע�ⰲȫ
	//0x20,0x20,0x20,0x56,0x37,0x2E,0x30,0x2E,0x30,0x20,0x20,0x20,// V7.0.0
};

// �汾��Ϣ
const uint8_t ReceiveData_Head[66]={
	0xA0, 0x07, 0x56, 0x06, 0x21, 0x4E, 0x05, 0x5C, 0x56, 0x00, 0xA0, 0x78, 0x39, 0x5B, 0x8D, 0xAD,
0x3A, 0x61, 0xF5, 0x01, 0xD2, 0x27, 0x7C, 0x51, 0x70, 0xB1, 0xF9, 0x3D, 0x38, 0x33, 0x07, 0x00,
0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// ��Ŀ��Ϣ
const uint8_t JiemuData_Head[66]={
	0xA0, 0x07, 0x56, 0x06, 0x21, 0x4E, 0x05, 0x5C, 0x56, 0x00, 0xA0, 0x78, 0x39, 0x5B, 0x8D, 0xAD,
0x3A, 0x61, 0xF5, 0x01, 0xD2, 0x27, 0x7C, 0x51, 0x70, 0xB1, 0xF9, 0x3D, 0x38, 0x33, 0x07, 0x00,
0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


const uint8_t monthdays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const u8 const_speed[19] = { 
	0x01, 0x02, 0x00, 0x01, 0x05, 0xCA, 0xB1, 0xCB, 0xD9, 0xA3, 
	0xBA, 0x58, 0x58, 0x58, 0x20, 0x6B, 0x6D, 0x2F, 0x48,
};

//
void Data_Init(void)
{
	u8  i,flag;
	u16 x;
	u8 *p,*pp;
	
	
	// ɨ��
	LED_Index = 0;								// LED����ָʾ��
	LineIndex = 0;								// ɨ��ڼ���
						
	DisplayIndex = 0;							// ��������ѡ��

	PictureIndex = 0;
	Light = 7;
	ID    = 0;
	// ����
//	ZhuanYi = 0;								// 1 ��һ��Ҫת�� 0 ����ת��
	Updata_Text = 0;									// ���ı����ݸ���
	Updata_GPS = 0;							// gps����
	RxIndex = 0;								// ����USART����
	HeadIndex = 0;
	NowIndex = 0;
	ReceiveUSART1Data = 0;						// ����һ���ֽ�
	ReceiveUSART1True = FALSE;					// �ж��Ƿ��н����ֽ�
	if_display_none = 0;
	
	
	if_change_clor = 0;
	
	timeout_index = 0;
	timeout_index_p = 0;
	timeout_flag = 1;
	timeout_onece_text = 0;

	NeedGPS = 0;
	receive_update = 0;
	text_index = 0;
	text_flag = 0;
	sys_move = 0;
	diplay_data.text_GB_flag = 0;

	
		// �鿴�ı�
	
	ClrNSS();
	Read_Data(PromptSartAddr1);
	for(i = 0; i < 66; i++){
		if(JiemuData_Head[i] != Get_Byte()) break;
	}
	SetNSS();
	

	if(i > 65) {
		ClrNSS();
		Read_Data(PromptSartAddr2);
		for(x = 0; x < 1566; x++)
		{
			*(&receive_data[0].head + x) = Get_Byte();
		}
		SetNSS();
		
	}else {
		for(x = 0; x < 1566; x++)
		{
			*(&receive_data[0].head + x) = 0;
		}
	}

	for(i = 0; i < 4; i++)
	{
		*(&gps_date.flag + i) = 0;
	}
	
	//for(i = 0; i < 15; i++)
	{
		//*(&gps_date.time_hour_H + i) = 0x2D;
		gps_date.time_hour_H = 0x2D;gps_date.time_hour_L = 0x2D;
		gps_date.time_munute_H = 0x2D;gps_date.time_munute_L = 0x2D;
		gps_date.time_second_H = 0x2D;gps_date.time_second_L = 0x2D;
		gps_date.speed_H = 0x2D;gps_date.speed_M = 0x2D;gps_date.speed_L = 0x2D;
		gps_date.date_year_H = 0x2D;gps_date.date_year_L = 0x2D;
		gps_date.date_month_H = 0x2D;gps_date.date_month_L = 0x2D;
		gps_date.date_day_H = 0x2D;gps_date.date_day_L = 0x2D;
	}
	
	
	
	for(x = 0; x < sizeof(RxBuffer_Gps); x++)
	{
		RxBuffer_Gps[x] = 0;
	}
		for(x = 0; x < sizeof(RxBuffer_Text); x++)
	{
		RxBuffer_Text[x] = 0;
	}
	
	RxCounter_Gps = 0;
	
	for(i = 0; i < sizeof(ReceiveData_Head); i++) {
		
		USART1->DR = ReceiveData_Head[i]; while((USART1->SR & 0x40) == 0);
	}
	
	/*
	01 02 00 01 05 CA B1 CB D9 A3 BA 58 58 58 
 20 6B 6D 2F 48
	01 02 00 01 05 CA B1 CB D9 A3 BA 58 58 58 
 20 6B 6D 2F 48

	*/
	
	for(x = 0; x < 1566; x++) {
		USART1->DR = *(&receive_data[0].head + x); while((USART1->SR & 0x40) == 0);
	}
}


void load_diplay(void)
{
	u8  i,len;
	u8  data_xor;
	u16 x;
	u8 *p,*pp;
	u8 CMD;
	u8 temp8,ret;
//	u8 temp[8] = {0,0,0,0,0,0,0,0};
	
display_done = 0;
display_done_p = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}// 	if(DisplayIndex)  p = DisplayBuf0, pp = DisplayBuf1;
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}// 	else              p = DisplayBuf1, pp = DisplayBuf0;
	
	display_list[0] = 0;
	display_list[1] = 0;
	display_list[2] = 0;
	display_list[3] = 0;
	display_list_index = 0;
	
	Bemove        = 0;
	HaveDisplayNum = 0;							// �ϴ��ƶ���ɨ����ٴ�

	updata_flag = 0;
	update_agreement = 0;
	text_count = 0;
	
	data_flag = 0;
	
	display_color = Yellow;
	
	time_upindex = 0;
	time_upsec = 0;
	suoping = 0;
}

void do_load_up(void)
{
	u8 i,j,temp,ret;
	u16 x;
	u8 text[12];

	XuehuaIndex = 0;
	Xuehua_dibu = 0;
	// �ı�
	if(receive_data[text_index].tpye == 0){ 
		for(i = 0; i < 12; i++){
			if(MovetextNum + i < diplay_data.length)
			{
				temp = receive_data[text_index].text[MovetextNum + i];
				if(temp > 0)
					text[i] = temp;
				else
					text[i] = 0x20;
			}
			else
				text[i] = 0x20;
		}
		
		j = 0;
		if(MovetextNum + 12 < diplay_data.length){
			for(i = 0;i < 12; i++){
				if(receive_data[text_index].text[MovetextNum + i] < 0x7f)
					j++;
			}
		}
		
		if((j%2) && (receive_data[text_index].text[MovetextNum + 11] > 0x7f)){
			text[11] = 0x20;
			MovetextNum += 11;
		}
		else
			MovetextNum += 12;

	}
	// ʱ��
	else if(1 == receive_data[text_index].tpye)
	{
		text[0] = 0x20;
		text[1] = 0x20;
		text[2] = gps_date.time_hour_H;
		text[3] = gps_date.time_hour_L;
		text[4] = 0x3A;
		text[5] = gps_date.time_munute_H;
		text[6] = gps_date.time_munute_L;
		text[7] = 0x3A;
		text[8] = gps_date.time_second_H;
		text[9] = gps_date.time_second_L;
		text[10] = 0x20;
		text[11] = 0x20;	
		MovetextNum += 12;
	}
	// ʱ��
	else if(2 == receive_data[text_index].tpye)
	{
		text[0] = 0xCA;
		text[1] = 0xB1;
		text[2] = 0xCB;
		text[3] = 0xD9;
		
		text[4] = 0x3A;
		text[5] = gps_date.speed_H;
		text[6] = gps_date.speed_M;
		text[7] = gps_date.speed_L;
		
		text[8] = 'k';
		text[9] = 'm';
		text[10] = '/';
		text[11] = 'h';
		MovetextNum += 12;
	}
	// ����
	else if(3 == receive_data[text_index].tpye)
	{
		text[0] = gps_date.date_year_H;
		text[1] = gps_date.date_year_L;
		text[2] = 0xC4;
		text[3] = 0xEA;
		
		text[4] = gps_date.date_month_H;
		text[5] = gps_date.date_month_L;
		text[6] = 0xD4;
		text[7] = 0xC2;
		
		text[8] = gps_date.date_day_H;
		text[9] = gps_date.date_day_L;
		text[10] = 0xC8;
		text[11] = 0xD5;
		MovetextNum += 12;
	}
	
	// ���������ʾ����
	//for (x = 0; x < DisplayBufMaxLength; x++)
	//	upturn_buf[x] = 0;

	ret = CheckFile(&upturn_buf[0], text, 12);
}

void do_text(void)
{
	u8 i,j;
	u8 temp;
	u8 text[12];
	u16 x;
	
	//tpye 00 �ı� 01 ʱ��  02 ʱ�� 03 ����
	//receive_data[text_index].tpye;
	
	for(i = 0; i < 12; i++){
		if(diplay_data.text_index + i < diplay_data.length)
		{
			temp = receive_data[text_index].text[diplay_data.text_index + i];
			if(temp > 0)
				text[i] = temp;
			else
				text[i] = 0x20;
		}
		else
			text[i] = 0x20;
	}
	
	#if 1
	j = 0;
	if(diplay_data.text_index + 12 < diplay_data.length){
		for(i = 0;i < 12; i++){
			if(receive_data[text_index].text[diplay_data.text_index + i] < 0x7f)
				j++;
		}
	}
	
	
	if((j%2) && (receive_data[text_index].text[diplay_data.text_index + 11] > 0x7f)){
		text[11] = 0x20;
		diplay_data.text_index += 11;
	}
	else
		diplay_data.text_index += 12;
	#endif


	
	check_timeout2(text,12);

}

void do_time(void)
{
	u8 text[12];
	
	text[0] = 0x20;
	text[1] = 0x20;
	text[2] = gps_date.time_hour_H;
	text[3] = gps_date.time_hour_L;
	text[4] = 0x3A;
	text[5] = gps_date.time_munute_H;
	text[6] = gps_date.time_munute_L;
	text[7] = 0x3A;
	text[8] = gps_date.time_second_H;
	text[9] = gps_date.time_second_L;
	text[10] = 0x20;
	text[11] = 0x20;	
	//USART1->DR = 0xaa; while((USART1->SR & 0x40) == 0);
	check_timeout2(text,12);
}

void do_data(void)
{
	u8 text[12];
	
	text[0] = gps_date.date_year_H;
	text[1] = gps_date.date_year_L;
	text[2] = 0xC4;
	text[3] = 0xEA;
	
	text[4] = gps_date.date_month_H;
	text[5] = gps_date.date_month_L;
	text[6] = 0xD4;
	text[7] = 0xC2;
	
	text[8] = gps_date.date_day_H;
	text[9] = gps_date.date_day_L;
	text[10] = 0xC8;
	text[11] = 0xD5;
	
	check_timeout2(text,12);
}

void do_speed(void)
{
	u8 text[12];
	
	text[0] = 0xCA;
	text[1] = 0xB1;
	text[2] = 0xCB;
	text[3] = 0xD9;
	
	text[4] = 0x3A;
	text[5] = gps_date.speed_H;
	text[6] = gps_date.speed_M;
	text[7] = gps_date.speed_L;
	
	text[8] = 'k';
	text[9] = 'm';
	text[10] = '/';
	text[11] = 'h';
	

	//for(i = 0; i < RxCounter_Gps; i++){
	//USART1->DR = RxBuffer_Gps[i]; while((USART1->SR & 0x40) == 0);
	//}
	//USART1->DR = 0x0d; while((USART1->SR & 0x40) == 0);

	check_timeout2(text,12);
}
//

// �Ϸ�
#if 1
void Move_Up(void)
{
	uint8_t  *p,*pp;
	uint8_t  i,j;
	uint16_t x, y;
	u8 temp8, ret;

	// �Ϸ�һ��
	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}

	for (i = 0; i < 15 - MoveIndex; i++) {
		for(x = i * 8; x < DisplayBufMaxLength; x += 128) {
			*(p + x)     = *(p + x +  8);
			*(p + x + 1) = *(p + x +  9);
			*(p + x + 2) = *(p + x + 10);
			*(p + x + 3) = *(p + x + 11);
			*(p + x + 4) = *(p + x + 12);
			*(p + x + 5) = *(p + x + 13);
			*(p + x + 6) = *(p + x + 14);
			*(p + x + 7) = *(p + x + 15);
		}
	}
	j = 0;
	for (; i < 16; i++) {
		y = j * 8;
		for(x = i * 8; x < DisplayBufMaxLength; x += 128) {
			*(p + x)     = upturn_buf[y];
			*(p + x + 1) = upturn_buf[y + 1];
			*(p + x + 2) = upturn_buf[y + 2];
			*(p + x + 3) = upturn_buf[y + 3];
			*(p + x + 4) = upturn_buf[y + 4];
			*(p + x + 5) = upturn_buf[y + 5];
			*(p + x + 6) = upturn_buf[y + 6];
			*(p + x + 7) = upturn_buf[y + 7];
			
			y += 128;
		}
		j++;
	}

	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	
	if(++MoveIndex > 15){
		MoveIndex = 0;
		time_upindex = 0;
		time_upsec = 0;
		sys_move = 0;
		delay_ms(100);
	}
}
#endif
// �·�
#if 1
void Move_Down(void)
{
	uint8_t  *p,*pp;
	uint8_t  i,j;
	uint16_t x, y;
	u8 temp8, ret;

	// ��һ��
	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}

	j = MoveIndex;
	for (i = 0; i < MoveIndex+1; i++) {
		y = (15 - j) * 8;
		for(x = i * 8; x < DisplayBufMaxLength; x += 128) {
			*(p + x)     = upturn_buf[y];
			*(p + x + 1) = upturn_buf[y + 1];
			*(p + x + 2) = upturn_buf[y + 2];
			*(p + x + 3) = upturn_buf[y + 3];
			*(p + x + 4) = upturn_buf[y + 4];
			*(p + x + 5) = upturn_buf[y + 5];
			*(p + x + 6) = upturn_buf[y + 6];
			*(p + x + 7) = upturn_buf[y + 7];
			
			y += 128;
		}
		j--;
	}
	for (j = 15; j != i - 1; j--) {
		for(x = j * 8; x < DisplayBufMaxLength; x += 128) {
			*(p + x)     = *(p + x -  8);
			*(p + x + 1) = *(p + x -  7);
			*(p + x + 2) = *(p + x - 6);
			*(p + x + 3) = *(p + x - 5);
			*(p + x + 4) = *(p + x - 4);
			*(p + x + 5) = *(p + x - 3);
			*(p + x + 6) = *(p + x - 2);
			*(p + x + 7) = *(p + x - 1);
		}
	}

	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	
	if(++MoveIndex > 15){
		MoveIndex = 0;
		time_upindex = 0;
		time_upsec = 0;
		sys_move = 0;
		delay_ms(100);
	}
}
#endif

void Move_piaoxue(void)
{
	uint8_t  *p,*pp;
	uint8_t  i,j,z;
	uint16_t x, y;
	u8 temp8, ret;

	// ��һ��
	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}
	
	// �ײ���û�Ƴ�
	if(!Xuehua_dibu)
	{
		j = XuehuaIndex;
		for (i = 0; i < XuehuaIndex+1; i++) {
			y = (15 - (j/4)) * 8;
			for(x = i * 8; x < DisplayBufMaxLength; x += 128) {
				*(p + x)     = upturn_buf[y];
				*(p + x + 1) = upturn_buf[y + 1];
				*(p + x + 2) = upturn_buf[y + 2];
				*(p + x + 3) = upturn_buf[y + 3];
				*(p + x + 4) = upturn_buf[y + 4];
				*(p + x + 5) = upturn_buf[y + 5];
				*(p + x + 6) = upturn_buf[y + 6];
				*(p + x + 7) = upturn_buf[y + 7];
				y += 128;
			}
			j--;
		}

		for (j = 15; j > i - 1; j--) {
			for(x = j * 8; x < DisplayBufMaxLength; x += 128) {
				*(p + x)     = *(p + x -  8);
				*(p + x + 1) = *(p + x -  7);
				*(p + x + 2) = *(p + x - 6);
				*(p + x + 3) = *(p + x - 5);
				*(p + x + 4) = *(p + x - 4);
				*(p + x + 5) = *(p + x - 3);
				*(p + x + 6) = *(p + x - 2);
				*(p + x + 7) = *(p + x - 1);
			}
		}
		//�ײ��Ƴ�
		if(++XuehuaIndex > 15)
		{
			Xuehua_dibu = 1;
			XuehuaIndex = 4;
			MoveIndex = 0;
			XuehuaIndex_p = 0;
		}
	}
	else
	{
		y = (15 - XuehuaIndex) * 8;
		
		for(x = i * 8; x < DisplayBufMaxLength; x += 128) {
			*(p + x)     = upturn_buf[y];
			*(p + x + 1) = upturn_buf[y + 1];
			*(p + x + 2) = upturn_buf[y + 2];
			*(p + x + 3) = upturn_buf[y + 3];
			*(p + x + 4) = upturn_buf[y + 4];
			*(p + x + 5) = upturn_buf[y + 5];
			*(p + x + 6) = upturn_buf[y + 6];
			*(p + x + 7) = upturn_buf[y + 7];
			y += 128;
		}
		
		if(XuehuaIndex != 15){
			for (j = 15 - XuehuaIndex_p; j != 0; j--) {
				for(x = j * 8; x < DisplayBufMaxLength; x += 128) {
					*(p + x)     = *(p + x -  8);
					*(p + x + 1) = *(p + x -  7);
					*(p + x + 2) = *(p + x - 6);
					*(p + x + 3) = *(p + x - 5);
					*(p + x + 4) = *(p + x - 4);
					*(p + x + 5) = *(p + x - 3);
					*(p + x + 6) = *(p + x - 2);
					*(p + x + 7) = *(p + x - 1);
				}
			}
		}

		if(XuehuaIndex < 15){
			XuehuaIndex++;
			if((XuehuaIndex == 6) || (XuehuaIndex == 9) || (XuehuaIndex == 12) || (XuehuaIndex == 15)){
				XuehuaIndex_p++;
			}
		}
		MoveIndex++;
		
		
	}

	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	
	if(MoveIndex >= 12){
		MoveIndex = 0;
		time_upindex = 0;
		time_upsec = 0;
		sys_move = 0;
		delay_ms(100);
	}

}


//
void Move_LMR(void)
{
	uint8_t  *p,*pp;
	uint8_t  i,j;
	uint16_t x, y;
	u8 temp8, ret;

	// �Ϸ�һ��
	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}


	//for (i = 0; i < MoveIndex; i++) {
		x = (MoveIndex % 8) + (MoveIndex/8) * 128;
		for(j = 0;j < 16; j++) {
			
			//for(x = i * 8; x < DisplayBufMaxLength; x += 128) {
				*(p + x) = upturn_buf[x];x+=8;
			//}
		}
	//}
	
	x = ((95-MoveIndex) % 8) + ((95 - MoveIndex)/8) * 128; // 1535
	for(j = 0;j < 16; j++) {
		//for(x = i * 8; x < DisplayBufMaxLength; x += 128) {
			*(p + x) = upturn_buf[x];x+=8;
		//}
	}
	



	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	
	if(++MoveIndex > 48){
		MoveIndex = 0;
		time_upindex = 0;
		time_upsec = 0;
		sys_move = 0;
		delay_ms(100);
	}
}

//
/********************************************************************************
 *								������ʾ��Ϣ										*
 ********************************************************************************
*/
u8 CheckFile(u8 *buf_p, u8 *text_p, u8 len)
{
	u8  i,h,l,*p;
	u16 x;
	s16 z;

	p = buf_p;
	x = 0;
	
	// ����
	//if (len < ScreenLength) {
	//	x = ((ScreenLength - len) >> 1)  << 7;
	//}

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
	
	// ��������
	#if 0
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
	#endif 
	
	return 0;
}

/************************************************************************************
 *                                      ��������										*
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
				#if 1
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
				
				#else
				*(p + i*8)     = (a & (0x01));
				*(p + i*8 + 1) = (a & (0x02)) >> 1;
				*(p + i*8 + 2) = (a & (0x04)) >> 2;
				*(p + i*8 + 3) = (a & (0x08)) >> 3;
				*(p + i*8 + 4) = (a & (0x10)) >> 4;
				*(p + i*8 + 5) = (a & (0x20)) >> 5;
				*(p + i*8 + 6) = (a & (0x40)) >> 6;
				*(p + i*8 + 7) = (a & (0x80)) >> 7;

				

				*(p + i*8 + 128) = (b & (0x01));
				*(p + i*8 + 129) = (b & (0x02)) >> 1;
				*(p + i*8 + 130) = (b & (0x04)) >> 2;
				*(p + i*8 + 131) = (b & (0x08)) >> 3;
				*(p + i*8 + 132) = (b & (0x10)) >> 4;
				*(p + i*8 + 133) = (b & (0x20)) >> 5;
				*(p + i*8 + 134) = (b & (0x40)) >> 6;
				*(p + i*8 + 135) = (b & (0x80)) >> 7;
				#endif
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
				#if 1
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
				
				#else
				*(p + i*8)     = (a & (0x01));
				*(p + i*8 + 1) = (a & (0x02)) >> 1;
				*(p + i*8 + 2) = (a & (0x04)) >> 2;
				*(p + i*8 + 3) = (a & (0x08)) >> 3;
				*(p + i*8 + 4) = (a & (0x10)) >> 4;
				*(p + i*8 + 5) = (a & (0x20)) >> 5;
				*(p + i*8 + 6) = (a & (0x40)) >> 6;
				*(p + i*8 + 7) = (a & (0x80)) >> 7;


				*(p + i*8 + 128) = (b & (0x01));
				*(p + i*8 + 129) = (b & (0x02)) >> 1;
				*(p + i*8 + 130) = (b & (0x04)) >> 2;
				*(p + i*8 + 131) = (b & (0x08)) >> 3;
				*(p + i*8 + 132) = (b & (0x10)) >> 4;
				*(p + i*8 + 133) = (b & (0x20)) >> 5;
				*(p + i*8 + 134) = (b & (0x40)) >> 6;
				*(p + i*8 + 135) = (b & (0x80)) >> 7;
				#endif
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
				#if 1
				 *(p + i*8)     = (a & (0x80)) >> 7;
				*(p + i*8 + 1) = (a & (0x40)) >> 6;
				*(p + i*8 + 2) = (a & (0x20)) >> 5;
				*(p + i*8 + 3) = (a & (0x10)) >> 4;
				*(p + i*8 + 4) = (a & (0x08)) >> 3;
				*(p + i*8 + 5) = (a & (0x04)) >> 2;
				*(p + i*8 + 6) = (a & (0x02)) >> 1;
				*(p + i*8 + 7) = (a & (0x01));

				#else
				*(p + i*8)     = (a & (0x01));
				*(p + i*8 + 1) = (a & (0x02)) >> 1;
				*(p + i*8 + 2) = (a & (0x04)) >> 2;
				*(p + i*8 + 3) = (a & (0x08)) >> 3;
				*(p + i*8 + 4) = (a & (0x10)) >> 4;
				*(p + i*8 + 5) = (a & (0x20)) >> 5;
				*(p + i*8 + 6) = (a & (0x40)) >> 6;
				*(p + i*8 + 7) = (a & (0x80)) >> 7;
				#endif
    	}
    }
    SetNSS();
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


/********************************************************************************
 *								��ʼ��											*
 ********************************************************************************
*/
void init_system(void)
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		// ����AFIOʱ��
	AFIO->MAPR      |= 2 << 24;									// �ر�JTAG,����SWD
	
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);			// �ر�DMA1ʱ��
	DMA1_Channel5->CCR   &= ~1;
	EXTI->PR  = 0x000FFFFFu;									// ����жϱ�־
    EXTI->EMR = 0;												// �ر��¼�
    EXTI->IMR = 0;												// �ر��ж�
	SysTick->CTRL = 0;
	TIM3->CR1  &= ~(1u << 3);									// ���ù���ģʽ
	__disable_irq();
	
	// ��ʱ��TIM3��ʼ��  ��ʾʱ��5��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		// ʱ��ʹ��
	TIM_TimeBaseStructure.TIM_Period = SysDisplaySpeed;			// ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 719;					// ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				// ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	// ��ʱ��TIM2��ʼ��  ɨ����ʾ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		// ʱ��ʹ��
	TIM_TimeBaseStructure.TIM_Period = SysDisplaySpeed;//9;//			// ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					// ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				// ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	// ��ʱ��TIM4��ʼ��  ���嶨ʱ
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		// ʱ��ʹ��
// 	TIM_TimeBaseStructure.TIM_Period = 9999;
// 	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );					// ʹ��ָ����TIM2�ж�,��������ж�
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );					// ʹ��ָ����TIM3�ж�,��������ж�
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );
	
	// ����ʱ�� USART1 ʱ�ӿ�����IO�˿�ABCʱ�ӿ���
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);		
	
	USART_InitStructure.USART_BaudRate 				= 115200;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;				//0
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;					//0
	USART_InitStructure.USART_Parity 				= USART_Parity_No;					//0
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None | USART_DMAReq_Rx;	// 0 | 0x0040
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	//4 | 8
	
	USART_Init(USART1, &USART_InitStructure);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			// ʹ�� �����ж�
	
	// ʹ�� USART1
	USART_Cmd(USART1, ENABLE);
	while(!(USART1->SR & 0x40));
	
	
	
// 	USART1->BRR  = 0x1D4C;											 // ������ 9600bps
// 	USART1->CR1  = (1 << 13) | (1 << 3) | (1 << 2);		             // ʹ��USART1,ʹ�ܽ��պͷ���
// 	USART1->CR3 |= (1 << 6);								         // ʹ��DMA����

    // ���� USART1 Tx (PA.09) //�����������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���� USART1 Rx (PA.10) //��������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	#if 1
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA1_Channel5->CCR   &= 0xFFFFFFFE;							// DMA �ر�
	DMA1_Channel5->CNDTR  = UARTMaxLength_Text;       				// DMA ����       				
	DMA1_Channel5->CPAR   = (uint32_t)(&USART1->DR);			// ͨ��x�����ַ
	DMA1_Channel5->CMAR   = (uint32_t)(&RxBuffer_Text[0]);			// ͨ��x�洢����ַ
	DMA1_Channel5->CCR    = (1 << 7) | (1 << 5);				// ִ�д洢����ַ����������ִ��ѭ������
	DMA1_Channel5->CCR   |= 1;									// DMA ����
	#endif
	
	//---------------------------------------------------
	//---------------------------------------------------

// ����ʱ�� USART3 ʱ�ӿ���
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitStructure.USART_BaudRate 				= 9600;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;				//0
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;					//0
	USART_InitStructure.USART_Parity 				= USART_Parity_No;					//0
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None | USART_DMAReq_Rx;	// 0 | 0x0040
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	//4 | 8
	//AFIO_MAPR_USART3_REMAP
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			// ʹ�� �����ж�
	
	// ʹ�� USART3
	USART_Cmd(USART3, ENABLE);
	while(!(USART3->SR & 0x40));

	// ���� USART3 Tx (PB.10) //�����������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ���� USART3 Rx (PB.11) //��������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		

	//---------------------------------------------------
	//---------------------------------------------------
	
	
	// ���� ��ʾ��ABCD (PC.9��8��7��6) �������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// ���� ���ݺ�ɫR12 (PB 10 13) �������
    //GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   

	// ���� ������ɫG12 (PB 11 14) �������
    //GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	// ���� ������ɫB12 (PB 12 15) �������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// ����595  OE��CLK��LAT (PA PC4 PC5) �������
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// RS485     PC12 �������
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	// LED  PA1 �������
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	LEDOFF();
	
	// ��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);             // ʹ������ʱ��
    SPI1->CR1  &= (~(1 << 6)) & 0xFFFF;                              // ��ֹSPI�豸
    SPI1->CR1   = (3 << 8) | (0 << 3) | (1 << 2);                    // ����������豸���� ������fPCLK/2 
	SPI1->CR1  |= 1 << 6;                                            // ����SPI�豸
	
	// ���� SPI1 NSS (PA.4) �������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	SetNSS();
	
	// ���� SPI1 CLK (PA.5) ��������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ���� SPI1 MISO (PA.6) ����
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ���� SPI1 MOSI (PA.7) ��������
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	
	NVIC->ICER[7  / 32]	|= 1u << (7  % 32);                          // ��ֹEXTI1�ж�
	NVIC->ICER[8  / 32]	|= 1u << (8  % 32);                          // ��ֹEXTI2�ж�
	// USART3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//TIM2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);								//��ʼ��NVIC�Ĵ���
	
	//
	//TIM3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);								//��ʼ��NVIC�Ĵ���
	
	//TIM4
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);								//��ʼ��NVIC�Ĵ���
	
	// ���Ź�
	//IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);     //��ʹ�ܶԼĴ��� I д���� IWDG->KR = 0x5555;
	//IWDG_SetPrescaler(4);            //������ IWDG Ԥ��Ƶֵ:���� IWDG Ԥ��Ƶֵ IWDG->PR = 4;
	//IWDG_SetReload(625);               //������ IWDG ��װ��ֵ
	//IWDG_ReloadCounter();   //�۰��� IWDG ��װ�ؼĴ�����ֵ��װ�� IWDG ������
	//IWDG_Enable();         //��ʹ�� IWDG
	
	
	Init_SST25VF();						//��ʼ��Flash

	RS485_R();							//485����
	//ClearScreen();						//����һ��
	
	Data_Init();
	
	TIM_Cmd(TIM3, DISABLE);
//	TIM_Cmd(TIM4, DISABLE);
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
	
	__enable_irq();						//ʹ�������ж� 
}
//
void UBX_CFG_MSG(void)
{
	uint8_t i;
	//uint8_t UBX_CFG_MSG_Buf[6] = {0x24,0x2B,0x32,0x39,0x40,0x47};
	uint8_t UBX_CFG_MSG_Buf[5] = {0x24,0x2B,0x32,};

// ���200ms
#if 0
//B5 62 06 07 14 00 40 0D 03 00 A0 86 01 00 01 01 00 00 34 03 00 00 00 00 00 00 D1 CA
const u8 dat_time[28]={ 0xB5, 0x62, 0x06, 0x07, 0x14, 0x00, 0x40, 0x0D, 0x03, 0x00, \
	0xA0, 0x86, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x34, \
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD1, 0xCA};


for(i = 0; i < sizeof(dat_time); i++)
	{
		USART3->DR = dat_time[i]; while((USART3->SR & 0x40) == 0);
	}
	#endif
	
		

//
	//$GPRMC
	//b5 62 06 01 08 00 f0 04 00 01 00 00 00 00 04 3c
	for(i = 0; i < 4; i++)
	{
		USART3->DR = 0xB5; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x62; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x06; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x08; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0xF0; while((USART3->SR & 0x40) == 0);
		USART3->DR = i;    while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
		USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
		//
		USART3->DR = i;    while((USART3->SR & 0x40) == 0);
		USART3->DR = UBX_CFG_MSG_Buf[i]; while((USART3->SR & 0x40) == 0);
	}
	//GPTXT
//b5 62 06 01 08 00 f0 0f 00 00 00 00 00 01 0f 8d
//b5 62 06 01 08 00 f0 10 00 00 00 00 00 01 10 94
	USART3->DR = 0xB5; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x62; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x06; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x08; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0xF0; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x10; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	//
	USART3->DR = 0x10; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x94; while((USART3->SR & 0x40) == 0);
	
	// ȡ��ZDA
	#if 1
//b5 62 06 01 08 00 f0 08 00 00 00 00 00 01 08 5c
	USART3->DR = 0xB5; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x62; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x06; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x08; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0xF0; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x08;    while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	//
	USART3->DR = 0x08;    while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x5c; while((USART3->SR & 0x40) == 0);
	#endif
		//$GPRMC
	//b5 62 06 01 08 00 f0 04 00 01 00 00 00 00 04 3c
	USART3->DR = 0xB5; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x62; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x06; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x08; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0xF0; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x04; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	//
	USART3->DR = 0x04; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x44; while((USART3->SR & 0x40) == 0);
	
	

	// ZDA
	#if 0
	USART3->DR = 0xB5; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x62; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x06; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x08; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0xF0; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x08; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x01; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);
	//
	USART3->DR = 0x08; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x60; while((USART3->SR & 0x40) == 0);
	#endif
	//
	// ����
	USART3->DR = 0xB5; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x62; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x06; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x09; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x0D; while((USART3->SR & 0x40) == 0);
	for(i = 0; i < 5; i++) {USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);}
	USART3->DR = 0xFF; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0xFF; while((USART3->SR & 0x40) == 0);
	for(i = 0; i < 6; i++) {USART3->DR = 0x00; while((USART3->SR & 0x40) == 0);}
	USART3->DR = 0x03; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0x1D; while((USART3->SR & 0x40) == 0);
	USART3->DR = 0xAB; while((USART3->SR & 0x40) == 0);
}
//
