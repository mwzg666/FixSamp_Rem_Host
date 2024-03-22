#ifndef __MAIN_H__
#define __MAIN_H__

#include "STC32G.h"

#include "stdio.h"
#include "intrins.h"
#include "string.h"

//#define  FOR_DEBUG

typedef     unsigned char    BOOL;
#define     TRUE    1
#define     FALSE   0

typedef     unsigned char    BYTE;
typedef     unsigned short    WORD;
typedef     unsigned long    DWORD;

typedef     unsigned char    u8;
typedef     unsigned short    u16;
typedef     unsigned long    u32;

typedef     unsigned int    uint;


#define alt_u8   BYTE
#define alt_u16 WORD
#define alt_u32 DWORD  


#define bool BYTE
#define true 1
#define false 0

#define BOOL BYTE
#define TRUE 1
#define FALSE 0

#define SENSOR_COUNT 4

#define CHANNLE_NUM  8
#define KEY_NUM 9

#include "uart.h"
#include "EepRom.h"
#include "ModBus.h"

typedef struct
{
    BYTE  Address;
    BYTE  res1;

    WORD  res2;
      
    BYTE  Enable;                               // ͨ��ʹ�ܣ�1λָʾһ��ͨ��
    //BYTE  LightEnable;
    //BYTE  Valve[KEY_NUM];                   // ��ŷ�
    bool  RemCtlFlag;                           //Զ�̿��Ʊ�־
    BYTE  Version;
    WORD  Sum;
}SYS_PARAM;

typedef struct
{
    bool  Running;
    bool  SlaveRun;
    DWORD RunTime;
    WORD  Alarm[8];
//    float Flow[CHANNLE_NUM];
//    float Volume[CHANNLE_NUM];                  // ȡ����� 
//    float TotleFlow;                            // ������
//    float TotleVol;                             // ��ȡ�����
}RUN_STATUS;

typedef struct
{
    WORD SypAddr;
    WORD StuAddr;
    WORD InfoAddr;
}
REM_REGADDR;



#define MAIN_Fosc       11059200UL              // 11.0592M

#define SysTick         9216                    // 10ms    // ��/��, ϵͳ�δ�Ƶ��, ��4000~16000֮��

#define Timer0_Reload   (65536UL - SysTick)     //Timer 0 �ж�Ƶ��

#define OUT_IO_COUNT      9

typedef enum
{
    KEY_0 = 0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_STOP
}KEY_ID;

typedef enum
{
    OFF_LIGHT = 0,GRE_LIGHT,YEL_LIGHT,RED_LIGHT
}LIGHT_STATUS;

typedef enum
{
    LIGHT0 = 0,LIGHT1,LIGHT2,LIGHT3,LIGHT4,LIGHT5,LIGHT6,LIGHT7,ALARM8
}LIGHT_ALM_ID;


#define MODE_TIME  1                // ��ʱȡ�� 
#define MODE_VOL   2                // ����ȡ��
#define MODE_MAN   3                // �ֶ�ȡ��


// output port 

#define ALM_CH_DISABLE    0
#define ALM_FLOW_NOR      1
#define ALM_FLOW_LOW      2
#define ALM_FLOW_HIGH     3
#define ALM_FLOW_ABNOR    4

#define LED_FLASH_TIME    500       // ms

#define RUN_LED(x) (x)?(P1 |= (1<<3)):(P1 &= ~(1<<3))     // ����LED

// ָʾ��

#define LIGHT0_OFF  LIGHT0_GRE(0);LIGHT0_YEL(0);LIGHT0_RED(0)       //�ر���ɫ��
#define LIGHT1_OFF  LIGHT1_GRE(0);LIGHT1_YEL(0);LIGHT1_RED(0)
#define LIGHT2_OFF  LIGHT2_GRE(0);LIGHT2_YEL(0);LIGHT2_RED(0)
#define LIGHT3_OFF  LIGHT3_GRE(0);LIGHT3_YEL(0);LIGHT3_RED(0)
#define LIGHT4_OFF  LIGHT4_GRE(0);LIGHT4_YEL(0);LIGHT4_RED(0)
#define LIGHT5_OFF  LIGHT5_GRE(0);LIGHT5_YEL(0);LIGHT5_RED(0)
#define LIGHT6_OFF  LIGHT6_GRE(0);LIGHT6_YEL(0);LIGHT6_RED(0)
#define LIGHT7_OFF  LIGHT7_GRE(0);LIGHT7_YEL(0);LIGHT7_RED(0)

