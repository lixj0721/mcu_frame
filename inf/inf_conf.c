#include "inf_conf.h"
#include "bsp_cfg.h"

int32_t InfConfInit(void)
{
    int32_t ret = 0;

#if ADC_CH_num > 0
    ret |= AdcInfInit();
#endif
#if UARTCh_Num > 0
    ret |= UartInfInit();
#endif

    return ret;
}
