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
#define OffDisplay	{	GPIOC->BSRR = GPIO_Pin_6; \
												GPIOC->BSRR = GPIO_Pin_7; \
												GPIOC->BSRR = GPIO_Pin_9; \
												GPIOC->BSRR = GPIO_Pin_8; }


#define SysButton			0xF020							// PB5 12 13 14 15
#define Button_4			0xE020							// PB12
#define Button_3			0xD020							// PB13
#define Button_2			0xB020							// PB14
#define Button_1			0x7020							// PB15
#define Button_5			0xF000							// PB5

#define SaoMiao				4
#define ScreenLength        12								// 扫描字长
//#define SysDisplaySpeed		(100u / SaoMiao)
#define SysDisplaySpeed		4u
#define MaxLine				64							// 行点像素
#define MaxRow				16								// 列点像素
#define DisplayBufMaxLength (ScreenLength * 16*8)
#define IDAddr				0x00070000						// ID存储地址
#define LightAddr			0x00071000						// 亮度存储地址
#define UpdataSatusAddr		0x0007D000						// 要升级标志
#define PictureStartAddr	0x00080000						// 图片存储地址 0x080000~0x1C0000

#define	CanMaxLength		0x100							// CAN接收最大字节数
#define RxHeadLength		8u


#define PromptSartAddr1      0x00070000		// 头
#define PromptSartAddr2      0x00071000		// 文本


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
__CONFIG_H_EXT__ uint16_t  LED_Index;							// LED运行指示灯
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



// 上翻
__CONFIG_H_EXT__ u8  upturn_buf[DisplayBufMaxLength];
__CONFIG_H_EXT__ u8  upturn_index; // 上翻行数
__CONFIG_H_EXT__ u8  uptext_index; // 上翻文本序号



// 移动
__CONFIG_H_EXT__ u8  Bemove;								//可以移动一位
__CONFIG_H_EXT__ uint8_t  HaveDisplayNum;						// 上次移动后扫描多少次
__CONFIG_H_EXT__ uint8_t  Bemove;								// 可以移动一位
__CONFIG_H_EXT__ uint8_t  MoveIndex;							// 移动计数，满8位加载下一个文字
__CONFIG_H_EXT__ uint8_t  Move_LoadGB;							// 当前移动字符是GB2312(1)或ASCII(0)
__CONFIG_H_EXT__ uint16_t MovetextNum;							// 移动第几个字节
__CONFIG_H_EXT__ uint8_t  MoveBuf[32*8];							// 移动一个字符缓存区

__CONFIG_H_EXT__ uint8_t  XuehuaIndex;					// 雪花
__CONFIG_H_EXT__ uint8_t  XuehuaIndex_p;					// 雪花
__CONFIG_H_EXT__ uint8_t  Xuehua_dibu;					// 底部移出后，可以飘雪

//__CONFIG_H_EXT__ u32  time_index;					// 时间序号
//__CONFIG_H_EXT__ u32  time_sec;					// 时间序号转换为秒
__CONFIG_H_EXT__ u16  time_upindex;
__CONFIG_H_EXT__ u16  time_upsec;					// 时间序号转换为秒

//
__CONFIG_H_EXT__ uint8_t  *MoveWord_p;								// 移动默认语\即时文本\预警文本

// 串口
//__CONFIG_H_EXT__ uint8_t  ZhuanYi;								// 1 下一个要转义 0 不用转义


__CONFIG_H_EXT__ uint8_t  Updata_GPS;								// 有文本内容更新 0 头 1 文本 2 升级


__CONFIG_H_EXT__ uint8_t  Updata_Text;								// 有文本内容更新 0 头 1 文本 2 升级
__CONFIG_H_EXT__ uint16_t RxIndex;								// USART计数
__CONFIG_H_EXT__ uint16_t HeadIndex;							// 头索引
__CONFIG_H_EXT__ uint16_t NowIndex;								// 当前处理数据索引
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1Data;					// 接收一个字节
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1True;					// 判断是否有接收字节


#define	UARTMaxLength_Text		0x1000							// UART1接收文本最大字节数
#define	UARTMaxLength_Gps		256							// UART3接收GPS最大字节数
__CONFIG_H_EXT__ uint8_t  RxCounter_Gps; // UART3接收GPS序号

__CONFIG_H_EXT__ uint8_t  RxBuffer_Text[UARTMaxLength_Text];				// 接收USART缓存区
__CONFIG_H_EXT__ uint8_t  RxBuffer_Gps[UARTMaxLength_Gps];				// 接收USART缓存区

// can
//__CONFIG_H_EXT__ uint8_t  CanRxBuffer[CanMaxLength];				// 接收显示协议缓存区
//__CONFIG_H_EXT__ uint8_t  CanRxIndex;
//__CONFIG_H_EXT__ uint8_t  CanDoIndexRx;

