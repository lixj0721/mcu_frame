#include "adp_stm32f1_uart.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_cfg.h"

pUartIntCallBackFunc UartIntProcessFunc = NULL;

struct UartCfgStruct{
    USART_TypeDef *uart;
    GPIO_TypeDef *txPort;
	uint16_t txPin;
    GPIO_TypeDef *rxPort;
	uint16_t rxPin;
    uint16_t baudrate;
    uint32_t remap;
};

#define DefUartCfg(a)	UART_##a##_Cfg,

const struct UartCfgStruct g_uartCfg[] = 
{
	UART_List(DefUartCfg)
};

void AdpUartRegCallbackFunc(pUartIntCallBackFunc func)
{
    UartIntProcessFunc = func;
}

static void uart_rcc_cfg(USART_TypeDef *uart, uint8_t *intCh)
{
    switch((uint32_t)uart)
    {
    case (uint32_t)USART1:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        *intCh = USART1_IRQn;
        break;
    case (uint32_t)USART2:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        *intCh = USART2_IRQn;
        break;
    case (uint32_t)USART3:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        *intCh = USART3_IRQn;
        break;
    case (uint32_t)UART4:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
        *intCh = UART4_IRQn;
        break;
    case (uint32_t)UART5:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
        *intCh = UART5_IRQn;
        break;
    default:
        break;
    }
}

static void uart_nvic_cfg(uint8_t ch)
{
    NVIC_InitTypeDef cfg;

    cfg.NVIC_IRQChannel = ch;
    cfg.NVIC_IRQChannelSubPriority = 1;
    cfg.NVIC_IRQChannelPreemptionPriority = 2;
    cfg.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&cfg);
}

int32_t AdpUartInit(void)
{
    USART_InitTypeDef cfg;
    GPIO_InitTypeDef gpio;
    uint8_t intCh=0;

    cfg.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    cfg.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    cfg.USART_Parity = USART_Parity_No;
    cfg.USART_StopBits = USART_StopBits_1;
    cfg.USART_WordLength = USART_WordLength_8b;

    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    for(int i=0;i<UARTCh_Num;i++)
    {
        /*时钟配置，目前配置方式还是比较暴力，后面看是否有其他方式*/
        uart_rcc_cfg(g_uartCfg[i].uart, &intCh);

        /*gpio的配置*/
        gpio.GPIO_Pin = g_uartCfg[i].txPin;
        GPIO_Init(g_uartCfg[i].txPort, &gpio);

        gpio.GPIO_Pin = g_uartCfg[i].rxPin;
        GPIO_Init(g_uartCfg[i].rxPort, &gpio);

        if(g_uartCfg[i].remap != 0)
        {
            GPIO_PinRemapConfig(g_uartCfg[i].remap, ENABLE);
        }

        /*uart的配置，目前只针对通用情况，若有其他情况，可以再开接口重新配置*/
        cfg.USART_BaudRate = g_uartCfg[i].baudrate;
        USART_Init(g_uartCfg[i].uart, &cfg);

        /*串口中断配置，先只开通接收中断，发送中断在FIFO处理中控制*/
        USART_ITConfig(g_uartCfg[i].uart, USART_IT_RXNE, ENABLE);

        uart_nvic_cfg(intCh);

        /*NVIC配置*/

        USART_Cmd(g_uartCfg[i].uart,ENABLE);
    }

    return 0;
}

int32_t AdpUartCfgTxInt(uint8_t obj, uint8_t state)
{
    if(obj >= UARTCh_Num)
    {
        return -1;
    }

    USART_ITConfig(g_uartCfg[obj].uart, USART_IT_TC, ENABLE);

    return 0;
}

int32_t AdpUartClrRxFlag(uint8_t obj)
{
    /*这部分可要可不要*/
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    USART_ClearITPendingBit(g_uartCfg[obj].uart, USART_IT_RXNE);

    return 0;
}

int32_t AdpUartClrTxFlag(uint8_t obj)
{
    /*这部分可要可不要*/
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    USART_ClearITPendingBit(g_uartCfg[obj].uart, USART_IT_TC);

    return 0;
}

static int8_t GetUartIndex(USART_TypeDef *uart)
{
    for(int i=0;i<UARTCh_Num;i++)
    {
        if(g_uartCfg[i].uart == uart)
            return i;
    }
    return -1;
}

void AdpUartSendByte(uint8_t obj, uint8_t data)
{
    if(obj >= UARTCh_Num)
    {
        return;
    }
    USART_SendData(g_uartCfg[obj].uart, data);
}

void AdpUartRecvByte(uint8_t obj, uint8_t *data)
{
    if(obj >= UARTCh_Num)
    {
        return;
    }
    *data = USART_ReceiveData(g_uartCfg[obj].uart);
}

uint8_t AdpUartGetRxIntFlag(uint8_t obj)
{
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    return (USART_GetITStatus(g_uartCfg[obj].uart, USART_IT_RXNE)?1:0);
}

uint8_t AdpUartGetTxIntFlag(uint8_t obj)
{
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    return (USART_GetITStatus(g_uartCfg[obj].uart, USART_IT_TC)?1:0);
}

void USART1_IRQHandler(void)
{
    /*获取序号这部分后续看是否可以有办法优化*/
    int8_t index = GetUartIndex(USART1);
    if(index != -1 && UartIntProcessFunc != NULL)
    {
        UartIntProcessFunc(index);
    }
}
