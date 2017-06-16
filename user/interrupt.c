#include "interrupt.h"
//#include "ds1302.h"
#include "crc.h"
#include "sst25vf.h"

extern void MoveWordLine1(void);
extern void MoveWordLine2(void);
extern void MoveWordLine3(void);

/********************************************************************************
 *							��ʱ��4�жϷ������									*
 ********************************************************************************
*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);		//���TIMx�����жϱ�־
		

			TIM_Cmd(TIM4, DISABLE);

	}
}

void SysTick_Handler(void)
{
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);               // �ر�DMA1ʱ��
// 	DMA1_Channel5->CCR   &= ~1;
// 	EXTI->PR  = 0x000FFFFFu;                                         // ����жϱ�־
//	EXTI->EMR = 0;                               // �ر��¼�
//	EXTI->IMR = 0;                               // �ر��ж�
// 	SysTick->CTRL = 0;
// 	TIM3->CR1  &= ~(1u << 3);                                         // ���ù���ģʽ
	SysTick->CTRL = 0;
}

/********************************************************************************
 *							��ʱ��3�жϷ������		ʱ�� Beep					*
 ********************************************************************************
*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//���TIMx�����жϱ�־
		TIM3->SR = (uint16_t)~TIM_IT_Update;				// 1
		//TIM_Cmd(TIM3, DISABLE);
		TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));	// 1
		OffDisplay();

	}
}

/********************************************************************************
 *							��ʱ��2�ж� ������ʾ	    							*
 ********************************************************************************
*/
void TIM2_IRQHandler(void)
{
	uint16_t i,z;
	uint16_t  j;
	uint8_t  *p;
	uint16_t x,y;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//���TIMx�����жϱ�־
		//
		if(DisplayIndex) p = &DisplayBuf1[0][0];
		else             p = &DisplayBuf0[0][0];
		GPIO_ResetBits(GPIOC,GPIO_Pin_5); //GPIOA->BRR = GPIO_Pin_11;						//595����
		//
		#if 1
		for(j = 0; j < 3; j++)
		{
			x = (uint16_t)(LineIndex + j * 32) * MaxLine;
			for(y = 0; y < MaxLine; y += 8)
			{
				for(z = 0; z < 8; z++)
				{
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					/*
					switch(*(p + x + y + z))
					{
						case 1: i = 2; break;
						case 2: i = 1; break;
						case 3: i = 0; break;
						default:i = 3; break;
					}
					switch(*(p + x + y + z + MaxLine * 16))
					{
						case 1: i |= 8; break;
						case 2: i |= 4; break;
						case 3: i |= 0; break;
						default:i |= 0x0C; break;
					}
					GPIOB->BSRR = ((i & 0x0F) << 8) | ((~i & 0x0F) << 24);
					*/
					GPIO_ResetBits(GPIOB,GPIO_Pin_10); // red
					GPIO_SetBits(GPIOB,GPIO_Pin_11); // greed
					GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					
					GPIO_SetBits(GPIOB,GPIO_Pin_13);
					GPIO_ResetBits(GPIOB,GPIO_Pin_14);
					GPIO_SetBits(GPIOB,GPIO_Pin_15);
					
					GPIO_SetBits(GPIOC,GPIO_Pin_4);
				}
			}
		}
		//
		#endif
		#if 0
		j = 0;
		for(i = LineIndex; i < 96*16; i += SaoMiao)
		{
			for(x = 0x80; x != 0; x >>=1)
			{
				z = (j & x) ? 0xF8 : 0xFF;
				//z = 0xFE;
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				WriteDisplayData(z);
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
			}
			j++;
		}
		#endif
		//
		
		OffDisplay();													// ����ʾ	 �͵�ƽ
		
		nop();nop();nop();nop();nop();nop();nop();
		
		
		if(LineIndex & 0x01) GPIO_SetBits(GPIOC,GPIO_Pin_6);			//��ѡ��
		else                 GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		
		if(LineIndex & 0x02) GPIO_SetBits(GPIOC,GPIO_Pin_7);
		else                 GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		
		if(LineIndex & 0x04) GPIO_SetBits(GPIOC,GPIO_Pin_9);
		else                 GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		
		
		
		
		GPIO_SetBits(GPIOC,GPIO_Pin_5);//GPIOA->BSRR  = GPIO_Pin_11;	//595����	
		
		
		nop();nop();nop();nop();nop();nop();nop();
		#if 1
		if(15 != Light)
		{
			TIM3->ARR  = SysDisplaySpeed * 10 * Light / 15;
 			TIM3->EGR |= TIM_EGR_UG;		// 1
 			TIM3->CR1 |= TIM_CR1_CEN;		// 1
		}
		#endif
		OnDisplay();													//����ʾ
		//
		if(++LineIndex >= SaoMiao)
		{
			LineIndex = 0;
			
			if(++LED_Index >= 30) LED_Index = 0;
			
			switch(LED_Index)
			{
				//case  0: LEDON();break;
				//case 15: LEDOFF();break;
				default: break;
			}
		}
	}
}
//
/********************************************************************************
 *								�����ж�											*
 ********************************************************************************
*/
void USART1_IRQHandler(void)
{
	#if 1
	uint8_t temp;
	
	if(USART1->SR & (1 << 5))									//���յ�����
	{
		
		temp = USART1->DR;				//��USART_DR�Ķ��������Խ�USART1->SR&(1<<5)λ����

	}
	#endif
}
//�޸��ֿ�
void UART_Receive_ZIKU(void)
{

}

//����һ���ֽ�
void UART_Rceive_RXNE(void)
{
	uint32_t i;
	
	for(i = 0; i < 0x01000000; i++)
	{
		if(USART1->SR & ( 1 << 5))
		{
			ReceiveUSART1Data = USART1->DR;
			
			ReceiveUSART1True = TURE;
			return;
		}
	}
	ReceiveUSART1True = FALSE;
}
//

//

//

/********************************************************************************
 *									������										*
 ********************************************************************************
*/
void ReturnByte(uint8_t byte)
{
	//RS485_T();
	USART_SendData(USART1, byte);
	//while(USART_GetFlagStatus(USART1, USART_IT_TXE)==RESET);//�ȴ�����
	while((USART1->SR & 0x40) == 0);
	//RS485_R();
}





