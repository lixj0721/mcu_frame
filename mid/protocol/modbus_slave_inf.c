#include "modbus_slave_inf.h"
#include "uart_inf.h"

#define MODBUS_OBJ_WORK_MODE_MASTER 0
#define MODBUS_OBJ_WORK_MODE_SLAVE  1
/*定义最多的modbus对象数量，后续可调*/
#define MAX_MODBUS_OBJ_NUM 5
/*最长长度定义后续也可以放到项目定义里面，这样就可以根据项目调整modbus占用的空间*/
#define MAX_MODBUS_MSG_LENGTH      sizeof(struct WriteMultiRegMsg_t)/*288*/
#define MODBUS_MAX_REG_NUM_PER_MSG 128

/*大小端转换*/
#define MODBUS_NET_DATA_ENDIAN_TYPE ENDIAN_TYPE_BIG
#define MODBUS_NET_CRC_ENDIAN_TYPE  ENDIAN_TYPE_LITTLE
#define MODBUS_NET_TO_HOST_16(num)  ENDIAN_TYPE_SWITCH_16(MODBUS_NET_DATA_ENDIAN_TYPE, num)
#define HOST_TO_MODBUS_NET_16       MODBUS_NET_TO_HOST_16
#define MODBUS_CRC_ENDIAN_CONVERT(crc) ENDIAN_TYPE_SWITCH_16(MODBUS_NET_CRC_ENDIAN_TYPE, crc)

/*MODBUS功能码说明*/
enum{
    MODBUS_FUNCTION_CODE_READ_OUTCTRL_REG = 0x01,
    MODBUS_FUNCTION_CODE_READ_INPUT_REG = 0x02,
    MODBUS_FUNCTION_CODE_READ_HOLD_REG = 0x03,
    MODBUS_FUNCTION_CODE_READ_ANALOG_REG = 0x04,
    MODBUS_FUNCTION_CODE_WRITE_SINGLE_OUTCTRL_REG = 0x05,
    MODBUS_FUNCTION_CODE_WRITE_SINGLE_HOLD_REG = 0x06,
    MODBUS_FUNCTION_CODE_WRITE_MULTI_OUTCTRL_REG = 0x0F,
    MODBUS_FUNCTION_CODE_WRITE_MULTI_HOLD_REG = 0x10,

    MODBUS_FUNCTION_ERR_CODE_MASK = 0x80,
};
/*MODBUS返回故障码说明*/
enum{
    MODBUS_ERR_CODE_PROCESS_OK,
    MODBUS_ERR_CODE_CMD_CODE_NOT_SUPPORT             = 0x01,
    MODBUS_ERR_CODE_INVALUED_REG_ADDR                = 0x02,
    MODBUS_ERR_CODE_REG_NUM_ERR                      = 0x03,
    MODBUS_ERR_CODE_CMD_PROCESS_ERR                  = 0x04,

    MODBUS_INTERNAL_ERR_CODE_MSG_CHECK_ERR  = 0x81,

    MODBUS_ERR_TYPE_BUTT};

enum{
    MODBUS_MSG_CNT_TYPE_TOTAL_RECV,
    MODBUS_MSG_CNT_TYPE_RECV_READ,
    MODBUS_MSG_CNT_TYPE_RECV_SINGLE_WRITE,
    MODBUS_MSG_CNT_TYPE_RECV_MULTI_WRITE,
    MODBUS_MSG_CNT_TYPE_SEND_ERR_MSG,

    MODBUS_MSG_CNT_TYPE_NUM
};

