#ifndef __LV_TTE_DB_H__
#define __LV_TTE_DB_H__

#include <stdint.h>

#define DbCoilRegList(_)        _(LoadCtrl) \
                                _(ChgCurrCtrl) \
                                _(IqCtrl) \
                                _(PwrSrcCtrl) \


#define DbInputRegList(_)       _(LedIn) \


#define DbHoldRegList(_)        _(StaticCurr) \
                                _(StaticVol) \
                                _(VirtualCellCurr) \
                                _(VirtualCellVol) \
                                _(PwrSrcCurr) \
                                _(PwrSrcVol) \


#define DbCfgRegList(_)         _(Default) \


#define DbCoilRegEnum(reg)          DbCoilReg_##reg,
#define DbInputRegEnum(reg)         DbInputReg_##reg,
#define DbHoldRegEnum(reg)          DbHoldReg_##reg,
#define DbCfgRegEnum(reg)           DbCfgReg_##reg,

enum{
    DbCoilRegList(DbCoilRegEnum)
    DbCoilRegEnum_Num
};

enum{
    DbInputRegList(DbInputRegEnum)
    DbInputRegEnum_Num
};

enum{
    DbHoldRegList(DbHoldRegEnum)
    DbHoldRegEnum_Num
};

enum{
    DbCfgRegList(DbCfgRegEnum)
    DbCfgRegEnum_Num
};

typedef int32_t (*DbOpFunc)(uint8_t opType, uint16_t oft, uint16_t *buf);

#define DEFINE_DB_FUNC(addr) \
__attribute__((weak)) int32_t DB_FUNC_##addr(uint8_t opType, uint16_t oft, uint16_t *buf)\
{\
    *buf = 0;\
    return 0;\
}\

#define DEFINE_DB_FUNC_POINTER(reg) DB_FUNC_##reg,


#endif // __LV_TTE_DB_H__
