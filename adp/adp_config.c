#include "adp_config.h"
#include "bsp_cfg.h"

int32_t AdpConfig(void)
{
    int32_t ret = 0;

    ret |= AdpCoreInit();
#if GpioCh_Num > 0
    ret |= AdpGpioInit();
#endif

#if IICCh_Num > 0
    ret |= AdpIicInit();
#endif

#if UARTCh_Num > 0
    ret |= AdpUartInit();
#endif

#if ADC_CH_num > 0
    ret |= AdpAdcInit();
#endif

    return ret;
}