#define LIGHT0_GREON  LIGHT0_GRE(1);LIGHT0_YEL(0);LIGHT0_RED(0)       //��ɫ����
#define LIGHT0_YELON  LIGHT0_GRE(0);LIGHT0_YEL(1);LIGHT0_RED(0)       //��ɫ����
#define LIGHT0_REDON  LIGHT0_GRE(0);LIGHT0_YEL(0);LIGHT0_RED(1)       //��ɫ����

#define LIGHT1_GREON  LIGHT1_GRE(1);LIGHT1_YEL(0);LIGHT1_RED(0)
#define LIGHT1_YELON  LIGHT1_GRE(0);LIGHT1_YEL(1);LIGHT1_RED(0)       
#define LIGHT1_REDON  LIGHT1_GRE(0);LIGHT1_YEL(0);LIGHT1_RED(1)

#define LIGHT2_GREON  LIGHT2_GRE(1);LIGHT2_YEL(0);LIGHT2_RED(0)
#define LIGHT2_YELON  LIGHT2_GRE(0);LIGHT2_YEL(1);LIGHT2_RED(0)
#define LIGHT2_REDON  LIGHT2_GRE(0);LIGHT2_YEL(0);LIGHT2_RED(1)

#define LIGHT3_GREON  LIGHT3_GRE(1);LIGHT3_YEL(0);LIGHT3_RED(0)       
#define LIGHT3_YELON  LIGHT3_GRE(0);LIGHT3_YEL(1);LIGHT3_RED(0)
#define LIGHT3_REDON  LIGHT3_GRE(0);LIGHT3_YEL(0);LIGHT3_RED(1)

#define LIGHT4_GREON  LIGHT4_GRE(1);LIGHT4_YEL(0);LIGHT4_RED(0)       
#define LIGHT4_YELON  LIGHT4_GRE(0);LIGHT4_YEL(1);LIGHT4_RED(0)
#define LIGHT4_REDON  LIGHT4_GRE(0);LIGHT4_YEL(0);LIGHT4_RED(1)

#define LIGHT5_GREON  LIGHT5_GRE(1);LIGHT5_YEL(0);LIGHT5_RED(0)   
#define LIGHT5_YELON  LIGHT5_GRE(0);LIGHT5_YEL(1);LIGHT5_RED(0)
#define LIGHT5_REDON  LIGHT5_GRE(0);LIGHT5_YEL(0);LIGHT5_RED(1)

#define LIGHT6_GREON  LIGHT6_GRE(1);LIGHT6_YEL(0);LIGHT6_RED(0)    
#define LIGHT6_YELON  LIGHT6_GRE(0);LIGHT6_YEL(1);LIGHT6_RED(0)
#define LIGHT6_REDON  LIGHT6_GRE(0);LIGHT6_YEL(0);LIGHT6_RED(1)

#define LIGHT7_GREON  LIGHT7_GRE(1);LIGHT7_YEL(0);LIGHT7_RED(0)       
#define LIGHT7_YELON  LIGHT7_GRE(0);LIGHT7_YEL(1);LIGHT7_RED(0)
#define LIGHT7_REDON  LIGHT7_GRE(0);LIGHT7_YEL(0);LIGHT7_RED(1)



#define LIGHT0_GRE(x)   (x)?(P5 |=(1<<2)):(P5 &= ~(1<<2))       //��ɫ��0�̵ƿ���
#define LIGHT0_YEL(x)   (x)?(P0 |=(1<<4)):(P0 &= ~(1<<4))       //��ɫ��0�Ƶƿ���
#define LIGHT0_RED(x)   (x)?(P0 |=(1<<3)):(P0 &= ~(1<<3))       //��ɫ��0��ƿ���

#define LIGHT1_GRE(x)   (x)?(P0 |=(1<<2)):(P0 &= ~(1<<2))       //��ɫ��1�̵ƿ���
#define LIGHT1_YEL(x)   (x)?(P0 |=(1<<1)):(P0 &= ~(1<<1))       //��ɫ��1�Ƶƿ���
#define LIGHT1_RED(x)   (x)?(P0 |=(1<<0)):(P0 &= ~(1<<0))       //��ɫ��1��ƿ���

