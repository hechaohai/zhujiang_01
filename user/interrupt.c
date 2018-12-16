#include "interrupt.h"
//#include "ds1302.h"
#include "crc.h"
#include "sst25vf.h"
//#include "can.h"

extern void MoveWordLine1(void);
extern void MoveWordLine2(void);
extern void MoveWordLine3(void);


/********************************************************************************
 *							串口3中断服务程序									*
 ********************************************************************************
*/
void USART3_IRQHandler(void)
{
	uint8_t  ReadUsartData;
	if(USART3->SR & (1 << 5))
	{
		ReadUsartData = USART3->DR;
		

		if((0 == RxCounter_Gps && '$' == ReadUsartData) || \
			(1 == RxCounter_Gps && 'G' == ReadUsartData) || \
			(2 == RxCounter_Gps && 'N' == ReadUsartData) || \
			(3 == RxCounter_Gps && 'R' == ReadUsartData) || \
			(4 == RxCounter_Gps && 'M' == ReadUsartData) || \
			(5 == RxCounter_Gps && 'C' == ReadUsartData)) {
			RxBuffer_Gps[RxCounter_Gps++] = ReadUsartData;
		} 
		else if(RxCounter_Gps > 5 && RxCounter_Gps < 90) {
			RxBuffer_Gps[RxCounter_Gps++] = ReadUsartData;
			if(0x0d == ReadUsartData){
				Updata_GPS = 1;
				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
			}
		}
		else {
			RxCounter_Gps = 0;
		}

	}
}