//__CONFIG_H_EXT__ uint8_t  CanZiBuffer[CanMaxLength];				// 接收字库缓存区
//__CONFIG_H_EXT__ uint8_t  CanZiIndex;
//__CONFIG_H_EXT__ uint8_t  CanDoIndexZi;

//__CONFIG_H_EXT__ uint8_t  CanUpBuffer[CanMaxLength];				// 接收升级缓存区
//__CONFIG_H_EXT__ uint8_t  CanUpIndex;
//__CONFIG_H_EXT__ uint8_t  CanDoIndexUp;




#define DISPLAY_STATIC 0
#define DISPLAY_MOVE1 1





__CONFIG_H_EXT__ u8 data_time;  // 协议过期
__CONFIG_H_EXT__ u8 data_flag;

__CONFIG_H_EXT__ u8 text_count; //文本包数量


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


__CONFIG_H_EXT__ u8  currnet_index;// 显示哪个
__CONFIG_H_EXT__ u8  next_index;// 下一次显示哪个
__CONFIG_H_EXT__ u8  updata_index;//更新文本插入序号
__CONFIG_H_EXT__ u8  updata_flag;// 更新文本标志

__CONFIG_H_EXT__ u8  if_change_clor;


struct GpsData{
	u8 flag;
	u8 add_day;
	u8 add_month;
	u8 add_year;
	u8 time_hour_H;
	u8 time_hour_L;
	u8 time_munute_H;
	u8 time_munute_L;
	u8 time_second_H;
	u8 time_second_L;
	u8 speed_H;
	u8 speed_M;
	u8 speed_L;
	u8 date_year_H;
	u8 date_year_L;
	u8 date_month_H;
	u8 date_month_L;
	u8 date_day_H;
	u8 date_day_L;
};

__CONFIG_H_EXT__ struct GpsData gps_date;

/*
head节目开始头： 01
tpye节目类型：00 文本 01 时间  02 时速 03 日期
donghua动画方式：00 直接显示   01 向左   02 向右   03 向上   04 向下   05 飘雪 06 左中右
speed动画速度：0~100
time停留时间：0~100
*/
struct ReceiveData{
	u8 head;
	u8 tpye;
	u8 donghua;
	u8 speed;
	u8 time;
	u8 text[256];
};

__CONFIG_H_EXT__ struct ReceiveData receive_data[6];
__CONFIG_H_EXT__ u8 NeedGPS;
__CONFIG_H_EXT__ u8 receive_update;
__CONFIG_H_EXT__ u8 text_index;//显示序号
__CONFIG_H_EXT__ u8 text_flag;//有效文本在显示
__CONFIG_H_EXT__ u8 sys_move;//系统有移动
__CONFIG_H_EXT__ u16 speed_num; //速度计数


typedef struct {
	u8 flag;
	u8 id;
	u8 display_time;//亮度 1~15
	u8 color;
	u8 style;
	u8 upstyle;
	u8 if_screen_off;
	u8 text_GB_flag;
	u16 text_index;
	u16 length;
	u8 speed;
	
	u8 gps_text[12];
	
	u8 display_count;
}CurrentData;

__CONFIG_H_EXT__ CurrentData diplay_data;//当前显示文本


/*
typedef struct {
	u8 head; // 0 不显示
	u8 tpye;    // 节目类型：00 文本 01 时间  02 时速 03 日期
	u8 donghua; // 动画方式
	u8 speed;    // 动画速度
	u8 time; // 停留时间
	u8 text[256];
	u16 text_length;
	GpsData gpsdate;
}TextData;

typedef struct {
	u8 flag;    // 文本更新
	u8 number;  // 总共多少条信息
	u8 id;      // 当前显示信息
	TextData textdata[6];
}ShowData;

__CONFIG_H_EXT__ ShowData showdata;
*/

//#define CMD_FINISH  0xFF
#define MAX_CMD_TEXT  15
//#define CMD_CONTROL 0xFA


//
__CONFIG_H_EXT__ uint32_t SysCounter;

typedef  void (*pFunction)(void);
__CONFIG_H_EXT__ void check_timeout1(void);
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
__CONFIG_H_EXT__ void UBX_CFG_MSG(void);

__CONFIG_H_EXT__ void do_load_up(void);
__CONFIG_H_EXT__ void do_speed(void);
__CONFIG_H_EXT__ void do_text(void);
__CONFIG_H_EXT__ void do_data(void);
__CONFIG_H_EXT__ void do_time(void);

__CONFIG_H_EXT__ void Move_Up(void);
__CONFIG_H_EXT__ void Move_Down(void);
__CONFIG_H_EXT__ void Move_piaoxue(void);
__CONFIG_H_EXT__ void Move_LMR(void);
#endif

