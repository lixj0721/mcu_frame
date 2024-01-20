#include "toolkit.h"
#include "adp_config.h"
#include "task_scheduler.h"
#include "led_app.h"

extern int32_t UsrAppInit(void);

int main(void)
{
    int32_t ret = 0;
    ret |= AdpConfig();
    ret |= TaskScheduleInit();
    ret |= LedAppInit();
    ret |= UsrAppInit();

    while(1)
    {
        TaskScheduleRun();
    }
}
