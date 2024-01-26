#ifndef __UART_INF_H__
#define __UART_INF_H__

#include "toolkit.h"

typedef void (*pUartMsgProcessCallBackFunc)(uint8_t obj, uint8_t *buf, uint16_t len);

void UartInfMsgProcess(void);
int32_t UartInfRegCallbackFunc(uint8_t obj, pUartMsgProcessCallBackFunc func);
int32_t UartInfRecvMsg(uint8_t obj, uint8_t *buf, uint16_t len);
int32_t UartInfSendMsg(uint8_t obj, uint8_t *buf, uint16_t len);
int32_t UartInfInit(void);

#endif //__UART_INF_H__
