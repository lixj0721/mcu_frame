#ifndef __ACTBAL_BSP_V10_H__
#define __ACTBAL_BSP_V10_H__

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

#define Led1_Cfg 		        {GPIOA,     GPIO_PIN_5,     GPIO_Mode_Out_PP,       1}
#define Led2_Cfg 		        {GPIOA,     GPIO_PIN_4,     GPIO_Mode_Out_PP,       1}
 
/**********GPIO接口设备数量配置**********/
#define MAX_LED_IO_COUNT 2

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

#define UART_LINK_Cfg {USART1, GPIOB, GPIO_PIN_7, GPIOB, GPIO_PIN_6, 57600, 0}

#endif //__ACTBAL_BSP_V10_H__
