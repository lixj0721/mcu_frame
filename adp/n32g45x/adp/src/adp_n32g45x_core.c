#include "adp_n32g45x_core.h"
#include "bsp_cfg.h"
#include "n32g45x.h"
#include "n32g45x_conf.h"

/*systick config*/
uint32_t g_systick;
void SysTick_Handler(void)
{
    g_systick++;
}

uint32_t AdpGetSystickCnt(void)
{
    return g_systick;
}

static int32_t AdpSystickInit(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
    return 0; 
}

static int32_t AdpClkInit(void)
{
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOC | RCC_APB2_PERIPH_GPIOD |
                            RCC_APB2_PERIPH_GPIOE | RCC_APB2_PERIPH_GPIOF | RCC_APB2_PERIPH_GPIOG | RCC_APB2_PERIPH_AFIO, ENABLE);

    return 0;
}

int32_t AdpCoreInit(void)
{
    int32_t ret = 0;
    
    /*时钟初始化，目前默认使用system_stm32f10x.c中的初始化函数*/
    ret |= AdpClkInit();
    /*这里初始化系统滴答定时器，滴答定时器需要用来给系统的任务调度函数使用*/
    ret |= AdpSystickInit();

    return ret;
}
