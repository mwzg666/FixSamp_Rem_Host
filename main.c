#include "main.h"
#include "ModBus.h"
#include "ModBusDev.h"

BYTE code VERSION = 100;  // V1.0.0

BYTE xdata StrTmp[64] = {0};
BYTE  Valve[CHANNLE_NUM] = {0}; 

#define PARAM_SIGN  0x3132
SYS_PARAM xdata SysParam;
RUN_STATUS xdata RunStatus;
REM_REGADDR xdata RemRegAddr;

alt_u8 g_Output[OUT_IO_COUNT]      = {0,0,0,0,0,0,0,0,0};   
alt_u8 g_OutStatus[OUT_IO_COUNT]   = {0,0,0,0,0,0,0,0,0};

u16 RemWriteTim = 0;    //远程控制写从机开始时间
BYTE RemWriteFlag = 0;  //远程控制写从机开始标志

u16 RemReadTim = 0;     //远程控制读从机开始时间
BYTE RemReadflag = 0;   //远程控制读从机开始标志

u16  Timer0Cnt = 0;

bool Remflag = false;
u16  RemOutTime = 0;    //远程控制断开连接时间    
bool RemOut = false;   //远程控制确认断开
bool StopFlag = false;

bool AlmOn = false;
WORD RunPHtim = 0;



void Error()
{
    while(1)
    {
        RUN_LED(1);
        Delay(50);
        RUN_LED(0);
        Delay(50);
    }
    
}

void SysInit()
{
    HIRCCR = 0x80;           // 启动内部高速IRC
    while(!(HIRCCR & 1));
    CLKSEL = 0;                
}

void IoInit()
{
    EAXFR = 1;
    WTST = 0x00;   //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快

    P0M1 = 0x00;   P0M0 = 0x1F;                        
    P1M1 = 0x00;   P1M0 |= (1<<1);               //设置为准双向口
    P2M1 = 0x00;   P2M0 = 0xFF;                   // P2 推挽输出
    P3M1 = 0x00;   P3M0 |= (1<<5);                      
    P4M1 = 0x00;   P4M0 = 0x7E;                        
    P5M1 = 0x00;   P5M0 |= (1<<2);              
    P6M1 = 0x00;   P6M0 = 0x00;                       
    P7M1 = 0x00;   P7M0 = 0xF0;  
   
}

void Timer0Init()
{
    AUXR = 0x00;    //Timer0 set as 12T, 16 bits timer auto-reload, 
    TH0 = (u8)(Timer0_Reload / 256);
    TL0 = (u8)(Timer0_Reload % 256);
    ET0 = 1;    //Timer0 interrupt enable
    TR0 = 1;    //Tiner0 run
    
    // 中断优先级3
    PT0  = 0;
    PT0H = 0;
}

// 10ms 中断一下
void Timer0Int (void) interrupt 1
{
    Timer0Cnt ++;
}

void CH_check()
{
    BYTE i = 0;
    for(i = 0;i < CHANNLE_NUM;i++)
    {
        if(Valve[i])
        {
            g_Output[i] = GRE_LIGHT; 
            //RunStatus.Alarm[i] = ALM_FLOW_NOR;
        }
        else
        {
            g_Output[i] = OFF_LIGHT;
            //RunStatus.Alarm[i] = ALM_CH_DISABLE;
        }
    }
    
}

void Alm_ON()
{
    if(AlmOn)
    {
        g_Output[ALARM8] = 4;
    }
    else
    {
        g_Output[ALARM8] = 0;
    }
}

//========================================================================
// 函数名称: void Alm_Check()
// 函数功能: 报警条件检测
// 入口参数: @无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023
// 当前作者:
// 其他备注: 
//=======================================================================

void Alm_Check()
{
    int i = 0;   
    for (i = 0;i < CHANNLE_NUM;i++)
    {
        if((RunStatus.Alarm[i]==ALM_FLOW_ABNOR)||(RunStatus.Alarm[i]==ALM_FLOW_HIGH)||(RunStatus.Alarm[i]==ALM_FLOW_LOW))
        {
            AlmOn = true;
            break;
        } 
        else
        {
            AlmOn = false;
        }
    }  
}