/*用于响应数据发送，目前配置到每一个*/
struct ModbusSendData_t{
    uint8_t *buf;/*地址需要申请，避免地址浪费*/
    uint8_t refId;/*可能出现接受与发送口不一样的情况*/
    uint16_t maxLength;/*配置的需求的缓冲区的长度，可以做成固定的，也可以配置*/
    uint16_t len;/*用于存储当前需要发送数据的长度*/
};
/*modbus的对象结构体*/
struct ModbusObjInf_t
{
    struct ModbusInitInf_t init;/*初始化接口，对应的硬件接口以及数据库的操作接口*/
    struct ModbusSendData_t sendObj; /*发送数组放到具体的对象中，需要注意控制发送数组长度，避免内存越界*/
    uint32_t msgCnt[MODBUS_MSG_CNT_TYPE_NUM];/*用于统计接受与发送的数据*/
};

struct ModbusObjInf_t *g_pModbusObj[MAX_MODBUS_OBJ_NUM] = {0};

/*modbus命令处理函数*/
typedef int32_t (*MODBUS_MSG_PROCESS_FUNC)(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);

/*根据功能码对应调用的处理函数*/
struct ModbusCmdProcess_t
{
    uint8_t  modbusWorkMode;
    uint8_t  cmdCode;
    uint16_t reserve;
    MODBUS_MSG_PROCESS_FUNC pfModbusMsgProcess;
};

#pragma pack(1)
/*通用协议头*/
struct ModbusMsgHeader_t
{
    uint8_t  addr;
    uint8_t  cmdCode;
};
/*通用读取寄存器命令*/
struct ReadRegMsg_t{
    struct ModbusMsgHeader_t header;
    uint16_t startAddr;
    uint16_t regNum;      
    uint16_t crc;
};
/*通用读取寄存器响应命令*/
struct ReadRegRespMsg_t{
    struct ModbusMsgHeader_t header;
    uint8_t byteNum;
    uint16_t regValueArray[MODBUS_MAX_REG_NUM_PER_MSG];
    uint16_t crc;
};
/*通用单个写寄存器命令*/
struct WriteSingleRegMsg_t{
    struct ModbusMsgHeader_t header;
    uint16_t regAddr;
    uint16_t regValue;
    uint16_t crc;
};
/*通用写多个寄存器命令*/
struct WriteMultiRegMsg_t{
    struct ModbusMsgHeader_t header;
    uint16_t startAddr;
    uint16_t regNum;
    uint8_t  byteNum;
    uint16_t regValueArray[MODBUS_MAX_REG_NUM_PER_MSG];
    uint16_t crc;
};
/*通用写多个寄存器响应命令*/
struct WriteMultiRegMsgResp_t{
    struct ModbusMsgHeader_t header;
    uint16_t startAddr;
    uint16_t regNum;
    uint8_t  byteNum;
    uint16_t crc;
};
/*错误响应报文*/
struct ErrRespMsg_t{
    struct ModbusMsgHeader_t header;
    uint8_t errCode;
    uint16_t crc;
};
/*tcp协议头*/
struct TcpMsgHead_t{
    uint16_t eventFlag;     
    uint16_t protocolFlag;
    uint16_t byteNum;	
};
#pragma pack()

#define MODBUS_MSG_MIN_SIZE   sizeof(struct ErrRespMsg_t)
#define MODBUS_MSG_MAX_SIZE   sizeof(struct WriteMultiRegMsgResp_t)

static int32_t Modbus_ReadOutCtrlMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);
static int32_t Modbus_ReadInputMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);
static int32_t Modbus_ReadHoldMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);
static int32_t Modbus_ReadAnalogMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);
static int32_t Modbus_WriteSingleOutCtrlMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);
static int32_t Modbus_WriteSingleHoldMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);
static int32_t Modbus_WriteMultiOutCtrlMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);
static int32_t Modbus_WriteMultiHoldMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len);

