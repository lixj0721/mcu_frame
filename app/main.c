#include "toolkit.h"
#include "adp_config.h"
#include "task_scheduler.h"
#include "led_app.h"
#include "debug.h"
#include "uart_inf.h"

extern int32_t UsrAppInit(void);

int main(void)
{
    int32_t ret = 0;
    ret |= AdpConfig();
    ret |= TaskScheduleInit();
    ret |= UartInfInit();
    ret |= LedAppInit();
    ret |= UsrAppInit();

    if(ret == 0)
    {
        Debug_Printf("SYSTEM START\r\n");
    }
    else
    {
        Debug_Printf("SYSTEM INIT FAILED: %d\r\n", ret);
    }

    while(1)
    {
        TaskScheduleRun();
    }
}