//========================================================================
// 函数名称: void LightAlm_Check()
// 函数功能: LED灯检查
// 入口参数: @无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023
// 当前作者:
// 其他备注: 
//========================================================================
void Light_Check()
{
    int i = 0;
    static BYTE RunLightFlag = 0;
    for(i = 0;i<CHANNLE_NUM;i++)
    { 
        switch(RunStatus.Alarm[i])
        {
                case ALM_CH_DISABLE:  g_Output[i] = OFF_LIGHT; break;
                case ALM_FLOW_NOR:    g_Output[i] = GRE_LIGHT; break;
                
                case ALM_FLOW_ABNOR:    
                case ALM_FLOW_HIGH:     
                case ALM_FLOW_LOW:    g_Output[i] = YEL_LIGHT; break;
                
                default:g_Output[i] = GRE_LIGHT;break;
        }           
    }
    Alm_Check();
}


//========================================================================
// 函数名称: void LightOutCtl(alt_u8 id, alt_u8 st)
// 函数功能: LED灯输出
// 入口参数: @alt_u8 id：灯的ID号 @alt_u8 st：灯的当前状态
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023
// 当前作者:
// 其他备注: 
//========================================================================
void LightAlmOutCtl(alt_u8 id, alt_u8 st)
{  
    if (g_OutStatus[id] == st)
    {
        return;
    }
    g_OutStatus[id] = st;
    
    switch(id)
    {   
        
        case LIGHT0: 
        {
            switch(st) 
            {
                case OFF_LIGHT: LIGHT0_OFF;     break;
                case GRE_LIGHT: LIGHT0_GREON;   break;
                case YEL_LIGHT: LIGHT0_YELON;   break;
                case RED_LIGHT: LIGHT0_REDON;   break;
                
            }
            break;
        }

        case LIGHT1: 
        {
            switch(st) 
            {
                case OFF_LIGHT: LIGHT1_OFF;    break;
                case GRE_LIGHT: LIGHT1_GREON;  break;
                case YEL_LIGHT: LIGHT1_YELON;  break;
                case RED_LIGHT: LIGHT1_REDON;  break;
                
            }
            break;
        }
        
        case LIGHT2: 
        {
            switch(st) 
            {
                case OFF_LIGHT: LIGHT2_OFF;    break;
                case GRE_LIGHT: LIGHT2_GREON;  break;
                case YEL_LIGHT: LIGHT2_YELON;  break;
                case RED_LIGHT: LIGHT2_REDON;  break;
                
            }
            break;
        }

        case LIGHT3: 
        {
            switch(st) 
            {
                case OFF_LIGHT: LIGHT3_OFF;    break;
                case GRE_LIGHT: LIGHT3_GREON;  break;
                case YEL_LIGHT: LIGHT3_YELON;  break;
                case RED_LIGHT: LIGHT3_REDON;  break;
                
            }
            break;
        }

        case LIGHT4: 
        {
            switch(st) 
            {
                case OFF_LIGHT: LIGHT4_OFF;    break;            
                case GRE_LIGHT: LIGHT4_GREON;  break;
                case YEL_LIGHT: LIGHT4_YELON;  break;
                case RED_LIGHT: LIGHT4_REDON;  break;
                
            }
            break;
        }

        case LIGHT5: 
        {
            switch(st) 
            {
                case OFF_LIGHT: LIGHT5_OFF;    break;
                case GRE_LIGHT: LIGHT5_GREON;  break;
                case YEL_LIGHT: LIGHT5_YELON;  break;
                case RED_LIGHT: LIGHT5_REDON;  break;
                
            }
            break;
        }
        
        case LIGHT6: 
        {
            switch(st) 
            {
                case OFF_LIGHT: LIGHT6_OFF;    break;
                case GRE_LIGHT: LIGHT6_GREON;  break;
                case YEL_LIGHT: LIGHT6_YELON;  break;
                case RED_LIGHT: LIGHT6_REDON;  break;
                
            }
            break;
        }

        case LIGHT7: 
        {
            switch(st) 
            {            
                case OFF_LIGHT: LIGHT7_OFF;    break;
                case GRE_LIGHT: LIGHT7_GREON;  break;
                case YEL_LIGHT: LIGHT7_YELON;  break;
                case RED_LIGHT: LIGHT7_REDON;  break;
                default:break;
                
            }
            break;
        }
        case ALARM8: 
        {
           (st)? ALARM(1) : ALARM(0);       
            break;
        }
        default:break;

    }
}

