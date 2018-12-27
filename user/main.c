/********************************************************************
 * Program:		IAP ��������											*
 * flash��ַ:	0x08000000~0x08003FFF								*
 * Э���ַ��	0x08004000											*
 * �����ɹ���	0x08004010����PC��0x08004014����SP					*
 * sst25vf��  0x040000~7FFFF
 ********************************************************************
   * SST25VFXXX �洢�ռ����
 *     0x080000~0x17FFFF: 32��ߵ��ֿ�(16x32��ASCII�ַ���32x32��GB2312�ַ�)
 *     0x180000~0x1BFFFF: ����
 *     0x1C0000~0x1CFFFF: 16��ߵ��ֿ�( 8x16��ASCII�ַ���16x16��GB2312�ַ�)
 *     //
 *     0x000000~0x003FFF: Bootloader,16KB
 *     0x004000~0x03FFFF: APP,240KB
 *     //
 *     0x040000~0x040FFF: ����0��Ĭ���ı�
 *     0x041000~0x041FFF: ����1��Ĭ���ı�
 *     0x042000~0x042FFF: ����2��Ĭ���ı�
 *     0x043000~0x043FFF: ����3��Ĭ���ı�
 *     0x044000~0x044FFF: ����4��Ĭ���ı�
 *     0x045000~0x045FFF: ����5��Ĭ���ı�
 *     0x046000~0x046FFF: ����6��Ĭ���ı�
 *     0x047000~0x047FFF: ����7��Ĭ���ı�
 *     0x048000~0x048FFF: ����8��Ĭ���ı�
 *     0x049000~0x049FFF: ����9��Ĭ���ı�
 *     0x04A000~0x04AFFF: ����10��Ĭ���ı�
 *     //
 *     0x07F000~0x07FFFF: ͨѶ��ַ(1�ֽ�)
 *     0x07E000~0x07EFFF: ���Ⱥ��ƶ��ٶ�
 *     0x07D000~0x07DFFF: �̼����±�־(1�ֽ�),Ϊ0x00ʱ����,����ֵ������
 *     0x07C000~0x07CFFF: ������ʾ����
  
	LAT   PA11   ����  
	CLK   PA12   ʱ��
	OE    PA8    ��ʾ
	A     PC9    ��A
	B     PC8
	C     PC7
	D     PC6
	
	R1    PB8
	G1    PB9
	R2    PB10
	G2    PB11
	R3    PB12
	G3    PB13
	R4    PB14
	G4    PB15
	
 */

/********************************************************************************
 *								ͷ�ļ�											*
 ********************************************************************************
 */

#include "interrupt.h"
#include "config.h"
//#include "ds1302.h"
#include "crc.h"
#include "sst25vf.h"
//#include "aes128.h"
//#include "can.h"

/////////////////////////////////////////////////////////////////////////////////////
extern const uint16_t CRC16Table[];
extern const uint8_t const_word[12];
extern const uint8_t const_word1[];
extern const uint8_t ReceiveData_Head[66];
extern const uint8_t monthdays[12];

void UpText(uint16_t len);				//����������

void UpGrade(uint16_t len);
//void ReturnUpdataError(void);
void ReturnUpdataOK(void);
void ReturnText(uint8_t ret);
void Communication(void);
void ReturnVN(void);					//���ذ汾��
void do_GNRMC(void);
void do_fenduan(u8 i,u8 j);

void do_load_move(void);

void MoveWordLine_text(void);
void Move_text_left(void);
void Move_text_right(void);

void MoveWordLine_gps(void);
void Move_gps_left(void);
void Move_gps_right(void);

void do_update_text(void);
void check_timeout(void);
void do_update_receive(void);
void check_timeout2(u8 *p,u8 m);
void do_save_receive(void);
void do_chaoshi(void);
//

#define VN	0x04

/********************************************************************************
 *								����ȫ�ֱ���										*
 ********************************************************************************
 */
 
 /*
__asm void StartAPP()
{
	CPSID I
	LDR   R0, =0x08004400
	LDR   R1, =0x08004404
	LDR   R2, [R0]
	MSR   MSP, R2
	LDR   R2, [R1]
	BX    R2
}
*/
/*__asm void nop(void)
{
    NOP
	NOP
}*/

/********************************************************************************
 *								������											*
 ********************************************************************************
 */
