/*
 ******************************************************************************
 * file       sst25vf.h
 * author     TianRongGang 
 * version    V1.00    
 ******************************************************************************
 *
 ******************************************************************************
 */
#ifndef __SST25VF_H__
#define __SST25VF_H__

#include "stm32f10x.h" 

// ¶Ë¿Ú // 
#define ClrNSS()             GPIOA->BRR  = 1u << 4
#define SetNSS()             GPIOA->BSRR = 1u << 4

// º¯Êý //
void Init_SST25VF(void);
void Send_Byte(uint8_t out);
void Read_Data(uint32_t address);
uint8_t Get_Byte(void);
uint8_t Read_Status_Register(void);
uint8_t Read_ID(void);
void EWSR(void);
void WRSR(void);
void WREN(void);
void WRDI(void);
void Byte_Program(uint8_t byte,uint32_t address);
void Chip_Erase(void);
void Sector_Erase(uint32_t address);
void Block_Erase_32K(uint32_t address);
void Block_Erase_64K(uint32_t address);
void Wait_Busy(void);
uint8_t Read_Byte(uint32_t address);
 
#endif  /* __SST25VF_H__ */ 