static const struct ModbusCmdProcess_t g_ModbusMsgProcessArray[] =
{
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_READ_OUTCTRL_REG,           0, Modbus_ReadOutCtrlMsgProcess},
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_READ_INPUT_REG,             0, Modbus_ReadInputMsgProcess},
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_READ_HOLD_REG,              0, Modbus_ReadHoldMsgProcess},
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_READ_ANALOG_REG,            0, Modbus_ReadAnalogMsgProcess},
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_WRITE_SINGLE_OUTCTRL_REG,   0, Modbus_WriteSingleOutCtrlMsgProcess},
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_WRITE_SINGLE_HOLD_REG,      0, Modbus_WriteSingleHoldMsgProcess},
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_WRITE_MULTI_OUTCTRL_REG,    0, Modbus_WriteMultiOutCtrlMsgProcess},
    {MODBUS_OBJ_WORK_MODE_SLAVE,   MODBUS_FUNCTION_CODE_WRITE_MULTI_HOLD_REG,       0, Modbus_WriteMultiHoldMsgProcess},
};

#define Modbus_CRC16 CRC16_Clac

static int32_t SendMsg(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
	int32_t ret = 0;
    SysInf_EnterCritical();/*避免多个设备同时访问modbus对象*/
	pModbusObj->sendObj.len = 0;
    if(pModbusObj->init.type == MODBUS_TCP)
    {
        if(pModbusObj->init.crcType == MODBUS_NO_CRC)
        {
            len = len - 2;
        }        
        if(pModbusObj->init.headType == MODBUS_HAS_HEAD)
        {
            struct TcpMsgHead_t header;
            header.byteNum = MODBUS_NET_TO_HOST_16(len);
            memcpy(pModbusObj->sendObj.buf,(uint8_t *)&header, sizeof(struct TcpMsgHead_t));
			pModbusObj->sendObj.len = sizeof(struct TcpMsgHead_t);
		}				
    }
    /*避免越界*/
    if(pModbusObj->sendObj.len+len > MAX_MODBUS_MSG_LENGTH)
    {
        return -1;
    }
	memcpy(pModbusObj->sendObj.buf + pModbusObj->sendObj.len,pMsg, len);
	pModbusObj->sendObj.len += len;
    SysInf_ExitCritical();
    if(pModbusObj->init.type == MODBUS_RTU)
    {
        UartInfSendMsg(pModbusObj->init.refId, pModbusObj->sendObj.buf, pModbusObj->sendObj.len);
    } else {
        /*增加tcp发送的接口*/
    }
	return ret;
}