void LightAlmOutFlash(alt_u8 id)
{
    static alt_u16 OutTimer[OUT_IO_COUNT] = {0,0,0,0,0,0,0,0,0};
    if (OutTimer[id] ++ > LED_FLASH_TIME/10)
    {
        OutTimer[id] = 0;
        if (g_OutStatus[id] == 1)
        {
            LightAlmOutCtl(id, 0);
        }
        else
        {
            LightAlmOutCtl(id, 1);
        }
    }
}


void IoCtlTask()
{
    alt_u8 i;
    for (i=0;i<OUT_IO_COUNT;i++)
    {
        if (g_Output[i] == 4)
        {
            LightAlmOutFlash(i);
        }
        else
        {
            LightAlmOutCtl(i, g_Output[i]);
        }
        
    }
}

// 板载指示灯
void RunLed(u16 dt)
{   
    static u16 tm = 0;
    u16 to = 3000;
    tm += dt;
    if (tm > to)
    {
        tm = 0;
        RUN_LED(0);
    }
    else if (tm > (to-100))
    {
        RUN_LED(1);
    }
}

void Task1s()
{
    static BYTE tm = 0;
    static BYTE tm2 = 0;
    CLR_WDT = 1;  // 喂狗
    
    tm++;
    tm2++;
    if(tm == 7)
    {
        tm = 0;
    } 
    if(tm == 3)
    {
        RemCtlWrite();
    }
    if((tm == 6)||(tm == 3))
    {
        //RemCtlWrite();
        //GetRetCode();
        
    }
     
    if(tm == 6)
    {
        RemCtlRead();   
    }
    if(tm2 == 9)
    {
        tm2 = 0;
        if(!RemOut)
        { 
            if(RunStatus.Running)
            {
                DevRun();
            }
            else
            {    
                RunPHtim = 0;
                AlmOn = false;
                CH_check();
            }
        }
        else
        {
            AlmOn = false;
            RunPHtim = 0;
            CH_check();
        }
    }
        
}


void TimerTask()
{
    u16 delta = 0;
    static u16 Time1s = 0;

    if (Timer0Cnt)
    {
        delta = Timer0Cnt * 10;
        Timer0Cnt = 0;

        if (RX1_Cnt > 0)
        {
            Rx1_Timer += delta;
        }

        if(Remflag)
        {
            RemOutTime += delta;
            if(RemOutTime > 3000)
            {
                RemOutTime = 0;
                RemOut = true;
                //RunPHtim = 0;
            }
        }

        Time1s += delta;
        if (Time1s >= 100)
        {
            Time1s = 0;
            Task1s();
        }

        //Light_Check();
        Alm_ON();
        RunLed(delta);
        IoCtlTask();
    }
}

void Delay(WORD ms)
{
    WORD t = 1000;
    while(ms--)
    {
        for (t=0;t<1000;t++) ;
    }
}

WORD ParamCheck(BYTE *buf, WORD len)
{
    WORD dwSum = 0;
    WORD i;

    for (i = 0; i < len; i++)
    {
        dwSum += buf[i];
    }

    return dwSum;
}

