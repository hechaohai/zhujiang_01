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

//#define OffDisplay()		GPIOA->BRR  = GPIO_Pin_8;		// 关显示
//#define OnDisplay()			GPIOA->BSRR = GPIO_Pin_8;		// 开显示	
#define OffDisplay()		GPIOA->BSRR = GPIO_Pin_8;		// 关显示	
#define OnDisplay()		GPIOA->BRR  = GPIO_Pin_8;		// 开显示	

#define SysButton			0xF020							// PB5 12 13 14 15
#define Button_4			0xE020							// PB12
#define Button_3			0xD020							// PB13
#define Button_2			0xB020							// PB14
#define Button_1			0x7020							// PB15
#define Button_5			0xF000							// PB5

#define SaoMiao				8
#define ScreenLength        20								// 扫描字长
#define SysDisplaySpeed		(100u / SaoMiao)
#define MaxLine				128u							// 行点像素
#define MaxRow				96u								// 列点像素
#define IDAddr				0x00070000						// ID存储地址
#define LightAddr			0x00071000						// 亮度存储地址
#define UpdataSatusAddr		0x0007D000						// 要升级标志
#define PictureStartAddr	0x00080000						// 图片存储地址 0x080000~0x1C0000
#define	UARTMaxLength		0x4000							// UART接收最大字节数
#define RxHeadLength		8u

#define RS485_R()			GPIOC->BRR  = 1u << 12			// 低
#define RS485_T()			GPIOC->BSRR = 1u << 12

#define BeepON()			GPIOB->BRR  = 1u << 14
#define BeepOFF()			GPIOB->BSRR = 1u << 14

#define LEDON()				GPIOA->BRR  = 1u << 1
#define LEDOFF()			GPIOA->BSRR = 1u << 1

#define nop __nop
#define WriteDisplayData(x)  GPIOB->BSRR = ((x & 0x0F) << 8) | ((~x & 0x0F) << 24)

#define SysMoveSpeed         3

// 扫描
__CONFIG_H_EXT__ uint8_t  LED_Index;							// LED运行指示灯
__CONFIG_H_EXT__ uint8_t  LineIndex;							// 扫描第几行
__CONFIG_H_EXT__ uint8_t  DisplayBuf0[MaxRow][MaxLine];			// 显示缓存区1
__CONFIG_H_EXT__ uint8_t  DisplayBuf1[MaxRow][MaxLine];			// 显示缓存区2
__CONFIG_H_EXT__ uint8_t  DisplayIndex;							// 处理缓存区选择
//__CONFIG_H_EXT__ uint8_t  DisplayIndex_Shadow;				// 显示缓存区选择
// ID Light Button
__CONFIG_H_EXT__ uint8_t  ID;
__CONFIG_H_EXT__ uint8_t  Light;
__CONFIG_H_EXT__ uint16_t Button;
// 
__CONFIG_H_EXT__ uint8_t  PictureIndex;
// 串口
__CONFIG_H_EXT__ uint8_t  ZhuanYi;								// 1 下一个要转义 0 不用转义
__CONFIG_H_EXT__ uint8_t  Updata;								// 有文本内容更新 0 头 1 文本 2 升级
__CONFIG_H_EXT__ uint16_t RxIndex;								// USART计数
__CONFIG_H_EXT__ uint16_t HeadIndex;							// 头索引
__CONFIG_H_EXT__ uint16_t NowIndex;								// 当前处理数据索引
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1Data;					// 接收一个字节
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1True;					// 判断是否有接收字节
__CONFIG_H_EXT__ uint8_t  RxBuffer[UARTMaxLength];				// 接收USART缓存区
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

