#include "display.h"



void Display(void)
{
}
//
//

void MoveDisplay(void)
{
	/*
	uint8_t  i,j,temp;
	uint8_t  h,l;
	uint16_t x;
	uint8_t  *p,*pp;
	
	if(DisplayIndex)  p = DisplayBuf0, pp = DisplayBuf1;
	else                  p = DisplayBuf1, pp = DisplayBuf0;
	
	if(!MoveIndex)
	{
		if(MovetextNum < PromptTextLength)
		{
			if(!Move_LoadGB)
			{
				h = PromptTextBuf[MovetextNum];
				l = PromptTextBuf[MovetextNum + 1];
				//
				if(h < 0xA1)
				{
					LoadChar(MoveBuf, (uint16_t)h);
				}
				else
				{
					LoadChar(MoveBuf, ((uint16_t)h << 8) | l);
					Move_LoadGB = 1;
				}
			}
			else
			{
				Move_LoadGB = 0;
				for(i = 0; i < 16; i++)
				{
					MoveBuf[i] = MoveBuf[i + 16];
				}
			}
		}
		else
		{
			for(i = 0; i < 16; i++)
				MoveBuf[i] = 0xFF;
		}
	}
	
	//Move_Left(p, ScreenSize + 2);
	for(i = 0; i < 16; i++)
	{
		x = 0x700 + i;
		for(j = 0; j < ScreenLength - 1; j++)
		{
			*(p + x) <<= 1;
			//*(p + x) |= ((*(p + x + 16) & 0x01) >> 7);
			temp = (*(p + x + 16) & 0x80) ? 1 : 0;
			*(p + x) |= temp;
			x += 16;
		}
		*(p + x) <<= 1;
		temp = (MoveBuf[i] & 0x80) ? 1 : 0;
		*(p + x) |= temp;
		
		MoveBuf[i] <<= 1;
	}

	//
	if(++MoveIndex >= 8)
	{
		MoveIndex = 0;
		if(++MovetextNum >= (PromptTextLength + 15))
			MovetextNum = 0;
	}
	
	if(DisplayIndex) 	DisplayIndex = 0;
	else					DisplayIndex = 1; 
	
	for(x = 0x700; x < 0x800; x++) *(pp + x) = *(p + x);
	
	MoveBit_Finish = 1;
	*/
}

//***********************************************//