/********************************************************************************
 *							定时器4中断服务程序									*
 ********************************************************************************
*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);		//清除TIMx更新中断标志
		

			TIM_Cmd(TIM4, DISABLE);

	}
}

void SysTick_Handler(void)
{
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);               // 关闭DMA1时钟
// 	DMA1_Channel5->CCR   &= ~1;
// 	EXTI->PR  = 0x000FFFFFu;                                         // 清除中断标志
//	EXTI->EMR = 0;                               // 关闭事件
//	EXTI->IMR = 0;                               // 关闭中断
// 	SysTick->CTRL = 0;
// 	TIM3->CR1  &= ~(1u << 3);                                         // 设置工作模式
	SysTick->CTRL = 0;
}

/********************************************************************************
 *							定时器3中断服务程序		时间 Beep					*
 ********************************************************************************
*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//清除TIMx更新中断标志
		TIM3->SR = (uint16_t)~TIM_IT_Update;				// 1
		//TIM_Cmd(TIM3, DISABLE);
		TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));	// 1
		//OffDisplay;

	}
}

/********************************************************************************
 *							定时器2中断 用于显示	    							*
 ********************************************************************************
*/
void TIM2_IRQHandler(void)
{
	uint16_t i, z;
	uint8_t  j,date;
	uint8_t  *p;
//	uint8_t date_temp;
	uint16_t x,y,m1,m2,n1,n2,tt;
	u8 temp[8];
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//清除TIMx更新中断标志

		//
		if(DisplayIndex) p = DisplayBuf1;
		else             p = DisplayBuf0;
		GPIOC->BRR = GPIO_Pin_5;//GPIO_ResetBits(GPIOC,GPIO_Pin_5); //595锁存
		//
		
			//GPIOB->BSRR = GPIO_Pin_15;
			//GPIOB->BRR = GPIO_Pin_13;
			GPIOB->BRR = GPIO_Pin_14;
			GPIOB->BRR = GPIO_Pin_10;
			GPIOB->BRR = GPIO_Pin_11;
		#if 1
			for(x = LineIndex; x < 192; x += 32)
			{
				m1 = x * 8 + 32;
				m2 = m1 + 128;
        n1 = m1 + 64;
				n2 = n1 + 128;
				for(z = 0; z < 8; z++) {
					GPIOC->BRR = GPIO_Pin_4;//GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					if(*(p + m1  + z) ) {
						GPIOB->BSRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					if(*(p + n1  + z) ) {
						GPIOB->BSRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
					GPIOC->BSRR = GPIO_Pin_4;
				}
				for(z = 0; z < 8; z++) {
					GPIOC->BRR = GPIO_Pin_4;//GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					if(*(p + m2  + z) ) {
						GPIOB->BSRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					if(*(p + n2  + z) ) {
						GPIOB->BSRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
					GPIOC->BSRR = GPIO_Pin_4;
				}
				//m1+=32;
				//n1+=32;
				//m2+=32;
				//n2+=32;
				m1-=32;
				n1-=32;
				m2-=32;
				n2-=32;
				for(z = 0; z < 8; z++) {
					GPIOC->BRR = GPIO_Pin_4;//GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					if(*(p + m1  + z) ) {
						GPIOB->BSRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					if(*(p + n1  + z) ) {
						GPIOB->BSRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
					GPIOC->BSRR = GPIO_Pin_4;
				}
				for(z = 0; z < 8; z++) {
					GPIOC->BRR = GPIO_Pin_4;//GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					if(*(p + m2  + z) ) {
						GPIOB->BSRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					if(*(p + n2  + z) ) {
						GPIOB->BSRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					else {
						GPIOB->BRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					}
					//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
					GPIOC->BSRR = GPIO_Pin_4;
				}
				
				
			}
			#endif
		
		nop();nop();nop();nop();nop();nop();nop();
		nop();nop();nop();nop();nop();nop();nop();
		
		#if 1
		OffDisplay;
		#endif
		
		
		nop();nop();nop();nop();nop();nop();nop();
		GPIOC->BSRR = GPIO_Pin_5; //GPIO_SetBits(GPIOC,GPIO_Pin_5);//	//595锁存	
		
		nop();nop();nop();nop();nop();nop();nop();
		#if 1
		if(LineIndex == 0x00) GPIOC->BRR = GPIO_Pin_6;//GPIO_SetBits(GPIOC,GPIO_Pin_6);			//行选择
		else                 GPIOC->BSRR = GPIO_Pin_6;//GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		
		if(LineIndex == 0x01) GPIOC->BRR = GPIO_Pin_7; // GPIO_SetBits(GPIOC,GPIO_Pin_7);
		else                 GPIOC->BSRR = GPIO_Pin_7; // GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		
		if(LineIndex == 0x02) GPIOC->BRR = GPIO_Pin_9;// GPIO_SetBits(GPIOC,GPIO_Pin_9);
		else                 GPIOC->BSRR = GPIO_Pin_9;// GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		
		if(LineIndex == 0x03) GPIOC->BRR = GPIO_Pin_8;
		else                 GPIOC->BSRR = GPIO_Pin_8;
		#endif
		

		//
		if(++LineIndex >= SaoMiao)
		{
			
			
			LineIndex = 0;
			
			//if(++LED_Index >= 30){Bemove = 1; LED_Index = 0;}
			LED_Index++;
			if(LED_Index > speed_num){
				LED_Index = 0;
				Bemove = 1;
			}
			
			if(!LED_Index)
				LEDON();
			else if(8 == LED_Index)
				LEDOFF();

		}
		if(++time_upindex > 1999){
			time_upindex = 0;
			time_upsec++;
		}
		#if 1
		#endif
		
		
	}
}
//
/********************************************************************************
 *								串口中断											*
 ********************************************************************************
*/
void USART1_IRQHandler(void)
{
	#if 1
	uint8_t temp;
	
	if(USART1->SR & (1 << 5))									//接收到数据
	{
		
		temp = USART1->DR;				//对USART_DR的读操作可以将USART1->SR&(1<<5)位清零

	}
	#endif
}
//修改字库
void UART_Receive_ZIKU(void)
{

}

//接收一个字节
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
 *									返回码										*
 ********************************************************************************
*/
void ReturnByte(uint8_t byte)
{
	//RS485_T();
	USART_SendData(USART1, byte);
	//while(USART_GetFlagStatus(USART1, USART_IT_TXE)==RESET);//等待发完
	while((USART1->SR & 0x40) == 0);
	//RS485_R();
}





