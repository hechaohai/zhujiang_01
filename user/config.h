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


#define Red      1
#define Green   2
#define Yellow 0

__CONFIG_H_EXT__ u8 display_color;

#define Invalid 0
#define Valid   1


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
#define ScreenLength        8								// 扫描字长
//#define SysDisplaySpeed		(100u / SaoMiao)
#define SysDisplaySpeed		1u
#define MaxLine				64							// 行点像素
#define MaxRow				16								// 列点像素
#define DisplayBufMaxLength (ScreenLength * 16*8)
#define IDAddr				0x00070000						// ID存储地址
#define LightAddr			0x00071000						// 亮度存储地址
#define UpdataSatusAddr		0x0007D000						// 要升级标志
#define PictureStartAddr	0x00080000						// 图片存储地址 0x080000~0x1C0000
#define	UARTMaxLength		0x100							// UART接收最大字节数
#define	CanMaxLength		0x100							// CAN接收最大字节数
#define RxHeadLength		8u


#define PromptSartAddr1      0x00071000				//默认语存放起始地址
#define PromptSartAddr2      0x00072000

#define RS485_R()			GPIOC->BRR  = 1u << 12			// 低
#define RS485_T()			GPIOC->BSRR = 1u << 12

#define BeepON()			GPIOB->BRR  = 1u << 14
#define BeepOFF()			GPIOB->BSRR = 1u << 14

#define LEDON()				GPIOA->BRR  = 1u << 1
#define LEDOFF()			GPIOA->BSRR = 1u << 1

#define nop __nop
#define WriteDisplayData(x)  GPIOB->BSRR = ((x & 0x6C) << 8) | ((~x & 0x6C) << 24)

#define SysMoveSpeed         3



// 扫描
__CONFIG_H_EXT__ uint8_t  LED_Index;							// LED运行指示灯
__CONFIG_H_EXT__ uint8_t  LineIndex;							// 扫描第几行
__CONFIG_H_EXT__ uint8_t  DisplayBuf0[DisplayBufMaxLength];			// 显示缓存区1
__CONFIG_H_EXT__ uint8_t  DisplayBuf1[DisplayBufMaxLength];			// 显示缓存区2
__CONFIG_H_EXT__ uint8_t  DisplayIndex;							// 处理缓存区选择
//__CONFIG_H_EXT__ uint8_t  DisplayIndex_Shadow;				// 显示缓存区选择
// ID Light Button
__CONFIG_H_EXT__ uint8_t  ID;
__CONFIG_H_EXT__ uint8_t  Light;
__CONFIG_H_EXT__ uint16_t Button;
// 
__CONFIG_H_EXT__ uint8_t  PictureIndex;
__CONFIG_H_EXT__ uint8_t  suoping;
//心跳包计数
__CONFIG_H_EXT__ uint16_t timeout_index; //计数
__CONFIG_H_EXT__ uint8_t timeout_index_p; //计数
__CONFIG_H_EXT__ uint8_t  timeout_flag;  // 超时标志
__CONFIG_H_EXT__ uint8_t  timeout_onece_text; // 有文本
__CONFIG_H_EXT__ uint8_t  timeout_doing_color; // 显示‘注意安全’


// 上翻
__CONFIG_H_EXT__ u8  upturn_buf[DisplayBufMaxLength];
__CONFIG_H_EXT__ u8  upturn_index; // 上翻行数
__CONFIG_H_EXT__ u8  uptext_index; // 上翻文本序号

// 默认语
#define WordLineMaxLength 100u
__CONFIG_H_EXT__ uint8_t  PromptTextLength1;					// 默认语长度
__CONFIG_H_EXT__ uint8_t  PromptTextLength2;
__CONFIG_H_EXT__ uint8_t  PromptTextLength3;
__CONFIG_H_EXT__ uint8_t  PromptTextBuf1[WordLineMaxLength];			// 默认语缓存
__CONFIG_H_EXT__ uint8_t  PromptTextBuf2[WordLineMaxLength];
__CONFIG_H_EXT__ uint8_t  PromptTextBuf3[WordLineMaxLength];

// 移动
__CONFIG_H_EXT__ u8  Bemove;								//可以移动一位
__CONFIG_H_EXT__ uint8_t  HaveDisplayNum;						// 上次移动后扫描多少次
__CONFIG_H_EXT__ uint8_t  Bemove;								// 可以移动一位
__CONFIG_H_EXT__ uint8_t  MoveIndex;							// 移动计数，满8位加载下一个文字
__CONFIG_H_EXT__ uint8_t  Move_LoadGB;							// 当前移动字符是GB2312(1)或ASCII(0)
__CONFIG_H_EXT__ uint16_t MovetextNum;							// 移动第几个字节
__CONFIG_H_EXT__ uint8_t  MoveBuf[32*8];							// 移动一个字符缓存区

__CONFIG_H_EXT__ u32  time_index;					// 时间序号
__CONFIG_H_EXT__ u32  time_upindex;
__CONFIG_H_EXT__ u32  time_sec;					// 时间序号转换为秒
__CONFIG_H_EXT__ u32  time_upsec;					// 时间序号转换为秒

//
__CONFIG_H_EXT__ uint8_t  *MoveWord_p;								// 移动默认语\即时文本\预警文本

