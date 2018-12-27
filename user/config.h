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


//#define OffDisplay()		GPIOA->BRR  = GPIO_Pin_8;		// ����ʾ
//#define OnDisplay()			GPIOA->BSRR = GPIO_Pin_8;		// ����ʾ	
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
#define ScreenLength        12								// ɨ���ֳ�
//#define SysDisplaySpeed		(100u / SaoMiao)
#define SysDisplaySpeed		4u
#define MaxLine				64							// �е�����
#define MaxRow				16								// �е�����
#define DisplayBufMaxLength (ScreenLength * 16*8)
#define IDAddr				0x00070000						// ID�洢��ַ
#define LightAddr			0x00071000						// ���ȴ洢��ַ
#define UpdataSatusAddr		0x0007D000						// Ҫ������־
#define PictureStartAddr	0x00080000						// ͼƬ�洢��ַ 0x080000~0x1C0000

#define	CanMaxLength		0x100							// CAN��������ֽ���
#define RxHeadLength		8u


#define PromptSartAddr1      0x00070000		// ͷ
#define PromptSartAddr2      0x00071000		// �ı�


#define RS485_R()			GPIOC->BRR  = 1u << 12			// ��
#define RS485_T()			GPIOC->BSRR = 1u << 12

#define BeepON()			GPIOB->BRR  = 1u << 14
#define BeepOFF()			GPIOB->BSRR = 1u << 14

#define LEDON()				GPIOA->BRR  = 1u << 1
#define LEDOFF()			GPIOA->BSRR = 1u << 1

#define nop __nop
#define WriteDisplayData(x)  GPIOB->BSRR = ((x & 0x6C) << 8) | ((~x & 0x6C) << 24)

#define SysMoveSpeed         3


// ɨ��
__CONFIG_H_EXT__ uint16_t  LED_Index;							// LED����ָʾ��
__CONFIG_H_EXT__ uint8_t  LineIndex;							// ɨ��ڼ���
__CONFIG_H_EXT__ uint8_t  DisplayBuf0[DisplayBufMaxLength];			// ��ʾ������1
__CONFIG_H_EXT__ uint8_t  DisplayBuf1[DisplayBufMaxLength];			// ��ʾ������2
__CONFIG_H_EXT__ uint8_t  DisplayIndex;							// ��������ѡ��
//__CONFIG_H_EXT__ uint8_t  DisplayIndex_Shadow;				// ��ʾ������ѡ��
// ID Light Button
__CONFIG_H_EXT__ uint8_t  ID;
__CONFIG_H_EXT__ uint8_t  Light;
__CONFIG_H_EXT__ uint16_t Button;
// 
__CONFIG_H_EXT__ uint8_t  PictureIndex;
__CONFIG_H_EXT__ uint8_t  suoping;
//����������
__CONFIG_H_EXT__ uint16_t timeout_index; //����
__CONFIG_H_EXT__ uint8_t timeout_index_p; //����
__CONFIG_H_EXT__ uint8_t  timeout_flag;  // ��ʱ��־
__CONFIG_H_EXT__ uint8_t  timeout_onece_text; // ���ı�



// �Ϸ�
__CONFIG_H_EXT__ u8  upturn_buf[DisplayBufMaxLength];
__CONFIG_H_EXT__ u8  upturn_index; // �Ϸ�����
__CONFIG_H_EXT__ u8  uptext_index; // �Ϸ��ı����



// �ƶ�
__CONFIG_H_EXT__ u8  Bemove;								//�����ƶ�һλ
__CONFIG_H_EXT__ uint8_t  HaveDisplayNum;						// �ϴ��ƶ���ɨ����ٴ�
__CONFIG_H_EXT__ uint8_t  Bemove;								// �����ƶ�һλ
__CONFIG_H_EXT__ uint8_t  MoveIndex;							// �ƶ���������8λ������һ������
__CONFIG_H_EXT__ uint8_t  Move_LoadGB;							// ��ǰ�ƶ��ַ���GB2312(1)��ASCII(0)
__CONFIG_H_EXT__ uint16_t MovetextNum;							// �ƶ��ڼ����ֽ�
__CONFIG_H_EXT__ uint8_t  MoveBuf[32*8];							// �ƶ�һ���ַ�������

__CONFIG_H_EXT__ uint8_t  XuehuaIndex;					// ѩ��
__CONFIG_H_EXT__ uint8_t  XuehuaIndex_p;					// ѩ��
__CONFIG_H_EXT__ uint8_t  Xuehua_dibu;					// �ײ��Ƴ��󣬿���Ʈѩ

//__CONFIG_H_EXT__ u32  time_index;					// ʱ�����
//__CONFIG_H_EXT__ u32  time_sec;					// ʱ�����ת��Ϊ��
__CONFIG_H_EXT__ u16  time_upindex;
__CONFIG_H_EXT__ u16  time_upsec;					// ʱ�����ת��Ϊ��

//
__CONFIG_H_EXT__ uint8_t  *MoveWord_p;								// �ƶ�Ĭ����\��ʱ�ı�\Ԥ���ı�

// ����
//__CONFIG_H_EXT__ uint8_t  ZhuanYi;								// 1 ��һ��Ҫת�� 0 ����ת��


