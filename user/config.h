#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifndef __CONFIG_H_INC__
	#define __CONFIG_H_EXT__ extern
#else
	#define __CONFIG_H_EXT__
#endif

#ifndef TURE
	#define TURE 0
#endif

#ifndef FALSE
	#define FALSE 1
#endif

#include "stm32f10x.h"

//#define OffDisplay()		GPIOA->BRR  = GPIO_Pin_8;		// ����ʾ
//#define OnDisplay()			GPIOA->BSRR = GPIO_Pin_8;		// ����ʾ	
#define OffDisplay()		GPIOA->BSRR = GPIO_Pin_8;		// ����ʾ	
#define OnDisplay()		GPIOA->BRR  = GPIO_Pin_8;		// ����ʾ	

#define SysButton			0xF020							// PB5 12 13 14 15
#define Button_4			0xE020							// PB12
#define Button_3			0xD020							// PB13
#define Button_2			0xB020							// PB14
#define Button_1			0x7020							// PB15
#define Button_5			0xF000							// PB5

#define SaoMiao				8
#define ScreenLength        20								// ɨ���ֳ�
#define SysDisplaySpeed		(100u / SaoMiao)
#define MaxLine				128u							// �е�����
#define MaxRow				96u								// �е�����
#define IDAddr				0x00070000						// ID�洢��ַ
#define LightAddr			0x00071000						// ���ȴ洢��ַ
#define UpdataSatusAddr		0x0007D000						// Ҫ������־
#define PictureStartAddr	0x00080000						// ͼƬ�洢��ַ 0x080000~0x1C0000
#define	UARTMaxLength		0x4000							// UART��������ֽ���
#define RxHeadLength		8u

#define RS485_R()			GPIOC->BRR  = 1u << 12			// ��
#define RS485_T()			GPIOC->BSRR = 1u << 12

#define BeepON()			GPIOB->BRR  = 1u << 14
#define BeepOFF()			GPIOB->BSRR = 1u << 14

#define LEDON()				GPIOA->BRR  = 1u << 1
#define LEDOFF()			GPIOA->BSRR = 1u << 1

#define nop __nop
#define WriteDisplayData(x)  GPIOB->BSRR = ((x & 0x0F) << 8) | ((~x & 0x0F) << 24)

#define SysMoveSpeed         3

// ɨ��
__CONFIG_H_EXT__ uint8_t  LED_Index;							// LED����ָʾ��
__CONFIG_H_EXT__ uint8_t  LineIndex;							// ɨ��ڼ���
__CONFIG_H_EXT__ uint8_t  DisplayBuf0[MaxRow][MaxLine];			// ��ʾ������1
__CONFIG_H_EXT__ uint8_t  DisplayBuf1[MaxRow][MaxLine];			// ��ʾ������2
__CONFIG_H_EXT__ uint8_t  DisplayIndex;							// ��������ѡ��
//__CONFIG_H_EXT__ uint8_t  DisplayIndex_Shadow;				// ��ʾ������ѡ��
// ID Light Button
__CONFIG_H_EXT__ uint8_t  ID;
__CONFIG_H_EXT__ uint8_t  Light;
__CONFIG_H_EXT__ uint16_t Button;
// 
__CONFIG_H_EXT__ uint8_t  PictureIndex;
// ����
__CONFIG_H_EXT__ uint8_t  ZhuanYi;								// 1 ��һ��Ҫת�� 0 ����ת��
__CONFIG_H_EXT__ uint8_t  Updata;								// ���ı����ݸ��� 0 ͷ 1 �ı� 2 ����
__CONFIG_H_EXT__ uint16_t RxIndex;								// USART����
__CONFIG_H_EXT__ uint16_t HeadIndex;							// ͷ����
__CONFIG_H_EXT__ uint16_t NowIndex;								// ��ǰ������������
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1Data;					// ����һ���ֽ�
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1True;					// �ж��Ƿ��н����ֽ�
__CONFIG_H_EXT__ uint8_t  RxBuffer[UARTMaxLength];				// ����USART������
//
__CONFIG_H_EXT__ uint32_t SysCounter;

typedef  void (*pFunction)(void);

__CONFIG_H_EXT__ void ReadLight(void);
__CONFIG_H_EXT__ void LoadPicture(uint8_t);
__CONFIG_H_EXT__ void ReadID(void);
__CONFIG_H_EXT__ void CarPageTurn(void);
__CONFIG_H_EXT__ void Data_Init(void);
__CONFIG_H_EXT__ void ClearScreen(void);
__CONFIG_H_EXT__ void init_system(void);
__CONFIG_H_EXT__ void LoadChar(uint8_t *p, uint16_t dat);
__CONFIG_H_EXT__ void delay_us(uint8_t num);
__CONFIG_H_EXT__ void delay_ms(uint8_t num);
__CONFIG_H_EXT__ void ClearDisplay(void);
#endif

