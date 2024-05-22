#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "main.h"
#include "ModBus.h"
#include "ModBusDev.h"
#include "uart.h"


MODBUS_PARAM xdata ModBusParam;
MODBUS_STATUS xdata ModBusStatus;
MODBUS_INFO xdata ModBusInfo;

HOST_SEND_FRAME xdata  HostSendFrame;   // 发送帧
DEVICE_READ_ACK xdata  ReadAckFrame;    // 读接收帧
DEVICE_WRITE_ACK xdata WriteAckFrame;  // 写接收帧c



/*
ModBus 帧格式
1. 发送帧
地址           命令     寄存器    寄存器数量    数据                                             CRC   
0A(固定值)     Cmd(1)   RX(2)     n(2)          无数据表示读取、有数据表示写对应的寄存器 

数据定义:  长度 + 数据
           n*2    dat(n*2)

2. 应答帧 -- 返回数据
地址           命令   数据长度    数据      CRC   
0A(固定值)     Cmd    n(1)        dat(n)

3. 应答帧 -- 返回状态
地址           命令   寄存器   寄存器数量     CRC   
0A(固定值)     Cmd    Rx(2)    n(2)                       
*/


DEVICE_READ_ACK xdata  DevReadAck;   
DEVICE_WRITE_ACK xdata DevWriteAck;

HOST_SEND_FRAME xdata RecvFrame;   


// 把浮点数转换为大端打包到发送数据区
void PackageFloatValue(WORD Offset, float val)
{
    BYTE temp[4] = {0};
    FloatToBytes(val,temp);
    memcpy(&DevReadAck.Data[Offset], temp, 4);  
}

void PackageDWordValue(WORD Offset, DWORD val)
{
    DWORD temp;
    temp = SwEndian(val);
    memcpy(&DevReadAck.Data[Offset], &temp, 4);  
}


void PackageWordValue(WORD Offset, WORD val)
{
    BYTE temp[2] = {0};
    temp[0] = (BYTE)(val >> 8);
    temp[1] = (BYTE)val;
    memcpy(&DevReadAck.Data[Offset], temp, 2);  
}

//保存读数据
void ModBusSave()
{
    BYTE i = 0;     
    RunStatus.SlaveRun = ModBusParam.RunStatus; 
    SysParam.ChModeCtl = ModBusParam.ChModeCtl; 
    for(i = 0;i<8;i++)
    {
        RunStatus.Alarm[i] = ModBusParam.Alarm[i];
    }
    
    WriteParam();
}



//从设备读应答处理
BYTE ReadAck()
{
    WORD i = 0;
    WORD reg = 0;   
    if(ReadAckFrame.DataLen <= 0)
    {
        return false;
    }
    memcpy(&reg, &ReadAckFrame.Data[0], 2);
    if(reg == RemRegAddr.SypAddr)
    { 
        memcpy(&ModBusParam, ReadAckFrame.Data, sizeof(MODBUS_PARAM));
    }

    ModBusSave();
    return true;
}


//写应答处理
bool WriteAckDev()
{
   //if (WriteAckFrame.Crc == 0)
   //{
       //return;
   //}

   return true;
}

void HndModBusRecv(BYTE *buf, BYTE len)
{
    if (!ValidRtuFrame(buf, len))
    {
        return;
    }
    
    memset(&ReadAckFrame, 0, sizeof(DEVICE_READ_ACK));
    memset(&WriteAckFrame, 0, sizeof(DEVICE_WRITE_ACK));
    if(buf[0]!= 2)//SysParam.Address)
    {
        return;
    }
    switch(buf[1])
    {
        case CMD_READ_REG:
        {
            memcpy(&ReadAckFrame, buf, len);
            ReadAck(); 
            break;     
        }
        case CMD_WRITE_REG:
        {
            memcpy(&WriteAckFrame, buf, len);
            WriteAckDev(); 
            break;
        }
    }

}


