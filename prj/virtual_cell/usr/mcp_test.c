#include "dev_mcp4551_inf.h"
#include "dev_tpa626_inf.h"
#include "task_scheduler.h"

uint16_t busvol = 0;
int16_t curr = 0;

void test_process(void *param)
{
    static int i = 0;
    volatile uint16_t vol = 0;   

    if(DevTpa626ReadBusVol(0, &vol)!=0)
    {
        vol = vol -1;
    }
    DevTpa626ReadShuntVol(0, &curr);
    busvol = vol * 10 / 8;
    curr /= 2;
    if(DevMcp4551_SetWiper(0, 127)!=0)
    {
        i--;
    }
}

int32_t test_init(void)
{
    int32_t ret = 0;
    ret |= RegTask(test_process, 2000, NULL);
    ret |= DevTpa626Init();
    return ret;
}

int32_t UsrAppInit(void)
{
    int32_t ret = 0;

    ret |= test_init();

    return ret;
}
