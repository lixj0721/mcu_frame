#ifndef __TASKSCHEDULE_H__
#define __TASKSCHEDULE_H__

#include "toolkit.h"

typedef void (*TaskFunc)(void *param);

int32_t TaskScheduleInit(void);
int32_t RegTask(TaskFunc func, int32_t period, void *param);
int32_t TaskScheduleRun(void);

#endif  //__TASKSCHEDULE_H__
