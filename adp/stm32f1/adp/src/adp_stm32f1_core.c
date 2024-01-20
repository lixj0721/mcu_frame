#include "adp_stm32f1_core.h"
#include "bsp_cfg.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);

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
