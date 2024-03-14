#include "io_op.h"
#include "bsp_cfg.h"
#include "task_scheduler.h"
#include "debug.h"

/*
输出功能，主要是作为LED输出功能
目前暂时只支持持续的控制方式，切换需要外部控制
*/

struct LedCtrlConfig g_ledConfigInfo[MAX_LED_IO_COUNT] = {0};

static void LedCtrlProcess(void *param)
{
    (void)param;

    for(int i=0;i<MAX_LED_IO_COUNT;i++)
    {
        if(g_ledConfigInfo[i].used == 0)/*未使用的接口，目前没有设置注销功能，其实可以直接返回结束*/
        {
            continue;
        }
        g_ledConfigInfo[i].stateLastTime++;
        if(g_ledConfigInfo[i].lastState == g_ledConfigInfo[i].onState 
            && g_ledConfigInfo[i].stateLastTime >= g_ledConfigInfo[i].onTime 
            && g_ledConfigInfo[i].offTime > 0)
        {
            AdpGpioSet(g_ledConfigInfo[i].ledio, !g_ledConfigInfo[i].onState);
            g_ledConfigInfo[i].lastState = !g_ledConfigInfo[i].onState;
            g_ledConfigInfo[i].stateLastTime = 0;
        }
        else if(g_ledConfigInfo[i].lastState == !g_ledConfigInfo[i].onState 
            && g_ledConfigInfo[i].stateLastTime >= g_ledConfigInfo[i].offTime 
            && g_ledConfigInfo[i].onTime > 0)
        {
            AdpGpioSet(g_ledConfigInfo[i].ledio, g_ledConfigInfo[i].onState);
            g_ledConfigInfo[i].lastState = g_ledConfigInfo[i].onState;
            g_ledConfigInfo[i].stateLastTime = 0;
        }
    }
}

int32_t LedCtrlRegLed(struct LedCtrlConfig *cfg)
{
    for(int i=0;i<MAX_LED_IO_COUNT;i++)
    {
        /*这里还有一个是否需要判断当前的IO是否已经被使用*/
        if(g_ledConfigInfo[i].used == 0)
        {
            memcpy(&g_ledConfigInfo[i], cfg, sizeof(struct LedCtrlConfig));
            g_ledConfigInfo[i].used = 1;
            g_ledConfigInfo[i].lastState = g_ledConfigInfo[i].initState;
            g_ledConfigInfo[i].stateLastTime = 0;
            AdpGpioSet(g_ledConfigInfo[i].ledio, g_ledConfigInfo[i].initState);/*默认设置为灭灯，后续可以设置初始状态*/
            g_ledConfigInfo[i].stateLastTime = 0;
            return 0;
        }
    }
    /*所有的配置都已经占用*/
    return -1;
}

/*输入功能，主要是使用按键输入
几个事件：
a、短按：持续时间大于300ms，短于3s
b、长按：持续时间大于5s
c、双击：每次按下都符合短按条件，两次间隔时间大于200ms小于500ms
*/
struct KeyInputConfig
{
    /* data */
    uint32_t keyId;
    uint8_t  validState; /*有效电平，即按下的电平*/
    uint8_t  lastState;
    uint8_t  shortFlag;/*前置状态是否为短按，用于判断双击*/
    uint16_t lastStateCnt;
    KeyEventFunc func[KEY_EVENT_BOT];
};

int32_t IOOpInit(void)
{
    int32_t ret = 0;
    memset(&g_ledConfigInfo[0], 0, sizeof(g_ledConfigInfo));
    ret |= RegTask(LedCtrlProcess, LED_CTRL_PROCESS_INTERVAL, NULL);

    return ret;
}
