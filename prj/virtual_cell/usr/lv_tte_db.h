#ifndef __LV_TTE_DB_H__
#define __LV_TTE_DB_H__

#include <stdint.h>
#include "toolkit.h"

#define DbCoilRegList(_)        _(LoadCtrl) \
                                _(ChgCurrCtrl) \
                                _(IqCtrl) \
                                _(PwrSrcCtrl) \

#define DbCoilRegEnum(reg)          DbCoilReg_##reg,

enum{
    DbCoilRegList(DbCoilRegEnum)
    DbCoilRegEnum_Num
};

#define DbDigInRegList(_)       _(LedIn) \

#define DbDigInRegEnum(reg)         DbDigInReg_##reg,

enum{
    DbDigInRegList(DbDigInRegEnum)
    DbDigInRegEnum_Num
};

#define DbHoldRegList(_)        _(CellVol) \
                                

#define DbHoldRegEnum(reg)          DbHoldReg_##reg,

enum{
    DbHoldRegList(DbHoldRegEnum)
    DbHoldRegEnum_Num
};

#define DbAnaInRegList(_)       _(StaticVol) \
                                _(StaticCurr) \
                                _(CellVol) \
                                _(CellCurr) \
                                _(PwrSrcVol) \
                                _(PwrSrcCurr) \

#define DbAnaInRegEnum(reg)         DbAnaInReg_##reg,

enum{
    DbAnaInRegList(DbAnaInRegEnum)
    DbAnaInRegEnum_Num
};

typedef int32_t (*DbOpFunc)(uint8_t opType, uint16_t oft, uint16_t *buf);

#define DEFINE_DB_FUNC(addr) \
__attribute__((weak)) int32_t DB_FUNC_##addr(uint8_t opType, uint16_t oft, uint16_t *buf)\
{\
    *buf = 0;\
    return 0;\
}\

#define DEFINE_DB_FUNC_POINTER(reg) DB_FUNC_##reg,

int32_t LvTteDbInit(void);

#endif // __LV_TTE_DB_H__
