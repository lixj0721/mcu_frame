#include "adp_n32g45x_iic.h"
#include "bsp_cfg.h"
#include "n32g45x.h"
#include "n32g45x_gpio.h"

struct IicCfgStruct
{
	GPIO_Module *sclport;
	uint16_t sclpin;
	GPIO_Module *sdaport;
	uint16_t sdapin;
};

#define DefIicCfg(a)	IIC_##a##_Cfg,

const struct IicCfgStruct g_iicCfg[] = 
{
	IIC_List(DefIicCfg)
};

int32_t AdpIicInit(void)
{
    GPIO_InitType cfg;
    cfg.GPIO_Mode = GPIO_Mode_Out_OD;
    cfg.GPIO_Speed = GPIO_Speed_50MHz;

    for(int i=0;i<IICCh_Num;i++)
    {
        cfg.Pin = g_iicCfg[i].sclpin;
        GPIO_InitPeripheral(g_iicCfg[i].sclport, &cfg);
        cfg.Pin = g_iicCfg[i].sdapin;
        GPIO_InitPeripheral(g_iicCfg[i].sdaport, &cfg);
    }

    return 0;
}

static void iic_delay()
{
    volatile int i=20;    while(i--);
}

#define SCL_SET(id, n) g_iicCfg[id].sclport->PBSC |= (g_iicCfg[id].sclpin<<(16*(1-(n?1:0))))
#define SDA_SET(id, n) g_iicCfg[id].sdaport->PBSC |= (g_iicCfg[id].sdapin<<(16*(1-(n?1:0))))
#define SDA_GET(id)    ((g_iicCfg[id].sdaport->PID & g_iicCfg[id].sdapin)?1:0)

static void iic_start(uint8_t id)
{
    iic_delay();
    SDA_SET(id, 1);
    SCL_SET(id, 1);
    iic_delay();
    SDA_SET(id, 0);
    iic_delay();
}

static void iic_stop(uint8_t id)
{
    SDA_SET(id, 0);
    iic_delay();
    SCL_SET(id, 1);
    iic_delay();
    SDA_SET(id, 1);
}

uint8_t iic_read_ack(uint8_t id)
{
    uint8_t r;

    SCL_SET(id, 0);
    SDA_SET(id, 1);
    iic_delay();
    r = SDA_GET(id);
    SCL_SET(id, 1);
    iic_delay();
    SCL_SET(id, 0);

    return r;
}

void iic_send_ack(uint8_t id, uint8_t last)
{
    SCL_SET(id, 0);
    SDA_SET(id, last);
    iic_delay();
    SCL_SET(id, 1);
    iic_delay();
    SCL_SET(id, 0);

    SDA_SET(id, 1);
}

uint8_t iic_write_byte(uint8_t id, uint8_t b)
{
    for(int i=7;i>=0;i--)
    {
        SCL_SET(id, 0);
        iic_delay();
        SDA_SET(id, (b&(1<<i)));
        SCL_SET(id, 1);
        iic_delay();
    }
    return iic_read_ack(id);
}

uint8_t iic_read_byte(uint8_t id, uint8_t last)
{
    uint8_t r=0;
    for(int i=7;i>=0;i--)
    {
        SCL_SET(id, 0);
        iic_delay();
        r = (r<<1) | SDA_GET(id);
        SCL_SET(id, 1);
        iic_delay();
    }
    iic_send_ack(id, last);
    return r;
}

/*
 * 函数名：I2C_ReadBytes
 * 描述  ：
 * 输入  ：-pBuffer 存放读取到的数据的缓冲区指针。
 *         -ReadAddr 读取数据的起始地址。
 *         -NumByteToRead 要读取的字节数。
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
int32_t AdpIicReadBytes(uint32_t i2cObjId,uint8_t devAddr, uint16_t ReadAddr, uint8_t AddrNum, uint8_t* pBuffer, uint16_t NumByteToRead)
{
	int i;
	iic_start(i2cObjId);                        //起始条件，开始数据通信

	//发送地址和数据读写方向
	if(iic_write_byte(i2cObjId,devAddr | 0)!=0)
	{
		iic_stop(i2cObjId);
		return -1;
	}
	//发送读写地址
	if(AddrNum == 2)
	{
		iic_write_byte(i2cObjId,ReadAddr>>8);
		iic_write_byte(i2cObjId,ReadAddr&0xff);
	}
	else
		iic_write_byte(i2cObjId,ReadAddr);

	iic_start(i2cObjId);                        //起始条件，开始数据通信

	//发送地址和数据读写方向
	iic_write_byte(i2cObjId,devAddr | 1);

	//读入数据
	for (i=0; i<NumByteToRead; i++)
		pBuffer[i] = iic_read_byte(i2cObjId,i==(NumByteToRead-1));

	iic_stop(i2cObjId);                     //终止条件，结束数据通信

	return 0;
}

/*
 * 函数名：I2C_WriteBytes
 * 描述  ：
 * 输入  ：-pBuffer 要写入的数据
 *         -WriteAddr  寄存器起始地址
 *         -NumByteToWrite 要写入 的字节数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
int32_t AdpIicWriteBytes(uint32_t i2cObjId,uint8_t devAddr, uint16_t WriteAddr, uint8_t AddrNum, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
	int i;
    //uint8_t readback[10];

	iic_start(i2cObjId);                        //起始条件，开始数据通信

	//发送地址和数据读写方向
	if(iic_write_byte(i2cObjId,devAddr | 0) != 0)
	{
		iic_stop(i2cObjId);
        return -1;
    }

	//发送读写地址
	if(AddrNum == 2)
	{
		iic_write_byte(i2cObjId,WriteAddr>>8);
		iic_write_byte(i2cObjId,WriteAddr&0xff);
	}
	else
		iic_write_byte(i2cObjId,WriteAddr);

	//写入数据
	for (i=0; i<NumByteToWrite; i++)
		iic_write_byte(i2cObjId,pBuffer[i]);

	iic_stop(i2cObjId);                     //终止条件，结束数据通信


    return 0;
}


