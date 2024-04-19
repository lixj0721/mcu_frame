#ifndef __ACTBAL_BSP_V10_H__
#define __ACTBAL_BSP_V10_H__

/*********************************************
*****************GPIO CONFIG******************
**********************************************/
#define Gpio_List(_)        _(Led1) \
                            _(Led2) \
							_(ChargerEn) \
							_(ChargerInt) \
							_(EvenBalEn) \
							_(OddBalEn) \
							_(EvenCtrl1) \
							_(EvenCtrl2) \
							_(EvenCtrl3) \
							_(EvenCtrl0) \
							_(OddCtrl1) \
							_(OddCtrl2) \
							_(OddCtrl3) \
							_(OddCtrl0) \


#define DefGpioChEnum(ch)	GpioCh_##ch,
enum
{
	Gpio_List(DefGpioChEnum)
	GpioCh_Num
};

#define Led1_Cfg 		        {GPIOA,     GPIO_PIN_5,     	GPIO_MODE_OUTPUT_PP,       1}
#define Led2_Cfg 		        {GPIOA,     GPIO_PIN_4,     	GPIO_MODE_OUTPUT_PP,       1}
#define ChargerEn_Cfg 		    {GPIOA,     GPIO_PIN_0,     	GPIO_MODE_OUTPUT_PP,       0}
#define ChargerInt_Cfg 		    {GPIOA,     GPIO_PIN_1,     	GPIO_MODE_INPUT,  1}
#define EvenBalEn_Cfg 		    {GPIOB,     GPIO_PIN_1,     	GPIO_MODE_OUTPUT_OD,       1}
#define OddBalEn_Cfg 		    {GPIOB,     GPIO_PIN_2,     	GPIO_MODE_OUTPUT_OD,       1}
#define EvenCtrl1_Cfg 		    {GPIOA,     GPIO_PIN_9,     	GPIO_MODE_OUTPUT_OD,       1}
#define EvenCtrl2_Cfg 		    {GPIOA,     GPIO_PIN_10,     	GPIO_MODE_OUTPUT_OD,       1}
#define EvenCtrl3_Cfg 		    {GPIOA,     GPIO_PIN_11,     	GPIO_MODE_OUTPUT_OD,       1}
#define EvenCtrl0_Cfg 		    {GPIOA,     GPIO_PIN_12,     	GPIO_MODE_OUTPUT_OD,       1}
#define OddCtrl1_Cfg 		    {GPIOA,     GPIO_PIN_15,     	GPIO_MODE_OUTPUT_OD,       1}
#define OddCtrl2_Cfg 		    {GPIOB,     GPIO_PIN_3,     	GPIO_MODE_OUTPUT_OD,       1}
#define OddCtrl3_Cfg 		    {GPIOB,     GPIO_PIN_4,     	GPIO_MODE_OUTPUT_OD,       1}
#define OddCtrl0_Cfg 		    {GPIOB,     GPIO_PIN_5,     	GPIO_MODE_OUTPUT_OD,       1}
 
/**********GPIO接口设备数量配置**********/
#define MAX_LED_IO_COUNT 2

/********************************************
*****************IIC CONFIG******************
*********************************************/
#define IIC_List(_) 	_(CHGER) \

#define DefIICChEnum(ch) IIC_CH_##ch,

enum
{
	IIC_List(DefIICChEnum)
	IICCh_Num
};

#define IIC_CHGER_Cfg 	{GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_2}

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

#define UART_UPPER_Cfg {USART1, GPIOB, GPIO_PIN_7, GPIOB, GPIO_PIN_6, 57600, 0}

#endif //__ACTBAL_BSP_V10_H__
