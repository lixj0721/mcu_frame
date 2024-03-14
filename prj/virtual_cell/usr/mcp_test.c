// #include "dev_mcp4551_inf.h"
// #include "dev_tpa626_inf.h"
#include "task_scheduler.h"
#include "lv_tte_db.h"
#include "modbus_slave_inf.h"
#include "db_inf.h"
#include "bsp_cfg.h"
#include "uart_inf.h"

uint16_t busvol = 0;
int16_t curr = 0;

// void test_process(void *param)
// {
//     static int i = 0;
//     volatile uint16_t vol = 0;   

//     if(DevTpa626ReadBusVol(0, (uint16_t *)&vol)!=0)
//     {
//         vol = vol -1;
//     }
//     DevTpa626ReadShuntVol(0, &curr);
//     busvol = vol * 10 / 8;
//     curr /= 2;
//     if(DevMcp4551_SetWiper(0, 127)!=0)
//     {
//         i--;
//     }
// }

// int32_t test_init(void)
// {
//     int32_t ret = 0;
//     ret |= RegTask(test_process, 2000, NULL);
//     ret |= DevTpa626Init();
//     return ret;
// }

int32_t g_modbusObjId = -1;

int32_t UsrUartMsgProcess(uint8_t obj, uint8_t *buf, uint16_t len)
{
    int32_t ret = 0;

    ret |= ModbusSlave_MsgProcess(g_modbusObjId, buf, len);

    return ret;
}

int32_t UsrModbusDbInit(void)
{
    struct ModbusInitInf_t modbusObj;
    int32_t ret = 0;

    modbusObj.type = MODBUS_RTU;
    modbusObj.refId = UARTCh_UPPER;
    modbusObj.devAddr = 0x1;
    modbusObj.pfReadCallback = DbReadProc;
    modbusObj.pfWriteCallback = DbWriteProc;

    ret |= ModbusSlave_InterfaceInit(&modbusObj);
    if(ret >= 0)
    {
        g_modbusObjId = ret;
    }

    ret |= UartInfRegCallbackFunc(UARTCh_UPPER, UsrUartMsgProcess);

    return ret;
}

int32_t UsrAppInit(void)
{
    int32_t ret = 0;

    ret |= UsrModbusDbInit();
    ret |= LvTteDbInit();

    return ret;
}