int main(void)
{
	OffDisplay;	//����ʾ
	SetNSS();
	init_system();				// ϵͳ��ʼ��
	//
	LEDON();//LEDOFF();
	
	UBX_CFG_MSG();
	
	//check_timeout();
	receive_update = 1;
	do_update_receive();
	//do_chaoshi();
	//do_data();
	while(1)
	{
		Communication();
	}

}
void Communication(void)
{
	u32 addr, ui32;
	u8 i,ret,j,m,n;
	u16 x, y;
	u8 text[8];
//	u8 id,index,i,j,z,ret,count,count_m,data_temp,col;
//	u8 temp[8] = {0,0,0,0,0,0,0,0};
//	u8 canbuf[8]={1,2,3,4,5,6,7,8};
	USART_InitTypeDef	USART_InitStructure;
//	u8 data;

//	u8 buf_recved[8]  = {RECVED,0,0,0,0,0,0,0};
//	u8 buf_require[8] = {REQUIRE,0,0,0,0,0,0,0};

		while(1) {
		RxIndex = UARTMaxLength_Text - DMA1_Channel5->CNDTR;

		
		if(NowIndex == RxIndex) {
			// GPSͨѶ
			#if 1
			if(Updata_GPS) {
				do_GNRMC();
				Updata_GPS = 0;
				RxCounter_Gps = 0;
				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//ʹ�ܽ����ж�
			}
			#endif
			// PC��ͨѶ
			if(receive_update){
				do_save_receive();
				do_update_receive();
			}
			// ʱ�䳬ʱ ��Ч�ı�����ʾ �ı�û�����ƶ�
			if(text_flag && !sys_move){
				if((time_upsec > receive_data[text_index].time) || (0 == receive_data[text_index].time))
				{
					do_chaoshi();
				}
			}
			if(sys_move && receive_data[text_index].tpye){
				MoveWordLine_gps();
			}
			else if(sys_move)
			{
				MoveWordLine_text();
			}
			
			continue;
			//return;
		}
		
		else {
			#if 1
			
			do {
				//�������ͣ� A0 07 00 00 11 27 00 00 00 00
				//if(9 < ((RxIndex - NowIndex) & (UARTMaxLength - 1))){
				if(
				(0xA0 == RxBuffer_Text[(NowIndex - 9) & (UARTMaxLength_Text - 1)]) &&
				(0x07 == RxBuffer_Text[(NowIndex - 8) & (UARTMaxLength_Text - 1)]) &&
				(0x00 == RxBuffer_Text[(NowIndex - 7) & (UARTMaxLength_Text - 1)]) &&
				(0x00 == RxBuffer_Text[(NowIndex - 6) & (UARTMaxLength_Text - 1)]) &&
				(0x11 == RxBuffer_Text[(NowIndex - 5) & (UARTMaxLength_Text - 1)]) &&
				(0x27 == RxBuffer_Text[(NowIndex - 4) & (UARTMaxLength_Text - 1)]) &&
				(0x00 == RxBuffer_Text[(NowIndex - 3) & (UARTMaxLength_Text - 1)]) &&
				(0x00 == RxBuffer_Text[(NowIndex - 2) & (UARTMaxLength_Text - 1)]) &&
				(0x00 == RxBuffer_Text[(NowIndex - 1) & (UARTMaxLength_Text - 1)]) &&
				(0x00 == RxBuffer_Text[NowIndex])) {
					for( i = 0; i < 10;i++){
						RxBuffer_Text[(NowIndex - i) & (UARTMaxLength_Text - 1)] = 0;
					}
					Updata_Text = 0;
					for(i = 0; i < sizeof(ReceiveData_Head); i++) {
						USART1->DR = ReceiveData_Head[i]; while((USART1->SR & 0x40) == 0);
					}
					for(x = 0; x < 1566; x++) {
						USART1->DR = *(&receive_data[0].head + x); while((USART1->SR & 0x40) == 0);
					}
					break;
				}
				//����6���ı� 
 //A0 07 52 06 F9 2A 00 00 00 00 A0 78 39 5B 8D AD
 //3A 61 F5 01 D2 27 7C 51 70 B1 F9 3D 38 33 07 00
				//else if( 1631 < ((RxIndex - NowIndex) & (UARTMaxLength_Text - 1))){
					if(
					(0xA0 == RxBuffer_Text[(NowIndex - 9) & (UARTMaxLength_Text - 1)]) &&
					(0x07 == RxBuffer_Text[(NowIndex - 8) & (UARTMaxLength_Text - 1)]) &&
					(0x52 == RxBuffer_Text[(NowIndex - 7) & (UARTMaxLength_Text - 1)]) &&
					(0x06 == RxBuffer_Text[(NowIndex - 6) & (UARTMaxLength_Text - 1)]) &&
					(0xF9 == RxBuffer_Text[(NowIndex - 5) & (UARTMaxLength_Text - 1)]) &&
					(0x2A == RxBuffer_Text[(NowIndex - 4) & (UARTMaxLength_Text - 1)]) &&
					(0x00 == RxBuffer_Text[(NowIndex - 3) & (UARTMaxLength_Text - 1)]) &&
					(0x00 == RxBuffer_Text[(NowIndex - 2) & (UARTMaxLength_Text - 1)]) &&
					(0x00 == RxBuffer_Text[(NowIndex - 1) & (UARTMaxLength_Text - 1)]) &&
					(0x00 == RxBuffer_Text[(NowIndex - 0) & (UARTMaxLength_Text - 1)])) {
						HeadIndex = (NowIndex - 9) & (UARTMaxLength_Text - 1);
						Updata_Text = 1;
						// ���� A0 07 00 00 09 52 00 08 46 81 
						USART1->DR = 0xa0; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x07; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x00; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x00; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x09; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x52; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x00; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x08; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x46; while((USART1->SR & 0x40) == 0);
						USART1->DR = 0x81; while((USART1->SR & 0x40) == 0);
						break;
					}

				
				// �ı���ʼͷ
				#if 0
				if( (0x55 == RxBuffer[(NowIndex - 6) & (UARTMaxLength - 1)]) &&
					(0xff == RxBuffer[(NowIndex - 5) & (UARTMaxLength - 1)]) &&
					(0x55 == RxBuffer[(NowIndex - 4) & (UARTMaxLength - 1)]) &&
					(0x53 == RxBuffer[(NowIndex - 3) & (UARTMaxLength - 1)]) &&
					(0x49 == RxBuffer[(NowIndex - 2) & (UARTMaxLength - 1)]) &&
					(0x4E == RxBuffer[(NowIndex - 1) & (UARTMaxLength - 1)]) &&
					((ID  == RxBuffer[NowIndex]) || (0xFF == RxBuffer[NowIndex])) )
				{
					HeadIndex = (NowIndex - 6) & (UARTMaxLength - 1);
					Updata = 1;
					break;
				}
				#endif
				// ��������ʼͷ
				#if 0
				else if(0xa5 == RxBuffer[NowIndex] && (1 != Updata))
				{
					HeadIndex = NowIndex;
					Updata = 2;
					break;
				}
				#endif
				// �ֿ⿪ʼͷ
				#if 0
				else if((0x55 == RxBuffer_Text[(NowIndex - 5) & (UARTMaxLength_Text - 1)]) &&
						(0xff == RxBuffer_Text[(NowIndex - 4) & (UARTMaxLength_Text - 1)]) &&
						(0x55 == RxBuffer_Text[(NowIndex - 3) & (UARTMaxLength_Text - 1)]) &&
						(0xff == RxBuffer_Text[(NowIndex - 2) & (UARTMaxLength_Text - 1)]) &&
						(0x00 == RxBuffer_Text[(NowIndex - 1) & (UARTMaxLength_Text - 1)]) &&
						(0xab == RxBuffer_Text[NowIndex]))
				{
					TIM_Cmd(TIM2, DISABLE);
					OffDisplay;					//����ʾ
					
					
					// 
					WRDI();
					for(addr = 0x1C0000; addr < 0x200000; addr += 0x8000)
					//for(addr = 0; addr < 0x40000; addr += 0x8000)
					{
						//ResetWatchDog();
						WREN();
						Block_Erase_32K(addr);
						delay_ms(40);//Wait_Busy();
					}
					WRDI();
					//
					// 57600bps
					USART_InitStructure.USART_BaudRate 				= 57600;
					USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;				//0
					USART_InitStructure.USART_StopBits 				= USART_StopBits_1;					//0
					USART_InitStructure.USART_Parity 				= USART_Parity_No;					//0
					USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;	//0
					USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	//4 | 8
					USART_Init(USART1, &USART_InitStructure);
					//
					addr = 0x1C0000;
					
					ui32 = 0;
					NowIndex = (NowIndex + 1) & (UARTMaxLength - 1);
					while(ui32 < 0x01000000)
					{
						RxIndex = UARTMaxLength - DMA1_Channel5->CNDTR;
						
						if(NowIndex != RxIndex)
						{
							WREN();
							Byte_Program(RxBuffer[NowIndex], addr);
							Wait_Busy();//delay_us(20);//
							
							if((++addr) >= 0x200000){ReturnByte(0xAA);break;}
							ui32 = 0;
							NowIndex = (NowIndex + 1) & (UARTMaxLength - 1);
						}
						else ui32++;
					}
					
					WRDI();
					//
					NVIC_SystemReset();						//ϵͳ��λ
				}
				#endif
				//
				// û�п�ʼͷ
				if(1 != Updata_Text) break;

				
				x = (NowIndex - HeadIndex) & (UARTMaxLength_Text - 1);
				if(x < 1631) break;
				if(
						(0xA0 == RxBuffer_Text[(NowIndex - 1631) & (UARTMaxLength_Text - 1)]) &&
						(0x07 == RxBuffer_Text[(NowIndex - 1630) & (UARTMaxLength_Text - 1)]) &&
						(0x52 == RxBuffer_Text[(NowIndex - 1629) & (UARTMaxLength_Text - 1)]) &&
						(0x06 == RxBuffer_Text[(NowIndex - 1628) & (UARTMaxLength_Text - 1)]) &&
						(0xF9 == RxBuffer_Text[(NowIndex - 1627) & (UARTMaxLength_Text - 1)]) &&
						(0x2A == RxBuffer_Text[(NowIndex - 1626) & (UARTMaxLength_Text - 1)])) {
							
						for (x = 0; x < sizeof(receive_data); x++) {
							*(&receive_data[0].head + x) = RxBuffer_Text[(NowIndex - 1565 + x) & (UARTMaxLength_Text - 1)];
						}
						receive_update = 1;
				}
				Updata_Text = 0;

				// ����
				#if 0
				else if(2 == Updata)
				{
					x = (HeadIndex + 1)  & (UARTMaxLength - 1);
					y = 0;
					while(x != NowIndex)
					{
						if(RxBuffer[x] != 0xAA)
						{
							y++;
							if(RxBuffer[(x - 1) & (UARTMaxLength - 1)] == 0xAA)
								RxBuffer[(HeadIndex + y) & (UARTMaxLength - 1)] = 0xA0 | RxBuffer[x];
							else 
								RxBuffer[(HeadIndex + y) & (UARTMaxLength - 1)] = RxBuffer[x];
						}
						
						x = (x + 1) & (UARTMaxLength - 1);
					}
					
					UpGrade(y);
				}
				#endif
				
			}while(0);
			
			#endif
		}
		//ReturnByte(RxBuffer[NowIndex]);
		NowIndex = (NowIndex + 1) & (UARTMaxLength_Text - 1);

	}

}
void do_update_receive(void)
{
	u16 x;
	u8 i,j,ret;
	u8 *p,*pp;
	/*
	head��Ŀ��ʼͷ�� 01
	tpye��Ŀ���ͣ�00 �ı� 01 ʱ��  02 ʱ�� 03 ����
	donghua������ʽ��00 ֱ����ʾ   01 ����   02 ����   03 ����   04 ����   05������
	speed�����ٶȣ�0~100
	timeͣ��ʱ�䣺0~100
	text[256]
	*/
	if(receive_update == 0) return;
	receive_update = 0;
	
	NeedGPS = 0;
	text_flag = 0;
	text_index = 0;

	for(i = 0; i < 6; i++){
		if(1 != receive_data[i].head) continue;
		
		text_flag++;
		
		if(0 != receive_data[i].tpye)
			NeedGPS = 1;
	}
	
	
	for(i = 0; i < 6; i++){
		if(1 == receive_data[i].head) {
			
			text_index = i;
			//if(DisplayIndex) {p = DisplayBuf0;pp=DisplayBuf1;}
			//else             {p = DisplayBuf1;pp= DisplayBuf0;}
			//for (x = 0; x < DisplayBufMaxLength; x++){
			//	upturn_buf[x] = 0;
			//}
			break;
		}
	}

	if(text_flag){
		//if(!NeedGPS)
		//	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		//else
		//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		// ��ֹ
		if(0 == receive_data[text_index].donghua){
			sys_move = 0;
			ret = receive_data[text_index].tpye;
			// �ı�
			if(!ret){
				diplay_data.text_index = 0;
				diplay_data.length = 0;
				for(x = 0; x < 256; x++){
					if(receive_data[text_index].text[x])
						diplay_data.length++;
				}
				do_text();
			}
			// ʱ��
			else if (1 == ret){
				do_time();
			}
			// �ٶ�
			else if (2 == ret){
				do_speed();
			}
				
			// ����
			else if (3 == ret){
				do_data();
			}
		}
		// �Ǿ�ֹ
		#if 1
		//else if(1 == receive_data[text_index].donghua)
		else
		{
			if(DisplayIndex) {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}
			else             {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}
			for (x = 0; x < DisplayBufMaxLength; x++)
				*(p + x) = 0;
			
			DisplayIndex = (DisplayIndex + 1) & 1;
			for (x = 0; x < DisplayBufMaxLength; x++)
				*(pp + x) = 0;

			do_load_move();
		}
		#endif
	}
	time_upindex = 0;
	time_upsec = 0;

}

