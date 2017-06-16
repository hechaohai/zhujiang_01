#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "config.h"




void ReturnByte(uint8_t byte);
void UART_Rceive_RXNE(void);			//接收一个字节
void DMA_Rceive_RXNE(void);
void UART_Receive_ZIKU(void);			//字库 
#endif