__CONFIG_H_EXT__ uint8_t  Updata_GPS;								// ���ı����ݸ��� 0 ͷ 1 �ı� 2 ����


__CONFIG_H_EXT__ uint8_t  Updata_Text;								// ���ı����ݸ��� 0 ͷ 1 �ı� 2 ����
__CONFIG_H_EXT__ uint16_t RxIndex;								// USART����
__CONFIG_H_EXT__ uint16_t HeadIndex;							// ͷ����
__CONFIG_H_EXT__ uint16_t NowIndex;								// ��ǰ������������
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1Data;					// ����һ���ֽ�
__CONFIG_H_EXT__ uint8_t  ReceiveUSART1True;					// �ж��Ƿ��н����ֽ�


#define	UARTMaxLength_Text		0x1000							// UART1�����ı�����ֽ���
#define	UARTMaxLength_Gps		256							// UART3����GPS����ֽ���
__CONFIG_H_EXT__ uint8_t  RxCounter_Gps; // UART3����GPS���

__CONFIG_H_EXT__ uint8_t  RxBuffer_Text[UARTMaxLength_Text];				// ����USART������
__CONFIG_H_EXT__ uint8_t  RxBuffer_Gps[UARTMaxLength_Gps];				// ����USART������

// can
//__CONFIG_H_EXT__ uint8_t  CanRxBuffer[CanMaxLength];				// ������ʾЭ�黺����
//__CONFIG_H_EXT__ uint8_t  CanRxIndex;
//__CONFIG_H_EXT__ uint8_t  CanDoIndexRx;

//__CONFIG_H_EXT__ uint8_t  CanZiBuffer[CanMaxLength];				// �����ֿ⻺����
//__CONFIG_H_EXT__ uint8_t  CanZiIndex;
//__CONFIG_H_EXT__ uint8_t  CanDoIndexZi;

//__CONFIG_H_EXT__ uint8_t  CanUpBuffer[CanMaxLength];				// ��������������
//__CONFIG_H_EXT__ uint8_t  CanUpIndex;
//__CONFIG_H_EXT__ uint8_t  CanDoIndexUp;




#define DISPLAY_STATIC 0
#define DISPLAY_MOVE1 1





__CONFIG_H_EXT__ u8 data_time;  // Э�����
__CONFIG_H_EXT__ u8 data_flag;

__CONFIG_H_EXT__ u8 text_count; //�ı�������


#define CMD_NOTIFY 	0
#define CMD_TEXT  	1
#define CMD_FINISH 	2
#define CMD_CONTROL 3
__CONFIG_H_EXT__ u8 cmd_data;

#define NOTIFY  0xFC
#define TEXT    15
#define FINISH  0xFF
#define CONTROL 0xFA

#define RECVED 0xFE // ���ݴ������������
#define REQUIRE 0xFD // ���ݲ���������
__CONFIG_H_EXT__ u8 if_display_none; // �Ƿ�����
__CONFIG_H_EXT__ u8 update_agreement;

__CONFIG_H_EXT__ u8 display_done; // 0 ��ʾ�в�������1 ��ʾ��һ����Ϣ
__CONFIG_H_EXT__ u8 display_done_p; // �Ѿ�������ʾ���

__CONFIG_H_EXT__ u8 display_list[4];
__CONFIG_H_EXT__ u8 display_list_index;


__CONFIG_H_EXT__ u8  currnet_index;// ��ʾ�ĸ�
__CONFIG_H_EXT__ u8  next_index;// ��һ����ʾ�ĸ�
__CONFIG_H_EXT__ u8  updata_index;//�����ı��������
__CONFIG_H_EXT__ u8  updata_flag;// �����ı���־

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
head��Ŀ��ʼͷ�� 01
tpye��Ŀ���ͣ�00 �ı� 01 ʱ��  02 ʱ�� 03 ����
donghua������ʽ��00 ֱ����ʾ   01 ����   02 ����   03 ����   04 ����   05 Ʈѩ 06 ������
speed�����ٶȣ�0~100
timeͣ��ʱ�䣺0~100
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
__CONFIG_H_EXT__ u8 text_index;//��ʾ���
__CONFIG_H_EXT__ u8 text_flag;//��Ч�ı�����ʾ
__CONFIG_H_EXT__ u8 sys_move;//ϵͳ���ƶ�
__CONFIG_H_EXT__ u16 speed_num; //�ٶȼ���


typedef struct {
	u8 flag;
	u8 id;
	u8 display_time;//���� 1~15
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

__CONFIG_H_EXT__ CurrentData diplay_data;//��ǰ��ʾ�ı�


/*
typedef struct {
	u8 head; // 0 ����ʾ
	u8 tpye;    // ��Ŀ���ͣ�00 �ı� 01 ʱ��  02 ʱ�� 03 ����
	u8 donghua; // ������ʽ
	u8 speed;    // �����ٶ�
	u8 time; // ͣ��ʱ��
	u8 text[256];
	u16 text_length;
	GpsData gpsdate;
}TextData;

typedef struct {
	u8 flag;    // �ı�����
	u8 number;  // �ܹ���������Ϣ
	u8 id;      // ��ǰ��ʾ��Ϣ
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

