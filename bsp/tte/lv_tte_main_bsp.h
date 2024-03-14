#ifndef __LV_TTE_MAIN_BSP_H__
#define __LV_TTE_MAIN_BSP_H__

/*********************************************
*****************GPIO CONFIG******************
**********************************************/
#define Gpio_List(_)        _(Led1) \
                            _(Led2) \
                            _(LoadCtrl) \
                            _(ChgCurrCtrl) \
                            _(BUTTON) \
                            _(IQ_TEST_CTRL) \
                            _(LED_IN) \
                            _(PWR_CTRL_SW) \
                            _(PWR_CTRL_EN) \


#define DefGpioChEnum(ch)	GpioCh_##ch,
enum
{
	Gpio_List(DefGpioChEnum)
	GpioCh_Num
};

#define Led1_Cfg 		        {GPIOE,     GPIO_PIN_0,     GPIO_Mode_Out_PP,       1}
#define Led2_Cfg 		        {GPIOE,     GPIO_PIN_1,     GPIO_Mode_Out_PP,       1}
#define LoadCtrl_Cfg 	        {GPIOE,     GPIO_PIN_2,     GPIO_Mode_Out_PP,       0}
#define ChgCurrCtrl_Cfg         {GPIOE,     GPIO_PIN_3,     GPIO_Mode_Out_PP,       0}
#define BUTTON_Cfg 		        {GPIOA,     GPIO_PIN_4,     GPIO_Mode_IN_FLOATING,  0}
#define IQ_TEST_CTRL_Cfg 		{GPIOE,     GPIO_PIN_15,    GPIO_Mode_Out_PP,       0}
#define LED_IN_Cfg 		        {GPIOB,     GPIO_PIN_12,    GPIO_Mode_IN_FLOATING,  0}
#define PWR_CTRL_SW_Cfg 		{GPIOA,     GPIO_PIN_8,     GPIO_Mode_Out_PP,       0}
#define PWR_CTRL_EN_Cfg 		{GPIOA,     GPIO_PIN_9,     GPIO_Mode_Out_PP,       1}

/**********GPIO接口设备数量配置**********/
#define MAX_LED_IO_COUNT 2


/********************************************
*****************IIC CONFIG******************
*********************************************/
#define IIC_List(_) 	_(CELL) \
						_(PWRSRC) \

#define DefIICChEnum(ch) IIC_CH_##ch,

enum
{
	IIC_List(DefIICChEnum)
	IICCh_Num
};

#define IIC_CELL_Cfg 	{GPIOB, GPIO_PIN_11, GPIOB, GPIO_PIN_10}
#define IIC_PWRSRC_Cfg 	{GPIOB, GPIO_PIN_13, GPIOB, GPIO_PIN_14}

/**********IIC接口设备数量配置**********/
/*MCP4551设备设定*/
#define DEV_MCP4551_List(_) _(ID1) \

#define DefMcp4551Enum(n) DEV_MCP4551_##n,

enum{
	DEV_MCP4551_List(DefMcp4551Enum)
	MCP4551_DEV_Num
};

#define DEV_MCP4551_ID1_Cfg {IIC_CH_CELL, 0x5C}

/*TPA626设备设定*/
#define DEV_TPA626_List(_) 	_(IQ) \
							_(VIRTUAL_CELL)	\
							_(PWR_SRC)	\

#define DefTPA626Enum(n) DEV_TPA626_##n,

enum{
	DEV_TPA626_List(DefTPA626Enum)
	TPA626_DEV_Num
};

#define DEV_TPA626_IQ_Cfg {IIC_CH_CELL, 0x82, {0, TPA626_CFG_CONVERSION_TIME_1085us, \
							TPA626_CFG_CONVERSION_TIME_1085us, TPA626_CFG_MODE_SHUNT_VBUS_CON}}
#define DEV_TPA626_VIRTUAL_CELL_Cfg {IIC_CH_CELL, 0x80, {0, TPA626_CFG_CONVERSION_TIME_1085us, \
							TPA626_CFG_CONVERSION_TIME_1085us, TPA626_CFG_MODE_SHUNT_VBUS_CON}}
#define DEV_TPA626_PWR_SRC_Cfg {IIC_CH_CELL, 0x80, {0, TPA626_CFG_CONVERSION_TIME_1085us, \
							TPA626_CFG_CONVERSION_TIME_1085us, TPA626_CFG_MODE_SHUNT_VBUS_CON}}


/********************************************
*****************UART CONFIG*****************
*********************************************/
#define UART_List(_) 	_(UPPER) \

#define DefUARTChEnum(ch) UARTCh_##ch,

enum
{
	UART_List(DefUARTChEnum)
	UARTCh_Num
};

#define UART_UPPER_Cfg {USART2, GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_3, 57600, 0}


/********************************************
*****************ADC CONFIG******************
*********************************************/
#define ADC_ChList(_)   _(IN_5V) \
						_(BAT_OUT) \

#define ADC_Ch_IN_5V_Cfg   {GPIOC, GPIO_PIN_1, ADC_CH_7}
#define ADC_Ch_BAT_OUT_Cfg {GPIOC, GPIO_PIN_0, ADC_CH_6}

#define ADC_PeriList(_)  _(ADC1) \

#define ADC_PERI_ADC1_Cfg {ADC1, 0, 2}

#define DefADCPeriEnum(a) ADC_PERI_##a,

enum{
	ADC_PeriList(DefADCPeriEnum)
	ADC_PERI_Num	
};

/**********ADC相关接口设备配置**********/

 
#endif //__LV_TTE_MAIN_BSP_H__
