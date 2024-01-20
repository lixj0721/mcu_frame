#include "task_scheduler.h"
#include "adp_config.h"

#define MAX_TASK_COUNT 100

struct TaskStruct{
    TaskFunc func;
    int32_t time;
    int32_t tick;
    void * param;
};

struct TaskStruct g_taskStruct[MAX_TASK_COUNT] = {0};

int32_t TaskScheduleInit(void)
{
    memset((void *)&g_taskStruct, 0, MAX_TASK_COUNT * sizeof(struct TaskStruct));
    return 0;
}

int32_t RegTask(TaskFunc func, int32_t period, void *param)
{
    for(int i=0;i<MAX_TASK_COUNT;i++)
    {
        if(g_taskStruct[i].func != 0)
        {
            continue;
        }
        g_taskStruct[i].func = func;
        g_taskStruct[i].time = period;
        g_taskStruct[i].tick = AdpGetSystickCnt();
        g_taskStruct[i].param = param;
        return 0;
    }
    return -1;
}

int32_t TaskScheduleRun(void)
{
    uint32_t tick = AdpGetSystickCnt();
    int i = 0;

    for(i=0;i<MAX_TASK_COUNT;i++)
    {
        if(g_taskStruct[i].func == 0)
        {
            return 0; //已经没有需要执行的任务了
        }
        if(tick - g_taskStruct[i].tick >= g_taskStruct[i].time)
        {
            g_taskStruct[i].func(g_taskStruct[i].param);//这里不需要判断空指针是因为前面已经处理了
            g_taskStruct[i].tick = tick;
        }
    }
    return 0;
}