static int32_t GetEmptyObjId(void)
{
    int32_t i=0;
    for (i=0;i<MAX_MODBUS_OBJ_NUM;i++)
    {
        if(g_pModbusObj[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

int32_t ModbusSlave_InterfaceInit(struct ModbusInitInf_t * param)
{
    int32_t i = GetEmptyObjId();
    if(i == -1){
        return -1;
    }
    g_pModbusObj[i] = (struct ModbusObjInf_t *)SysInf_MemAlloc(sizeof(struct ModbusObjInf_t));
    if(g_pModbusObj[i] == NULL){
        return -1;
    }
    memset(g_pModbusObj[i], 0, sizeof(struct ModbusObjInf_t));
    g_pModbusObj[i]->init.type              = param->type;
    g_pModbusObj[i]->init.devAddr           = param->devAddr;
    g_pModbusObj[i]->init.headType          = param->headType;
    g_pModbusObj[i]->init.crcType           = param->crcType;
    g_pModbusObj[i]->init.pfReadCallback    = param->pfReadCallback;
    g_pModbusObj[i]->init.pfWriteCallback   = param->pfWriteCallback;
    g_pModbusObj[i]->sendObj.buf = (uint8_t *)SysInf_MemAlloc(MAX_MODBUS_MSG_LENGTH);
    return i;
}

int32_t ModbusSlave_InterfaceDeInit(void)
{
    //处理方式不对，申请的内存暂时不支持释放，支持释放时需要释放内存，不能直接将指针清零
    // int32_t i;

    // for(i = 0;i < MAX_MODBUS_OBJ_NUM; ++i)
    // {
    //     g_pModbusObj[i] = NULL;
    // }
    return 0;
}


static struct ModbusObjInf_t * Modbus_GetModbusObjByModBusId(uint32_t modBusId)
{
    if((modBusId >= MAX_MODBUS_OBJ_NUM) || (g_pModbusObj[modBusId] == NULL))
    {
        return NULL;
    }

    return g_pModbusObj[modBusId];
}
/*暂时应该不需要用到，slave模式不需要主动发送数据*/
static int32_t Modbus_SendErrMsg(uint8_t modbusId, uint8_t cmdCode, uint8_t errType)
{
    struct ErrRespMsg_t errMsg;
	struct ModbusObjInf_t *pModbusObj = Modbus_GetModbusObjByModBusId(modbusId);
    if((NULL == pModbusObj))
    {
        //BMS_LOG_ERROR(LOG_SWITCH_FLAG_MODBUS, "Proc GetModbusObj err\r\n");
        return -1;
    }
    {
        pModbusObj->msgCnt[MODBUS_MSG_CNT_TYPE_SEND_ERR_MSG]++;
        errMsg.header.addr = pModbusObj->init.devAddr;
        errMsg.header.cmdCode = cmdCode;
        errMsg.errCode = errType;
        errMsg.crc = Modbus_CRC16((uint8_t *)&errMsg, sizeof(errMsg) - sizeof(uint16_t));
        return SendMsg(pModbusObj, (uint8_t *)&errMsg, sizeof(errMsg));
    }
}

static int32_t Modbus_SendErrRespMsg(struct ModbusObjInf_t * pModbusObj, uint8_t modbusAddr, uint8_t cmdCode, uint8_t errType)
{
    struct ErrRespMsg_t errMsg;

    pModbusObj->msgCnt[MODBUS_MSG_CNT_TYPE_SEND_ERR_MSG]++;

    errMsg.header.addr = modbusAddr;
    errMsg.header.cmdCode = cmdCode | MODBUS_FUNCTION_ERR_CODE_MASK;
    errMsg.errCode = errType;
    errMsg.crc = Modbus_CRC16((uint8_t *)&errMsg, sizeof(errMsg) - sizeof(uint16_t));


    return SendMsg(pModbusObj, (uint8_t *)&errMsg, sizeof(errMsg));
}

static int32_t ReadMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    struct ReadRegMsg_t *pRecv = (struct ReadRegMsg_t *)pMsg;
    struct ReadRegRespMsg_t resp;
    uint16_t regNum = MODBUS_NET_TO_HOST_16(pRecv->regNum);
    uint16_t msgLen, sLen;
    uint16_t crc = 0;
    int32_t ret = -1;

    memset((void *)&resp, 0, sizeof(resp));

    if(len != sizeof(struct ReadRegMsg_t))
    {
        return Modbus_SendErrRespMsg(pModbusObj, pModbusObj->init.devAddr, pRecv->header.cmdCode, MODBUS_ERR_CODE_CMD_PROCESS_ERR);
    }
    if(MODBUS_MAX_REG_NUM_PER_MSG < regNum)
    {
        return Modbus_SendErrRespMsg(pModbusObj, pModbusObj->init.devAddr, pRecv->header.cmdCode, MODBUS_ERR_CODE_REG_NUM_ERR);
    }
    if(pModbusObj->init.pfReadCallback != NULL)
    {
        sLen = regNum * 2;
        ret = pModbusObj->init.pfReadCallback(MODBUS_NET_TO_HOST_16(pRecv->startAddr), (uint8_t *)(resp.regValueArray), &sLen);
    }
    if(ret == 0)
    {
        resp.header.addr = pRecv->header.addr;
        resp.header.cmdCode = pRecv->header.cmdCode;
        resp.byteNum = sLen;
        msgLen = FIELD_OFFSET(struct ReadRegRespMsg_t, regValueArray)+sLen;
        crc = MODBUS_CRC_ENDIAN_CONVERT(Modbus_CRC16((uint8_t *)&resp, msgLen));
        memcpy((uint8_t *)(resp.regValueArray)+resp.byteNum, (uint8_t*)&crc, sizeof(crc));
        ret = SendMsg(pModbusObj, (uint8_t *)&resp, msgLen+2);
    }
    else
    {
        return Modbus_SendErrRespMsg(pModbusObj, pModbusObj->init.devAddr, pRecv->header.cmdCode, MODBUS_ERR_CODE_CMD_PROCESS_ERR);
    }
    return ret;
}

static int32_t SingleWriteMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    struct WriteSingleRegMsg_t *pRecv = (struct WriteSingleRegMsg_t *)pMsg;
    struct WriteSingleRegMsg_t resp;
    int32_t ret = -1;
    // uint16_t len;

    if(pModbusObj == NULL)
    {
        return -1;
    }
    memset((void *)&resp, 0, sizeof(resp));
    if(len != sizeof(struct WriteSingleRegMsg_t))
    {
        return Modbus_SendErrRespMsg(pModbusObj, pModbusObj->init.devAddr, pRecv->header.cmdCode, MODBUS_ERR_CODE_CMD_PROCESS_ERR);
    }
    if(pModbusObj->init.pfWriteCallback != NULL){
        ret = pModbusObj->init.pfWriteCallback(MODBUS_NET_TO_HOST_16(pRecv->regAddr), (uint8_t *)&(pRecv->regValue), NULL);
    }
    if(ret == 0){
        memcpy(&resp, pRecv, sizeof(resp));
        resp.crc = MODBUS_CRC_ENDIAN_CONVERT(Modbus_CRC16((uint8_t*)&resp, sizeof(resp) - sizeof(resp.crc)));
        ret = SendMsg(pModbusObj, (uint8_t *)&resp, sizeof(resp));
    }
    else{
        return Modbus_SendErrRespMsg(pModbusObj, pModbusObj->init.devAddr, pRecv->header.cmdCode, MODBUS_ERR_CODE_CMD_PROCESS_ERR);
    }
    return ret;
}

static int32_t MultiWriteMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    struct WriteMultiRegMsg_t *pRecv = (struct WriteMultiRegMsg_t *)pMsg;
    struct WriteMultiRegMsgResp_t resp;
    // uint16_t crc;
    uint16_t sLen;
    uint16_t regNum = MODBUS_NET_TO_HOST_16(pRecv->regNum);
    int32_t ret = -1;

    if(pModbusObj == NULL){
        return -1;
    }
    if(MODBUS_MAX_REG_NUM_PER_MSG < regNum)
    {
        return Modbus_SendErrRespMsg(pModbusObj, pModbusObj->init.devAddr, pRecv->header.cmdCode, MODBUS_ERR_CODE_REG_NUM_ERR);
    }
    if(pModbusObj->init.pfWriteCallback != NULL){
        sLen = regNum << 1;
        ret = pModbusObj->init.pfWriteCallback(MODBUS_NET_TO_HOST_16(pRecv->startAddr), (uint8_t *)(pRecv->regValueArray), &sLen);
    }
    if(ret == 0) {
        resp.header.addr = pRecv->header.addr;
        resp.header.cmdCode = pRecv->header.cmdCode;
        resp.regNum = sLen >> 1;
        resp.startAddr = pRecv->startAddr;
        resp.byteNum = sLen;
        resp.crc = MODBUS_CRC_ENDIAN_CONVERT(Modbus_CRC16((uint8_t*)&resp, sizeof(resp) - sizeof(resp.crc)));
        ret = SendMsg(pModbusObj, (uint8_t*)&resp, sizeof(resp));
    }
    return ret;
}

static int32_t Modbus_ReadOutCtrlMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return ReadMsgProcess(pModbusObj, pMsg, len);
}
static int32_t Modbus_ReadInputMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return ReadMsgProcess(pModbusObj, pMsg, len);
}
static int32_t Modbus_ReadHoldMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return ReadMsgProcess(pModbusObj, pMsg, len);
}
static int32_t Modbus_ReadAnalogMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return ReadMsgProcess(pModbusObj, pMsg, len);
}
static int32_t Modbus_WriteSingleOutCtrlMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return SingleWriteMsgProcess(pModbusObj, pMsg, len);
}
static int32_t Modbus_WriteSingleHoldMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return SingleWriteMsgProcess(pModbusObj, pMsg, len);
}
static int32_t Modbus_WriteMultiOutCtrlMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return MultiWriteMsgProcess(pModbusObj, pMsg, len);
}
static int32_t Modbus_WriteMultiHoldMsgProcess(struct ModbusObjInf_t * pModbusObj, uint8_t *pMsg, uint32_t len)
{
    return MultiWriteMsgProcess(pModbusObj, pMsg, len);
}


