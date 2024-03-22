#include "main.h"
#include "ModBusDev.h"

u16 Rx1_Timer  = 0;
u16 Rx2_Timer  = 0;

u8  TX1_Cnt;    //���ͼ���
u8  RX1_Cnt;    //���ռ���
bit B_TX1_Busy; //����æ��־

u8  TX2_Cnt;    //���ͼ���
u8  RX2_Cnt;    //���ռ���
bit B_TX2_Busy; //����æ��־

u8  RX1_Buffer[MAX_LENGTH]; //���ջ���
u8  RX2_Buffer[MAX_LENGTH]; //���ջ���

void UART1_config()
{
    /*********** ������ʹ�ö�ʱ��1 *****************/
    {
        TR1 = 0;
        AUXR &= ~0x01;      //S1 BRT Use Timer1;
        AUXR |=  (1<<6);    //Timer1 set as 1T mode
        TMOD &= ~(1<<6);    //Timer1 set As Timer
        TMOD &= ~0x30;      //Timer1_16bitAutoReload;
        TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3) / 256);
        TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3) % 256);
        ET1 = 0;    //��ֹ�ж�
        INTCLKO &= ~0x02;  //�����ʱ��
        TR1  = 1;
    }
  
    /*************************************************/
    //UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 
    //           0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨���� 

    SCON = (SCON & 0x3f) | 0x40; 

    PS  = 1;    //�жϸ����ȼ�
    PSH = 0;
    //PS  = 1;    //�����ȼ��ж�
    ES  = 1;    //�����ж�
    REN = 1;    //�������
    
    //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 
    //                 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
    P_SW1 &= 0x3f;
    P_SW1 |= 0x40;  
    
    B_TX1_Busy = 0;
    TX1_Cnt = 0;
    RX1_Cnt = 0;
}

void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;
        Rx1_Timer = 0;
        RX1_Buffer[RX1_Cnt] = SBUF;
        
        if(++RX1_Cnt >= MAX_LENGTH)   
        {
            RX1_Cnt = 0;
        }
    }

    if(TI)
    {
        TI = 0;
        B_TX1_Busy = 0;
    }
}

void Uart1Send(u8 *buf, u8 len)
{
    u8 i;
    RS485_EN(0);
    for (i=0;i<len;i++)     
    {
        SBUF = buf[i];
        B_TX1_Busy = 1;
        while(B_TX1_Busy);
    }
    RS485_EN(1);
}

//�ض���Printf
char putchar(char c )
{
    RS485_EN(0);
    SBUF = c;
    while(!TI);
    TI = 0;
    RS485_EN(1);
    return c;
}


void Uart1Hnd()
{
    BYTE i = 0;
    if (Rx1_Timer > 20)
    {
        Rx1_Timer = 0;
        //printf("ok\r\n");
        //Error();
        Remflag = false;
        RemOut = false;
        RemOutTime = 0;
        HndModBusRecv(RX1_Buffer,RX1_Cnt);
        ClearUart1Buf();
    }
    else
    {
        Remflag = true;
    }

}

void ClearUart1Buf()
{
    memset(RX1_Buffer,0,MAX_LENGTH);
    RX1_Cnt = 0;
}

