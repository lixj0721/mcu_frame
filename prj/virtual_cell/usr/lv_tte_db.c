#include "lv_tte_db.h"
#include "db_inf.h"
// #include "modbus_slave_inf.h"

#define DB_OP_FUNC_ARRAY(reg)  \
reg##List(DEFINE_DB_FUNC) \
const DbOpFunc reg##List_FuncArray[] = \
{\
    reg##List(DEFINE_DB_FUNC_POINTER)\
};\
int32_t reg##Proc(uint8_t opType, uint16_t addr, uint16_t *buf) \
{\
    if(((addr)&0x7f) < sizeof(reg##List_FuncArray)/sizeof(reg##List_FuncArray[0]));\
    {\
        return reg##List_FuncArray[(addr)&0x7f](opType, addr, buf);\
    }\
    return -1;\
}\

DB_OP_FUNC_ARRAY(DbCoilReg)
DB_OP_FUNC_ARRAY(DbDigInReg)
DB_OP_FUNC_ARRAY(DbHoldReg)
DB_OP_FUNC_ARRAY(DbAnaInReg)

static int32_t DbDOProc(uint8_t opType, uint16_t reg, uint8_t *buf, uint16_t len)
{
    int32_t ret = 0;

    reg &= 0xfff;
    if(reg + (len>>1) > DbCoilRegEnum_Num)
    {
        return -1;
    } 
    for(int32_t i=0;i<len/2;i++)
    {
        ret |= DbCoilRegList_FuncArray[reg+i](opType, 0, (uint16_t *)buf);
    }
    // TteCtrlProcess(opType, reg, len/2, (uint16_t *)buf);

    return ret;
}

static int32_t DbDIProc(uint8_t opType, uint16_t reg, uint8_t *buf, uint16_t len)
{
    int32_t ret = 0;

    reg &= 0xfff;
    if(reg+(len>>1) > DbDigInRegEnum_Num)
    {
        return -1;
    }
    for(int32_t i=0;i<len/2;i++)
    {
        ret |= DbDigInRegList_FuncArray[reg+i](opType, 0, (uint16_t *)buf);
    }
    return 0;
}

static int32_t DbCfgProc(uint8_t opType, uint16_t reg, uint8_t *buf, uint16_t len)
{
    return -1;
}

static int32_t DbSampInProc(uint8_t opType, uint16_t reg, uint8_t *buf, uint16_t len)
{
    int32_t ret = 0;

    reg &= 0xfff;
    if(reg+(len>>1) > DbAnaInRegEnum_Num)
    {
        return -1;
    }
    for(int32_t i=0;i<len/2;i++)
    {
        ret |= DbAnaInRegList_FuncArray[reg+i](opType, 0, (uint16_t *)buf);
    }
    return 0;
}

static int32_t GetDiInf(void)
{
    return 0;
}

/*周期性获取状态数据及信息*/
static int32_t DbInputInfUpdate(void)
{
    return 0;
}

int32_t LvTteDbInit(void)
{
    int32_t ret = 0;

    ret |= DbRegProcFunc(REG_TYPE_DO, DbDOProc);
    ret |= DbRegProcFunc(REG_TYPE_DI, DbDIProc);
    ret |= DbRegProcFunc(REG_TYPE_CFG, DbCfgProc);
    ret |= DbRegProcFunc(REG_TYPE_AI, DbSampInProc);

    return ret;
}