void ReadParam()
{
    EEPROM_read(0, (BYTE *)&SysParam, sizeof(SYS_PARAM));

    #if 0
    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(SYS_PARAM));
    DebugMsg((char *)StrTmp);

    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(HOST_SENSOR_PARAM));
    DebugMsg((char *)StrTmp);

    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(HOST_ALRAM_PARA));
    DebugMsg((char *)StrTmp);

    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(float));
    DebugMsg((char *)StrTmp);
    
    //Rs485Send((BYTE *)&SysParam, sizeof(SYS_PARAM));
    
    
    if (SysParam.Sign != PARAM_SIGN)
    {
        DebugMsg("Sign error. \r\n");
    }

    if (SysParam.Sum != ParamCheck((BYTE *)&SysParam, sizeof(SYS_PARAM)-2))
    {
        DebugMsg("Param Check error. \r\n");
    }
    #endif

   
    if ((SysParam.Sum != ParamCheck((BYTE *)&SysParam, sizeof(SYS_PARAM)-2)))
    {
        ParamDef();

        WriteParam();
    }
}


void WriteParam()
{
    EA = 0;    
    
    EEPROM_SectorErase(0);
    EEPROM_SectorErase(512);
    SysParam.Sum = ParamCheck((BYTE *)&SysParam, sizeof(SYS_PARAM)-2);
    if (!EEPROM_write(0, (BYTE *)&SysParam, sizeof(SYS_PARAM)))
    {
        Error();
    }

    EA = 1;     //打开总中断
}

void LedAlm_Init()
{
    LIGHT0_OFF;
    LIGHT1_OFF;
    LIGHT2_OFF;
    LIGHT3_OFF;
    
    LIGHT4_OFF;
    LIGHT5_OFF;
    LIGHT6_OFF;
    LIGHT7_OFF;
    
    ALARM(0);           // 报警音
}

void ParamDef()
{
    BYTE i;
    
    SysParam.Address = 2;

    for (i=0;i<CHANNLE_NUM;i++)
    {
        RunStatus.Alarm[i] = ALM_FLOW_NOR;
    }

    SysParam.Enable = 0x00;
    SysParam.RemCtlFlag = true;
    RunStatus.SlaveRun = true;
    SysParam.Version = 100;
    RemRegAddr.SypAddr = MODBUS_PARAM_ADD;
}

void SaveParam()
{
    //CString t;
    //t.Format(_T("SaveParam: %02X\r\n"), SysParam.Enable);
    //Log(t);
    //DebugMsg("123");
    WriteParam();
}


// 1秒运行一次
void DevRun()
{
    RunStatus.RunTime ++;
    RunPHtim ++;
    if (RunPHtim > 18)
    {
        // 运行时间大于10秒才检测
        Light_Check();
    }
    else
    {
        CH_check();
    }
}


//========================================================================
// 函数名称: void ScanKey()
// 函数功能: 按钮扫描
// 入口参数: @无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023
// 当前作者:
// 其他备注: 
//========================================================================
void ScanKey()
{
    BYTE i =0;
    //static bool key_up = true;
    if((KEY0() == 0)||(KEY1() == 0)||(KEY2() == 0)||(KEY3() == 0)||
        (KEY4() == 0)||(KEY5() == 0)||(KEY6() == 0)||(KEY7() == 0)||(STOP_M() == 0))
    {
        Delay(10);
        if(KEY0() == 0)
        {
           Valve[KEY_0] = 1; 
        }
        else
        {
           Valve[KEY_0] = 0;
        }

        if(KEY1() == 0)
        {
           Valve[KEY_1] = 1; 
        }
        else
        {
           Valve[KEY_1] = 0;
        }

        if(KEY2() == 0)
        {
           Valve[KEY_2] = 1; 
        }
        else
        {
           Valve[KEY_2] = 0;
        }

        if(KEY3() == 0)
        {
           Valve[KEY_3] = 1; 
        }
        else
        {
           Valve[KEY_3] = 0;
        }

        if(KEY4() == 0)
        {
           Valve[KEY_4] = 1; 
        }
        else
        {
           Valve[KEY_4] = 0;
        }

        if(KEY5() == 0)
        {
           Valve[KEY_5] = 1; 
        }
        else
        {
           Valve[KEY_5] = 0;
        }

        if(KEY6() == 0)
        {
           Valve[KEY_6] = 1; 
        }
        else
        {
           Valve[KEY_6] = 0;
        }

        if(KEY7() == 0)
        {
           Valve[KEY_7] = 1; 
        }
        else
        {
           Valve[KEY_7] = 0;
        }

        if(STOP_M() == 0)
        {
            StopFlag = true;
        }
        else
        {
            StopFlag = false;
        }
     }
     GetChannel();
}


