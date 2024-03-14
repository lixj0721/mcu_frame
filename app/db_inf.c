#include "db_inf.h"

// struct DbProcInf_t g_dbProcInf = {0};
const uint16_t REG_ADDR_START[REG_TYPE_NUM] = {0x0000, 0x2000, 0x4000, 0xA000};
DbProcFunc g_dbProcFunc[REG_TYPE_NUM] = {0};

int32_t DbReadProc(uint16_t addr, uint8_t *pBuf, uint16_t *len)
{
    for (int i=REG_TYPE_NUM-1;i>=0;i--){
        if (addr >= REG_ADDR_START[i]) {
            if(g_dbProcFunc[i] != NULL) {
                return g_dbProcFunc[i](DB_OP_READ, addr&0xFFF, pBuf, *len);
            }
        }
    }

    return -1;
}

int32_t DbWriteProc(uint16_t addr, uint8_t *pBuf, uint16_t *len)
{
    for (int i=REG_TYPE_NUM-1;i>=0;i--){
        if (addr >= REG_ADDR_START[i]) {
            if(g_dbProcFunc[i] != NULL) {
                return g_dbProcFunc[i](DB_OP_WRITE, addr&0xFFF, pBuf, *len);
            }
        }
    }

    return -1;
}

int32_t DbRegProcFunc(uint8_t type, DbProcFunc func)
{
    if(type >= REG_TYPE_NUM) return -1;
    g_dbProcFunc[type] = func;
	
    return 0;
}
