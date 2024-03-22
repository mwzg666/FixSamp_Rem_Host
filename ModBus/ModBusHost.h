#ifndef __MODBUS_HOST_H__
#define __MODBUS_HOST_H__

extern WORD HostBufIndex;
extern HOST_SEND_FRAME xdata  HostSendFrame;   // ����֡
extern DEVICE_READ_ACK xdata  ReadAckFrame;    // ������֡
extern DEVICE_WRITE_ACK xdata WriteAckFrame;  // д����֡

BYTE HostSendCmd(BYTE Mode, BYTE Addr, BYTE Cmd, WORD Reg, WORD Count, BYTE * Data);
void SendBuffer(BYTE Mode, BYTE * Data, WORD Length);

WORD PopReg();


#endif /*PROTOCOL_H_*/
