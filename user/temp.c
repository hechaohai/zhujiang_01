/********************************************************************
 * Program:		IAP 升级程序											*
 * flash地址:	0x08000000~0x08003FFF								*
 * 协议地址：	0x08004000											*
 * 升级成功：	0x08004010赋给PC；0x08004014赋给SP					*
 * sst25vf：  0x040000~7FFFF
 ********************************************************************
 
  
	LAT   PA11   锁存  
	CLK   PA12   时钟
	OE    PA8    显示
	A     PC9    行A
	B     PC8
	C     PC7
	D     PC6
	
	R1    PB8
	G1    PB9
	R2    PB10
	G2    PB11
	R3    PB12
	G3    PB13
	R4    PB14
	G4    PB15
	
 */

/********************************************************************************
 *								头文件											*
 ********************************************************************************
 */
#include "stm32f10x.h"
#include "main.h"
//#include "aes128.h"


extern const uint8_t ziku[1520];
/********************************************************************************
 *								函数声明											*
 ********************************************************************************
 */

void ReturnByte(uint8_t byte);

void EraseExternalFlash(void);		//清除外部App

uint8_t CrcExternalFlash(void);		//校验外部Flash

void init_system(void);					//系统初始化
void UART_Rceive_RXNE(void);			//接收一个字节
void do_BZQ_Receive(void);				//处理串口数据
void clear_displayBuf(void);			//清除显示缓存
void clear_TextBuf(void);
void Read_MyID(void);
void Read_Light(void);
void Display(void);
void LoadScreen(void);
void ClearScreen(void);
void LoadWord_Dis(uint8_t word, uint8_t x, uint8_t y);
void LoadWord_Text(uint8_t word, uint8_t x);
void Load_MoveBuf(uint8_t word);
/********************************************************************************
 *								定义全局变量										*
 ********************************************************************************
 */
 
 /*
__asm void StartAPP()
{
	CPSID I
	LDR   R0, =0x08004400
	LDR   R1, =0x08004404
	LDR   R2, [R0]
	MSR   MSP, R2
	LDR   R2, [R1]
	BX    R2
}
*/
/*__asm void nop(void)
{
    NOP
	NOP
}*/
#define nop __nop

#define WriteDisplayData(x)  GPIOB->BSRR = ((x & 0xFF) << 8) | ((~x & 0xFF) << 24)

/********************************************************************************
 *								主函数											*
 ********************************************************************************
 */
