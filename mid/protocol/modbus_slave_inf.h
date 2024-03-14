#ifndef __MODBUS_SLAVE_INF_H__
#define __MODBUS_SLAVE_INF_H__

#include "toolkit.h"

enum{
    MODBUS_RTU,
    MODBUS_TCP,
    MODBUS_TYPE_NUM
};//MODBUS_TYPE;

/*modbus tcp相关的配置*/
/*主要用于MODBUS TCP的配置，是否需要配置MODBUS TCP协议头*/
enum{MODBUS_NO_HEAD = 0, MODBUS_HAS_HEAD =! MODBUS_NO_HEAD  };//MODBUS_HEAD_TYPE;
enum{MODBUS_NO_CRC  = 0, MODBUS_CRC16    =! MODBUS_NO_CRC   };//MODBUS_CRC_TYPE;

typedef int32_t (*ModbusDbWRFunc)(uint16_t startAddr, uint8_t *pData, uint16_t *pLength);

/*用于初始化MODBUS对象*/
struct ModbusInitInf_t
{
    uint8_t type;       /*modbus类型，0->RTU，1->TCP*/
    uint8_t refId;      /*相关id，RTU时对应着串口id，TCP时对应socket*/
    uint8_t devAddr;    /*设备地址，独立地址或者跟随设备地址变化，当地址设置为0xff时即跟随设备地址*/
    uint8_t headType;   /*modbus tcp的配置项*/
    uint8_t crcType;    /*modbus tcp的配置项*/
    ModbusDbWRFunc pfReadCallback;   
    ModbusDbWRFunc pfWriteCallback;  
};

int32_t ModbusSlave_InterfaceInit(struct ModbusInitInf_t * param);
int32_t ModbusSlave_ChangeDevAddr(uint8_t type, uint8_t devAddrOld, uint8_t devAddrNew);
int32_t ModbusSlave_SetDevAddr(uint8_t modbusId, uint8_t devAddr);
int32_t ModbusSlave_MsgProcess(uint8_t modbusId, uint8_t *pMsg, int32_t len);

#endif  //__MODBUS_SLAVE_INF_H__