uint32_t Modbus_MsgLenCheck(uint8_t *pMsg, uint32_t len)
{		
	struct ModbusMsgHeader_t *pHeader;
	pHeader = (struct ModbusMsgHeader_t *)pMsg;

	if(len < MODBUS_MSG_MIN_SIZE)
	{
		return MODBUS_INTERNAL_ERR_CODE_MSG_CHECK_ERR;
	}
	switch(pHeader->cmdCode)
	{
        case MODBUS_FUNCTION_CODE_READ_OUTCTRL_REG:
            break;
        case MODBUS_FUNCTION_CODE_READ_INPUT_REG:      
            break;  
        case MODBUS_FUNCTION_CODE_READ_HOLD_REG:    
            break;
        case MODBUS_FUNCTION_CODE_READ_ANALOG_REG:   
            break;
        case MODBUS_FUNCTION_CODE_WRITE_SINGLE_OUTCTRL_REG:
        case MODBUS_FUNCTION_CODE_WRITE_SINGLE_HOLD_REG:  
            if(len != sizeof(struct WriteSingleRegMsg_t))
            {
                return -1;
            }
            break;
        case MODBUS_FUNCTION_CODE_WRITE_MULTI_OUTCTRL_REG:
            break;
        case MODBUS_FUNCTION_CODE_WRITE_MULTI_HOLD_REG:   
            break;
		default:
			break;
	}
	return MODBUS_ERR_CODE_PROCESS_OK;
}