int main(void)
{
	init_system();
	
	//LoadWord_Dis(0x38,0,0);
	//LoadWord_Dis(0x31,8,0);
	
	//LoadWord_Dis(0x32,0,16);
	//LoadWord_Dis(0x31,8,16);
	//LoadWord_Dis(0x32,0,24);
	//LoadWord_Dis(0x31,8,24);
	//LoadWord_Dis(0x32,72,24);
	
	//LoadWord_Dis(0x30,72,32);
	//LoadWord_Dis(0x31,64,32);
	//LoadWord_Dis(0x32,56,32);
	//LoadWord_Dis(0x33,48,32);
	//LoadWord_Dis(0x34,40,32);
	//LoadWord_Dis(0x35,32,32);
	//LoadWord_Dis(0x36,24,32);
	//LoadWord_Dis(0x37,16,32);
	//LoadWord_Dis(0x33,0,0);
	//LoadWord_Dis(0x30,0,32);
	while(1)
	{
		LoadScreen();		//加载屏幕
		//Display();			//显示屏幕
	}
}
//
void LoadScreen(void)
{	
	uint8_t  i,j,z;
	uint8_t  temp;
	uint32_t addr;
	uint8_t *p;
	//uint8_t x,y;
	
	//处理更新内容
	if(Updata)
	{
		Updata = 0;
		
		TIM_Cmd(TIM3, DISABLE);
		TIM_Cmd(TIM2, DISABLE);
		
		TextRemainTimeTemp 	= 0;
		DisTextIndex		= 0;
		HuanPing			= 0;
		
		LeftMove	= 0;
		RightMove	= 0;
		TurnUp		= 0;
		TurnDown	= 0;
		Blink		= 0;
		//加载图片
		clear_displayBuf();			//清除显示缓存区
		DisplayBufChoose = 0;		//选择第0个显示区
		/*
		ClrNSS(); 
		Read_Data(ExternalFlashTupiaoAddr + RxBuffer[10] * 0x1000);
		for(i = 0; i < length; i ++)
		{
			ReturnByte(Get_Byte());
		}
		SetNSS();
		*/
		
		if(TupianIndex)				//取图片
		{
			//ReturnByte(0x88);
			
			addr = ExternalFlashTupiaoAddr + TupianIndex * 0x1000;
			ClrNSS();
			Read_Data(addr);
			for(i = 0; i < 48; i++)
			{
				for(j = 0;j < 80; j++)
				{
					temp = Get_Byte();
					DisplayBuf0[i][j] = temp;
					DisplayBuf1[i][j] = temp;
					//ReturnByte(temp);
				}
			}
			SetNSS();
		}
		
				
		if(TextLength || TupianIndex) TIM_Cmd(TIM2, ENABLE);
		//for(i = 0 ;i < TextLength; i++ )
		//{
		//	LoadWord_Dis(TextBuf[i],i*8,32);
		//}
		#if 1
		/*
		//文字属性
			TextLength      = length - 8;			//文字内容长度（字节）
			TextLocation_X  = RxBuffer[11];			//X
			TextLocation_Y  = RxBuffer[12];			//Y
			DisTextPointNum = RxBuffer[13]; 		//文字显示长度（点阵）
			TextColor       = RxBuffer[14];			//文字颜色
			TextMode        = RxBuffer[15]; 		//文字模式
			TextModerGrade  = 16 - RxBuffer[16]; 	//文字模式等级
			TextRemainTime  = RxBuffer[17];			//显示文字停留时间
			TextType
		*/
		
		if(!TextLength) return;		//没有文本显示
		
		//静止、闪烁、上翻、下翻
		if(Mode_Static == TextMode || Mode_Blink == TextMode || Mode_Upturn == TextMode || Mode_Downturn == TextMode)
		{
			DisLocation_X = TextLocation_X;						//默认靠左
			if(TextLength - DisTextIndex < TextPointNum/8)		//显示样式 00 居中，01 靠左，02 靠右
			{
				if(0 == TextType)
				{
					DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 4;
				}
				else if(2 == TextType)
				{
					DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 8;
				}
			}
			
			//清除文字显示缓存区
			clear_TextBuf();
			
			//加载要显示的文字到文字显示缓存区
			for(i = 0; i < 10; )
			{
				//ReturnByte(1);
				if( (i + 1) * 8 > DisTextPointNum) {i = 0; break;}
				
				LoadWord_Text(TextBuf[i],i*8);
				i++;
				if(i > TextLength ){ i = 0; break;}
			}
			DisTextIndex = i;
			
			//在静止或闪烁模式下
			if( Mode_Static == TextMode || Mode_Blink == TextMode )
			{
				//加载到显示区域
				for(i = 0; i < DisTextPointNum; i++)
				{
					for(j = 0; j < 16; j++)
					{
						DisplayBuf0[TextLocation_Y + j][DisLocation_X + i] = DisTextBuf[j][i];
						if(j + TextLocation_Y >= 47) break;
						if(DisLocation_X + i >= 79)  break;
					}
				}
				//当文字长于屏显位置且有停留时间，启动定时
				if( TextLength > DisTextIndex && TextRemainTime )
				//if( Mode_Blink == TextMode || (TextLength > DisTextIndex && TextRemainTime) )
				{
					TIM3->CNT = 0;
					TIM_Cmd(TIM3, ENABLE);						//使能TIMx
					TextRemainTimeTemp = 0;
					Time_IntCounter = 0;
				}
				if(Mode_Blink == TextMode)
				{
					ShanShuoNum = 0;
					Blink = 1;
				}
				
			}
			if(Mode_Upturn == TextMode )       {TurnUp   = 1; TurnIndex = 0;TurnWaitTime = 0; HaveDoDisplay = 1;}
			else if(Mode_Downturn == TextMode) {TurnDown = 1; TurnIndex = 0;TurnWaitTime = 0; HaveDoDisplay = 1;}
			
		}
		else if(Mode_LeftMove == TextMode )
		{
			LeftMove  = 1;
			MoveIndex = 0;
			HaveDoDisplay = 0;
			MoveWaitTime = 0;
			
			DisLocation_X = TextLocation_X;						//默认靠左
			if(TextLength < TextPointNum/8)		//显示样式 00 居中，01 靠左，02 靠右
			{
				if(0 == TextType)
				{
					DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength )) * 4;
				}
				else if(2 == TextType)
				{
					DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength )) * 8;
				}
			}
			
			
			
		}
		else if(Mode_RightMove ==  TextMode)
		{
			RightMove = 1;
			MoveIndex = 0;
		}
		
		#endif
		
	}
	#if 1
	//处理显示方式
	if(TextLength && HaveDoDisplay)			//有内容且显示了一次，处理下一次显示
	{
		HaveDoDisplay = 0;
		
		if(Blink)
		{
			if(DisplayBufChoose)  p = &DisplayBuf0[0][0];
			else                  p = &DisplayBuf1[0][0];
			
			if(1 == Blink)
			{
				if(++ShanShuoNum >= TextModerGrade*10)
				{
					//是否换屏
					if(HuanPing)
					{
						//文字对齐后起始坐标
						DisLocation_X = TextLocation_X;							//默认靠左
						if(TextLength - DisTextIndex < TextPointNum / 8 )		//显示样式 00 居中，01 靠左，02 靠右
						{
							if(0 == TextType)
							{
								DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 4;
							}
							else if(2 == TextType)
							{
								DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 8;
							}
							TIM_Cmd(TIM2, DISABLE);
							if(DisLocation_X > TextLocation_X)
							{
								for(i = TextLocation_X; i < DisLocation_X; i++)
								{
									for(j = 0; j < 16; j++)
									{
										DisplayBuf0[TextLocation_Y + j][i] = 0;
										DisplayBuf1[TextLocation_Y + j][i] = 0;
										//*(p + (TextLocation_Y + j) * 80 + i) = 0;
										if(j +  TextLocation_Y >= 47) break;
									}
								}
							}
							TIM_Cmd(TIM2, ENABLE);
						}
						
						//加载要显示的文字到文字显示缓存区
						clear_TextBuf();
						if(DisTextIndex >= TextLength) DisTextIndex = 0;
						for(i = 0; i < 10; )
						{
							if( (i + 1) * 8 > DisTextPointNum) { break;}
							
							LoadWord_Text(TextBuf[i+DisTextIndex],i*8);
							i++;
							if(i+DisTextIndex >= TextLength ) {DisTextIndex = 0; i = 0; break;}
						}
						DisTextIndex += i;
						if(DisTextIndex >= TextLength) DisTextIndex = 0;
						HuanPing = 0;
					}
					
					//加载到显示区域
					for(i = 0; i < DisTextPointNum; i++)
					{
						for(j = 0; j < 16; j++)
						{
							*(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = DisTextBuf[j][i];
							if(j +  TextLocation_Y >= 47) break;
							if(i + DisLocation_X >= 79)   break;
						}
					}
					
					if(DisplayBufChoose)  DisplayBufChoose = 0;
					else                  DisplayBufChoose = 1;
					
					ShanShuoNum = 0;
					Blink = 2;
				}
			}
			else //if(2 == Blink)
			{
				if(++ShanShuoNum >= TextModerGrade*10)
				{
					
					for(i = 0; i < DisTextPointNum; i++)
					{
						for(j = 0; j < 16; j++)
						{
							*(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = 0;
							if(j +  TextLocation_Y >= 47) break;
							if(i + DisLocation_X >= 79)   break;
						}
					}
					
					if(DisplayBufChoose)  DisplayBufChoose = 0;
					else                  DisplayBufChoose = 1;
					
					ShanShuoNum = 0;
					Blink = 1;
				}
			}
		}
		
		//上翻
		else if(TurnUp)							
		{
			//上翻前部分
			if(1 == TurnUp)
			{
				if( TurnIndex >= 16 )
				{
					if(TextRemainTime)
					{
						TurnUp = 0;
						TIM3->CNT = 0;
						TextRemainTimeTemp = 0;
						TIM_Cmd(TIM3, ENABLE);						//使能TIMx
					}
					else
					{
						TurnUp = 0;//2;
						TurnIndex = 0;
					}
					return;
				}
				
				if(++TurnWaitTime < TextModerGrade) return;
				TurnWaitTime = 0;
				
				if(DisplayBufChoose)  p = &DisplayBuf0[0][0];
				else                  p = &DisplayBuf1[0][0];
				
				//加载到显示区域
				for(i = 0; i < DisTextPointNum; i++)
				{
					for(j = 0; j < 16; j++)
					{
						if(j >= (15 - TurnIndex) ) *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = DisTextBuf[j - (15 - TurnIndex)][i];
						else                       *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = 0;
						if(j + TextLocation_Y >= 47) break;
						if(i + DisLocation_X >= 79)  break;
					}
				}
				
				if(DisplayBufChoose)  DisplayBufChoose = 0;
				else                  DisplayBufChoose = 1;
				
				TurnIndex++;
			}
			
			//上翻后部分 
			else //if(2 == TurnUp)
			{
				if( TurnIndex >= 16 )
				{
					//文字对齐后起始坐标
					DisLocation_X = TextLocation_X;						//默认靠左
					if(TextLength - DisTextIndex < TextPointNum/8)		//显示样式 00 居中，01 靠左，02 靠右
					{
						if(0 == TextType)
						{
							DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 4;
						}
						else if(2 == TextType)
						{
							DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 8;
						}
						TIM_Cmd(TIM2, DISABLE);
						if(DisLocation_X > TextLocation_X)
						{
							for(i = TextLocation_X; i < DisLocation_X; i++)
							{
								for(j = 0; j < 16; j++)
								{
									DisplayBuf0[TextLocation_Y + j][i] = 0;
									DisplayBuf1[TextLocation_Y + j][i] = 0;
									//*(p + (TextLocation_Y + j) * 80 + i) = 0;
									if(j +  TextLocation_Y >= 47) break;
								}
							}
						}
						TIM_Cmd(TIM2, ENABLE);
					}
					
					//加载要显示的文字到文字显示缓存区
					clear_TextBuf();								//清除文字显示缓存区
					if(DisTextIndex >= TextLength) DisTextIndex = 0;
					for(i = 0; i < 10; )							
					{
						if( (i + 1) * 8 > DisTextPointNum) { break;}
					
						LoadWord_Text(TextBuf[i+DisTextIndex],i*8);
						i++;
						if(i+DisTextIndex >= TextLength ) {DisTextIndex = 0; i = 0; break;}
					}
					DisTextIndex += i;
					if(DisTextIndex >= TextLength) DisTextIndex = 0;
					
					TurnUp = 1;
					TurnIndex = 0;
					return;
				}
				
				if(++TurnWaitTime < TextModerGrade) return;
				TurnWaitTime = 0;
				
				if(DisplayBufChoose)  p = &DisplayBuf0[0][0];
				else                  p = &DisplayBuf1[0][0];
				
				//加载到显示区域
				for(i = 0; i < DisTextPointNum; i++)
				{
					for(j = 0; j < 16; j++)
					{
						if(15 - TurnIndex > j ) *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = DisTextBuf[j + TurnIndex +1][i];
						else                    *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = 0;
						if(j + TextLocation_Y >= 47) break;
						if(i + DisLocation_X >= 79)  break;
					}
				}
				
				if(DisplayBufChoose)  DisplayBufChoose = 0;
				else                  DisplayBufChoose = 1;
				
				TurnIndex++;
			}
		}
		
		//下翻
		else if(TurnDown)					
		{
			//下翻前部分
			if(1 == TurnDown)
			{
				if( TurnIndex >= 16 )
				{
					if(TextRemainTime)
					{
						TurnDown = 0;
						TIM3->CNT = 0;
						TextRemainTimeTemp = 0;
						TIM_Cmd(TIM3, ENABLE);						//使能TIMx
					}
					else
					{
						TurnDown = 0;//2;
						TurnIndex = 0;
					}
					return;
				}
				
				if(++TurnWaitTime < TextModerGrade) return;
				TurnWaitTime = 0;
				
				
				if(DisplayBufChoose)  p = &DisplayBuf0[0][0];
				else                  p = &DisplayBuf1[0][0];
				
				//加载到显示区域
				for(i = 0; i < DisTextPointNum; i++)
				{
					for(j = 0; j < 16; j++)
					{
						if(j <= TurnIndex ) *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = DisTextBuf[j + (15 - TurnIndex)][i];
						else                *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = 0;
						if(j + TextLocation_Y >= 47) break;
						if(DisLocation_X + i >= 79)  break;
					}
				}
				
				if(DisplayBufChoose)  DisplayBufChoose = 0;
				else                  DisplayBufChoose = 1;
				
				TurnIndex++;
			}
			
			//下翻后部分 
			else //if(2 == TurnDown)
			{
				if( TurnIndex >= 16 )
				{
					DisLocation_X = TextLocation_X;						//默认靠左
					if(TextLength - DisTextIndex  < TextPointNum/8)		//显示样式 00 居中，01 靠左，02 靠右
					{
						if(0 == TextType)
						{
							DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex )) * 4;
						}
						else if(2 == TextType)
						{
							DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 8;
						}
						
						TIM_Cmd(TIM2, DISABLE);
						if(DisLocation_X > TextLocation_X)
						{
							for(i = TextLocation_X; i < DisLocation_X; i++)
							{
								for(j = 0; j < 16; j++)
								{
									DisplayBuf0[TextLocation_Y + j][i] = 0;
									DisplayBuf1[TextLocation_Y + j][i] = 0;
									//*(p + (TextLocation_Y + j) * 80 + i) = 0;
									if(j +  TextLocation_Y >= 47) break;
								}
							}
						}
						TIM_Cmd(TIM2, ENABLE);
						
					}
					
					//加载要显示的文字到文字显示缓存区
					clear_TextBuf();								//清除文字显示缓存区
					if(DisTextIndex >= TextLength) DisTextIndex = 0;
					for(i = 0; i < 10; )							
					{
						if( (i + 1) * 8 > DisTextPointNum) { break;}
					
						LoadWord_Text(TextBuf[i+DisTextIndex],i*8);
						i++;
						if(i+DisTextIndex >= TextLength ) {DisTextIndex = 0; i = 0; break;}
					}
					DisTextIndex += i;
					if(DisTextIndex >= TextLength) DisTextIndex = 0;
					
					TurnDown = 1;
					TurnIndex = 0;
					return;
				}
				
				if(++TurnWaitTime < TextModerGrade) return;
				TurnWaitTime = 0;
				
				if(DisplayBufChoose)  p = &DisplayBuf0[0][0];
				else                  p = &DisplayBuf1[0][0];
				
				//加载到显示区域
				for(i = 0; i < DisTextPointNum; i++)
				{
					for(j = 0; j < 16; j++)
					{
						if( j > TurnIndex ) *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = DisTextBuf[j - TurnIndex - 1][i];
						else                *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = 0;
						if(j + TextLocation_Y >= 47) break;
						if(DisLocation_X + i >= 79) break; 
					}
				}
				
				if(DisplayBufChoose)  DisplayBufChoose = 0;
				else                  DisplayBufChoose = 1;
				
				TurnIndex++;
			}			
		}
		
		//左移
		else if(LeftMove)
		{
			if(++MoveWaitTime < TextModerGrade) return;
			MoveWaitTime = 0;
				
			//移动完一个文字后判断
			if(0 == MoveIndex)		
			{
				//停留时间-单屏
				if(TextRemainTime && DisTextIndex >= TextPointNum / 8)
				{
					LeftMove = 0;
					TIM3->CNT = 0;
					TextRemainTimeTemp = 0;
					TIM_Cmd(TIM3, ENABLE);						//使能TIMx
					return;
				}
				
				if(DisTextIndex >= TextLength )
				{
					DisTextIndex = 0;
				}
				
				Load_MoveBuf(TextBuf[DisTextIndex]);
				
				DisTextIndex++;
					
			}
			
			if(DisplayBufChoose)  p = &DisplayBuf0[0][0];
			else                  p = &DisplayBuf1[0][0];
			
			//加载到显示区域
			for(i = 0; i < DisTextPointNum-1; i++)
			{
				for(j = 0; j < 16; j++)
				{
					*(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = *(p + (TextLocation_Y + j) * 80 + DisLocation_X + i + 1);
					
					if(j + TextLocation_Y >= 47) break;
					if(DisLocation_X + i >= 78) {i++;break;} 
				}
			}
			for(j = 0; j < 16; j++)
			{
				*(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = MoveBuf[j][MoveIndex];
			}
			
			if(DisplayBufChoose)  DisplayBufChoose = 0;
			else                  DisplayBufChoose = 1;
			
			MoveIndex ++;
			if(MoveIndex >= 8) MoveIndex = 0;
		}
		
		//右移
		else if(RightMove)					
		{
			
		}
		
	}
//
	#endif
}



//
void Display(void)
{
	
	uint16_t  line,i,j,z;
	uint8_t * p;
	uint16_t xx,yy;			//x轴y轴坐标
	uint8_t  temp_Light;
	
	
	//uint16_t length;
	if(!DisplayBufChoose) p = &DisplayBuf0[0][0];
	else                  p = &DisplayBuf1[0][0];
	for(line = 0; line < 4; line++)
	{
		temp_Light = 0;
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);		//595锁存
		
		#if 0
		for(i = 0; i < 160 * 8 / 4; i += 8)
		{
			/*j = (i / 8) & 255;
			z = (j & 0x80) ? 1 : 0;
			WriteDisplayData(z);
			z = (j & 0x40) ? 1 : 0;
			WriteDisplayData(z);
			z = (j & 0x20) ? 1 : 0;
			WriteDisplayData(z);
			z = (j & 0x10) ? 1 : 0;
			WriteDisplayData(z);
			z = (j & 0x08) ? 1 : 0;
			WriteDisplayData(z);
			z = (j & 0x04) ? 1 : 0;
			WriteDisplayData(z);
			z = (j & 0x02) ? 1 : 0;
			WriteDisplayData(z);
			z = (j & 0x01) ? 1 : 0;
			WriteDisplayData(z);*/
			
			j = (i / 8) & 255;
			
			z = (j & 0x80) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			z = (j & 0x40) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			z = (j & 0x20) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			z = (j & 0x10) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			z = (j & 0x08) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			z = (j & 0x04) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			z = (j & 0x02) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			z = (j & 0x01) ? 1 : 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			WriteDisplayData(z);
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			
			
		}
		
		#endif
	#if 1	
		#if 1
		//p = &DisplayBuf[line][0];
		for(i = 0; i < 5 ; i++)
		{
			//
			#if 1
			for(j = 0; j < 16; j++)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
				switch( *( p + (line + 4)*80 + i*16 +j) ) //switch(DisplayBuf[line + 4][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
				}
				switch( *( p + (line + 12)*80 + i*16 +j) ) //switch(DisplayBuf[line + 12][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
				}
				switch( *( p + (line + 20)*80 + i*16 +j) ) //switch(DisplayBuf[line + 20][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
				}
				switch( *( p + (line + 28)*80 + i*16 +j) ) //switch(DisplayBuf[line + 28][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
				}
				GPIO_SetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
			}
			#endif
			temp_Light++;
			if(temp_Light >= Light) GPIO_SetBits(GPIOA,GPIO_Pin_8);				//关显示
			//
			for(j = 0; j < 16; j++)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
				switch( *( p + (line)*80 + i*16 +j) ) //switch(DisplayBuf[line][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
				}
				switch( *( p + (line + 8)*80 + i*16 +j) ) //switch(DisplayBuf[line + 8][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
				}
				switch( *( p + (line + 16)*80 + i*16 +j) ) //switch(DisplayBuf[line + 16][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
				}
				switch( *( p + (line + 24)*80 + i*16 +j) ) //switch(DisplayBuf[line + 24][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
				}
				GPIO_SetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
			}
			temp_Light++;
			if(temp_Light >= Light) GPIO_SetBits(GPIOA,GPIO_Pin_8);				//关显示
		}

		#endif
		//
		#if 1
		for(i = 0; i < 5 ; i++)
		{
			#if 1
			for(j = 0; j < 16; j++)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
				switch( *( p + (line + 20)*80 + i*16 +j) ) //switch(DisplayBuf[line + 20][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
				}
				switch( *( p + (line + 28)*80 + i*16 +j) ) //switch(DisplayBuf[line + 28][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
				}
				switch( *( p + (line + 36)*80 + i*16 +j) ) //switch(DisplayBuf[line + 36][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
				}
				switch( *( p + (line + 44)*80 + i*16 +j) ) //switch(DisplayBuf[line + 44][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
				}
				GPIO_SetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
			}
			temp_Light++;
			if(temp_Light >= Light) GPIO_SetBits(GPIOA,GPIO_Pin_8);				//关显示
			#endif
			
			for(j = 0; j < 16; j++)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
				switch( *( p + (line + 16)*80 + i*16 +j) ) //switch(DisplayBuf[line+16][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_8);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						break;
				}
				switch( *( p + (line + 24)*80 + i*16 +j) ) //switch(DisplayBuf[line + 24][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_10);
						GPIO_SetBits(GPIOB,GPIO_Pin_11);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_10);
						GPIO_ResetBits(GPIOB,GPIO_Pin_11);
						break;
				}
				switch( *( p + (line + 32)*80 + i*16 +j) ) //switch(DisplayBuf[line + 32][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
				}
				switch( *( p + (line + 40)*80 + i*16 +j) ) //switch(DisplayBuf[line + 40][i*16 + j])
				{
					case 1:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
					case 2:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					case 3:
						GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
					default:
						GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
				}
				GPIO_SetBits(GPIOA,GPIO_Pin_12);		//595移位时钟
			}
			temp_Light++;
			if(temp_Light >= Light) GPIO_SetBits(GPIOA,GPIO_Pin_8);				//关显示
			//
		}
		#endif
		//
		
	#endif	
		
		
		GPIO_SetBits(GPIOA,GPIO_Pin_8);				//关显示
		nop();nop();nop();nop();nop();nop();nop();
		nop();nop();nop();nop();nop();nop();nop();
		nop();nop();nop();nop();nop();nop();nop();
		GPIO_SetBits(GPIOA,GPIO_Pin_11);				//595锁存
	
		if(line & 0x08) GPIO_SetBits(GPIOC,GPIO_Pin_6);	//行选择
		else            GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		
		if(line & 0x04) GPIO_SetBits(GPIOC,GPIO_Pin_7);
		else            GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		
		if(line & 0x02) GPIO_SetBits(GPIOC,GPIO_Pin_8);
		else            GPIO_ResetBits(GPIOC,GPIO_Pin_8);
		
		if(line & 0x01) GPIO_SetBits(GPIOC,GPIO_Pin_9);
		else            GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		
		nop();nop();nop();nop();nop();nop();nop();
		nop();nop();nop();nop();nop();nop();nop();
		nop();nop();nop();nop();nop();nop();nop();
		
		//if(!Blink) GPIO_ResetBits(GPIOA,GPIO_Pin_8);			//闪烁 测试			
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);						//开显示
		
		
	}
	
	for(i = 0; i < 3000; i++)									//延迟一段时间关显示
	{
		//__nop();__nop();//__nop();//__nop();__nop();
		if(i/150 > Light) {GPIO_SetBits(GPIOA,GPIO_Pin_8);return;}	
		//__nop();__nop();__nop();__nop();__nop();
	}
	GPIO_SetBits(GPIOA,GPIO_Pin_8);	
	
	
	
}
//
/********************************************************************************
 *								初始化											*
 ********************************************************************************
*/
void init_system(void)
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 9999;//9;					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//根据指定的参数初始化TIMx的时间基数单位
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 99;//9;//9;					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	//TIM_TimeBaseStructure.TIM_Prescaler = 7199;					//设置用来作为TIMx时钟频率除数的预分频值
	//TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割:TDTS = Tck_tim
	//TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	USART_InitStructure.USART_BaudRate 				= 57600;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;				//0
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;					//0
	USART_InitStructure.USART_Parity 				= USART_Parity_No;					//0
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;	//0
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	//4 | 8

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
	
	// 外设时钟 SPI1 时钟开启，IO端口ABC时钟开启
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);		
	//0x00004000 | 4 , 1
    // 配置 USART1 Tx (PA.09) //复用推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 USART1 Tx (PA.10) //上拉输入
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_Init(USART1, &USART_InitStructure);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		//使能 接收中断
	
	// 使能 USART1
	USART_Cmd(USART1, ENABLE);
	while(!(USART1->SR & 0x40));

	// 配置 显示行ABCD (PC.9、8、7、6) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 配置 数据红色R1234 (PB.8、10、12、14) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 配置 数据绿色G1234 (PB.9、11、13、15) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	//配置485  OE、CLK、LAT (PA8、11、12) 推挽输出
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);             // ?a??SPI1ê±?ó
    SPI1->CR1  &= (~(1 << 6)) & 0xFFFF;                              // 1?±?SPI1
    SPI1->CR1   = (3 << 8) | (0 << 3) | (1 << 2);                    // ê±?ó=APB2/2=36M,?÷?￡ê?
	SPI1->CR1  |= 1 << 6;                                            // ?a??SPI1
	
	// 配置 SPI1 NSS (PA.4) 推挽输出
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置 SPI1 CLK (PA.5) 复用推挽
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置 SPI1 MISO (PA.6) 输入
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置 SPI1 MOSI (PA.7) 复用推挽
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//TIM2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);								//初始化NVIC寄存器
	
	//TIM3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);								//初始化NVIC寄存器


	//TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
	
	__enable_irq();
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);		//关显示
	Init_SST25VF();
	clear_displayBuf();
	Read_MyID();
	Read_Light();
	
	ClearScreen();
}

