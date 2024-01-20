#ifndef __IO_OP_H__
#define __IO_OP_H__

#include "toolkit.h"
#include "adp_config.h"

/*LED功能配置*/
#define LED_CTRL_PROCESS_INTERVAL 100/*LED任务的间隔时间设置*/

struct LedCtrlConfig{
    uint32_t ledio;
    uint16_t onState;
    uint16_t initState;
    uint16_t onTime;
    uint16_t offTime;
    uint8_t  used;
    uint16_t lastState;
    uint16_t stateLastTime;
};

/*按键功能配置*/
typedef void (*KeyEventFunc)(uint32_t keyId, uint16_t event, void * param);

enum{
    KEY_EVENT_SHORT_CLICK,
    KEY_EVENT_LONG_CLICK,
    KEY_EVENT_DOUBLE_CLICK,

    KEY_EVENT_BOT
};

int32_t IOOpInit(void);
int32_t LedCtrlRegLed(struct LedCtrlConfig *cfg);

#endif //__IO_OP_H__
