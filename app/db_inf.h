#ifndef __DB_INF_H__
#define __DB_INF_H__

#include <stdint.h>
#include "toolkit.h"

enum{REG_TYPE_DO, REG_TYPE_DI, REG_TYPE_CFG, REG_TYPE_AI, 
    REG_TYPE_NUM};

typedef int32_t (*DbProcFunc)(uint8_t opType, uint16_t reg, uint8_t *buf, uint16_t len);

int32_t DbRegProcFunc(uint8_t type, DbProcFunc func);
int32_t DbWriteProc(uint16_t addr, uint8_t *pBuf, uint16_t *len);
int32_t DbReadProc(uint16_t addr, uint8_t *pBuf, uint16_t *len);

#endif  //__DB_INF_H__
