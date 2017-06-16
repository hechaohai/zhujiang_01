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

/////////////////////////////////////////////////////////////////////////////////////
extern const uint16_t CRC16Table[];

void UpText(uint16_t len);				//����������
void MoveWordLine1(void);
void MoveWordLine2(void);
void MoveWordLine3(void);
void UpGrade(uint16_t len);
void ReturnUpdataError(void);
void ReturnUpdataOK(void);
void ReturnText(uint8_t ret);
void Communication(void);
void ReturnVN(void);					//���ذ汾��
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
	u8 res;
	int key = 0;
	u8 canbuf[8];
	//
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
	
	SetNSS();
	init_system();				// ϵͳ��ʼ��
	//CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_7tq,5,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������450Kbps 
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_7tq,5,mode);//CAN��ͨģʽ��ʼ��, ������450Kbps 
	//
	LEDOFF();
	res=Can_Send_Msg(canbuf,8);//����8���ֽ� 
	while(1)
	{
		//IWDG_ReloadCounter();
		
		//Communication();
		key=Can_Receive_Msg(canbuf);
		if(key) {
			LEDON();
			delay_ms(40);
			LEDOFF();
			res=Can_Send_Msg(canbuf,8);//����8���ֽ� 
		}
		
		delay_ms(400);
		
	}
}
//
void Communication(void)
{
	uint32_t addr,ui32;
	uint16_t x,y;
	USART_InitTypeDef	USART_InitStructure;
	
	while(1)
	{
		RxIndex = UARTMaxLength - DMA1_Channel5->CNDTR;
		
		if(NowIndex == RxIndex) return;
		
		else
		{
			#if 1
			do
			{
				// �ı���ʼͷ
				//if( (2 != Updata) && (0x55 == RxBuffer[(NowIndex - 6) & (UARTMaxLength - 1)]) &&
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
				// ��������ʼͷ
				#if 0
				else if(0xa5 == RxBuffer[NowIndex] && (1 != Updata))
				{
					HeadIndex = NowIndex;
					Updata = 2;
					break;
				}
				// �ֿ⿪ʼͷ
				
				else if((0x55 == RxBuffer[(NowIndex - 5) & (UARTMaxLength - 1)]) &&
						(0xff == RxBuffer[(NowIndex - 4) & (UARTMaxLength - 1)]) &&
						(0x55 == RxBuffer[(NowIndex - 3) & (UARTMaxLength - 1)]) &&
						(0xff == RxBuffer[(NowIndex - 2) & (UARTMaxLength - 1)]) &&
						(0x00 == RxBuffer[(NowIndex - 1) & (UARTMaxLength - 1)]) &&
						(0xab == RxBuffer[NowIndex]))
				{
					TIM_Cmd(TIM2, DISABLE);
					OffDisplay();					//����ʾ
					
					
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
				if(1 != Updata) break;
				
				// ������ʶ //
// 				if(((1 == Updata) && ((0xFF != RxBuffer[(NowIndex - 3) & (UARTMaxLength - 1)]) ||
// 					(0x27 != RxBuffer[(NowIndex - 2) & (UARTMaxLength - 1)]) || 
// 					(0xFF != RxBuffer[(NowIndex - 1) & (UARTMaxLength - 1)]) ||
// 					(0x28 != RxBuffer[NowIndex])) )
// 					|| 
// 					((2 == Updata) && (0xae != RxBuffer[NowIndex])))
				if( (0xFF != RxBuffer[(NowIndex - 3) & (UARTMaxLength - 1)]) ||
					(0x27 != RxBuffer[(NowIndex - 2) & (UARTMaxLength - 1)]) || 
					(0xFF != RxBuffer[(NowIndex - 1) & (UARTMaxLength - 1)]) ||
					(0x28 != RxBuffer[NowIndex]) )
				{
					break;
				}
				
				
				/****************************** ���Խ��յ������� ***********************************/
				#if 0
// 				x = HeadIndex;
// 				while( x != NowIndex)
// 				{
// 					USART1->DR = RxBuffer[x];	
// 					while((USART1->SR & 0x40) == 0);
// 					x = (x + 1) & (UARTMaxLength - 1);
// 				}
// 				USART1->DR = RxBuffer[NowIndex];	
// 				while((USART1->SR & 0x40) == 0);
				
				
				for(x = 0; x < 32; x ++)
				{
					USART1->DR = RxBuffer[x];	
					while((USART1->SR & 0x40) == 0);
				}
				Updata = 0;
				break;
				#endif
				/**********************************************************************************/
				
				x = (NowIndex - HeadIndex) & (UARTMaxLength - 1);
				if     ((1 == Updata) && ((x <= 8) || (x >= 0x4000 ))) break;
				else if((2 == Updata) && ((x <= 8) || (x >= 600  ))) break;
				//���Ȳ�����
// 				if     ((1 == Updata) && (((NowIndex - HeadIndex) <= 8) ||  ((NowIndex - HeadIndex) & (UARTMaxLength - 1)) >= 1030)) break;
// 				else if((2 == Updata) && (((NowIndex - HeadIndex) <= 8) ||  ((NowIndex - HeadIndex) & (UARTMaxLength - 1)) >= 600)) break;
				
				// ���ı����ݸ���
				if(1 == Updata)
				{
					x = ((uint16_t)RxBuffer[(HeadIndex + 8) & (UARTMaxLength - 1)] << 8) + RxBuffer[(HeadIndex + 9) & (UARTMaxLength - 1)];
					UpText(x);
				}
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
				Updata = 0;
			}while(0);
			
			#endif
		}
		
		NowIndex = (NowIndex + 1) & (UARTMaxLength - 1);
//		NowIndex = (NowIndex + 1) & (0x800 - 1);
	
	}
}
//
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
		OffDisplay();					//����ʾ
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
			LoadPicture(PictureIndex - 1);
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
		OffDisplay();					//����ʾ
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
		OffDisplay();					//����ʾ
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
//
void ReturnText(uint8_t ret)
{
	uint8_t  returnbuf[7];
	uint16_t crc16;
	
	returnbuf[0] = RxBuffer[(HeadIndex + 1) & (UARTMaxLength - 1)];
	returnbuf[1] = RxBuffer[(HeadIndex + 2) & (UARTMaxLength - 1)];
	returnbuf[2] = RxBuffer[(HeadIndex + 3) & (UARTMaxLength - 1)];
	returnbuf[3] = RxBuffer[(HeadIndex + 4) & (UARTMaxLength - 1)];
	returnbuf[4] = 0;
	returnbuf[5] = 1;
	returnbuf[6] = ret;
	
	crc16 = CRC_16_pData(returnbuf,7);
	
	USART1->DR = 0x7e;	                        //��ʼ��	
	while((USART1->SR & 0x40) == 0);
	
	
	USART1->DR = returnbuf[0];
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = returnbuf[1];
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = returnbuf[2];
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = returnbuf[3];
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = returnbuf[4];
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = returnbuf[5];
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = returnbuf[6];
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = (uint8_t)(crc16 >> 8);
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = (uint8_t)(crc16 & 0xff);
	while((USART1->SR & 0x40) == 0);
	
	USART1->DR = 0x7e;	                        //������	
	while((USART1->SR & 0x40) == 0);
	
	
	
	
}
//
//���� OffDisplay

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
//
/********************************************************************************
 *									END_FILE									*
 ********************************************************************************
*/