void do_chaoshi(void)
{
	u8 i,j,ret;
	u16 x;
	
	
	time_upindex = 0;
	time_upsec = 0;
	#if 0
	USART1->DR = 0x2A; while((USART1->SR & 0x40) == 0);
	USART1->DR = text_index; while((USART1->SR & 0x40) == 0);
	USART1->DR = receive_data[0].tpye; while((USART1->SR & 0x40) == 0);
	USART1->DR = 0x2A; while((USART1->SR & 0x40) == 0);
	#endif
	
	// �ı�   
	if(!receive_data[text_index].tpye){
		// ��ֹ  ����û��
		if(!receive_data[text_index].donghua && diplay_data.text_index < diplay_data.length)
		{
			do_text();
			return;
		}
		// �Ϸ��·� ����û��
		//else if((3 == receive_data[text_index].donghua && MovetextNum < diplay_data.length) || \
		//		(4 == receive_data[text_index].donghua && MovetextNum < diplay_data.length))
		else if(receive_data[text_index].donghua > 2 && MovetextNum < diplay_data.length)
		{
			sys_move = 1;

			MoveIndex     = 0;							// �ƶ���������8λ������һ������
//			MovetextNum   = 0;							// �ƶ��ڼ����ֽ�
			time_upindex = 0;
			time_upsec = 0;
			do_load_up();
			// ����һ������ʾ
			return;
		}
		else if(1)
		{
			
		}
	}
	
	for(i = 1; i < 7; i++)
	{
		ret = text_index + i;
		if( ret > 5)
			ret -= 6;
			
		if(receive_data[ret].head)
		{
			text_index = ret;
			
			if(0 == receive_data[text_index].donghua){
				sys_move = 0;
				ret = receive_data[text_index].tpye;
				if(1 == ret){
					do_time();
				}
				if(2 == ret){
					do_speed();
				}
				if(3 == ret){
					do_data();
				}
				if(0 == ret){
					diplay_data.text_index = 0;
					diplay_data.length = 0;
					for(x = 0; x < 256; x++){
						if(receive_data[text_index].text[x])
							diplay_data.length++;
					}
					do_text();
				}
			}
			#if 1
			//else if(1 == receive_data[text_index].donghua)
			else
			{
				do_load_move();
			}
			#endif
			
			break;
		}
	}
}
//
void check_timeout(void)
{
	u8 text[12], ret, i,j;
	u8 *p,*pp;

	if(DisplayIndex) {p = DisplayBuf0;pp=DisplayBuf1;}
	else             {p = DisplayBuf1;pp= DisplayBuf0;}

	for(i = 0; i < 12; i++)
	{
		text[i] = const_word[i];//

	}
	ret = CheckFile(p, text, 12);
	DisplayIndex = (DisplayIndex + 1 ) &1;
	time_upindex = 0;
	time_upsec = 0;
	while(time_upsec < 5){
		
	}
}

