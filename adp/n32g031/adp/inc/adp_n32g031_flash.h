#ifndef __ADP_N32G031_FLASH_H__
#define __ADP_N32G031_FLASH_H__

#include "toolkit.h"

int32_t AdpFlashErase(uint32_t addr, uint32_t lenInByte);
int32_t AdpFlashWrite(uint32_t addr, uint32_t *buf, uint32_t lenInWord);

#endif  //__ADP_N32G031_FLASH_H__
