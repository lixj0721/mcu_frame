#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "toolkit.h"

extern uint32_t g_printEnable;
extern uint32_t g_printLevel;
extern uint32_t g_printMask;

void Debug_Printf(char *fmt, ...);

typedef enum
{
	RESP_DATA_AND_FRAMEWORK,	/*需要加框架信息*/
	RESP_DATA					/*不需要框架信息，直接发送*/
}RESP_DATA_TYPE_ENUM;

#define LOG_CLOSE 0
#define LOG_ERROR 1
#define LOG_WAEN  2
#define LOG_INFO  3
#define LOG_DEBUG 4
#define LOG_DUMP  5
#define LOG_TRACE 6

#define LOG_SWITCH_FLAG_DEBUG           (0x01<<0 )
#define LOG_SWITCH_FLAG_CAN             (0x01<<1 )
#define LOG_SWITCH_FLAG_CANOPEN         (0x01<<2 )
#define LOG_SWITCH_FLAG_485             (0x01<<3 )
#define LOG_SWITCH_FLAG_TIMER           (0x01<<4 )
#define LOG_SWITCH_FLAG_MMC             (0x01<<5 )
#define LOG_SWITCH_FLAG_DATA_PROC       (0x01<<6 )
#define LOG_SWITCH_FLAG_HAEDWARE        (0x01<<7 )
#define LOG_SWITCH_FLAG_DB              (0x01<<8 )
#define LOG_SWITCH_FLAG_MODBUS          (0x01<<9 )
#define LOG_SWITCH_FLAG_CANRBUS         (0X01<<10)
#define LOG_SWITCH_FLAG_DATAHUB         (0X01<<11)
#define LOG_SWITCH_FLAG_MAINCTRL        (0X01<<12)
#define LOG_SWITCH_FLAG_SOC             (0X01<<13)
#define LOG_SWITCH_FLAG_SLAVE           (0X01<<14)
#define LOG_SWITCH_FLAG_BALANCE         (0X01<<15)
#define LOG_SWITCH_FLAG_CURRENT         (0X01<<16)
#define LOG_SWITCH_FLAG_HVB             (0X01<<17)
#define LOG_SWITCH_FLAG_PCS             (0X01<<18)
#define LOG_SWITCH_FLAG_UPDATE          (0X01<<19)
#define LOG_SWITCH_FLAG_BALANCE_ALG     (0X01<<20)
#define LOG_SWITCH_FLAG_STRATEGY        (0X01<<21)
#define LOG_SWITCH_FLAG_COMPROL         (0X01<<22)
#define LOG_SWITCH_FLAG_STORAGE         (0X01<<23)
#define LOG_SWITCH_FLAG_KEY             (0X01<<24)
#define LOG_SWITCH_FLAG_PROTOCOL        (0X01<<25)
#define LOG_SWITCH_FLAG_ALARM           (0x01<<26)
#define LOG_SWITCH_FLAG_SAMPLE          (0X01<<27)
#define LOG_SWITCH_FLAG_CHARGER         (0X01<<28)
#define LOG_SWITCH_FLAG_PROJECT_FUNC    (0X01<<29)
#define LOG_SWITCH_FLAG_LCC             (0X01<<30)
#define LOG_SWITCH_FLAG_WAKEUP          (0X01<<31)

#define BMS_RAW_PRINT(switchFalg, msg, arg...) \
    do{ \
    if((uint32_t)switchFalg&g_debugMask)Debug_Printf(msg, ##arg); \
    }while(0)

#define BMS_LOG_ERROR(switchFalg, msg, arg...) \
    do{ \
        if(g_debugLevel >= LOG_ERROR){ \
            if((uint32_t)switchFalg&g_debugMask)Debug_Printf(msg, ##arg); \
        }\
    }while(0)

#define BMS_LOG_WAEN(switchFalg, msg, arg...)\
    do{ \
        if(g_debugLevel >= LOG_WAEN){ \
            if((uint32_t)switchFalg&g_debugMask)Debug_Printf(msg, ##arg); \
        }\
    }while(0)

#define BMS_LOG_INFO(switchFalg, msg, arg...) \
    do{ \
        if(g_debugLevel >= LOG_INFO){ \
            if((uint32_t)switchFalg&g_debugMask)Debug_Printf(msg, ##arg); \
        }\
    }while(0)

#define BMS_LOG_DEBUG(switchFalg, msg, arg...) \
    do{ \
        if(g_debugLevel >= LOG_DEBUG){ \
            if((uint32_t)switchFalg&g_debugMask)Debug_Printf(msg, ##arg); \
        }\
    }while(0)

#define BMS_LOG_TRACE(switchFalg, msg, arg...)  \
    do{ \
        if(g_debugLevel >= LOG_TRACE){ \
            if((uint32_t)switchFalg&g_debugMask)Debug_Printf(msg, ##arg); \
        }\
    }while(0)

#define BMS_LOG_DUMP(switchFalg, msg, arg...)  \
    do{ \
        if(g_debugLevel >= LOG_DUMP){ \
            if((uint32_t)switchFalg&g_debugMask)Debug_Printf(msg, ##arg); \
        }\
    }while(0)


#endif  //__DEBUG_H__