//
// �ƶ�ǰ������Ϣ
void do_load_move(void)
{
	u16 x;
	u8 i,j,ret;
	#if 1
	//tpye 00 �ı� 01 ʱ��  02 ʱ�� 03 ����
	ret = receive_data[text_index].tpye;
	if(ret == 0) { // �ı�
		diplay_data.text_GB_flag = 0;
		diplay_data.length = 0;
		for(x = 0; x < 256; x++){
			if(receive_data[text_index].text[x])
				diplay_data.length++;
		}
		// �ı�û��һ��
		for(; x < ScreenLength; x++){
			receive_data[text_index].text[x] = 0x20;
		}
		#if 1
		if(diplay_data.length > ScreenLength){
			//for(x = 0; x < diplay_data.length; x++){
			j = 0;
			if(receive_data[text_index].donghua == 1){
				for(i=1;i<13;i++){
					if(receive_data[text_index].text[diplay_data.length - i] < 0x7f)
						j++;
				}
				if((j % 2) && (receive_data[text_index].text[diplay_data.length - 12] > 0x7f))
					diplay_data.text_GB_flag = 1;
			}
			else if(receive_data[text_index].donghua == 2){
				for(i=0;i<12;i++){
					if(receive_data[text_index].text[i] < 0x7f)
						j++;
				}
				if((j % 2) && (receive_data[text_index].text[0] > 0x7f))
					diplay_data.text_GB_flag = 1;
			}
		}
		#endif
	}
	// ʱ��
	else if(1 == ret) {
		diplay_data.length = 12;
		diplay_data.gps_text[0]= 0x20;
		diplay_data.gps_text[1] = 0x20;
		diplay_data.gps_text[2] = gps_date.time_hour_H;
		diplay_data.gps_text[3] = gps_date.time_hour_L;
		diplay_data.gps_text[4] = 0x3A;
		diplay_data.gps_text[5] = gps_date.time_munute_H;
		diplay_data.gps_text[6] = gps_date.time_munute_L;
		diplay_data.gps_text[7] = 0x3A;
		diplay_data.gps_text[8] = gps_date.time_second_H;
		diplay_data.gps_text[9] = gps_date.time_second_L;
		diplay_data.gps_text[10] = 0x20;
		diplay_data.gps_text[11] = 0x20;	
	}
	
	// ʱ��
	else if(2 == ret) {
		diplay_data.length = 12;
		diplay_data.gps_text[0] = 0xCA;
		diplay_data.gps_text[1] = 0xB1;
		diplay_data.gps_text[2] = 0xCB;
		diplay_data.gps_text[3] = 0xD9;
		
		diplay_data.gps_text[4] = 0x3A;
		diplay_data.gps_text[5] = gps_date.speed_H;
		diplay_data.gps_text[6] = gps_date.speed_M;
		diplay_data.gps_text[7] = gps_date.speed_L;
		
		diplay_data.gps_text[8] = 'k';
		diplay_data.gps_text[9] = 'm';
		diplay_data.gps_text[10] = '/';
		diplay_data.gps_text[11] = 'h';
	}
	// ����
	else if(3 == ret) {
		diplay_data.length = 12;
		diplay_data.gps_text[0] = gps_date.date_year_H;
		diplay_data.gps_text[1] = gps_date.date_year_L;
		diplay_data.gps_text[2] = 0xC4;
		diplay_data.gps_text[3] = 0xEA;
		
		diplay_data.gps_text[4] = gps_date.date_month_H;
		diplay_data.gps_text[5] = gps_date.date_month_L;
		diplay_data.gps_text[6] = 0xD4;
		diplay_data.gps_text[7] = 0xC2;
		
		diplay_data.gps_text[8] = gps_date.date_day_H;
		diplay_data.gps_text[9] = gps_date.date_day_L;
		diplay_data.gps_text[10] = 0xC8;
		diplay_data.gps_text[11] = 0xD5;
	}
	
	speed_num = 0x10;
	if(receive_data[text_index].speed) {
		if(receive_data[text_index].speed>10)
			speed_num = 0x2000;
		else if(receive_data[text_index].speed>2){
			speed_num +=  (u16)4 << (receive_data[text_index].speed - 1);
		}
	}
	#endif
	sys_move = 1;
	
	MoveIndex     = 0;							// �ƶ���������8λ������һ������
	Move_LoadGB   = 0;							// ��ǰ�ƶ��ַ���GB2312(1)��ASCII(0)
	MovetextNum   = 0;							// �ƶ��ڼ����ֽ�

	ret = receive_data[text_index].donghua;

	//  01 ����   02 ����   03 ����   04 ����   05 Ʈѩ 06 ������
	if(1 == ret || 2 == ret){
	}
	// ����
	//else if(3 == ret){
	else{
		do_load_up();
	}
	
}


void check_timeout2(u8 *test,u8 len)
{
	u16 x;
	u8 ret;
	u8 *p,*pp;
		
		if(DisplayIndex) {p = DisplayBuf0;pp=DisplayBuf1;}
		else             {p = DisplayBuf1;pp= DisplayBuf0;}
		//for (x = 0; x < DisplayBufMaxLength; x++){
		//	*(p + x )  = 0;
		//}


		ret = CheckFile(p, test, len);
		
		DisplayIndex = (DisplayIndex + 1 ) &1;
		for (x = 0; x < DisplayBufMaxLength; x++){
			*(pp + x)  = *(p + x);
		}
		delay_ms(100);
		//USART1->DR = 0xff; while((USART1->SR & 0x40) == 0);
}


//
#if 1
void MoveWordLine_text(void)
{
	//  01 ����   02 ����   03 ����   04 ����   05 Ʈѩ 06 ������
	if(1 == receive_data[text_index].donghua)
		Move_text_left();
	
		// ����
	else if(2 == receive_data[text_index].donghua){
		Move_text_right();
	}
	// ����
	else if(3 == receive_data[text_index].donghua){
		Move_Up();
	}
	// ����
	else if(4 == receive_data[text_index].donghua){
		Move_Down();
	}
	// Ʈѩ
	else if(5 == receive_data[text_index].donghua){
		Move_piaoxue();
	}
	// ������
	else if(6 == receive_data[text_index].donghua){
		Move_LMR();
	}
}


void Move_text_left(void)
{
	uint8_t  i,j,z;
	uint8_t  h,l;
	uint16_t x;
	uint8_t  *p,*pp;
//	uint8_t  *word_p;
	uint8_t temp[8] = {0,0,0,0,0,0,0,0};

	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}// 	if(DisplayIndex)  p = DisplayBuf0, pp = DisplayBuf1;
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}// 	else              p = DisplayBuf1, pp = DisplayBuf0;
	
	if(!MoveIndex)
	{
		if(MovetextNum < diplay_data.length)
		{
			if(!Move_LoadGB)
			{
				h = receive_data[text_index].text[MovetextNum];

				if(h < 0xA1)
				{
					LoadChar(MoveBuf, (u16)h);
				}
				else
				{
					l = receive_data[text_index].text[MovetextNum + 1];
					LoadChar(MoveBuf, ((u16)h << 8) | l);
					Move_LoadGB = 1;
				}
			}
			else
			{
				Move_LoadGB = 0;
				for(i = 0; i < 128; i++)
				{
					MoveBuf[i] = MoveBuf[i + 128];
				}
			}
		}
		else
		{
			for(i = 0; i < 128; i++)
				MoveBuf[i] = 0;
		}
		
	}
	
	for(i = 0; i < 16; i++)
	{
		x = i*8;
		for(j = 0; j < ScreenLength - 1; j++)
		{
			*(p + x)       = *(p + x + 1);
			*(p + x + 1) = *(p + x + 2);
			*(p + x + 2) = *(p + x + 3);
			*(p + x + 3) = *(p + x + 4);
			*(p + x + 4) = *(p + x + 5);
			*(p + x + 5) = *(p + x + 6);
			*(p + x + 6) = *(p + x + 7);
			*(p + x + 7) = *(p + x + 128);
			x += 128;
		}
		*(p + x)       = *(p + x + 1);
		*(p + x + 1) = *(p + x + 2);
		*(p + x + 2) = *(p + x + 3);
		*(p + x + 3) = *(p + x + 4);
		*(p + x + 4) = *(p + x + 5);
		*(p + x + 5) = *(p + x + 6);
		*(p + x + 6) = *(p + x + 7);
		*(p + x + 7) = MoveBuf[i*8];
		
		MoveBuf[i*8]       = MoveBuf[i*8 + 1];
		MoveBuf[i*8 + 1] = MoveBuf[i*8 + 2];
		MoveBuf[i*8 + 2] = MoveBuf[i*8 + 3];
		MoveBuf[i*8 + 3] = MoveBuf[i*8 + 4];
		MoveBuf[i*8 + 4] = MoveBuf[i*8 + 5];
		MoveBuf[i*8 + 5] = MoveBuf[i*8 + 6];
		MoveBuf[i*8 + 6] = MoveBuf[i*8 + 7];
		MoveBuf[i*8 + 7] = 0;
	}
	
	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	//
	if(++MoveIndex >= 8)
	{
		MoveIndex = 0;
		if(++MovetextNum >= (diplay_data.length))
		{
			if(diplay_data.text_GB_flag){
				diplay_data.text_GB_flag = 0;
				return;
			}
			time_upindex = 0;
			time_upsec = 0;
			sys_move = 0;
			//delay_ms(100);
		}
	}
}