uint32_t Modbus_MsgCrcCheck(uint8_t *pMsg, uint32_t len)
{
	uint16_t crc16 = 0;
	memcpy(&crc16, pMsg + len - sizeof(crc16), sizeof(crc16));
	crc16 = MODBUS_CRC_ENDIAN_CONVERT(crc16);
    uint16_t crcCalc = Modbus_CRC16(pMsg, (uint8_t)(len - sizeof(crc16)));

	if(crcCalc == crc16)
	{
		return MODBUS_ERR_CODE_PROCESS_OK;
	}
	else
	{
		return MODBUS_INTERNAL_ERR_CODE_MSG_CHECK_ERR + 3;
	}
}


static uint32_t Modbus_CheckMsg(struct ModbusObjInf_t *pModbusObj, uint8_t *pMsg, uint32_t len)
{
		uint32_t ret = 0;
    
    if(pModbusObj->init.crcType == MODBUS_NO_CRC)
    {
        len += 2;
    }
    ret = Modbus_MsgLenCheck(pMsg,len);
    if(ret != MODBUS_ERR_CODE_PROCESS_OK)
    {
        return ret;
    }    

    if(pModbusObj->init.crcType == MODBUS_NO_CRC)
    {
        return MODBUS_ERR_CODE_PROCESS_OK;
    } else {
        return Modbus_MsgCrcCheck(pMsg, len);
    }
}


