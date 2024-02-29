#include "lv_tte_db.h"

#define DB_OP_FUNC_ARRAY(reg)  \
reg##List(DEFINE_DB_FUNC) \
const DEFINE_DB_FUNC reg##List_FuncArray[] = \
{\
    reg##List(DEFINE_DB_FUNC_POINTER)\
};\
int32_t reg##Proc(uint8_t opType, uint16_t addr, uint16_t *buf) \
{\
    if(((addr)&0x7f) < sizeof(reg##List_FuncArray)/sizeof(reg##List_FuncArray[0]));\
    {\
        return reg##List_FuncArray[(addr)&0x7f](opType, addr, buf);\
    }\
}\


DB_OP_FUNC_ARRAY(DbCoilReg)

// __weak void DbReadReg_##reg(uint16_t reg, uint16_t *value){ \
//     if(value!=0) \
//     { \
//         *value = 0; \
//     } \
// } \



// DbList(DB_OP_FUNC_ARRAY)

// __weak void DbReadFunc_##
