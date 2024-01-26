#ifndef __DAPLINK_BSP_H__
#define __DAPLINK_BSP_H__

/*********************************************
*****************GPIO CONFIG******************
**********************************************/
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

/**********GPIO接口设备数量配置**********/
#define MAX_LED_IO_COUNT 2


/********************************************
*****************IIC CONFIG******************
*********************************************/
#define IIC_List(_) 	_(IIC1) \

#define DefIICChEnum(ch) IICCh_##ch,

enum
{
	IIC_List(DefIICChEnum)
	IICCh_Num
};

#define IIC1_Cfg {GPIOC, GPIO_Pin_15, GPIOC, GPIO_Pin_14}

/**********IIC接口设备数量配置**********/
/*MCP4551设备设定*/
#define DEV_MCP4551_List(_) _(ID1) \

#define DefMcp4551Enum(n) DEV_MCP4551_##n,

enum{
	DEV_MCP4551_List(DefMcp4551Enum)
	MCP4551_DEV_Num
};

#define DEV_MCP4551_ID1_Cfg {IICCh_IIC1, 0x5C}

/*TPA626设备设定*/
#define DEV_TPA626_List(_) _(ID1) \

#define DefTPA626Enum(n) DEV_TPA626_##n,

enum{
	DEV_TPA626_List(DefTPA626Enum)
	TPA626_DEV_Num
};

#define DEV_TPA626_ID1_Cfg {IICCh_IIC1, 0x80, {0, TPA626_CFG_CONVERSION_TIME_1085us, \
							TPA626_CFG_CONVERSION_TIME_1085us, TPA626_CFG_MODE_SHUNT_VBUS_CON}}


/********************************************
*****************UART CONFIG*****************
*********************************************/
#define UART_List(_) 	_(LINK) \

#define DefUARTChEnum(ch) UARTCh_##ch,

enum
{
	UART_List(DefUARTChEnum)
	UARTCh_Num
};

#define UART_LINK_Cfg {USART1, GPIOB, GPIO_Pin_7, GPIOB, GPIO_Pin_6, 57600, GPIO_Remap_USART1}


/********************************************
*****************ADC CONFIG******************
*********************************************/
#define ADC_PERIPH(_) _(ADC1)

#endif //__DAPLINK_BSP_H__
