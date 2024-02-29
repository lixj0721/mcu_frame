#ifndef __DEV_TPA626_INF_H__
#define __DEV_TPA626_INF_H__

#include "toolkit.h"

enum{
    TPA626_CFG_CONVERSION_TIME_66us = 0,
    TPA626_CFG_CONVERSION_TIME_134us,
    TPA626_CFG_CONVERSION_TIME_269us,
    TPA626_CFG_CONVERSION_TIME_542us,
    TPA626_CFG_CONVERSION_TIME_1085us,
    TPA626_CFG_CONVERSION_TIME_2170us,
    TPA626_CFG_CONVERSION_TIME_4341us,
    TPA626_CFG_CONVERSION_TIME_8682us,

    TPA626_CFG_CONVERSION_TIME_BOT
};

enum{
    TPA626_CFG_MODE_PWR_DOWN = 0,
    TPA626_CFG_MODE_SHUNT_TRG,
    TPA626_CFG_MODE_VBUS_TRG,
    TPA626_CFG_MODE_SHUNT_VBUS_TRG,
    TPA626_CFG_MODE_PWR_DOWN_,
    TPA626_CFG_MODE_SHUNT_CON,
    TPA626_CFG_MODE_VBUS_CON,
    TPA626_CFG_MODE_SHUNT_VBUS_CON,

    TPA626_CFG_MODE_BOT
};

int32_t DevTpa626Init();
/**shuntVol 单位为uV*/
int32_t DevTpa626ReadShuntVol(uint8_t id, int16_t *shuntVol);
/**busVol 单位为mV*/
int32_t DevTpa626ReadBusVol(uint8_t id, uint16_t *busVol);
int32_t DevTpa626ReadCfg(uint8_t id, uint16_t *cfg);
int32_t DevTpa626ReadAll(uint8_t id, uint16_t *buf, uint8_t num);

#endif // __DEV_TPA626_INF_H__