void Move_text_right(void)
{
	uint8_t  i,j,z;
	uint8_t  h,l;
	uint16_t x;
	uint8_t  *p,*pp;
//	uint8_t  *word_p;
	uint8_t temp[8] = {0,0,0,0,0,0,0,0};

	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}// 	if(DisplayIndex)  p = DisplayBuf0, pp = DisplayBuf1;
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}// 	else              p = DisplayBuf1, pp = DisplayBuf0;
	
	if(!MoveIndex)
	{
		if(MovetextNum < diplay_data.length)
		{
			if(!Move_LoadGB)
			{
				h = receive_data[text_index].text[diplay_data.length - MovetextNum - 1];

				if(h < 0xA1)
				{
					LoadChar(MoveBuf, (u16)h);
					for(i = 0; i < 128; i++)
							MoveBuf[i + 128] = MoveBuf[i];
				}
				else
				{
					if(diplay_data.length - MovetextNum - 1 != 0){
						l = receive_data[text_index].text[diplay_data.length - MovetextNum - 2];
						LoadChar(MoveBuf, ((u16)l << 8) | h);
						Move_LoadGB = 1;
					}
					else{
						for(i = 0; i < 128; i++)
							MoveBuf[i + 128] = 0;
					}
				}
			}
			else
			{
				Move_LoadGB = 0;
				for(i = 0; i < 128; i++)
				{
					MoveBuf[i + 128] = MoveBuf[i];
				}
			}
		}
		else
		{
			for(i = 0; i < 128; i++)
				MoveBuf[i + 128] = 0;
		}
		
	}
	
	for(i = 0; i < 16; i++)
	{
		x = (1408 + 7) + i*8;
		//x = i*8;
		for(j = 0; j < ScreenLength - 1; j++)
		{
			*(p + x)       = *(p + x -1);
			*(p + x - 1) = *(p + x - 2);
			*(p + x - 2) = *(p + x - 3);
			*(p + x - 3) = *(p + x - 4);
			*(p + x - 4) = *(p + x - 5);
			*(p + x - 5) = *(p + x - 6);
			*(p + x - 6) = *(p + x - 7);
			*(p + x - 7) = *(p + x - 128);
			x -= 128;
		}
		*(p + x)     = *(p + x - 1);
		*(p + x - 1) = *(p + x - 2);
		*(p + x - 2) = *(p + x - 3);
		*(p + x - 3) = *(p + x - 4);
		*(p + x - 4) = *(p + x - 5);
		*(p + x - 5) = *(p + x - 6);
		*(p + x - 6) = *(p + x - 7);
		*(p + x - 7) = MoveBuf[i*8 + 7 + 128];
		
		MoveBuf[i*8 + 7 + 128] = MoveBuf[i*8 + 6 + 128];
		MoveBuf[i*8 + 6 + 128] = MoveBuf[i*8 + 5 + 128];
		MoveBuf[i*8 + 5 + 128] = MoveBuf[i*8 + 4 + 128];
		MoveBuf[i*8 + 4 + 128] = MoveBuf[i*8 + 3 + 128];
		MoveBuf[i*8 + 3 + 128] = MoveBuf[i*8 + 2 + 128];
		MoveBuf[i*8 + 2 + 128] = MoveBuf[i*8 + 1 + 128];
		MoveBuf[i*8 + 1 + 128] = MoveBuf[i*8  + 128];
		MoveBuf[i*8 + 128] = 0;
	}
	
	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	//
	if(++MoveIndex >= 8)
	{
		MoveIndex = 0;
		if(++MovetextNum >= (diplay_data.length))
		{
			if(diplay_data.text_GB_flag){
				diplay_data.text_GB_flag = 0;
				return;
			}
			time_upindex = 0;
			time_upsec = 0;
			sys_move = 0;
			//delay_ms(100);
		}
	}
}




#endif

#if 1
void MoveWordLine_gps(void)
{
	//  01 ����   02 ����   03 ����   04 ����   05 Ʈѩ 06 ������
	if(1 == receive_data[text_index].donghua){
		Move_gps_left();
	}
	// ����
	else if(2 == receive_data[text_index].donghua){
		Move_gps_right();
	}
	// ����
	else if(3 == receive_data[text_index].donghua){
		
		Move_Up();
	}
	// ����
	else if(4 == receive_data[text_index].donghua){
		Move_Down();
	}
	// Ʈѩ
	else if(5 == receive_data[text_index].donghua){
		Move_piaoxue();
	}
	// ������
	else if(6 == receive_data[text_index].donghua){
		Move_LMR();
	}

}
#endif
void Move_gps_left(void)
{
	uint8_t  i,j,z;
	uint8_t  h,l;
	uint16_t x;
	uint8_t  *p,*pp;
//	uint8_t  *word_p;

	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}
	
	if(!MoveIndex)
	{
		if(MovetextNum < diplay_data.length)
		{
			if(!Move_LoadGB)
			{
				h = diplay_data.gps_text[MovetextNum];

				if(h < 0xA1)
				{
					LoadChar(MoveBuf, (u16)h);
				}
				else
				{
					l = diplay_data.gps_text[MovetextNum + 1];
					LoadChar(MoveBuf, ((u16)h << 8) | l);
					Move_LoadGB = 1;
				}
			}
			else
			{
				Move_LoadGB = 0;
				for(i = 0; i < 128; i++)
				{
					MoveBuf[i] = MoveBuf[i + 128];
				}
			}
		}
		else
		{
			for(i = 0; i < 128; i++)
				MoveBuf[i] = 0;
		}
		
	}
	
	for(i = 0; i < 16; i++)
	{
		x = i*8;
		for(j = 0; j < ScreenLength - 1; j++)
		{
			*(p + x)       = *(p + x + 1);
			*(p + x + 1) = *(p + x + 2);
			*(p + x + 2) = *(p + x + 3);
			*(p + x + 3) = *(p + x + 4);
			*(p + x + 4) = *(p + x + 5);
			*(p + x + 5) = *(p + x + 6);
			*(p + x + 6) = *(p + x + 7);
			*(p + x + 7) = *(p + x + 128);
			x += 128;
		}
		*(p + x)       = *(p + x + 1);
		*(p + x + 1) = *(p + x + 2);
		*(p + x + 2) = *(p + x + 3);
		*(p + x + 3) = *(p + x + 4);
		*(p + x + 4) = *(p + x + 5);
		*(p + x + 5) = *(p + x + 6);
		*(p + x + 6) = *(p + x + 7);
		*(p + x + 7) = MoveBuf[i*8];
		
		MoveBuf[i*8]     = MoveBuf[i*8 + 1];
		MoveBuf[i*8 + 1] = MoveBuf[i*8 + 2];
		MoveBuf[i*8 + 2] = MoveBuf[i*8 + 3];
		MoveBuf[i*8 + 3] = MoveBuf[i*8 + 4];
		MoveBuf[i*8 + 4] = MoveBuf[i*8 + 5];
		MoveBuf[i*8 + 5] = MoveBuf[i*8 + 6];
		MoveBuf[i*8 + 6] = MoveBuf[i*8 + 7];
		MoveBuf[i*8 + 7] = 0;
	}
	
	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	//
	if(++MoveIndex >= 8)
	{
		MoveIndex = 0;
		if(++MovetextNum >= (diplay_data.length))
		{
			time_upindex = 0;
			time_upsec = 0;
			sys_move = 0;
		}
	}
}

