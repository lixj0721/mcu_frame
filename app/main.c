#include "toolkit.h"
#include "adp_config.h"
#include "task_scheduler.h"
#include "led_app.h"

int main(void)
{
    int32_t ret = 0;
    ret |= AdpConfig();
    ret |= TaskScheduleInit();
    ret |= LedAppInit();

    while(1)
    {
        TaskScheduleRun();
    }
}
