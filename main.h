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
      
    BYTE  Enable;                               // 通道使能：1位指示一个通道
    //BYTE  LightEnable;
    //BYTE  Valve[KEY_NUM];                   // 电磁阀
    bool  RemCtlFlag;                           //远程控制标志
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
//    float Volume[CHANNLE_NUM];                  // 取样体积 
//    float TotleFlow;                            // 总流量
//    float TotleVol;                             // 总取样体积
}RUN_STATUS;

typedef struct
{
    WORD SypAddr;
    WORD StuAddr;
    WORD InfoAddr;
}
REM_REGADDR;



#define MAIN_Fosc       11059200UL              // 11.0592M

#define SysTick         9216                    // 10ms    // 次/秒, 系统滴答频率, 在4000~16000之间

#define Timer0_Reload   (65536UL - SysTick)     //Timer 0 中断频率

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


#define MODE_TIME  1                // 定时取样 
#define MODE_VOL   2                // 定量取样
#define MODE_MAN   3                // 手动取样


// output port 

#define ALM_CH_DISABLE    0
#define ALM_FLOW_NOR      1
#define ALM_FLOW_LOW      2
#define ALM_FLOW_HIGH     3
#define ALM_FLOW_ABNOR    4

#define LED_FLASH_TIME    500       // ms

#define RUN_LED(x) (x)?(P1 |= (1<<3)):(P1 &= ~(1<<3))     // 板载LED

// 指示灯

#define LIGHT0_OFF  LIGHT0_GRE(0);LIGHT0_YEL(0);LIGHT0_RED(0)       //关闭三色灯
#define LIGHT1_OFF  LIGHT1_GRE(0);LIGHT1_YEL(0);LIGHT1_RED(0)
#define LIGHT2_OFF  LIGHT2_GRE(0);LIGHT2_YEL(0);LIGHT2_RED(0)
#define LIGHT3_OFF  LIGHT3_GRE(0);LIGHT3_YEL(0);LIGHT3_RED(0)
#define LIGHT4_OFF  LIGHT4_GRE(0);LIGHT4_YEL(0);LIGHT4_RED(0)
#define LIGHT5_OFF  LIGHT5_GRE(0);LIGHT5_YEL(0);LIGHT5_RED(0)
#define LIGHT6_OFF  LIGHT6_GRE(0);LIGHT6_YEL(0);LIGHT6_RED(0)
#define LIGHT7_OFF  LIGHT7_GRE(0);LIGHT7_YEL(0);LIGHT7_RED(0)

#define LIGHT0_GREON  LIGHT0_GRE(1);LIGHT0_YEL(0);LIGHT0_RED(0)       //绿色灯亮
#define LIGHT0_YELON  LIGHT0_GRE(0);LIGHT0_YEL(1);LIGHT0_RED(0)       //黄色灯亮
#define LIGHT0_REDON  LIGHT0_GRE(0);LIGHT0_YEL(0);LIGHT0_RED(1)       //红色灯亮

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



#define LIGHT0_GRE(x)   (x)?(P5 |=(1<<2)):(P5 &= ~(1<<2))       //三色灯0绿灯控制
#define LIGHT0_YEL(x)   (x)?(P0 |=(1<<4)):(P0 &= ~(1<<4))       //三色灯0黄灯控制
#define LIGHT0_RED(x)   (x)?(P0 |=(1<<3)):(P0 &= ~(1<<3))       //三色灯0红灯控制

#define LIGHT1_GRE(x)   (x)?(P0 |=(1<<2)):(P0 &= ~(1<<2))       //三色灯1绿灯控制
#define LIGHT1_YEL(x)   (x)?(P0 |=(1<<1)):(P0 &= ~(1<<1))       //三色灯1黄灯控制
#define LIGHT1_RED(x)   (x)?(P0 |=(1<<0)):(P0 &= ~(1<<0))       //三色灯1红灯控制

#define LIGHT2_GRE(x)   (x)?(P4 |=(1<<6)):(P4 &= ~(1<<6))       //三色灯2绿灯控制
#define LIGHT2_YEL(x)   (x)?(P4 |=(1<<5)):(P4 &= ~(1<<5))       //三色灯2黄灯控制
#define LIGHT2_RED(x)   (x)?(P7 |=(1<<7)):(P7 &= ~(1<<7))       //三色灯2红灯控制

#define LIGHT3_GRE(x)   (x)?(P7 |=(1<<6)):(P7 &= ~(1<<6))       //三色灯3绿灯控制
#define LIGHT3_YEL(x)   (x)?(P7 |=(1<<5)):(P7 &= ~(1<<5))       //三色灯3黄灯控制
#define LIGHT3_RED(x)   (x)?(P7 |=(1<<4)):(P7 &= ~(1<<4))       //三色灯3红灯控制

#define LIGHT4_GRE(x)   (x)?(P2 |=(1<<7)):(P2 &= ~(1<<7))       //三色灯4绿灯控制
#define LIGHT4_YEL(x)   (x)?(P2 |=(1<<6)):(P2 &= ~(1<<6))       //三色灯4黄灯控制
#define LIGHT4_RED(x)   (x)?(P2 |=(1<<5)):(P2 &= ~(1<<5))       //三色灯4红灯控制

#define LIGHT5_GRE(x)   (x)?(P2 |=(1<<4)):(P2 &= ~(1<<4))       //三色灯5绿灯控制
#define LIGHT5_YEL(x)   (x)?(P2 |=(1<<3)):(P2 &= ~(1<<3))       //三色灯5黄灯控制
#define LIGHT5_RED(x)   (x)?(P2 |=(1<<2)):(P2 &= ~(1<<2))       //三色灯5红灯控制

#define LIGHT6_GRE(x)   (x)?(P2 |=(1<<1)):(P2 &= ~(1<<1))       //三色灯6绿灯控制
#define LIGHT6_YEL(x)   (x)?(P2 |=(1<<0)):(P2 &= ~(1<<0))       //三色灯6黄灯控制
#define LIGHT6_RED(x)   (x)?(P4 |=(1<<4)):(P4 &= ~(1<<4))       //三色灯6红灯控制

#define LIGHT7_GRE(x)   (x)?(P4 |=(1<<3)):(P4 &= ~(1<<3))       //三色灯7绿灯控制
#define LIGHT7_YEL(x)   (x)?(P4 |=(1<<2)):(P4 &= ~(1<<2))       //三色灯7黄灯控制
#define LIGHT7_RED(x)   (x)?(P4 |=(1<<1)):(P4 &= ~(1<<1))       //三色灯7红灯控制

#define STOP_M()   (P1 & (1<<0))                           //启停控制

#define RS485_EN(x)  (x)?(P3 |= (1<<5)):(P3 &= ~(1<<5)) 
#define ALARM(x)    (x)?(P1 |= (1<<1)):(P1 &= ~(1<<1))          // 报警音

#define KEY0()   (P5 & (1<<3))             //按键0控制
#define KEY1()   (P0 & (1<<5))             //按键1控制
#define KEY2()   (P0 & (1<<6))             //按键2控制
#define KEY3()   (P0 & (1<<7))             //按键3控制
#define KEY4()   (P6 & (1<<0))             //按键4控制
#define KEY5()   (P6 & (1<<1))             //按键5控制
#define KEY6()   (P6 & (1<<2))             //按键6控制
#define KEY7()   (P6 & (1<<3))             //按键7控制


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

