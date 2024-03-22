#ifndef __UART_H__
#define __UART_H__

#define Baudrate   115200UL
#define Baudrate3   9600UL

#define MAX_LENGTH             200



extern u8  TX1_Cnt;    //发送计数
extern u8  RX1_Cnt;    //接收计数
extern bit B_TX1_Busy; //发送忙标志
extern u16 Rx1_Timer;

//extern u8  TX2_Cnt;    //发送计数
//extern u8  RX2_Cnt;    //接收计数
//extern bit B_TX2_Busy; //发送忙标志
//extern u16 Rx2_Timer;

//extern u8  xdata RX2_Buffer[]; //接收缓冲

void UART1_config();
//void UART2_config();


void Uart1Send(u8 *buf, u8 len);
//void Uart2Send(u8 *buf, u8 len);


void ClearUart1Buf();
//void ClearUart2Buf();


void Uart1Hnd();
//void Uart2Hnd();

#endif