#include "adp_n32g031_gpio.h"
#include "bsp_cfg.h"
#include "n32g031.h"
#include "n32g031_gpio.h"

/**/
struct GpioCfgStruct
{
	GPIO_Module *port;
	uint16_t pin;
	uint8_t dir;
	uint8_t state;	// 0-low 1-high
};

#define DefGpCfg(a)	a##_Cfg,

const struct GpioCfgStruct g_gpioCfg[] = 
{
	Gpio_List(DefGpCfg)
};

int32_t AdpGpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;
    for(int32_t i=0;i<GpioCh_Num;i++)
    {
        if(g_gpioCfg[i].port == NULL) continue;
        GPIO_InitStructure.Pin = g_gpioCfg[i].pin;
        GPIO_InitStructure.GPIO_Mode = g_gpioCfg[i].dir;
        GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
        if(g_gpioCfg[i].state)  GPIO_SetBits(g_gpioCfg[i].port, g_gpioCfg[i].pin);
        else                    GPIO_ResetBits(g_gpioCfg[i].port, g_gpioCfg[i].pin);
        GPIO_InitPeripheral(g_gpioCfg[i].port, &GPIO_InitStructure);
    }

    return 0;
}

int32_t AdpGpioSet(int32_t io, uint8_t state)
{
    if(io >= GpioCh_Num) return -1;

    if(state)  GPIO_SetBits(g_gpioCfg[io].port, g_gpioCfg[io].pin);
    else       GPIO_ResetBits(g_gpioCfg[io].port, g_gpioCfg[io].pin);

    return 0;
}

int32_t AdpGpioGet(int32_t io)
{
    if(io >= GpioCh_Num) return -1;
    return GPIO_ReadInputDataBit(g_gpioCfg[io].port, g_gpioCfg[io].pin);
    return 0;
}
