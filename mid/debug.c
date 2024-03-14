#include "debug.h"
#include <stdarg.h>
#include "bsp_cfg.h"
#include "uart_inf.h"

uint32_t g_printEnable = TRUE;
uint32_t g_printLevel = 0;
uint32_t g_printMask = 0;

int32_t Transfer_RespData(uint32_t type, uint8_t *buf, uint32_t len)
{
#ifdef DEBUG_METHOD_UART
    // void(type);/*暂时不区分*/
    return UartInfSendMsg(DEBUG_PORT_INDEX, buf, len);
#endif
    return 0;
}

void Debug_Printf(char *fmt, ...)
{
    va_list ap;
    uint8_t buf[UART_INF_MSG_FIFO];
    /*uint32_t localHwType;*/

    va_start(ap,fmt);
#ifdef my_vsnprintf
    my_vsnprintf((char *)buf,sizeof(buf),fmt,ap);
#else
    my_vsprintf((char *)buf,fmt,ap);
#endif
    if(g_printEnable == TRUE)
    {
        Transfer_RespData(RESP_DATA, buf,(uint32_t)strlen((char *)buf));
    }
    va_end(ap);
}