#define LIGHT2_GRE(x)   (x)?(P4 |=(1<<6)):(P4 &= ~(1<<6))       //��ɫ��2�̵ƿ���
#define LIGHT2_YEL(x)   (x)?(P4 |=(1<<5)):(P4 &= ~(1<<5))       //��ɫ��2�Ƶƿ���
#define LIGHT2_RED(x)   (x)?(P7 |=(1<<7)):(P7 &= ~(1<<7))       //��ɫ��2��ƿ���

#define LIGHT3_GRE(x)   (x)?(P7 |=(1<<6)):(P7 &= ~(1<<6))       //��ɫ��3�̵ƿ���
#define LIGHT3_YEL(x)   (x)?(P7 |=(1<<5)):(P7 &= ~(1<<5))       //��ɫ��3�Ƶƿ���
#define LIGHT3_RED(x)   (x)?(P7 |=(1<<4)):(P7 &= ~(1<<4))       //��ɫ��3��ƿ���

#define LIGHT4_GRE(x)   (x)?(P2 |=(1<<7)):(P2 &= ~(1<<7))       //��ɫ��4�̵ƿ���
#define LIGHT4_YEL(x)   (x)?(P2 |=(1<<6)):(P2 &= ~(1<<6))       //��ɫ��4�Ƶƿ���
#define LIGHT4_RED(x)   (x)?(P2 |=(1<<5)):(P2 &= ~(1<<5))       //��ɫ��4��ƿ���

#define LIGHT5_GRE(x)   (x)?(P2 |=(1<<4)):(P2 &= ~(1<<4))       //��ɫ��5�̵ƿ���
#define LIGHT5_YEL(x)   (x)?(P2 |=(1<<3)):(P2 &= ~(1<<3))       //��ɫ��5�Ƶƿ���
#define LIGHT5_RED(x)   (x)?(P2 |=(1<<2)):(P2 &= ~(1<<2))       //��ɫ��5��ƿ���

#define LIGHT6_GRE(x)   (x)?(P2 |=(1<<1)):(P2 &= ~(1<<1))       //��ɫ��6�̵ƿ���
#define LIGHT6_YEL(x)   (x)?(P2 |=(1<<0)):(P2 &= ~(1<<0))       //��ɫ��6�Ƶƿ���
#define LIGHT6_RED(x)   (x)?(P4 |=(1<<4)):(P4 &= ~(1<<4))       //��ɫ��6��ƿ���

#define LIGHT7_GRE(x)   (x)?(P4 |=(1<<3)):(P4 &= ~(1<<3))       //��ɫ��7�̵ƿ���
#define LIGHT7_YEL(x)   (x)?(P4 |=(1<<2)):(P4 &= ~(1<<2))       //��ɫ��7�Ƶƿ���
#define LIGHT7_RED(x)   (x)?(P4 |=(1<<1)):(P4 &= ~(1<<1))       //��ɫ��7��ƿ���

#define STOP_M()   (P1 & (1<<0))                           //��ͣ����

#define RS485_EN(x)  (x)?(P3 |= (1<<5)):(P3 &= ~(1<<5)) 
#define ALARM(x)    (x)?(P1 |= (1<<1)):(P1 &= ~(1<<1))          // ������

#define KEY0()   (P5 & (1<<3))             //����0����
#define KEY1()   (P0 & (1<<5))             //����1����
#define KEY2()   (P0 & (1<<6))             //����2����
#define KEY3()   (P0 & (1<<7))             //����3����
#define KEY4()   (P6 & (1<<0))             //����4����
#define KEY5()   (P6 & (1<<1))             //����5����
#define KEY6()   (P6 & (1<<2))             //����6����
#define KEY7()   (P6 & (1<<3))             //����7����


extern SYS_PARAM xdata SysParam;
extern RUN_STATUS xdata RunStatus;
extern REM_REGADDR xdata RemRegAddr;

extern float SimFlow;
extern alt_u8 g_Output[OUT_IO_COUNT];
extern BYTE  Valve[CHANNLE_NUM];
extern bool Remflag;
extern bool RemOut;
extern u16 RemOutTime;

void WriteParam();
void Delay(WORD ms);
void delay_ms(u16 ms);

void Error();
void LightOutCtl(alt_u8 id, alt_u8 st);

void ParamDef();

void SaveParam();

void CloseKey();
void ScanKey();
void GetChannel();
void StartStop();
void RemCtlRead();
void RemCtlWrite();


void DevRun();
void SyncModBusDev();

void DebugMsg(char *msg);
void DebugInt(int msg);
void DumpCmd(BYTE *dat, BYTE len);

#endif

