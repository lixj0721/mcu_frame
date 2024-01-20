#include "adp_stm32f1_gpio.h"
#include "bsp_cfg.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

/**/
typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
	uint8_t dir;
	uint8_t state;	// 0-low 1-high
}GpioCfg_t;

const GpioCfg_t g_gpioCfg[] = 
{
	Gpio_List(DefGpCfg)
};

int32_t AdpGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    for(int32_t i=0;i<GpioCh_Num;i++)
    {
        if(g_gpioCfg[i].port == NULL) continue;
        GPIO_InitStructure.GPIO_Pin = g_gpioCfg[i].pin;
        GPIO_InitStructure.GPIO_Mode = g_gpioCfg[i].dir;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        if(g_gpioCfg[i].state)  GPIO_SetBits(g_gpioCfg[i].port, g_gpioCfg[i].pin);
        else                    GPIO_ResetBits(g_gpioCfg[i].port, g_gpioCfg[i].pin);
        GPIO_Init(g_gpioCfg[i].port, &GPIO_InitStructure);
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
