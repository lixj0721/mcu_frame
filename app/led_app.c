#include "led_app.h"
#include "io_op.h"
#include "bsp_cfg.h"

int32_t LedAppRunConfig(void)
{
    struct LedCtrlConfig cfg;
    int32_t ret = 0;

#ifdef RUN_LED_NUM
    cfg.ledio = Gpio(Led1);
    cfg.initState = 1;
    cfg.offTime = 500/LED_CTRL_PROCESS_INTERVAL;
    cfg.onTime = 500/LED_CTRL_PROCESS_INTERVAL;
    cfg.onState = 0;
    ret |= LedCtrlRegLed(&cfg);
#if (RUN_LED_NUM == 2)
    cfg.initState = 0;
    cfg.ledio = Gpio(Led2);
    ret |= LedCtrlRegLed(&cfg);
#endif
#endif

    return ret;
}

int32_t LedAppInit(void)
{
    int32_t ret = 0;

    ret |= IOOpInit();
    ret |= LedAppRunConfig();

    return ret;
}
