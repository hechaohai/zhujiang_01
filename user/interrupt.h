#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "config.h"




void ReturnByte(uint8_t byte);
void UART_Rceive_RXNE(void);			//����һ���ֽ�
void DMA_Rceive_RXNE(void);
void UART_Receive_ZIKU(void);			//�ֿ� 
#endif