/*
void MYDMA1CH5_Config(u32 cpar) 
{ 
	uint32_t DR_Base;    //做缓冲用,不知道为什么.非要不可 
	RCC->AHBENR|=1<<0; //开启DMA1时钟 
	delay_ms(2);//加延时试试。 
	DR_Base=cpar; 
	//DMA1_Channel5->CPAR=DR_Base;			//DMA1 外设地址  
	DMA1_Channel5->CPAR=(u32)&USART2->DR;	//DMA1 外设地址  
	DMA1_Channel5->CCR=0X00000000;			//复位 
	DMA1_Channel5->CCR &= ~(1<<4);			//从外设读 
	DMA1_Channel5->CCR &= ~(1<<5);			//普通模式，不执行循环操作 
	DMA1_Channel5->CCR &= ~(1<<6);			//外设地址非增量模式 
	DMA1_Channel5->CCR |= 1<<7;				//存储器增量模式 
	DMA1_Channel5->CCR &= ~(3<<8);			//外设数据宽度为8位 
	DMA1_Channel5->CCR &= ~(3<<10);			//存储器数据宽度8位 
	DMA1_Channel5->CCR |= 1<<12;			//中等优先级 
	DMA1_Channel5->CCR &= ~(1<<14);			//非存储器到存储器模式 

	//传输完成中断 
	DMA1_Channel6->CCR |= 1<<1;                         //允许传输完成中断 
	MY_NVIC_Init(2,2,DMA1_Channel6_IRQChannel,2);//组2，最低优先级  
}  
*/


