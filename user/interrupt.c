#include "interrupt.h"
//#include "ds1302.h"
#include "crc.h"
#include "sst25vf.h"
#include "can.h"

extern void MoveWordLine1(void);
extern void MoveWordLine2(void);
extern void MoveWordLine3(void);

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
		OffDisplay();

	}
}

/********************************************************************************
 *							定时器2中断 用于显示	    							*
 ********************************************************************************
*/
void TIM2_IRQHandler(void)
{
	uint16_t i,z;
	uint16_t  j;
	uint8_t  *p;
	uint8_t date_temp;
	uint16_t x,y,tt;
	u8 temp[8];
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//清除TIMx更新中断标志
		
		//if(!diplay_data.display_time)
		//{
		//	OffDisplay();
		//}
		
		
		if(++timeout_index >= 20000)
		{
			timeout_index = 0;
			if (++timeout_index_p >= 3) {
				timeout_index_p = 0;

				//更新文本
				//updata_flag = 2;
				
				// 之前没有警告
				if(timeout_doing_color != Red)
				{
					timeout_flag = 1;
					timeout_doing_color = Red;
				}
			}
		}
		//
		if(DisplayIndex) p = DisplayBuf1;
		else             p = DisplayBuf0;
		GPIOC->BRR = GPIO_Pin_5;//GPIO_ResetBits(GPIOC,GPIO_Pin_5); //595锁存
		//
		
		//x = (u16)LineIndex * MaxLine;
		//for(y = 0; y < MaxLine; y += 16)
		//for(x = (u16)LineIndex * 8; x < DisplayBufMaxLength; x += 128)
		#if 0
		for(x = LineIndex ; x < 32; x += 16)
		//x = LineIndex;
		{
			tt = x * 8;
			for(z = 0; z < 8; z++) {
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				//date_temp  = *(p +x);
				//date_temp |= *(p +x + 512) << 4;

				switch (*(p + tt  + z) ) {
					//ReturnByte(*(p + x  + z));
					case 1: 	

							GPIO_SetBits(GPIOB,GPIO_Pin_10);
							GPIO_ResetBits(GPIOB,GPIO_Pin_11);
							
							break;
					case 2: 	GPIO_SetBits(GPIOB,GPIO_Pin_10);
							GPIO_ResetBits(GPIOB,GPIO_Pin_11);
							break;
					case 3: 	GPIO_SetBits(GPIOB,GPIO_Pin_10);
							GPIO_ResetBits(GPIOB,GPIO_Pin_11);
							break;
					default: 	GPIO_ResetBits(GPIOB,GPIO_Pin_10); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_11); 
							break;
				}
				switch (*(p + tt  + z + 64)  ) {
					
					case 1:
						
							GPIO_SetBits(GPIOB,GPIO_Pin_13);
							GPIO_ResetBits(GPIOB,GPIO_Pin_14);
							
							break;
					case 2: 	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
							GPIO_SetBits(GPIOB,GPIO_Pin_14);
							break;
					case 3: 	GPIO_SetBits(GPIOB,GPIO_Pin_13);
							GPIO_SetBits(GPIOB,GPIO_Pin_14);
							break;

					default: 	GPIO_ResetBits(GPIOB,GPIO_Pin_13); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_14); 
							break;
				}
				GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
				GPIO_ResetBits(GPIOB,GPIO_Pin_15); // blue

				//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
			}
		}
		
				for(x = LineIndex +32; x < 64; x += 16)
		//x = LineIndex + 16;
		{
			tt = x * 8;
			for(z = 0; z < 8; z++) {
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);
				//date_temp  = *(p +x);
				//date_temp |= *(p +x + 512) << 4;

				switch (*(p + tt  + z) ) {
					//ReturnByte(*(p + x  + z));
					case 1: 	

							GPIO_ResetBits(GPIOB,GPIO_Pin_10);
							GPIO_SetBits(GPIOB,GPIO_Pin_11);
							
							break;
					case 2: 	GPIO_SetBits(GPIOB,GPIO_Pin_10);
							GPIO_ResetBits(GPIOB,GPIO_Pin_11);
							break;
					case 3: 	GPIO_SetBits(GPIOB,GPIO_Pin_10);
							GPIO_ResetBits(GPIOB,GPIO_Pin_11);
							break;
					default: 	GPIO_ResetBits(GPIOB,GPIO_Pin_10); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_11); 
							break;
				}
				switch (*(p + tt  + z + 64)  ) {
					
					case 1:
						
							GPIO_ResetBits(GPIOB,GPIO_Pin_13);
							GPIO_SetBits(GPIOB,GPIO_Pin_14);
							
							break;
					case 2: 	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
							GPIO_SetBits(GPIOB,GPIO_Pin_14);
							break;
					case 3: 	GPIO_SetBits(GPIOB,GPIO_Pin_13);
							GPIO_SetBits(GPIOB,GPIO_Pin_14);
							break;

					default: 	GPIO_ResetBits(GPIOB,GPIO_Pin_13); 
							GPIO_ResetBits(GPIOB,GPIO_Pin_14); 
							break;
				}
				GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
				GPIO_ResetBits(GPIOB,GPIO_Pin_15); // blue

				//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
				GPIO_SetBits(GPIOC,GPIO_Pin_4);
			}
		}
		#endif
		// 
		if (Red == diplay_data.color)
			display_color = Red;
		else if (Green== diplay_data.color)
			display_color = Green;
		else
			display_color = Yellow;
		i = 0;
		if(display_color == Yellow) {
			for(x = LineIndex; x < ScreenLength * 16; x += 16)
			{
				//if(++i >= diplay_data.display_time)
				//	OffDisplay();
				tt = x * 8;
				for(z = 0; z < 8; z++) {
					GPIOC->BRR = GPIO_Pin_4;//GPIO_ResetBits(GPIOC,GPIO_Pin_4);

					if(*(p + tt  + z) ) {
						GPIOB->BSRR = GPIO_Pin_10;//GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIOB->BSRR = GPIO_Pin_11;//GPIO_SetBits(GPIOB,GPIO_Pin_11);
					}
					else {
						GPIOB->BRR = GPIO_Pin_10;//GPIO_ResetBits(GPIOB,GPIO_Pin_10); 
						GPIOB->BRR = GPIO_Pin_11;//GPIO_ResetBits(GPIOB,GPIO_Pin_11); 
					}
					if(*(p + tt  + z + 64)  ) {
						GPIOB->BSRR = GPIO_Pin_13;//GPIO_SetBits(GPIOB,GPIO_Pin_13);
						GPIOB->BSRR = GPIO_Pin_14;//GPIO_SetBits(GPIOB,GPIO_Pin_14);
					}			
					else {
						GPIOB->BRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_13); 
						GPIOB->BRR = GPIO_Pin_14;//GPIO_ResetBits(GPIOB,GPIO_Pin_14); 
					}
					GPIOB->BRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					GPIOB->BRR = GPIO_Pin_15;//GPIO_ResetBits(GPIOB,GPIO_Pin_15); // blue

					//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
					GPIOC->BSRR = GPIO_Pin_4;//GPIO_SetBits(GPIOC,GPIO_Pin_4);
				}
				
			}
		}
		//
		else if(display_color == Red) {
			for(x = LineIndex; x < ScreenLength * 16; x += 16)
			{
				//if(++i >= diplay_data.display_time)
				//	OffDisplay();
				tt = x * 8;
				for(z = 0; z < 8; z++) {
					GPIOC->BRR = GPIO_Pin_4;//GPIO_ResetBits(GPIOC,GPIO_Pin_4);

					if(*(p + tt  + z) ) {
						GPIOB->BSRR = GPIO_Pin_10;//GPIO_SetBits(GPIOB,GPIO_Pin_10);
					}
					else {
						GPIOB->BRR = GPIO_Pin_10;//GPIO_ResetBits(GPIOB,GPIO_Pin_10); 
					}
					if(*(p + tt  + z + 64)  ) {
						GPIOB->BSRR = GPIO_Pin_13;//GPIO_SetBits(GPIOB,GPIO_Pin_13);
					}			
					else {
						GPIOB->BRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_13); 
					}
					GPIOB->BRR = GPIO_Pin_11;//GPIO_ResetBits(GPIOB,GPIO_Pin_11); 
					GPIOB->BRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					GPIOB->BRR = GPIO_Pin_14;//GPIO_ResetBits(GPIOB,GPIO_Pin_14); 
					GPIOB->BRR = GPIO_Pin_15;//GPIO_ResetBits(GPIOB,GPIO_Pin_15); // blue

					//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
					GPIOC->BSRR = GPIO_Pin_4;//GPIO_SetBits(GPIOC,GPIO_Pin_4);
				}
			}
		}
		//
		else{
			for(x = LineIndex; x < ScreenLength * 16; x += 16)
			{
				//if(++i >= diplay_data.display_time)
				//	OffDisplay();
				tt = x * 8;
				for(z = 0; z < 8; z++) {
					GPIOC->BRR = GPIO_Pin_4;//GPIO_ResetBits(GPIOC,GPIO_Pin_4);

					if(*(p + tt  + z) ) {
						GPIOB->BSRR = GPIO_Pin_11;//GPIO_SetBits(GPIOB,GPIO_Pin_11);
					}
					else {
						GPIOB->BRR = GPIO_Pin_11;//GPIO_ResetBits(GPIOB,GPIO_Pin_11); 
					}
					if(*(p + tt  + z + 64)  ) {
						GPIOB->BSRR = GPIO_Pin_14;//GPIO_SetBits(GPIOB,GPIO_Pin_14);
					}			
					else {
						GPIOB->BRR = GPIO_Pin_14;//GPIO_ResetBits(GPIOB,GPIO_Pin_14); 
					}
					GPIOB->BRR = GPIO_Pin_10;//GPIO_ResetBits(GPIOB,GPIO_Pin_10); 
					GPIOB->BRR = GPIO_Pin_12;//GPIO_ResetBits(GPIOB,GPIO_Pin_12); // blue
					GPIOB->BRR = GPIO_Pin_13;//GPIO_ResetBits(GPIOB,GPIO_Pin_13); 
					GPIOB->BRR = GPIO_Pin_15;//GPIO_ResetBits(GPIOB,GPIO_Pin_15); // blue

					//GPIOB->BSRR = ((date_temp & 0x) << 8) | ((~date_temp & 0x0F) << 24);
					GPIOC->BSRR = GPIO_Pin_4;//GPIO_SetBits(GPIOC,GPIO_Pin_4);
				}
			}
		}
		//
		
		//

		#if 0
		for(j = 0; j < 3; j++)
		{
			x = (uint16_t)(LineIndex + j * 32) * MaxLine;
			for(y = 0; y < MaxLine; y += 8)
			{
				for(z = 0; z < 8; z++)
				{
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);

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

		
		TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));	// 1
		OffDisplay();													// 关显示	 低电平
		
		nop();nop();nop();nop();nop();nop();nop();
		nop();nop();nop();nop();nop();nop();nop();
		
		
		if(LineIndex & 0x01) GPIOC->BSRR = GPIO_Pin_6;//GPIO_SetBits(GPIOC,GPIO_Pin_6);			//行选择
		else                 GPIOC->BRR = GPIO_Pin_6;//GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		
		if(LineIndex & 0x02) GPIOC->BSRR = GPIO_Pin_7; // GPIO_SetBits(GPIOC,GPIO_Pin_7);
		else                 GPIOC->BRR = GPIO_Pin_7; // GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		
		if(LineIndex & 0x04) GPIOC->BSRR = GPIO_Pin_9;// GPIO_SetBits(GPIOC,GPIO_Pin_9);
		else                 GPIOC->BRR = GPIO_Pin_9;// GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		
		
		
		
		GPIOC->BSRR = GPIO_Pin_5; //GPIO_SetBits(GPIOC,GPIO_Pin_5);//	//595锁存	
		
		
		nop();nop();nop();nop();nop();nop();nop();
		#if 1
		if(diplay_data.display_time > 0)
		{
			//TIM3->ARR  = SysDisplaySpeed * 10 * diplay_data.display_time / 15;
			TIM3->ARR  = diplay_data.display_time;//(10 * diplay_data.display_time) / 15;
 			TIM3->EGR |= TIM_EGR_UG;		// 1
 			TIM3->CR1 |= TIM_CR1_CEN;		// 1
		}
		#endif
		OnDisplay();													//开显示

		//
		if(++LineIndex >= SaoMiao)
		{
			LineIndex = 0;
			
			if(++LED_Index >= 30) LED_Index = 0;
			
			switch(LED_Index)
			{
				case  0: LEDON();break;
				case 15: LEDOFF();break;
				default: break;
			}
			
			HaveDisplayNum++;
			if ((diplay_data.style) && (HaveDisplayNum >= diplay_data.style)) {
			//if ((++HaveDisplayNum >= diplay_data.style) &&  (DISPLAY_STATIC != diplay_data.style)) {
				Bemove = 1;
				HaveDisplayNum = 0;
			}
			//else if(diplay_data.length > ScreenLength)
			else
			{
				if((upturn_index < 16) && (HaveDisplayNum >= diplay_data.upstyle)) {
						Bemove = 1;
						HaveDisplayNum = 0;upturn_index++;
				}
				if(upturn_index == 16){
					time_upsec = 0;
					time_upindex = 0;
					upturn_index = 17;
				}
				//else if(HaveDisplayNum > 90) {
				//	Bemove = 1;
				//	HaveDisplayNum = 0;
				//	upturn_index = 0;
				//}
				
			}
			//if(!diplay_data.follow_up_action && !display_done_p) {
				//time_index++;
				time_upindex++;
				//time_sec = time_index / 250;
				time_upsec = time_upindex / 1000;
			//}
			//if(time_upsec > 0){
			//	temp[0] = 0x99;
			//	temp[1] = time_upsec;
			//	temp[2] = time_sec;
				
				//Can_Send_Msg(temp, 8);
			//}
		}
		#if 1
		//if(!diplay_data.display_time)
		//{
		//	OffDisplay();
		//}
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





