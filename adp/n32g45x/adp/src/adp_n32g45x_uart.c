#include "adp_n32g45x_uart.h"
#include "n32g45x.h"
#include "n32g45x_conf.h"
#include "bsp_cfg.h"

pUartIntCallBackFunc UartIntProcessFunc = NULL;

struct UartCfgStruct{
    USART_Module *uart;
    GPIO_Module *txPort;
	uint16_t txPin;
    GPIO_Module *rxPort;
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

static void uart_rcc_cfg(USART_Module *uart, uint8_t *intCh)
{
    switch((uint32_t)uart)
    {
    case (uint32_t)USART1:
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, ENABLE);
        *intCh = USART1_IRQn;
        break;
    case (uint32_t)USART2:
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2, ENABLE);
        *intCh = USART2_IRQn;
        break;
    case (uint32_t)USART3:
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART3, ENABLE);
        *intCh = USART3_IRQn;
        break;
    case (uint32_t)UART4:
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_UART4, ENABLE);
        *intCh = UART4_IRQn;
        break;
    case (uint32_t)UART5:
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_UART5, ENABLE);
        *intCh = UART5_IRQn;
        break;
    default:
        break;
    }
}

static void uart_nvic_cfg(uint8_t ch)
{
    NVIC_InitType cfg;

    cfg.NVIC_IRQChannel = ch;
    cfg.NVIC_IRQChannelSubPriority = 1;
    cfg.NVIC_IRQChannelPreemptionPriority = 2;
    cfg.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&cfg);
}

int32_t AdpUartInit(void)
{
    USART_InitType cfg;
    GPIO_InitType gpio;
    uint8_t intCh=0;

    cfg.HardwareFlowControl = USART_HFCTRL_NONE;
    cfg.Mode = USART_MODE_RX | USART_MODE_TX;
    cfg.Parity = USART_PE_NO;
    cfg.StopBits = USART_STPB_1;
    cfg.WordLength = USART_WL_8B;

    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    for(int i=0;i<UARTCh_Num;i++)
    {
        /*时钟配置，目前配置方式还是比较暴力，后面看是否有其他方式*/
        uart_rcc_cfg(g_uartCfg[i].uart, &intCh);

        /*gpio的配置*/
        gpio.Pin = g_uartCfg[i].txPin;
        gpio.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitPeripheral(g_uartCfg[i].txPort, &gpio);

        gpio.Pin = g_uartCfg[i].rxPin;
        gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitPeripheral(g_uartCfg[i].rxPort, &gpio);

        if(g_uartCfg[i].remap != 0)
        {
            GPIO_ConfigPinRemap(g_uartCfg[i].remap, ENABLE);
        }

        /*uart的配置，目前只针对通用情况，若有其他情况，可以再开接口重新配置*/
        cfg.BaudRate = g_uartCfg[i].baudrate;
        USART_Init(g_uartCfg[i].uart, &cfg);

        /*串口中断配置，先只开通接收中断，发送中断在FIFO处理中控制*/
        USART_ConfigInt(g_uartCfg[i].uart, USART_INT_RXDNE, ENABLE);

        uart_nvic_cfg(intCh);

        /*NVIC配置*/

        USART_Enable(g_uartCfg[i].uart,ENABLE);
    }

    return 0;
}

int32_t AdpUartCfgTxInt(uint8_t obj, uint8_t state)
{
    if(obj >= UARTCh_Num)
    {
        return -1;
    }

    USART_ConfigInt(g_uartCfg[obj].uart, USART_INT_TXC, state);

    return 0;
}

int32_t AdpUartClrRxFlag(uint8_t obj)
{
    /*这部分可要可不要*/
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    USART_ClrIntPendingBit(g_uartCfg[obj].uart, USART_INT_RXDNE);

    return 0;
}

int32_t AdpUartIsTxIntEnable(uint8_t obj)
{
    /*这部分可要可不要*/
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    return ((g_uartCfg[obj].uart->CTRL1 & USART_CTRL1_TXCIEN)?1:0);
}

int32_t AdpUartClrTxFlag(uint8_t obj)
{
    /*这部分可要可不要*/
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    USART_ClrIntPendingBit(g_uartCfg[obj].uart, USART_INT_TXC);

    return 0;
}

static int8_t GetUartIndex(USART_Module *uart)
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
    return (USART_GetIntStatus(g_uartCfg[obj].uart, USART_INT_RXDNE)?1:0);
}

uint8_t AdpUartGetTxIntFlag(uint8_t obj)
{
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    return (USART_GetIntStatus(g_uartCfg[obj].uart, USART_INT_TXC)?1:0);
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

void USART2_IRQHandler(void)
{
    /*获取序号这部分后续看是否可以有办法优化*/
    int8_t index = GetUartIndex(USART2);
    if(index != -1 && UartIntProcessFunc != NULL)
    {
        UartIntProcessFunc(index);
    }
}