#if 1
void Move_gps_right(void)
{
	uint8_t  i,j,z;
	uint8_t  h,l;
	uint16_t x;
	uint8_t  *p,*pp;
//	uint8_t  *word_p;

	if (!Bemove)
		return;
	
	Bemove = 0;
	
	if(DisplayIndex) {p = &DisplayBuf0[0]; pp = &DisplayBuf1[0];}
	else             {p = &DisplayBuf1[0]; pp = &DisplayBuf0[0];}
	
	if(!MoveIndex)
	{
		if(MovetextNum < diplay_data.length)
		{
			if(!Move_LoadGB)
			{
				h = diplay_data.gps_text[diplay_data.length - MovetextNum - 1];

				if(h < 0xA1)
				{
					LoadChar(MoveBuf, (u16)h);
					for(i = 0; i < 128; i++)
					{
						MoveBuf[i + 128] = MoveBuf[i];
					}
				}
				else
				{
					if(diplay_data.length - MovetextNum - 1 != 0){
						l = diplay_data.gps_text[diplay_data.length - MovetextNum - 2];
						LoadChar(MoveBuf, ((u16)l << 8) | h);
						Move_LoadGB = 1;
					}
					else {
						for(i = 0; i < 128; i++)
							MoveBuf[i + 128] = 0;
					}
				}
			}
			else
			{
				Move_LoadGB = 0;
				for(i = 0; i < 128; i++)
				{
					MoveBuf[i + 128] = MoveBuf[i];
				}
			}
		}
		else
		{
			for(i = 0; i < 128; i++)
				MoveBuf[i + 128] = 0;
		}
		
	}
	
	for(i = 0; i < 16; i++)
	{
		x = (1408 + 7) + i*8;
		//x = i*8;
		for(j = 0; j < ScreenLength - 1; j++)
		{
			*(p + x)       = *(p + x -1);
			*(p + x - 1) = *(p + x - 2);
			*(p + x - 2) = *(p + x - 3);
			*(p + x - 3) = *(p + x - 4);
			*(p + x - 4) = *(p + x - 5);
			*(p + x - 5) = *(p + x - 6);
			*(p + x - 6) = *(p + x - 7);
			*(p + x - 7) = *(p + x - 128);
			x -= 128;
		}
		*(p + x)     = *(p + x - 1);
		*(p + x - 1) = *(p + x - 2);
		*(p + x - 2) = *(p + x - 3);
		*(p + x - 3) = *(p + x - 4);
		*(p + x - 4) = *(p + x - 5);
		*(p + x - 5) = *(p + x - 6);
		*(p + x - 6) = *(p + x - 7);
		*(p + x - 7) = MoveBuf[i*8 + 7 + 128];
		
		MoveBuf[i*8 + 7 + 128] = MoveBuf[i*8 + 6 + 128];
		MoveBuf[i*8 + 6 + 128] = MoveBuf[i*8 + 5 + 128];
		MoveBuf[i*8 + 5 + 128] = MoveBuf[i*8 + 4 + 128];
		MoveBuf[i*8 + 4 + 128] = MoveBuf[i*8 + 3 + 128];
		MoveBuf[i*8 + 3 + 128] = MoveBuf[i*8 + 2 + 128];
		MoveBuf[i*8 + 2 + 128] = MoveBuf[i*8 + 1 + 128];
		MoveBuf[i*8 + 1 + 128] = MoveBuf[i*8  + 128];
		MoveBuf[i*8 + 128] = 0;
	}
	
	//������ʾ��
	DisplayIndex = (DisplayIndex + 1) & 1;
	
	for (x = 0; x < DisplayBufMaxLength; x++)
		 *(pp + x) = *(p + x);
	//
	if(++MoveIndex >= 8)
	{
		MoveIndex = 0;
		if(++MovetextNum >= (diplay_data.length))
		{
			time_upindex = 0;
			time_upsec = 0;
			sys_move = 0;
		}
	}
}

#endif

/*
	head��Ŀ��ʼͷ�� 01
	tpye��Ŀ���ͣ�00 �ı� 01 ʱ��  02 ʱ�� 03 ����
	donghua������ʽ��00 ֱ����ʾ   01 ����   02 ����   03 ����   04 ����   05������
	speed�����ٶȣ�0~100
	timeͣ��ʱ�䣺0~100
	text[256]
	*/


void do_save_receive(void)
{
	uint8_t  i;
	uint16_t x;
	__disable_irq();
	OffDisplay;
	WRDI();WREN(); Sector_Erase(PromptSartAddr1); Wait_Busy();
	WRDI();WREN(); Sector_Erase(PromptSartAddr2); Wait_Busy();
	
	for(i = 0; i < sizeof(ReceiveData_Head); i++) 
	{
		WREN();
		Byte_Program(ReceiveData_Head[i], PromptSartAddr1 + i);
		Wait_Busy();
	}
	for(x = 0; x < 1566; x++)
	{
		WREN();
		Byte_Program( *(&receive_data[0].head + x), PromptSartAddr2 + x );
		Wait_Busy();
	}
	
	//NVIC_SystemReset(); 	// ϵͳ��λ
	__enable_irq();	
}


#if 0
void UpText(uint16_t len)
{
	uint8_t  i, XOR;
	uint16_t x;
	uint32_t addr;
	
	XOR = 0;						// У��
	for(x = 0; x < 11 + len; x++)
	{
		XOR ^= RxBuffer[(HeadIndex + x) & ((UARTMaxLength - 1))];
	}
	
	if(XOR) return;
	
	// ����ͼƬ
	if(0xA1 == RxBuffer[(HeadIndex + 7) & ((UARTMaxLength - 1))] )
	{
		__disable_irq();
		OffDisplay;					//����ʾ
		addr = PictureStartAddr + (RxBuffer[(HeadIndex + 10) & ((UARTMaxLength - 1))] - 1) * 0x4000;
		WREN();Sector_Erase(addr);         Wait_Busy();
		WREN();Sector_Erase(addr + 0x1000);Wait_Busy();
		WREN();Sector_Erase(addr + 0x2000);Wait_Busy();
		WREN();Sector_Erase(addr + 0x3000);Wait_Busy();
		WRDI();
		// ��������
		for(x = 0; x < 11 + len; x++)
		{
			WREN();
			Byte_Program(RxBuffer[(HeadIndex + x) & ((UARTMaxLength - 1))], addr + x);
			Wait_Busy();
			
		}
		WRDI();
		ReturnByte(0xaa);
		
		__enable_irq();
		
	}
	// ��ʾ����
	else if(0xA2 == RxBuffer[(HeadIndex + 7) & ((UARTMaxLength - 1))])
	{
		PictureIndex = RxBuffer[(HeadIndex + 10) & ((UARTMaxLength - 1))];		// ͼƬ��
		if(!PictureIndex)
		{
			ClearDisplay();
		}
		else if(PictureIndex <= 16)												// ȡͼƬ
		{
			//LoadPicture(PictureIndex - 1);
		}
		ReturnByte(0xaa);
	}
	// ��������
	else if(0xA3 == RxBuffer[(HeadIndex + 7) & ((UARTMaxLength - 1))])
	{	
		Light = RxBuffer[(HeadIndex + 10) & ((UARTMaxLength - 1))];
		addr = LightAddr;
		//
		__disable_irq();
		OffDisplay;					//����ʾ
		//
		WREN();
		Sector_Erase(addr);
		Wait_Busy();
		WRDI();
		//
		for(i = 0; i < 11 + len; i++)
		{
			WREN();
			Byte_Program(RxBuffer[(HeadIndex + i) & ((UARTMaxLength - 1))],addr++);
			Wait_Busy();
		}
		WRDI();
		//
		ReturnByte(0xaa);
		__enable_irq();
	}
	// ��������
	else if(0xA4 == RxBuffer[(HeadIndex + 7) & ((UARTMaxLength - 1))])
	{
		addr = IDAddr;
		//
		__disable_irq();
		OffDisplay;					//����ʾ
		//
		WREN();
		Sector_Erase(addr);
		Wait_Busy();
		WRDI();
		//
		for(i = 0; i < 11 + len; i++)
		{
			WREN();
			Byte_Program(RxBuffer[(HeadIndex + i) & ((UARTMaxLength - 1))],addr++);
			Wait_Busy();
		}
		WRDI();
		ID = Read_Byte(IDAddr + 10);
		ReturnByte(ID);
		__enable_irq();
	}
	// ��ȡ����
	else if(0xA5 == RxBuffer[(HeadIndex + 7) & ((UARTMaxLength - 1))])
	{
		ReturnByte(ID);
	}
}

#endif
//

//


