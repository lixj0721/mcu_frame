#ifndef __ADP_STM32F1_IIC_H__
#define __ADP_STM32F1_IIC_H__

#include <stdint.h>

int32_t AdpIicInit(void);
int32_t AdpIicReadBytes(uint32_t i2cObjId,uint8_t devAddr, uint16_t ReadAddr, uint8_t AddrNum, uint8_t* pBuffer, uint16_t NumByteToRead);
int32_t AdpIicWriteBytes(uint32_t i2cObjId,uint8_t devAddr, uint16_t WriteAddr, uint8_t AddrNum, uint8_t* pBuffer, uint16_t NumByteToWrite);

#endif // __ADP_STM32F1_IIC_H__