void GetChannel()
{
    int i = 0;
    for (i = 0;i < CHANNLE_NUM;i++)
    {
        if(Valve[i])
        {
            SysParam.Enable |= (1<<i);
        }
        else
        {
            SysParam.Enable  &= ~(1<<i);
        }
    }
    
    if(SysParam.Enable == 0x00)
    {
        RunStatus.Running = false;
    }
    else
    {
        if(StopFlag)
        {
                RunStatus.Running = false;
        }
        else
        {
                RunStatus.Running = true;
        }
    }
}

void SyncModBusDev()
{
    memset(&ModBusParam, 0, sizeof(MODBUS_PARAM));
    
    ModBusParam.Addr = RemRegAddr.SypAddr;
    ModBusParam.ChEnable = SysParam.Enable  ;
    ModBusParam.RemCtlFlag = SysParam.RemCtlFlag;

    ModBusParam.RunStatus = RunStatus.Running;          
    ModBusParam.Address = SysParam.Address;

}


//远程控制读写命令处理
BYTE SendRemCtlCmd(BYTE Addr, BYTE Cmd, WORD Reg, WORD Count, BYTE * Data)
{
   
    WORD i = 0;
    WORD crc, SendLen;
    memset(&HostSendFrame, 0, sizeof(HOST_SEND_FRAME));
    
    HostSendFrame.Address = Addr; 
    HostSendFrame.FunctionCode = Cmd;
    HostSendFrame.RegAddr = RegSw(Reg);
    HostSendFrame.RegCount =  RegSw(Count);

    SendLen = 6;

    if (((Count*2) > 0) && (Data != NULL))
    {
        HostSendFrame.Data[0] = Count*2;  // 数据长度
        SendLen ++;
        i++;
        memcpy(&HostSendFrame.Data[1], Data,(BYTE)Count*2);
        SendLen += Count*2;
        i += Count*2;
    }
        
    // 计算CRC , 并添加到数据后面
    crc = CRC16Calc((BYTE *)&HostSendFrame, SendLen);
    HostSendFrame.Data[i]  = (BYTE)(crc);
    HostSendFrame.Data[i+1] = (BYTE)(crc>>8);
    
    SendLen += 2; 
    Uart1Send((BYTE *)&HostSendFrame, (BYTE)SendLen);
    return true;
}


//远程控制向从机写
void RemCtlWrite()
{
    WORD RegCnt = 13;
    SyncModBusDev();
    SendRemCtlCmd(2, CMD_WRITE_REG, MODBUS_PARAM_ADD, RegCnt, (BYTE *)&ModBusParam);
}

void RemCtlRead()
{
    WORD RegCnt = 13;
    SendRemCtlCmd(2, CMD_READ_REG, MODBUS_PARAM_ADD, RegCnt,NULL);
}

void main(void)
{
    SysInit();
    IoInit();   

    LedAlm_Init();
    
    RUN_LED(1);

    Delay(200);
    Timer0Init();
    Delay(200); 
    
    UART1_config();
    ClearUart1Buf();
    // 待CPU稳定了再读参数
    Delay(500);
    ReadParam();
    
    Delay(200);
    SyncModBusDev();
    RUN_LED(0);
    #if 0
    while(1)
    {
        RUN_LED(0);
        Delay(800);
        RUN_LED(1);
        Delay(200);
    }
    #endif
    
    
    EA = 1;                     //打开总中断

    WDT_CONTR |= (1<<5) |  7;   // 启动开门狗，约8秒

    Delay(200);
    SysParam.RemCtlFlag = true;
    
    while(1)
    {  
        ScanKey();
        TimerTask();
        Uart1Hnd();
    }
}