/********************************************************************************
 *							定时器2中断 用于显示	    							*
 ********************************************************************************
*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//清除TIMx更新中断标志 
		Display();
		HaveDoDisplay = 1;
	}
}
//


/********************************************************************************
 *							定时器3中断服务程序									*
 ********************************************************************************
*/
void TIM3_IRQHandler(void)
{
	uint8_t i,j,z,temp,*p;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//清除TIMx更新中断标志 
		
		
		
		//
		//if(++Time_IntCounter >= 10)
		{
			TextRemainTimeTemp++;
			//Time_IntCounter = 0;
			if(TextRemainTimeTemp >= TextRemainTime)
			{
				TIM_Cmd(TIM3, DISABLE);
				TextRemainTimeTemp = 0;
				
				if(Mode_Static == TextMode )
				{
					if(DisplayBufChoose)  p = &DisplayBuf0[0][0];
					else                  p = &DisplayBuf1[0][0];
					
					DisLocation_X = TextLocation_X;						//默认靠左
					if(TextLength - DisTextIndex < TextPointNum / 8 )		//显示样式 00 居中，01 靠左，02 靠右
					{
						if(0 == TextType)
						{
							DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 4;
						}
						else if(2 == TextType)
						{
							DisLocation_X = TextLocation_X + (TextPointNum / 8 - (TextLength - DisTextIndex)) * 8;
						}
						
						TIM_Cmd(TIM2, DISABLE);
						if(DisLocation_X > TextLocation_X)
						{
							for(i = TextLocation_X; i < DisLocation_X; i++)
							{
								for(j = 0; j < 16; j++)
								{
									DisplayBuf0[TextLocation_Y + j][i] = 0;
									DisplayBuf1[TextLocation_Y + j][i] = 0;
									//*(p + (TextLocation_Y + j) * 80 + i) = 0;
									if(j +  TextLocation_Y >= 47) break;
								}
							}
						}
						TIM_Cmd(TIM2, ENABLE);
					}
					
					
					
					//清除文字显示缓存区
					clear_TextBuf();
					//加载要显示的文字到文字显示缓存区
					if(DisTextIndex >= TextLength) DisTextIndex = 0;
					for(i = 0; i < 10; )
					{
						if( (i + 1) * 8 > DisTextPointNum) { break;}
					
						LoadWord_Text(TextBuf[i+DisTextIndex],i*8);
						i++;
						if(i+DisTextIndex >= TextLength ) {DisTextIndex = 0; i = 0; break;}
					}
					DisTextIndex += i;
					//加载到显示区域
					for(i = 0; i < DisTextPointNum; i++)
					{
						for(j = 0; j < 16; j++)
						{
							*(p + (TextLocation_Y + j) * 80 + DisLocation_X + i) = DisTextBuf[j][i];
							if(j +  TextLocation_Y >= 47) break;
							if(i + DisLocation_X >= 79)   break;
						}
					}
					if(DisplayBufChoose)  DisplayBufChoose = 0;
					else                  DisplayBufChoose = 1;
					
					TIM3->CNT = 0;
					Time_IntCounter = 0;
					TIM_Cmd(TIM3, ENABLE);						//使能TIMx
				}
				
				else if( Mode_Blink == TextMode )
				{
					HuanPing = 1;
					TIM_Cmd(TIM3, ENABLE);						//使能TIMx
				}
				
				else if( Mode_Upturn == TextMode )
				{
					TurnUp = 2;
					TurnIndex = 0;
				}
				else if(Mode_Downturn == TextMode)
				{
					TurnDown = 2;
					TurnIndex = 0;
				}
				else if(Mode_LeftMove == TextMode )
				{
					LeftMove = 1;
				}
				else if(Mode_RightMove ==  TextMode)
				{
					RightMove = 1;
				}
				
			}
		}
		
	}
}

