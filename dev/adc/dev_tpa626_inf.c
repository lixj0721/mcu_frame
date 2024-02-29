#include "dev_tpa626_inf.h"
#include "adp_config.h"
#include "bsp_cfg.h"

#define MAX_DEV_TPA626_NUM 8

#define TPA626_CFG_AVG_OFFSET 9
#define TPA626_CFG_AVG_MASK   0x7

#define TPA626_CFG_VBUS_CONVEERSION_TIME_OFFSET 6
#define TPA626_CFG_VBUS_CONVEERSION_TIME_MASK   0x7 

#define TPA626_CFG_SHUNT_CONVEERSION_TIME_OFFSET 3
#define TPA626_CFG_SHUNT_CONVEERSION_TIME_MASK   0x7 

#define TPA626_CFG_MODE_OFFSET 0
#define TPA626_CFG_MODE_MASK   0x7 

#define DEV_TPA626_ENDBIAN_TYPE ENDIAN_TYPE_BIG

enum{
    TPA626_REG_CONF = 0x0,
    TPA626_REG_SHUNT,
    TPA626_REG_VBUS,
    TPA626_REG_POWER,
    TPA626_REG_CURRENT,
    TPA626_REG_CALIBRATION,
    TPA626_REG_MASK_ENABLE,
    TPA626_REG_ALERT_LIMIT,

    TPA626_REG_INF_BOT,

    TPA626_REG_MANUF_ID = 0xFE,
    TPA626_REG_DIE_ID = 0xFF
};

struct Tpa626DevCfgInfo{
    uint8_t avg;
    uint8_t vbusConvertionTime;
    uint8_t shuntConvertionTime;
    uint8_t mode;
};

struct Tpa626DevInfo{
    uint8_t iicId;
    uint8_t devId;
    struct Tpa626DevCfgInfo cfg;
};

#define DefTPA626Cfg(a) DEV_TPA626_##a##_Cfg,

const struct Tpa626DevInfo s_devTPA626InfoStru[] = 
{
	DEV_TPA626_List(DefTPA626Cfg)
};

#define TPA_WERITE_DATA(id, addr, buf, len) AdpIicWriteBytes(s_devTPA626InfoStru[id].iicId, \
                                                s_devTPA626InfoStru[id].devId, addr, 1, buf, len)

#define TPA_READ_DATA(id, addr, buf, len)   AdpIicReadBytes(s_devTPA626InfoStru[id].iicId, \
                                                s_devTPA626InfoStru[id].devId, addr, 1, buf, len)

int32_t DevTpa626Init()
{
    int32_t ret = 0;
    for(int i=0;i<TPA626_DEV_Num;i++)
    {
        uint16_t cfg = 0x4000   \
                                | ((s_devTPA626InfoStru[i].cfg.avg&TPA626_CFG_AVG_MASK)<<TPA626_CFG_AVG_OFFSET) \
                                | ((s_devTPA626InfoStru[i].cfg.vbusConvertionTime&TPA626_CFG_VBUS_CONVEERSION_TIME_MASK)<<TPA626_CFG_VBUS_CONVEERSION_TIME_OFFSET) \
                                | ((s_devTPA626InfoStru[i].cfg.shuntConvertionTime&TPA626_CFG_SHUNT_CONVEERSION_TIME_MASK)<<TPA626_CFG_SHUNT_CONVEERSION_TIME_OFFSET) \
                                | ((s_devTPA626InfoStru[i].cfg.mode&TPA626_CFG_MODE_MASK)<<TPA626_CFG_MODE_OFFSET);

        cfg = ENDIAN_TYPE_SWITCH_16(DEV_TPA626_ENDBIAN_TYPE, cfg);
        ret |= TPA_WERITE_DATA(i, TPA626_REG_CONF, (uint8_t *)&cfg, 2);
    }

    return ret;
}

int32_t DevTpa626ReadShuntVol(uint8_t id, int16_t *shuntVol)
{
    uint16_t data;
    int32_t ret = 0;
    ret |= TPA_READ_DATA(id, TPA626_REG_SHUNT, (uint8_t *)&data, 2);
    
    *shuntVol = ENDIAN_TYPE_SWITCH_16(DEV_TPA626_ENDBIAN_TYPE, data);/*可能存在数据转换的问题*/
    *shuntVol = ((int32_t)(*shuntVol) * 5) / 2;/*单位换算，输出单位为1uV*/
    return ret;
}

int32_t DevTpa626ReadBusVol(uint8_t id, uint16_t *busVol)
{
    int32_t ret = 0;
    ret |= TPA_READ_DATA(id, TPA626_REG_VBUS, (uint8_t *)busVol, 2);
    *busVol = ENDIAN_TYPE_SWITCH_16(DEV_TPA626_ENDBIAN_TYPE, *busVol);
    *busVol = (uint32_t)(*busVol) * 10 / 8;/*单位换算，输出单位为1mV*/
    return ret;
}

int32_t DevTpa626ReadCfg(uint8_t id, uint16_t *cfg)
{
    uint16_t data;
    int32_t ret = 0;
    ret |= TPA_READ_DATA(id, TPA626_REG_CONF, (uint8_t *)cfg, 2);
    *cfg = ENDIAN_TYPE_SWITCH_16(DEV_TPA626_ENDBIAN_TYPE, *cfg);
    // *busVol = (int16_t)data;/*可能存在数据转换的问题*/
    return ret;
}

int32_t DevTpa626ReadAll(uint8_t id, uint16_t *buf, uint8_t num)
{
    return TPA_READ_DATA(id, TPA626_REG_CONF, (uint8_t *)buf, 2*num);
}
