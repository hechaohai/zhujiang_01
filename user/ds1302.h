/*
 ******************************************************************************
 * file       ds1302.h
 * author     TianRongGang 
 * version    V1.00    
 ******************************************************************************
 */

#ifndef __DS1302_H__
#define __DS1302_H__

#include "stm32f10x.h"

#ifndef __DS1302_H_INC__
    #define __DS1302_H_EXT__ extern
#else
    #define __DS1302_H_EXT__
#endif

typedef struct
{
	uint8_t Busy;
    uint8_t Update;
    uint8_t Year;
    uint8_t Month;
    uint8_t Date;
    uint8_t Day;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
}RTCStruct;

// GPIO工作模式配置宏 //
#define ConfigGPIO(x,y,z)    x = (x & (~(15u << ((y & 7) * 4)))) | \
                                  (z << ((y & 7) * 4))

// 端口 //
#define Clr_DS1302_CE()      GPIOB->BRR  = 1u << 5
#define Set_DS1302_CE()      GPIOB->BSRR = 1u << 5
#define Clr_DS1302_CK()      GPIOB->BRR  = 1u << 6
#define Set_DS1302_CK()      GPIOB->BSRR = 1u << 6
#define Clr_DS1302_IO()      GPIOB->BRR  = 1u << 7
#define Set_DS1302_IO()      GPIOB->BSRR = 1u << 7
//
#define State_DS1302_IO()    (GPIOB->IDR & (1u << 7))
//
#define OUT_DS1302_IO()      ConfigGPIO(GPIOB->CRL,  7, 0x03u)
#define IN_DS1302_IO()       ConfigGPIO(GPIOB->CRL,  7, 0x08u)

// 变量 //
__DS1302_H_EXT__ RTCStruct DS1302;

// 函数 //
__DS1302_H_EXT__ void Init_DS1302(void);
__DS1302_H_EXT__ void Read_Time(void);
__DS1302_H_EXT__ void Write_Time(void);
//
__DS1302_H_EXT__ uint8_t DS1302_Read_Byte(void);
__DS1302_H_EXT__ void  DS1302_Write_Byte(uint8_t dat);
__DS1302_H_EXT__ void  DS1302_Delay(void);



#endif /* __DS1302_H__ */