/********************************************************************************
 *								串口中断											*
 ********************************************************************************
*/
void USART1_IRQHandler(void)
{
	uint8_t  ReadUsartData;
	
	//if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	if(USART1->SR&(1<<5))									//接收到数据
	{
		ReadUsartData = USART1->DR;				//对USART_DR的读操作可以将USART1->SR&(1<<5)位清零
		//ReturnByte(ReadUsartData);
		//if(RxCounter >= NbrOfDataToRead)
		//{
		//	RxCounter = 0;
		//}
		/********************************/
		//RxBuffer[RxCounter ++] = USART_ReceiveData(USART1);
		//return;
		
		if(0 == RxCounter && 0x55 == ReadUsartData)				// 55							
		{
			RxBuffer[RxCounter] = 0x55;
			RxCounter++;
		}
		
		else if(1 == RxCounter && 0xff == ReadUsartData)		// ff					
		{
			RxBuffer[RxCounter]	= 0xff;
			RxCounter++;
			/*
			if(USART_ReceiveData(USART1) != 0x68)
			{
				RxCounter = 0;
				return;
			}
			RxBuffer[RxCounter ++] 	= 0x68;
			*/
		}
		
		else if(2 == RxCounter && 0x55 == ReadUsartData)		// 55					
		{
			RxBuffer[RxCounter] = 0x55;
			RxCounter++;
		}
		
		else if(RxCounter >= 3 && RxCounter <= 5)			// 显示 53 49 4E  字库 FF 00 AB
		{
			RxBuffer[RxCounter] = ReadUsartData;
			RxCounter++;
			
			//ReturnByte(ReadUsartData);
			
			if(RxCounter >= 6)
			{
				if(0x53 == RxBuffer[3] && 0x49 == RxBuffer[4] && 0x4E == RxBuffer[5])
				{
					//ReturnByte(ReadUsartData);
					do_BZQ_Receive();
				}
				RxCounter = 0;
			}
		}
		
		else
		{
			RxCounter = 0;
		}
	}
}
//
void do_BZQ_Receive(void)
{
	uint16_t length,index16;
	uint8_t  CrcXOR;
	uint16_t  i,j;
	uint32_t addr;
	
	
	UART_Rceive_RXNE();
	if(ReceiveUSART1True && (ID == ReceiveUSART1Data || 0xff == ReceiveUSART1Data) )
	{
		RxBuffer[6] = ReceiveUSART1Data;		//ID
		
		
		
		UART_Rceive_RXNE();
		if( !ReceiveUSART1True ) return;
		RxBuffer[7] = ReceiveUSART1Data;		//CMD
		
		
		
		UART_Rceive_RXNE();						//Length
		if(!ReceiveUSART1True) return;
		RxBuffer[8] = ReceiveUSART1Data;
		length = ReceiveUSART1Data;
		length <<= 8;
		
		UART_Rceive_RXNE();
		if(!ReceiveUSART1True) return;
		RxBuffer[9] = ReceiveUSART1Data;
		length += ReceiveUSART1Data;
		
		
		
		for(i = 0; i < length + 1; i++)			//DATA + crc16
		{
			UART_Rceive_RXNE();
			if(!ReceiveUSART1True) return;
			RxBuffer[i+10] = ReceiveUSART1Data;
		}
		
		//ReturnByte(3);
		//ReturnByte(2);
		//ReturnByte(1);
		
		CrcXOR = 0;						//校验
		for(i = 0; i < 11+length; i++)
		{
			CrcXOR ^= RxBuffer[i];
		}
		//ReturnByte(CrcXOR);
		
		//for(i = 0; i < 11+length; i++)
		//{
		//	ReturnByte(RxBuffer[i]);
		//}
		
		if(CrcXOR) return;
		
		
		
		//储存图片
		if(0xA1 == RxBuffer[7])
		{
			addr = ExternalFlashTupiaoAddr + RxBuffer[10] * 0x1000;
			WREN();
			Sector_Erase(addr);
			Wait_Busy();WRDI();
			//储存数据
			for(index16 = 11; index16 < 10 + length; index16++)
			{
				WREN();
				Byte_Program(RxBuffer[index16], addr++);
				Wait_Busy();
			}
			WRDI();
			ReturnByte(0xaa);
			
			/*
			ClrNSS(); 
			Read_Data(ExternalFlashTupiaoAddr + RxBuffer[10] * 0x1000);
			for(i = 0; i < length; i ++)
			{
				
				ReturnByte(Get_Byte());
				
			}
			SetNSS();
			*/
			
			
			/*
			for(index16 = 11; index16 < 11 + 480; index16++)
			{
				//WREN();
				//Byte_Program(RxBuffer[index16], addr++);
				//Wait_Busy();
				
				temp = RxBuffer[index16];
				for(i = 0; i < 8; i++)
				{
					WREN();
					if(temp & 0x80) Byte_Program(1, addr++);
					else 			Byte_Program(0, addr++);
					temp <<= 1;
					Wait_Busy();
				}
			}
			for( ; index16 < 11 + 960; index16++)
			{
				temp = RxBuffer[index16];
				for(i = 0; i < 8; i++)
				{
					WREN();
					if(temp & 0x80) Byte_Program(2, addr++);
					else 			Byte_Program(0, addr++);
					temp <<= 1;
					Wait_Busy();
				}
			}
			*/
			//WRDI();
		}
		//显示内容
		else if(0xA2 == RxBuffer[7])
		{
			if(RxBuffer[16] > 15) return;
			
			//文字属性
			TextLength      = length - 9;			//文字内容长度（字节）
			TupianIndex = RxBuffer[10];				//图片号
			TextLocation_X  = RxBuffer[11];			//X
			TextLocation_Y  = RxBuffer[12];			//Y
			DisTextPointNum = RxBuffer[13]; 		//文字显示长度（点阵）
			TextColor       = RxBuffer[14];			//文字颜色
			TextMode        = RxBuffer[15]; 		//文字模式
			TextModerGrade  = 16 - RxBuffer[16]; 	//文字模式等级
			TextRemainTime  = RxBuffer[17];			//显示文字停留时间
			TextType		= RxBuffer[18];			//文字样式 00 居中 01 靠左 02 靠右
			
			if(!DisTextPointNum)
			{
				DisTextPointNum = (uint16_t)TextLength * 8;
				
				if(DisTextPointNum + TextLocation_X > 80)
				{
					DisTextPointNum =  80 - TextLocation_X;
				}
				TextPointNum    = 80 - TextLocation_X;
			}
			else 
			{
				if(DisTextPointNum + TextLocation_X > 80)
				{
					DisTextPointNum =  80 - TextLocation_X;
				}
				TextPointNum = DisTextPointNum;				
			}
			
			//加载文字
			for(i = 0; i < TextLength; i++)
			{
				TextBuf[i] = RxBuffer[i + 19];
			}
			
			Updata = 1;
			ReturnByte(0xaa);
		}
		
		//亮度设置
		else if(0xA3 == RxBuffer[7])
		{	
			Light	= RxBuffer[10] * 4 / 3;
			
			
			addr = ExternalFlashLightAddr;
			
			WREN();
			Sector_Erase(addr);
			Wait_Busy();WRDI();
			
			for(i = 0; i < 11 + length; i++)
			{
				WREN();
				Byte_Program(RxBuffer[i],addr++);
				Wait_Busy();
			}
			WRDI();
			ReturnByte(0xaa);
			
		}
		//设置屏号
		else if(0xA4 == RxBuffer[7])
		{
			
			addr = ExternalFlashIDAddr;
			
			WREN();
			Sector_Erase(addr);
			Wait_Busy();WRDI();
			
			for(i = 0; i < 11 + length; i++)
			{
				WREN();
				Byte_Program(RxBuffer[i],addr++);
				Wait_Busy();
			}
			WRDI();
			
			ID		= RxBuffer[10];
			ReturnByte(ID);
		}
		//读取屏号
		else if(0xA5 == RxBuffer[7])
		{
			ReturnByte(ID);
		}
	
	}
}
//接收一个字节
void UART_Rceive_RXNE(void)
{
	uint16_t i;
	
	for(i = 0; i < 0x1000; i++)
	{
		if(USART1->SR&(1<<5))
		{
			ReceiveUSART1Data = USART1->DR;
			
			ReceiveUSART1True = 1;
			return;
		}
	}
	ReceiveUSART1True = 0;
}
//

