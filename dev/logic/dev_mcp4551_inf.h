#ifndef __DEV_MCP4551_INF_H__
#define __DEV_MCP4551_INF_H__

#include <stdint.h>

int32_t DevMcp4551_SetWiper(uint8_t id, uint16_t wiper);
int32_t DevMcp4551_GetWiper(uint8_t id, uint16_t *wiper);

#endif  //__DEV_MCP4551_INF_H__

