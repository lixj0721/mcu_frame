#include "uart_inf.h"
#include "bsp_cfg.h"
#include "adp_config.h"
#include "task_scheduler.h"

#define MAX_UART_INF_CALLBACL_FUNC_NUM 5
#define UART_MSG_INTERVAL_TIME_IN_ms 5

struct UartInfInfo_t {
    uint8_t obj;
    FifoInf_Handel txFifo;
    FifoInf_Handel rxFifo;
    uint32_t lastRxTickCnt;/*用于判断接收数据包完成*/
    // uint8_t msgPackRxCompleteFlag;
    pUartMsgProcessCallBackFunc func[MAX_UART_INF_CALLBACL_FUNC_NUM];
};

struct UartInfInfo_t g_uartInfInfo[UARTCh_Num];

void UartInfIntProcess(uint8_t obj);
void UartInfMsgProcess(void *param);

int32_t UartInfInit(void)
{
    for(int i=0;i<UARTCh_Num;i++)
    {
        g_uartInfInfo[i].obj = i;
        // g_uartInfInfo[i].msgPackRxCompleteFlag = 0;
        g_uartInfInfo[i].rxFifo = FifoInf_Init(UART_INF_MSG_FIFO, sizeof(uint8_t));
        g_uartInfInfo[i].txFifo = FifoInf_Init(UART_INF_MSG_FIFO, sizeof(uint8_t));
        if(g_uartInfInfo[i].rxFifo == NULL || g_uartInfInfo[i].txFifo == NULL)
        {
            return -1;
        }
        for(int j=0;j<MAX_UART_INF_CALLBACL_FUNC_NUM;j++)
        {
            g_uartInfInfo[i].func[j] = NULL;
        }
    }

    AdpUartRegCallbackFunc(UartInfIntProcess);
    /*周期性处理接收数据，暂定10ms处理一次*/
    RegTask(UartInfMsgProcess, 10, NULL);

    return 0;
}

int32_t UartInfSendMsg(uint8_t obj, uint8_t *buf, uint16_t len)
{
    int32_t ret = 0;
    if(obj >= UARTCh_Num || len == 0) return -1;

    // if(FifoInf_Len(g_uartInfInfo[obj].txFifo) == 0)
    if(AdpUartIsTxIntEnable(obj) == 0)
    {
        AdpUartClrTxFlag(obj);
        AdpUartSendByte(obj, buf[0]);
        AdpUartCfgTxInt(obj, TRUE);
        buf++;
        len--;
    }
    if(len > 0)
    {
        ret = (FifoInf_Add(g_uartInfInfo[obj].txFifo, buf, len)==len)?0:-1;
    }

    return ret;
}

int32_t UartInfRecvMsg(uint8_t obj, uint8_t *buf, uint16_t len)
{
    uint16_t ret = 0;
    uint32_t tick = AdpGetSystickCnt();

    if(obj>=UARTCh_Num) return -1;

    // if(g_uartInfInfo[obj].msgPackRxCompleteFlag == FALSE)
    if((tick - g_uartInfInfo[obj].lastRxTickCnt) <= UART_MSG_INTERVAL_TIME_IN_ms)
    {
        return 0;
    }
    ret = FifoInf_Del(g_uartInfInfo[obj].rxFifo, buf, MIN(len, UART_INF_MSG_FIFO));

    return ret;
}

void UartInfTxIntProcess(uint8_t obj)
{
    uint8_t ret, data;
    if(AdpUartIsTxIntEnable(obj) == 0)return;
    if(AdpUartGetTxIntFlag(obj))
    {
        ret = FifoInf_Del(g_uartInfInfo[obj].txFifo, &data, 1);
        if(ret == 1)
        {
            AdpUartClrTxFlag(obj);
            AdpUartSendByte(obj, data);
        }
        else
        {
            AdpUartCfgTxInt(obj, FALSE);
        }
    }
}

void UartInfRxIntProcess(uint8_t obj)
{
    uint8_t data;
    if(AdpUartGetRxIntFlag(obj))
    {
        AdpUartClrRxFlag(obj);
        AdpUartRecvByte(obj, &data);
        FifoInf_Add(g_uartInfInfo[obj].rxFifo, &data, 1);
        g_uartInfInfo[obj].lastRxTickCnt = AdpGetSystickCnt();
    }
}

void UartInfIntProcess(uint8_t obj)
{
    UartInfTxIntProcess(obj);
    UartInfRxIntProcess(obj);
}

int32_t UartInfRegCallbackFunc(uint8_t obj, pUartMsgProcessCallBackFunc func)
{
    if(obj >= UARTCh_Num)
    {
        return -1;
    }
    for(int i=0;i<MAX_UART_INF_CALLBACL_FUNC_NUM;i++)
    {
        // if(g_uartInfInfo[obj].funcUsed[i])
        if(g_uartInfInfo[obj].func[i] == NULL)
        {
            g_uartInfInfo[obj].func[i] = func;
            return 0;
        }
    }
    return -1;
}

void UartInfMsgProcess(void *param)
{
    uint8_t buf[UART_INF_MSG_FIFO];
    uint16_t len;
    (void)param;
    for(int i=0;i<UARTCh_Num;i++)
    {
        len = UartInfRecvMsg(i, buf, UART_INF_MSG_FIFO);
        // len = FifoInf_Del(g_uartInfInfo[i].rxFifo, buf, UART_INF_MSG_FIFO);
        if(len > 0)
        {
            for(int j=0;j<MAX_UART_INF_CALLBACL_FUNC_NUM;j++)
            {
                if(g_uartInfInfo[i].func[j] != NULL)
                {
                    g_uartInfInfo[i].func[j](i, buf, len);
                }
            }
        }
        
    }
}