//

/********************************************************************************
 *									返回码										*
 ********************************************************************************
*/
void ReturnByte(uint8_t byte)
{
	USART_SendData(USART1, byte);	                        //开始码	
	//while(USART_GetFlagStatus(USART1, USART_IT_TXE)==RESET);//等待发完
	while((USART1->SR & 0x40) == 0);
}

//
void clear_displayBuf(void)
{
	uint16_t i,j;
	uint16_t addr;
	uint8_t temp;
	
	for(i = 0; i < 48; i++)
	{
		for(j = 0; j < 80; j++)
		{
			DisplayBuf0[i][j] = 0;
			DisplayBuf1[i][j] = 0;
		}
	}
}
//
void Read_MyID(void)
{
	uint8_t tmp_table[12],i,crc;
	
	ClrNSS();
	Read_Data(ExternalFlashIDAddr);
	for(i = 0; i < 12; i++)
	{
		tmp_table[i] = Get_Byte();
	}
	SetNSS();  
	
	if(0x55 == tmp_table[0] && 0xff == tmp_table[1] && 0x55 == tmp_table[2] && 0x53 == tmp_table[3] &&
		0x49 == tmp_table[4] && 0x4E == tmp_table[5] && 0xa4 == tmp_table[7] ) 
	{
		crc = 0;
		for(i = 0; i < 12; i++ )
		{
			crc ^= tmp_table[i];
		}
		if(!crc)
		{
			ID		= tmp_table[10];
			return;
		}
	}
	ID = 0xff;
}
//
void Read_Light(void)
{
	uint8_t tmp_table[12],i,crc;
	
	ClrNSS();
	Read_Data(ExternalFlashLightAddr);
	for(i = 0; i < 12; i++)
	{
		tmp_table[i] = Get_Byte();
	}
	SetNSS();  
	
	if(0x55 == tmp_table[0] && 0xff == tmp_table[1] && 0x55 == tmp_table[2] && 0x53 == tmp_table[3] &&
		0x49 == tmp_table[4] && 0x4E == tmp_table[5] && 0xa3 == tmp_table[7] ) 
	{
		crc = 0;
		for(i = 0; i < 12; i++ )
		{
			crc ^= tmp_table[i];
		}
		if(!crc)
		{
			Light	= tmp_table[10] * 4 / 3;
			return;
		}
	}
	Light = 20;
}
//
/********************  清屏  *****************************/
void ClearScreen(void)
{
	uint8_t i;

    // 清屏 //
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
    for(i = 0; i < ScreenLength; i++)
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_12);
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();
    }
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
	//
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}
//
void clear_TextBuf(void)
{
	uint8_t i,j;
	
	for(i = 0; i < 16; i++)
	{
		for(j = 0;j < 80; j++)
			DisTextBuf[i][j] = 0;
	}
}
//
void LoadWord_Dis(uint8_t word, uint8_t x, uint8_t y)
{
	uint16_t addr;
	uint8_t  i,j;
	uint8_t  temp;
	
	addr = (uint16_t)(word - 0x20);
	addr *= 16;
	
	for(i = 0; i < 16; i++)
	{
		temp = ziku[addr++];
		for(j = 0; j < 8; j++)
		{
			if(temp & 0x80) DisplayBuf0[y+i][x+j] = 1 + TextColor;
			else            DisplayBuf0[y+i][x+j] = 0;
			temp <<= 1;
		}
	}
}
//
void LoadWord_Text(uint8_t word, uint8_t x)
{
	uint16_t addr;
	uint8_t  i,j;
	uint8_t  temp;
	
	addr = (uint16_t)(word - 0x20);
	addr *= 16;
	
	for(i = 0; i < 16; i++)
	{
		temp = ziku[addr++];
		for(j = 0; j < 8; j++)
		{
			if(temp & 0x80) DisTextBuf[i][x+j] = 1 + TextColor;
			else            DisTextBuf[i][x+j] = 0;
			temp <<= 1;
		}
	}
}
//
void Load_MoveBuf(uint8_t word)
{
	uint16_t addr;
	uint8_t  i,j;
	uint8_t  temp;
	
	addr = (uint16_t)(word - 0x20);
	addr *= 16;
	
	
	for(i = 0; i < 16; i++)
	{
		temp = ziku[addr++];
		for(j = 0; j < 8; j++)
		{
			if(temp & 0x80) MoveBuf[i][j] = 1 + TextColor;
			else            MoveBuf[i][j] = 0; 
			temp <<= 1;
		}
	}
}

//
/********************************************************************************
 *									END_FILE									*
 ********************************************************************************
*/



