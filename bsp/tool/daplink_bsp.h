#ifndef __DAPLINK_BSP_H__
#define __DAPLINK_BSP_H__

/******************GPIO CONFIG*******************/
#define Gpio_List(_)        _(Led1) \
                            _(Led2) \


#define DefGpioChEnum(ch)	GpioCh_##ch,
enum
{
	Gpio_List(DefGpioChEnum)
	GpioCh_Num
};

#define DefGpCfg(a)	a##_Cfg,

#define Led1_Cfg {GPIOB, GPIO_Pin_10, GPIO_Mode_Out_PP, 1}
#define Led2_Cfg {GPIOB, GPIO_Pin_11, GPIO_Mode_Out_PP, 1}

/*接口数量配置*/
#define MAX_LED_IO_COUNT 2

#endif //__DAPLINK_BSP_H__
