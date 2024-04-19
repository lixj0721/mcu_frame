#ifndef __ADP_N32G031_UART_H__
#define __ADP_N32G031_UART_H__

#include <stdint.h>

typedef void (*pUartIntCallBackFunc)(uint8_t obj);

int32_t AdpUartInit(void);
void AdpUartRegCallbackFunc(pUartIntCallBackFunc func);

int32_t AdpUartCfgTxInt(uint8_t obj, uint8_t state);
int32_t AdpUartClrRxFlag(uint8_t obj);
int32_t AdpUartClrTxFlag(uint8_t obj);

int8_t AdpUartGetRxIntFlag(uint8_t obj);
int8_t AdpUartGetTxIntFlag(uint8_t obj);

void AdpUartSendByte(uint8_t obj, uint8_t data);
void AdpUartRecvByte(uint8_t obj, uint8_t *data);
/*判断发送中断是否使能*/
int32_t AdpUartIsTxIntEnable(uint8_t obj);

#endif //__ADP_N32G031_UART_H__
