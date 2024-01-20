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

#define Led1_Cfg {GPIOB, GPIO_Pin_10, GPIO_Mode_Out_PP, 1}
#define Led2_Cfg {GPIOB, GPIO_Pin_11, GPIO_Mode_Out_PP, 1}

/*接口数量配置*/
#define MAX_LED_IO_COUNT 2


/******************IIC CONFIG*******************/
#define IIC_List(_) 	_(IIC1) \

#define DefIICChEnum(ch) IICCh_##ch,

enum
{
	IIC_List(DefIICChEnum)
	IICCh_Num
};

#define IIC1_Cfg {GPIOC, GPIO_Pin_15, GPIOC, GPIO_Pin_14}


/*MCP4551设备设定*/
#define DEV_MCP4551_List(_) _(ID1) \

#define DefMcp4551Enum(n) DEV_MCP4551_##n,

enum{
	DEV_MCP4551_List(DefMcp4551Enum)
};

#define DEV_MCP4551_ID1_Cfg {IICCh_IIC1, 0x5C}

#endif //__DAPLINK_BSP_H__
