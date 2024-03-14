#include "adp_n32g45x_adc.h"
#include "n32g45x_conf.h"
#include "bsp_cfg.h"

struct AdpAdcChCfg_t{
    GPIO_Module *port;
	uint16_t pin;
    uint8_t chIndex;/*对应的adc输入通道*/
};

struct AdpAdcChInfo_t{
    ADC_Module *adc;
    uint8_t startCh;
    uint8_t chCnt;
};

#define DefAdcChCfg(a) ADC_Ch_##a##_Cfg,

const struct AdpAdcChCfg_t g_adcChCfg[] = 
{
    ADC_ChList(DefAdcChCfg)
};

#define DefAdcPeriCfg(a) ADC_PERI_##a##_Cfg,
struct AdpAdcChInfo_t g_adcPeriphCfg[] = 
{
    ADC_PeriList(DefAdcPeriCfg)
};

static int32_t AdcClkInit(void)
{
    uint32_t clk;
    for(int i=0;i<ADC_PERI_Num;i++)
    {
        clk = (g_adcPeriphCfg[i].adc==ADC1)?RCC_AHB_PERIPH_ADC1:
                ((g_adcPeriphCfg[i].adc==ADC2)?RCC_AHB_PERIPH_ADC2:
                    ((g_adcPeriphCfg[i].adc==ADC3)?RCC_AHB_PERIPH_ADC3:-1));
        if(clk==-1) return -1;
        RCC_EnableAHBPeriphClk(clk, ENABLE);
    }
    return 0;
}

static int32_t AdcGpioInit(void)
{
    return 0;
}

