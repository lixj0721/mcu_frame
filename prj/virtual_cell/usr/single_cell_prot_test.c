#include "bsp_cfg.h"
#include "adp_config.h"
#include "dev_tpa626_inf.h"
#include "lv_tte_db.h"
#include "toolkit.h"

/*切换电源输出电压值
0：无输出
1：5V输出
2：10V输出
*/
static void set_pwr_src_value(uint8_t value)
{
    switch(value){
    case 0:
        AdpGpioSet(GpioCh_PWR_CTRL_EN, 1);
        break;
    case 1:
        AdpGpioSet(GpioCh_PWR_CTRL_SW, 0);
        AdpGpioSet(GpioCh_PWR_CTRL_EN, 0);
        break;
    case 2:
        AdpGpioSet(GpioCh_PWR_CTRL_SW, 1);
        AdpGpioSet(GpioCh_PWR_CTRL_EN, 0);
        break;
    default:
        break;
    }
}

static uint8_t get_pwr_src_value(void)
{
    if(AdpGpioGet(GpioCh_PWR_CTRL_EN)) return 0;
    if(AdpGpioGet(GpioCh_PWR_CTRL_SW)) return 2;
    return 1;
}

/*负载开关
0：负载关断
1：负载挂载
*/
static void set_load_state(uint8_t state)
{
    AdpGpioSet(GpioCh_LoadCtrl, (state)?1:0);
}

static uint8_t get_load_state(void)
{
    return AdpGpioGet(GpioCh_LoadCtrl);
}

static void set_chg_current_state(uint8_t state)
{
    AdpGpioSet(GpioCh_ChgCurrCtrl, (state)?1:0);
}

static uint8_t get_chg_current_state(void)
{
    return AdpGpioGet(GpioCh_ChgCurrCtrl);
}

static void set_iq_state(uint8_t state)
{
    AdpGpioSet(GpioCh_IQ_TEST_CTRL, (state)?1:0);
}

static uint8_t get_iq_state(void)
{
    return AdpGpioGet(GpioCh_IQ_TEST_CTRL);
}

static uint8_t get_ledin_state(void)
{
    return AdpGpioGet(GpioCh_LED_IN);
}

/*静态电流测试控制
0：直连
1：静态测试
*/
static void set_static_current_test_state(uint8_t state)
{
    AdpGpioSet(GpioCh_IQ_TEST_CTRL, (state)?1:0);
}
/*获取静态功耗及输出电压值*/
#define IQ_SAMPLE_RESISTOR 1000
static int32_t get_static_current(int16_t *iq, uint16_t *batVol)
{
    int16_t  shuntVol;
    uint16_t busVol;
    int32_t ret = 0;

    ret = DevTpa626ReadShuntVol(DEV_TPA626_IQ, &shuntVol);
    ret |= DevTpa626ReadBusVol(DEV_TPA626_IQ, &busVol);
    if(iq != NULL)
    {
        *iq = shuntVol / IQ_SAMPLE_RESISTOR;
    }
    if(batVol != NULL)
    {
        *batVol = busVol;
    }
    return ret;
}
#define VIRTUAL_CELL_SAMPLE_RESISTOR 5
static int32_t get_virtual_cell_info(int16_t *curr, uint16_t *vol)
{
    int16_t  shuntVol;
    uint16_t busVol;
    int32_t ret = 0;

    ret = DevTpa626ReadShuntVol(DEV_TPA626_IQ, &shuntVol);
    ret |= DevTpa626ReadBusVol(DEV_TPA626_IQ, &busVol);
    if(curr != NULL)
    {
        *curr = shuntVol / VIRTUAL_CELL_SAMPLE_RESISTOR;
    }
    if(vol != NULL)
    {
        *vol = busVol;
    }
    return ret;
} 
#define PWR_SRC_SAMPLE_RESISTOR 5
static int32_t get_pwr_src_output(int16_t *curr, uint16_t *vol)
{
    int16_t  shuntVol;
    uint16_t busVol;
    int32_t ret = 0;

    ret = DevTpa626ReadShuntVol(DEV_TPA626_IQ, &shuntVol);
    ret |= DevTpa626ReadBusVol(DEV_TPA626_IQ, &busVol);
    if(curr != NULL)
    {
        *curr = shuntVol / VIRTUAL_CELL_SAMPLE_RESISTOR;
    }
    if(vol != NULL)
    {
        *vol = busVol;
    }
    return ret;
}

int32_t DB_FUNC_LoadCtrl(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_READ) {
        *buf = get_load_state(); 
    } else {
        set_load_state(*buf?1:0);
    }
    return 0;
}

int32_t DB_FUNC_ChgCurrCtrl(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_READ) {
        *buf = get_chg_current_state(); 
    } else {
        set_chg_current_state(*buf?1:0);
    }
    return 0;
}

int32_t DB_FUNC_IqCtrl(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_READ) {
        *buf = get_iq_state(); 
    } else {
        set_iq_state(*buf?1:0);
    }
    return 0;
}

int32_t DB_FUNC_PwrSrcCtrl(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_READ) {
        *buf = get_pwr_src_value(); 
    } else {
        set_pwr_src_value((*buf) >> 8);
    }
    return 0;
}

int32_t DB_FUNC_LedIn(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_READ){
        *buf = get_ledin_state();
        return 0;
    }
    return -1;
}

int32_t DB_FUNC_StaticVol(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_WRITE)
    {
        return -1;
    }
    get_static_current(NULL, buf);
    return 0;
}

int32_t DB_FUNC_StaticCurr(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_WRITE)
    {
        return -1;
    }
    get_static_current(buf, NULL);
    return 0;
}

int32_t DB_FUNC_CellVol(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_WRITE)
    {
        return -1;
    }
    get_virtual_cell_info(NULL, buf);
    return 0;
}

int32_t DB_FUNC_CellCurr(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_WRITE)
    {
        return -1;
    }
    get_virtual_cell_info(buf, NULL);
    return 0;
}

int32_t DB_FUNC_PwrSrcVol(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_WRITE)
    {
        return -1;
    }
    get_pwr_src_output(NULL, buf);
    return 0;
}

int32_t DB_FUNC_PwrSrcCurr(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    if(opType == DB_OP_WRITE)
    {
        return -1;
    }
    get_pwr_src_output(buf, NULL);
    return 0;
}

int32_t DB_FUNC_CellVol(uint8_t opType, uint16_t addr, uint16_t *buf)
{
    
}