static int32_t Modbus_ProcessOneMsg(struct ModbusObjInf_t *pModbusObj, uint8_t *pMsg, uint32_t len)
{
    int32_t ret = 0;
    uint32_t i;
    struct ModbusMsgHeader_t *pMsgHeader = (struct ModbusMsgHeader_t *)pMsg;

    uint8_t tableSize = ARRAY_SIZE(g_ModbusMsgProcessArray);
		
    for(i = 0; i < tableSize; i++)
    {
        if(pMsgHeader->cmdCode  == g_ModbusMsgProcessArray[i].cmdCode)
        {
            ret = 0;
            if(g_ModbusMsgProcessArray[i].pfModbusMsgProcess != NULL)
            {
                ret = g_ModbusMsgProcessArray[i].pfModbusMsgProcess(pModbusObj, pMsg, len);
            }
            if(ret < 0)
            {
                //BMS_LOG_ERROR(LOG_SWITCH_FLAG_MODBUS,
                    //"Modbus Msg Proc err, cmdcode 0x%x\r\n", pMsgHeader->cmdCode);
            }
            return ret;
        }
    }

    Modbus_SendErrRespMsg(pModbusObj, pMsgHeader->addr, pMsgHeader->cmdCode|0x80, MODBUS_ERR_CODE_CMD_CODE_NOT_SUPPORT);

    return -1;
}


int32_t ModbusSlave_MsgProcess(uint8_t modbusId, uint8_t *pMsg, int32_t len)
{
    uint32_t ret = 0;
    struct ModbusObjInf_t *pModbusObj = Modbus_GetModbusObjByModBusId(modbusId);
    struct ModbusMsgHeader_t *pHeader = (struct ModbusMsgHeader_t *)pMsg;
	// g_modbusSendLen = 0;
    if((NULL == pModbusObj) || (NULL == pMsg))
    {
        return -1;
    }
    pModbusObj->sendObj.len = 0;
    ret = ret;
    pModbusObj->msgCnt[MODBUS_MSG_CNT_TYPE_TOTAL_RECV]++;

    if((pModbusObj->init.type == MODBUS_TCP) && (pModbusObj->init.headType == MODBUS_HAS_HEAD))
    {
        struct TcpMsgHead_t *pTcpHeader = (struct TcpMsgHead_t *)pMsg;
        if(pTcpHeader->byteNum != len - 6)
        {
            return -1;
        }
        pMsg += sizeof(struct TcpMsgHead_t);
        len -= sizeof(struct TcpMsgHead_t);
        pHeader = (struct ModbusMsgHeader_t*)pMsg;
    }

    if((pHeader->addr != pModbusObj->init.devAddr) && (pHeader->addr != 0xff))
    {
        return 0;
    }

    if((ret = Modbus_CheckMsg(pModbusObj, pMsg, len)) != MODBUS_ERR_CODE_PROCESS_OK)
    {
        return -1;
    }

    return Modbus_ProcessOneMsg(pModbusObj, pMsg, len);
}

int32_t ModbusSlave_SetDevAddr(uint8_t modbusId, uint8_t devAddr)
{
    struct ModbusObjInf_t *pModbusObj = Modbus_GetModbusObjByModBusId(modbusId);
    if(NULL == pModbusObj)
    {
        return -1;
    }
    pModbusObj->init.devAddr = devAddr;
    return 0;
}

int32_t ModbusSlave_ChangeDevAddr(uint8_t type, uint8_t devAddrOld, uint8_t devAddrNew)
{
    int32_t i = 0;
    for( i = 0; i < MAX_MODBUS_OBJ_NUM; ++i)
    {
        if(g_pModbusObj[i] == NULL)
        {
            return 0;
        }
        if((g_pModbusObj[i]->init.type == type) && (g_pModbusObj[i]->init.devAddr == devAddrOld))
        {
            g_pModbusObj[i]->init.devAddr = devAddrNew;
        }
    }
    return 0;
}