// 串口
__CONFIG_H_EXT__ uint8_t  ZhuanYi;								// 1 下一个要转义 0 不用转义
__CONFIG_H_EXT__ uint8_t  Updata;								// 有文本内容更新 0 头 1 文本 2 升级
__CONFIG_H_EXT__ uint16_t RxIndex;								// USART计数
__CONFIG_H_EXT__ uint16_t HeadIndex;							// 头索引
__CONFIG_H_EXT__ uint16_t NowIndex;								// 当前处理数据索引
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1Data;					// 接收一个字节
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1True;					// 判断是否有接收字节
__CONFIG_H_EXT__ uint8_t  RxBuffer[UARTMaxLength];				// 接收USART缓存区

// can
__CONFIG_H_EXT__ uint8_t  CanRxBuffer[CanMaxLength];				// 接收显示协议缓存区
__CONFIG_H_EXT__ uint8_t  CanRxIndex;
__CONFIG_H_EXT__ uint8_t  CanDoIndexRx;

__CONFIG_H_EXT__ uint8_t  CanZiBuffer[CanMaxLength];				// 接收字库缓存区
__CONFIG_H_EXT__ uint8_t  CanZiIndex;
__CONFIG_H_EXT__ uint8_t  CanDoIndexZi;

__CONFIG_H_EXT__ uint8_t  CanUpBuffer[CanMaxLength];				// 接收升级缓存区
__CONFIG_H_EXT__ uint8_t  CanUpIndex;
__CONFIG_H_EXT__ uint8_t  CanDoIndexUp;


//__CONFIG_H_EXT__ struct control_date control[4];
// 协议
typedef struct {
	//u8 cmd;
	//u8 id;
	//u8 reserve[6];
	u8 data[8];
}CmdNotify;
typedef struct  {
	//u8 index;
	//u8 test[7];
	u8 data[8];
}CmdText;
typedef struct  {
	//u8 cmd;
	//u8 package_id;
	//u16 test_length;
	//u8 reserve[4];
	u8 data[8];
}CmdFinish;

#define DISPLAY_STATIC 0
#define DISPLAY_MOVE1 1

typedef struct {
	//u8  cmd;
	//u8  package_id;
	//u8 display_time;
	//u8 color;
	//u8 style;
	//u8 change_time;
	u8 data[8];
}CmdControl;


typedef struct{
	 CmdNotify  notify;
	 CmdText    text[15];
	 CmdFinish  finish;
	 CmdControl control;
}AgreementData;

__CONFIG_H_EXT__ u8 data_time;  // 协议过期
__CONFIG_H_EXT__ u8 data_flag;

__CONFIG_H_EXT__ u8 text_count; //文本包数量

__CONFIG_H_EXT__ AgreementData agreement_data;

#define CMD_NOTIFY 	0
#define CMD_TEXT  	1
#define CMD_FINISH 	2
#define CMD_CONTROL 3
__CONFIG_H_EXT__ u8 cmd_data;

#define NOTIFY  0xFC
#define TEXT    15
#define FINISH  0xFF
#define CONTROL 0xFA

#define RECVED 0xFE // 数据传输完成命令字
#define REQUIRE 0xFD // 数据补传命令字
__CONFIG_H_EXT__ u8 if_display_none; // 是否清屏
__CONFIG_H_EXT__ u8 update_agreement;

__CONFIG_H_EXT__ u8 display_done; // 0 显示中不更换，1 显示完一条信息
__CONFIG_H_EXT__ u8 display_done_p; // 已经发送显示完成

__CONFIG_H_EXT__ u8 display_list[4];
__CONFIG_H_EXT__ u8 display_list_index;

typedef struct {
	u8 flag;
	u8 id;
	u8 display_time;//亮度 1~15
	u8 color;
	u8 style;
	u8 upstyle;
	u8 if_screen_off;
	u8 change_time;
	u8 text[100];
	u8 length;
	u8 speed;
	
	u8 display_count;
}CurrentData;
#define DEFAULT 0
#define IMMEDIATELY1 1
#define IMMEDIATELY2 2
#define IMMEDIATELY3 3
__CONFIG_H_EXT__ CurrentData currnet_data[4]; // 文本缓存
__CONFIG_H_EXT__ CurrentData diplay_data;//当前显示文本

__CONFIG_H_EXT__ u8  currnet_index;// 显示哪个
__CONFIG_H_EXT__ u8  next_index;// 下一次显示哪个
__CONFIG_H_EXT__ u8  updata_index;//更新文本插入序号
__CONFIG_H_EXT__ u8  updata_flag;// 更新文本标志

__CONFIG_H_EXT__ u8  if_change_clor;

typedef struct {
	u8 flag;
	u8 id;
	u8 display_time;
	u8 color;
	u8 style;
	u8 upstyle;
	u8 if_screen_off;
	u8 change_time;
	u8 text[100];
	u8 length;
	u8 speed;
	
	u8 display_count;
}DisplayData;
__CONFIG_H_EXT__ DisplayData display_data;

#define CMD_FINISH  0xFF
#define MAX_CMD_TEXT  15
#define CMD_CONTROL 0xFA


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
__CONFIG_H_EXT__ u8 CheckFile(u8 *buf_p, u8 *p, u8 len);
__CONFIG_H_EXT__ void load_diplay(void);
#endif

