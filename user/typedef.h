#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef unsigned char   INT8U;         /*  Unsigned  8 bit quantity    */                    
typedef signed   char   INT8S;         /*  Signed    8 bit quantity    */                    
typedef unsigned short  INT16U;        /*  Unsigned 16 bit quantity    */
typedef signed   short  INT16S;        /*  Signed   16 bit quantity    */
typedef unsigned int    INT32U;        /*  Unsigned 32 bit quantity    */
typedef signed   int    INT32S;        /*  Signed   32 bit quantity    */
typedef float           FP32;          /*  Single precision floating point */

typedef struct
{
    INT8U  State;
    INT8U  Addr;
    INT8U  RBuf[1024];
    INT8U  TBuf[32];
	INT8U  ACK;
	INT8U  RPacketID;
	INT8U  RLastPacketID;
    INT16U RLength;
    INT16U TLength;
    INT16U TIndex;
    INT16U Index1;
    INT16U Index2;
    INT16U Index3;
    INT16U Index4;
    INT32U TimeOut;
    INT16U CheckSum;
} UARTStruct;

typedef struct
{
	INT8U  State;
	INT8U  TextBuf[520];
	INT8U  MoveBuf[512];
	INT8U  Speed;
    INT8U  SpeedTemp;
    INT16U Pause;
    INT32U PauseTemp;
    INT8U  Color;
    INT16U TextSize;
    INT16U TextIndex;
    INT16U MoveCnt0;
    INT16U MoveCnt1;
    INT16U MoveCnt2;
    INT8U  MoveMode;
	INT16U X;
	INT16U Y;
	INT16U W;
	INT16U H;
} WindowStruct;
/*
typedef struct
{
    INT8U Update;
    INT8U Year;
    INT8U Month;
    INT8U Date;
    INT8U Day;
    INT8U Hour;
    INT8U Minute;
    INT8U Second;
} RTCStruct;
*/
typedef struct
{
	INT8U Busy;
    INT8U Update;
    INT8U Year;
    INT8U Month;
    INT8U Date;
    INT8U Day;
    INT8U Hour;
    INT8U Minute;
    INT8U Second;
} RTCStruct;
#endif /* __TYPEDEF_H__ */ 
