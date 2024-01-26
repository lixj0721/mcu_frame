#include "adp_config.h"

int32_t AdpConfig(void)
{
    int32_t ret = 0;

    ret |= AdpCoreInit();
    ret |= AdpGpioInit();
    ret |= AdpIicInit();
    ret |= AdpUartInit();

    return ret;
}