#if 0
void UpGrade(uint16_t len)
{
	uint16_t x;
	uint16_t sum;
	uint32_t addr;
	uint16_t Index;
	uint16_t Length;
	uint32_t crc32;
	//
	sum = 0;
	for(x = 0; x < len - 2; x++)		// ��ȥ����У���
	{
		sum += RxBuffer[(HeadIndex + x + 1) & (UARTMaxLength - 1)];
		
		//USART1->DR = RxBuffer[x];
		//while((USART1->SR & 0x40) == 0);
	}
	//
	sum -= RxBuffer[(HeadIndex + x + 1) & (UARTMaxLength - 1)];
	sum -= (uint16_t)RxBuffer[(HeadIndex + x + 2) & (UARTMaxLength - 1)] << 8;
	if(sum) {ReturnUpdataError(); return;}
	//
	SysCounter = 0;
	if(0x50 == RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)])
	{
		ReturnUpdataOK();
	}
	else if(0x51 == RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)])
	{
		TIM_Cmd(TIM2, DISABLE);
		GPIOA->BRR = GPIO_Pin_8;
		Index  = (uint16_t)RxBuffer[(HeadIndex + 8) & (UARTMaxLength - 1)];
		Index |= (uint16_t)RxBuffer[(HeadIndex + 9) & (UARTMaxLength - 1)] << 8;
		addr = 0x0004000 + (uint32_t)Index * 512u;
		// flash����
		if(!(addr & 0xfff))
		{
			WRDI();
			WREN();
			Sector_Erase(addr);
			Wait_Busy();
		}
		// Flash��д
		Length  = (uint16_t)RxBuffer[(HeadIndex + 6) & (UARTMaxLength - 1)];
		Length |= (uint16_t)RxBuffer[(HeadIndex + 7) & (UARTMaxLength - 1)] << 8;
		WRDI();
		for(x = 0; x < Length; x++)
		{
			WREN();
			Byte_Program(RxBuffer[(HeadIndex + x + 12) & (UARTMaxLength - 1)], addr++);
			Wait_Busy();
		}
		ReturnUpdataOK();
		TIM_Cmd(TIM2, ENABLE);
	}
	else if(0x52 == RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)])
	{
		// �ļ���ʶ
// 		if(Read_Byte(0x00004000) != 0x4E || Read_Byte(0x00004001) != 0x4C || 
// 		   Read_Byte(0x00004002) != 0x4B || Read_Byte(0x00004003) != 0x01)
// 		{
// 			ReturnUpdataError();
// 			return;
// 		}
		TIM_Cmd(TIM2, DISABLE);
		GPIOA->BRR = GPIO_Pin_8;
		//
		ClrNSS();
		Read_Data(0x004000);
		addr  = (uint32_t)Get_Byte() << 24;
		addr |= (uint32_t)Get_Byte() << 16;
		addr |= (uint32_t)Get_Byte() <<  8;
		addr |= (uint32_t)Get_Byte() <<  0;
		SetNSS();
		if(addr != 0x4E4C4B01) {ReturnUpdataError(); TIM_Cmd(TIM2, ENABLE);return;}
		
		// ��鳤���Ƿ񳬳�
		ClrNSS();
		Read_Data(0x004000 + 0x0C);
		addr  = (uint32_t)Get_Byte() << 24;
		addr |= (uint32_t)Get_Byte() << 16;
		addr |= (uint32_t)Get_Byte() <<  8;
		addr |= (uint32_t)Get_Byte() <<  0;
		SetNSS();
		if(addr > 240u * 1024u) {ReturnUpdataError();TIM_Cmd(TIM2, ENABLE);return;}
		
		// CRC32У��
		ClrNSS();
		Read_Data(0x004000 + 0x10 + addr);
		crc32  = (uint32_t)Get_Byte() << 24;
		crc32 |= (uint32_t)Get_Byte() << 16;
		crc32 |= (uint32_t)Get_Byte() <<  8;
		crc32 |= (uint32_t)Get_Byte() <<  0;
		SetNSS();
		if(crc32 != CalculateCRC32(0x00004000, addr + 0x10)) {ReturnUpdataError();TIM_Cmd(TIM2, ENABLE);return;}
		//
		WRDI();
		WREN();
		Byte_Program(0x00, 0x07D000);
		Wait_Busy();
		//
		ReturnUpdataOK();
		__disable_irq();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | \
			RCC_APB2Periph_SPI1 |RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5, DISABLE);
		NVIC_SystemReset();						//ϵͳ��λ
	}
}
//
/********************************************************************************
 *									������										*
 ********************************************************************************
*/

void ReturnUpdataOK(void)
{
	uint16_t sum;
	//
	USART1->DR = 0xa5;	                        // ��ʼ��	
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0xe8;							// ������
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0x32;							// ��������
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = RxBuffer[(HeadIndex + 3) & (UARTMaxLength - 1)];						//��ID
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)];						//������(CMD)
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0x01;							//���ر��
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0x01;							//����
	while((USART1->SR & 0x40) == 0);
	//
	sum = 0x00a5 + 0x00e8 + 0x0032 + (uint16_t)RxBuffer[(HeadIndex + 3) & (UARTMaxLength - 1)] +
		(uint16_t)RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)] + 0x0002;
	//
	USART1->DR = (uint8_t)sum;
	while((USART1->SR & 0x40) == 0);
	//
	sum = sum >> 8;
	USART1->DR = (uint8_t)sum;
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0xae;							//����
	while((USART1->SR & 0x40) == 0);
}
#if 0
void ReturnUpdataError(void)
{
	uint16_t sum;
	//
	USART1->DR = 0xa5;	                        //��ʼ��	
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0xe8;							//������
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0x32;							//��������
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = RxBuffer[(HeadIndex + 3) & (UARTMaxLength - 1)];						//��ID
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)];						//������(CMD)
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0x01;							//���ر��
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0x00;							//����
	while((USART1->SR & 0x40) == 0);
	//
	sum = 0x00a5 + 0x00e8 + 0x0032 + (uint16_t)RxBuffer[(HeadIndex + 3) & (UARTMaxLength - 1)] +
		(uint16_t)RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)] + 0x0001;
	//
	USART1->DR = (uint8_t)sum;
	while((USART1->SR & 0x40) == 0);
	//
	sum = sum >> 8;
	USART1->DR = (uint8_t)sum;
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0xae;							//����
	while((USART1->SR & 0x40) == 0);
}
#endif
//
void ReturnVN(void)
{
	uint8_t  returnbuf[7];
	uint16_t crc16;
	//
	returnbuf[0] = 1;
	returnbuf[1] = 1;
	returnbuf[2] = 0xff;
	returnbuf[3] = 0xc6;
	returnbuf[4] = 0;
	returnbuf[5] = 1;
	returnbuf[6] = VN;
	//
	crc16 = CRC_16_pData(returnbuf,7);
	//
	USART1->DR = 0x7e;	                        //��ʼ��	
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = returnbuf[0];
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = returnbuf[1];
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = returnbuf[2];
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = returnbuf[3];
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = returnbuf[4];
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = returnbuf[5];
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = returnbuf[6];
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = (uint8_t)(crc16 >> 8);
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = (uint8_t)(crc16 & 0xff);
	while((USART1->SR & 0x40) == 0);
	//
	USART1->DR = 0x7e;	                        //������	
	while((USART1->SR & 0x40) == 0);
}
#endif

