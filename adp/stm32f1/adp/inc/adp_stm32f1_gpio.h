#ifndef __ADP_STM32F1_GPIO_H__
#define __ADP_STM32F1_GPIO_H__

#include <stdint.h>

int32_t AdpGpioInit(void);
int32_t AdpGpioSet(int32_t io, uint8_t state);
int32_t AdpGpioGet(int32_t io);

#define Gpio(Ch) GpioCh_##Ch

#endif //__ADP_STM32F1_GPIO_H__
