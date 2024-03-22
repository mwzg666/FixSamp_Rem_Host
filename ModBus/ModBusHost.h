#ifndef __MODBUS_HOST_H__
#define __MODBUS_HOST_H__

extern WORD HostBufIndex;
extern HOST_SEND_FRAME xdata  HostSendFrame;   // 发送帧
extern DEVICE_READ_ACK xdata  ReadAckFrame;    // 读接收帧
extern DEVICE_WRITE_ACK xdata WriteAckFrame;  // 写接收帧

BYTE HostSendCmd(BYTE Mode, BYTE Addr, BYTE Cmd, WORD Reg, WORD Count, BYTE * Data);
void SendBuffer(BYTE Mode, BYTE * Data, WORD Length);

WORD PopReg();


#endif /*PROTOCOL_H_*/
