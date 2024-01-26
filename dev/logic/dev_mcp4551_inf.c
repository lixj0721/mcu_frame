#include "dev_mcp4551_inf.h"
#include "adp_config.h"
#include "bsp_cfg.h"

#define MAX_MCP4551_NUM 18

struct DevMcp4551Cfg_t{
    uint8_t iicId; /**/
    uint8_t devId;
};

#define DefMcp4551Cfg(a) DEV_MCP4551_##a##_Cfg,

const struct DevMcp4551Cfg_t s_devMcp4551InfoStru[] = 
{
	DEV_MCP4551_List(DefMcp4551Cfg)
};

int32_t DevMcp4551_SetWiper(uint8_t id, uint16_t wiper)
{
    uint8_t w = (uint8_t)wiper;
    if(id >= MCP4551_DEV_Num)
    {
        return -1;
    }
    return AdpIicWriteBytes(s_devMcp4551InfoStru[id].iicId, s_devMcp4551InfoStru[id].devId, 0x0, 1, &w, 1);
}

int32_t DevMcp4551_GetWiper(uint8_t id, uint16_t *wiper)
{
    uint8_t w;
    int32_t ret = 0;

    if(id >= MCP4551_DEV_Num)
    {
        return -1;
    }
    ret = AdpIicReadBytes(s_devMcp4551InfoStru[id].iicId, s_devMcp4551InfoStru[id].devId, 0x0, 1, &w, 1);
    *wiper = (uint16_t)w;
    return ret;
}