void do_monthdays(void)
{
	u8 year,month,day;
	
	day  = (gps_date.date_day_H - 0x30) << 4;
	day |= gps_date.date_day_L - 0x30;
	
	day++;
	
	month  = (gps_date.date_month_H - 0x30) << 4;
	month |= (gps_date.date_month_L - 0x30);
	if(month < 13) {
		if(day > monthdays[month]){
			year  = (gps_date.date_year_H - 0x30) << 4;
			year |= (gps_date.date_year_L - 0x30);
			if(2 != month) {
				day = 1;
				if(++month>12){
					month = 1;
					year++;
				}
			}else {
				if(year % 4) {
					day = 1;
					month++;
				}
			}
			gps_date.date_month_H = (month >> 4) | 0x30;
			gps_date.date_month_L = (month & 0x0f) | 0x30;
			gps_date.date_year_H = (year >> 4) | 0x30;
			gps_date.date_year_L = (year & 0x0f) | 0x30;
		}
	}
	gps_date.date_day_H = (day >> 4) | 0x30;
	gps_date.date_day_L = (day & 0x0f) | 0x30;

}
void do_fenduan(u8 a,u8 b)
{
	u8 i,j,m;
	uint32_t ret;
	
		// (1)    UTC ʱ�䣬hhmmss��ʱ���룩 
		if(b == 0){
			if(0x2c == RxBuffer_Gps[a + 1]){
				gps_date.time_hour_H = 0x2D;
				gps_date.time_hour_L = 0x2D;
				gps_date.time_munute_H = 0x2D;
				gps_date.time_munute_L = 0x2D;
				gps_date.time_second_H = 0x2D;
				gps_date.time_second_L = 0x2D;
				gps_date.add_day = 0;
				
			}else {
				m = (RxBuffer_Gps[a + 1] - 0x30) * 10 + \
					(RxBuffer_Gps[a + 2] - 0x30) + 8;
				if(m > 23){
					m -= 24;
					gps_date.add_day = 1;
				}else {
					gps_date.add_day = 0;
				}
				gps_date.time_hour_H = (m / 10) + 0x30;
				gps_date.time_hour_L = (m % 10) + 0x30;
				gps_date.time_munute_H = RxBuffer_Gps[a + 3];
				gps_date.time_munute_L = RxBuffer_Gps[a + 4];
				gps_date.time_second_H = RxBuffer_Gps[a + 5];
				gps_date.time_second_L = RxBuffer_Gps[a + 6];
			}
		}
		
		// (7)    �������ʣ�000.0~999.9 �ڣ� һ��Ҳ��1.852ǧ�ף�Сʱ��.2E
		else if(6 == b){
			if(0x2c == RxBuffer_Gps[a + 1]){
			gps_date.speed_H = 0x2D;
			gps_date.speed_M = 0x2D;
			gps_date.speed_L = 0x2D;
			}
			ret = 0;
			for(i = a+1; i < a+8; i ++){
				if(0x2c == RxBuffer_Gps[i]) break;
				if(0x2e == RxBuffer_Gps[i]){
					j = i - a;
					if(j > 1){
						ret  = (uint32_t)(RxBuffer_Gps[i - 1] - 0x30) * 100;
					}
					if(j > 2){
						ret += (uint32_t)(RxBuffer_Gps[i - 2] - 0x30) * 1000;
					}
					if(j > 3){
						ret += (uint32_t)(RxBuffer_Gps[i - 3] - 0x30) * 10000;
					}
					
					ret += (uint32_t)(RxBuffer_Gps[i + 1] - 0x30) * 10;
					ret += (uint32_t)(RxBuffer_Gps[i + 2] - 0x30);

					
					ret *= 1852;
					#if 0
					USART1->DR = 0x2D; while((USART1->SR & 0x40) == 0);
					USART1->DR = (u8)((ret / 100000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
					USART1->DR = (u8)((ret / 10000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
					USART1->DR = (u8)((ret / 1000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
					USART1->DR = (u8)((ret / 100) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
					USART1->DR = (u8)((ret / 10) %10 ) | 0x30; while((USART1->SR & 0x40) == 0);
					USART1->DR = (u8)(ret % 10) | 0x30; while((USART1->SR & 0x40) == 0);
					#endif
					break;
				}
			}
			gps_date.speed_H = 0x20;
			gps_date.speed_M = 0x20;
			gps_date.speed_L = 0x30;
			if(ret){
				ret <<= 1; // ��������
				ret /= 100000;
				
				#if 0
				USART1->DR = 0x2D; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 100000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 10000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 1000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 100) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 10) %10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)(ret % 10) | 0x30; while((USART1->SR & 0x40) == 0);
				#endif
				
				ret >>= 1;
				
				#if 0
				USART1->DR = 0x2D; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 100000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 10000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 1000) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 100) % 10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)((ret / 10) %10 ) | 0x30; while((USART1->SR & 0x40) == 0);
				USART1->DR = (u8)(ret % 10) | 0x30; while((USART1->SR & 0x40) == 0);
				#endif
				
				m = (u8)(ret / 100);
				if(m)
					gps_date.speed_H = (m / 100) + 0x30;
				m = (ret / 10) % 10;
				if(m)
					gps_date.speed_M = ((ret / 10) % 10) + 0x30;
				m = ret % 10;
				if(m)
					gps_date.speed_L = (ret % 10) + 0x30;
				
				#if 0
				USART1->DR = 0x2D; while((USART1->SR & 0x40) == 0);
				USART1->DR = gps_date.speed_H;
				USART1->DR = gps_date.speed_M;
				USART1->DR = gps_date.speed_L;
				#endif
				
			}


		}
		
		// (9)    UTC ���ڣ�ddmmyy�������꣩  monthdays
		if(b == 8) {
			if(0x2c == RxBuffer_Gps[a + 1]) {
				gps_date.date_year_H = 0x2D;
				gps_date.date_year_L = 0x2D;
				gps_date.date_month_H = 0x2D;
				gps_date.date_month_L = 0x2D;
				gps_date.date_day_H = 0x2D;
				gps_date.date_day_L = 0x2D;
				gps_date.add_day = 0;
				gps_date.add_month = 0;
				gps_date.add_year = 0;
			}else{
				gps_date.date_day_H   = RxBuffer_Gps[a + 1];
				gps_date.date_day_L   = RxBuffer_Gps[a + 2];
				gps_date.date_month_H = RxBuffer_Gps[a + 3];
				gps_date.date_month_L = RxBuffer_Gps[a + 4];
				gps_date.date_year_H  = RxBuffer_Gps[a + 5];
				gps_date.date_year_L  = RxBuffer_Gps[a + 6];
				if(gps_date.add_day)
					do_monthdays();
			}
		}
	
}
void do_GNRMC(void)
{
	u8 i,j,m,n;
	u8 XOR, ret;
	#if 0
		USART1->DR = 0x2A; while((USART1->SR & 0x40) == 0);
	USART1->DR = receive_data[text_index].time; while((USART1->SR & 0x40) == 0);
	USART1->DR = time_upsec;while((USART1->SR & 0x40) == 0);
	USART1->DR = 0x2A; while((USART1->SR & 0x40) == 0);
#endif
	// ����У��
	XOR = 0;
	for(i = 1; i < RxCounter_Gps - 4; i++)
	{
		XOR ^= RxBuffer_Gps[i];
		//USART1->DR = RxBuffer_Gps[i]; while((USART1->SR & 0x40) == 0);
	}
	

	m = RxBuffer_Gps[RxCounter_Gps - 3] - 0x30;
	n = RxBuffer_Gps[RxCounter_Gps - 2] - 0x30;
	
	if(m > 0x09) m -= 7;
	if(n > 0x09) n -= 7;
	ret = (m<<4) | n;
	
	if(XOR != ret) {
		return;
	}
	
	// �ֶ� $GNRMC,014930.000,A,3022.90585,N,12002.34201,E,1.97,10.80,231218,,,A*45
	j = 0;
	for(i = 0; i < RxCounter_Gps; i++){
		if(0x2c == RxBuffer_Gps[i]){
			do_fenduan(i,j);
			j++;
		}
	}
	gps_date.flag = 1;
	
	//USART1->DR = 0x0d; while((USART1->SR & 0x40) == 0);
	//USART1->DR = 0x0a; while((USART1->SR & 0x40) == 0);

	if(!sys_move && text_flag)
	{
		ret = receive_data[text_index].tpye;
		if(1 == ret)
				do_time();
		if(2 == ret)
				do_speed();
		if(3 == ret)
				do_data();
	}
}



/********************************************************************************
 *									END_FILE									*
 ********************************************************************************
*/



