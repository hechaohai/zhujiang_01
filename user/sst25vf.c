/*
 ******************************************************************************
 * file       sst25vf.c
 * author     TianRongGang 
 * version    V1.00    
 ******************************************************************************
 *
 ******************************************************************************
 */
 

#include "sst25vf.h"
 
/*
 *
 */
void Init_SST25VF(void)
{
	EWSR();
	WRSR();
}

/*
 *
 */
void Send_Byte(uint8_t out)
{
	while(!(SPI1->SR & 2));                     // 等待发送缓冲为空
	SPI1->DR = out;                             // 写入数据
	while(!(SPI1->SR & 1));                     // 等待传输结束
	
	if(SPI1->DR);                               // 防止溢出 
}

/*
 *
 */
uint8_t Get_Byte(void)
{
	while(!(SPI1->SR & 2));                     // 等待发送缓冲为空
	SPI1->DR = 0;                               // 写入数据
	while(!(SPI1->SR & 1));                     // 等待传输结束

	return SPI1->DR;                            // 返回数据
}

/*
 *
 */
uint8_t Read_Status_Register(void)
{
	uint8_t byte;
	
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x05);						/* send RDSR command */
	byte = Get_Byte();						/* receive byte */
	SetNSS();          						/* disable device */
	
	return byte;
}

/*
 *
 */
uint8_t Read_ID(void)
{
	uint8_t byte;
	
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x90);						/* send read ID command (90h or ABh) */
   	Send_Byte(0x00);						/* send address */
	Send_Byte(0x00);						/* send address */
	Send_Byte(0x00);						/* send address - either 00H or 01H */
	byte = Get_Byte();						/* receive byte */
	SetNSS();          						/* disable device */
	
	return byte;
}

/*
 *
 */
void EWSR(void)
{
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x50);						/* enable writing to the status register */
	SetNSS();          						/* disable device */
}

/*
 *
 */
void WRSR(void)
{
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x01);						/* select write to status register */
	Send_Byte(0x00);						/* data that will change the status of BPx 
					   						   or BPL (only bits 2,3,4,5,7 can be written) */
	SetNSS();          						/* disable device */
}

/*
 *
 */
void WREN(void)
{
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x06);						/* send WREN command */
	SetNSS();          						/* disable device */
}

/*
 *
 */
void WRDI(void)
{
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x04);						/* send WRDI command */
	SetNSS();          						/* disable device */
}

/*
 *
 */
void Read_Data(uint32_t address)
{
	Send_Byte(0x03); 		                /* read command */
	Send_Byte(((address & 0xFFFFFF) >> 16));/* send 3 address bytes */
	Send_Byte(((address & 0xFFFF) >> 8));
	Send_Byte(address & 0xFF);
}

/*
 *
 */
uint8_t Read_Byte(uint32_t address)
{
	uint8_t byte;
	
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x03); 		                /* read command */
	Send_Byte(((address & 0xFFFFFF) >> 16));/* send 3 address bytes */
	Send_Byte(((address & 0xFFFF) >> 8));
	Send_Byte(address & 0xFF);
	byte = Get_Byte();
	SetNSS();          						/* disable device */
	
    return byte;
}

/*
 *
 */
void Byte_Program(uint8_t byte,uint32_t address)
{
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x02); 						/* send Byte Program command */
	Send_Byte(((address & 0xFFFFFF) >> 16));/* send 3 address bytes */
	Send_Byte(((address & 0xFFFF) >> 8));
	Send_Byte(address & 0xFF);
	Send_Byte(byte);						/* send byte to be programmed */
	SetNSS();          						/* disable device */
}

/*
 *
 */
void Chip_Erase(void)
{
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x60);						/* send Chip Erase command (60h or C7h) */
	SetNSS();          						/* disable device */
}

/*
 *
 */
void Sector_Erase(uint32_t address)
{
	ClrNSS(); 						        /* enable device */
	Send_Byte(0x20);						/* send Sector Erase command */
	Send_Byte(((address & 0xFFFFFF) >> 16));/* send 3 address bytes */
	Send_Byte(((address & 0xFFFF) >> 8));
	Send_Byte(address & 0xFF);
	SetNSS();          						/* disable device */
}

/*
 *
 */
void Block_Erase_32K(uint32_t address)
{
	ClrNSS(); 	    	    		        /* enable device */
	Send_Byte(0x52);						/* send 32 KByte Block Erase command */
	Send_Byte(((address & 0xFFFFFF) >> 16));/* send 3 address bytes */
	Send_Byte(((address & 0xFFFF) >> 8));
	Send_Byte(address & 0xFF);
	SetNSS();          						/* disable device */
}

/*
 *
 */
void Block_Erase_64K(uint32_t address)
{
	ClrNSS(); 	    	    		        /* enable device */
	Send_Byte(0xD8);    	    	    	/* send 64KByte Block Erase command */
	Send_Byte(((address & 0xFFFFFF) >> 16));/* send 3 address bytes */
	Send_Byte(((address & 0xFFFF) >> 8));
	Send_Byte(address & 0xFF);
	SetNSS();          						/* disable device */
}

/*
 *
 */
void Wait_Busy(void)
{
	while (Read_Status_Register() == 0x03);	/* waste time until not busy */
}
