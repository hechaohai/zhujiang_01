/*
 ******************************************************************************
 * file       ds1302.c
 * author     TianRongGang 
 * version    V1.00    
 ******************************************************************************
 */

#define  __DS1302_H_INC__

#include "ds1302.h"

/*
 *
 */
void Init_DS1302(void)
{
    uint8_t dat;
    
	Set_DS1302_CE();
	Set_DS1302_CK();
	GPIOB->CRL &= 0XF00FFFFF;
	GPIOB->CRL |= 0X03300000;	
	IN_DS1302_IO();
	
    Clr_DS1302_CE();
    Clr_DS1302_CK();
    DS1302_Delay();
    
    // 解除写保护 //
    Set_DS1302_CE();
    DS1302_Write_Byte(0x8E);
    DS1302_Write_Byte(0x00);
    Clr_DS1302_CE();
    DS1302_Delay();
    
    // 使能计时 //
    Set_DS1302_CE();
    DS1302_Write_Byte(0x81);
    dat = DS1302_Read_Byte();
    Clr_DS1302_CE();
    DS1302_Delay();
    //
    if(dat & 0x80)
    {
        Set_DS1302_CE();
        DS1302_Write_Byte(0x80);
        DS1302_Write_Byte(0x00);
        Clr_DS1302_CE();
        DS1302_Delay();
    }
    
    // 24小时模式 //
    Set_DS1302_CE();
    DS1302_Write_Byte(0x85);
    dat = DS1302_Read_Byte();
    Clr_DS1302_CE();
    DS1302_Delay();
    //
    if(dat & 0x80)
    {
        Set_DS1302_CE();
        DS1302_Write_Byte(0x84);
        DS1302_Write_Byte(0x00);
        Clr_DS1302_CE();
        DS1302_Delay();
    }
        
    // 充电(2 Diodes + 8k) //
    Set_DS1302_CE();
    DS1302_Write_Byte(0x90);
    DS1302_Write_Byte(0xAB);
    Clr_DS1302_CE();
    DS1302_Delay();
}

/*
 *
 */
void Read_Time(void)
{
	DS1302.Busy = 1;
	//
    Set_DS1302_CE();
    DS1302_Write_Byte(0xBF);
    DS1302.Second = DS1302_Read_Byte();
    DS1302.Minute = DS1302_Read_Byte();
    DS1302.Hour   = DS1302_Read_Byte();
    DS1302.Date   = DS1302_Read_Byte();
    DS1302.Month  = DS1302_Read_Byte();
    DS1302.Day    = DS1302_Read_Byte();
    DS1302.Year   = DS1302_Read_Byte();
    Clr_DS1302_CE();
    //
    DS1302_Delay();
	//
	DS1302.Busy = 0;
}

/*
 *
 */
void Write_Time(void)
{
    DS1302.Busy = 1;
	//
    Set_DS1302_CE();
    DS1302_Write_Byte(0xBE);
    DS1302_Write_Byte(DS1302.Second & 0x7F);
    DS1302_Write_Byte(DS1302.Minute);
    DS1302_Write_Byte(DS1302.Hour & 0x7F);
    DS1302_Write_Byte(DS1302.Date);
    DS1302_Write_Byte(DS1302.Month);
    DS1302_Write_Byte(DS1302.Day);
    DS1302_Write_Byte(DS1302.Year);
    DS1302_Write_Byte(0x00);
    Clr_DS1302_CE();
    //
    DS1302_Delay();
	//
	DS1302.Busy = 0;
}

/*
 *
 */
uint8_t DS1302_Read_Byte(void)
{
    uint8_t i, dat;

    for(i = 0; i < 8; i++)
    {
        dat >>= 1;
        if(State_DS1302_IO())
            dat |= 0x80;
        else
            dat &= 0x7F;
        //
        Set_DS1302_CK();
        DS1302_Delay();
        Clr_DS1302_CK();
        DS1302_Delay();
    }
    
    return (dat & 0xFF);
}

/*
 *
 */
void  DS1302_Write_Byte(uint8_t dat)
{
    uint8_t i;
    
    OUT_DS1302_IO();
    //
    for(i = 0; i < 8; i++)
    {
        if(dat & 1)
            Set_DS1302_IO();
        else
            Clr_DS1302_IO();
        dat >>= 1;
        //
        DS1302_Delay();
        Set_DS1302_CK();
        DS1302_Delay();
        if(i == 7)
            IN_DS1302_IO();
        Clr_DS1302_CK();
    }
    //
    DS1302_Delay();
}

/*
 *
 */
void  DS1302_Delay(void)
{
    __nop(); __nop(); __nop(); __nop(); 
    __nop(); __nop(); __nop(); __nop(); 
    __nop(); __nop(); __nop(); __nop(); 
    __nop(); __nop(); __nop(); __nop();  
} 
