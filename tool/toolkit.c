#include "toolkit.h"

struct RingBuffer
{
    uint8_t *buf;
    uint32_t usize;
    uint32_t size;
    uint32_t in;
    uint32_t out;
};

void SysInf_MemFree(void * ptr)
{

}

static int32_t is_power_of_2(uint32_t x);
static uint32_t roundup_pow_of_two(uint32_t x);

FifoInf_Handel FifoInf_Init(uint32_t size, uint32_t unitSize)
{
    FifoInf_Handel fifo = (FifoInf_Handel)SysInf_MemAlloc(sizeof(struct RingBuffer));

    if(fifo != NULL)
    {
        if(is_power_of_2(size) != 0)
        {
            if(size > 0x80000000UL)
            {
                SysInf_MemFree(fifo);
                return NULL;
            }
            size = roundup_pow_of_two(size);
        }
        fifo->buf = (uint8_t *)SysInf_MemAlloc(size * unitSize);

        if(fifo->buf == NULL)
        {
            SysInf_MemFree(fifo);
            return NULL;
        }
        fifo->usize = unitSize;
        fifo->size = size;
        fifo->in = 0;
        fifo->out = 0;
    }

    return fifo;
}

int32_t  FifoInf_DeInit(FifoInf_Handel fifo)
{
    SysInf_MemFree(fifo->buf);
    SysInf_MemFree(fifo);
    return 0;
}

static uint32_t RingBuffer_Size(FifoInf_Handel fifo)
{
    return fifo->size;
}
 

static uint32_t RingBuffer_Len(FifoInf_Handel fifo)
{
    return fifo->in - fifo->out;
}
 
static uint32_t RingBuffer_Avail(FifoInf_Handel fifo)
{
    return RingBuffer_Size(fifo) - RingBuffer_Len(fifo);
}


int32_t FifoInf_Add(FifoInf_Handel fifo,void *data, uint32_t len)
{
  
	len = MIN(len, RingBuffer_Avail(fifo));

    /* First put the data starting from fifo->in to buffer end. */
    uint32_t l = MIN(len, fifo->size - (fifo->in & (fifo->size - 1)));
    memcpy(fifo->buf + (fifo->in & (fifo->size - 1)) * fifo->usize, data, l * fifo->usize);

    /* Then put the rest (if any) at the beginning of the buffer. */
    memcpy(fifo->buf, (uint8_t *)data + l * fifo->usize, (len - l)*fifo->usize);

    fifo->in += len;

    return len;
}

int32_t FifoInf_AddForce(FifoInf_Handel fifo,void *data, uint32_t len)
{
    int32_t fullFlag = 0;
    len = MIN(len, RingBuffer_Size(fifo));
    if(len > RingBuffer_Avail(fifo))
    {
        fullFlag = 1;
    }

    /* First put the data starting from fifo->in to buffer end. */
    uint32_t l = MIN(len, fifo->size - (fifo->in & (fifo->size - 1)));
    memcpy(fifo->buf + (fifo->in & (fifo->size - 1)) * fifo->usize, data, l * fifo->usize);

    /* Then put the rest (if any) at the beginning of the buffer. */
    memcpy(fifo->buf, (uint8_t *)data + l * fifo->usize, (len - l)*fifo->usize);

    fifo->in += len;
    if(fullFlag == 1)
    {
        fifo->out = fifo->in - RingBuffer_Size(fifo);
    }

    return len;
}

int32_t FifoInf_Del(FifoInf_Handel fifo,void *data, uint32_t len)
{
    len = MIN(len, RingBuffer_Len(fifo));

    /* First get the data from fifo->out until the end of the buffer. */
    uint32_t l = MIN(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(data, fifo->buf + (fifo->out & (fifo->size - 1)) * fifo->usize, l * fifo->usize);

    /* Then get the rest (if any) from the beginning of the buffer. */
    memcpy((uint8_t *)data + l * fifo->usize, fifo->buf, (len - l)*fifo->usize);

    fifo->out += len;

    return len;
}

int32_t FifoInf_Get(FifoInf_Handel fifo,void *data, uint32_t len)
{
    len = MIN(len, RingBuffer_Len(fifo));

    /* First get the data from fifo->out until the end of the buffer. */
    uint32_t l = MIN(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(data, fifo->buf + (fifo->out & (fifo->size - 1)) * fifo->usize, l * fifo->usize);

    /* Then get the rest (if any) from the beginning of the buffer. */
    memcpy((uint8_t *)data + l * fifo->usize, fifo->buf, (len - l)*fifo->usize);
    return len;
}

uint32_t FifoInf_Size(FifoInf_Handel fifo)
{
    return fifo->size; 
}

uint32_t FifoInf_Len(FifoInf_Handel fifo)
{
    return fifo->in - fifo->out;
}


void FifoInf_Reset(FifoInf_Handel fifo)
{
    fifo->in = 0;
    fifo->out = 0;
}


static int32_t is_power_of_2(uint32_t x)
{
    if ((x != 0) && ((x & (x - 1)) == 0))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static uint32_t roundup_pow_of_two(uint32_t x)
{
    uint32_t b = 0;

    for(int i = 0; i < 32; i++)
    {
        b = 1UL << i;

        if(x <= b)
        {
            break;
        }
    }

    return b;
}

#define AllocLen        (16 * 1024)
static uint8_t allocBuf[AllocLen];

uint8_t * freePtr = allocBuf;
uint32_t freeLen = AllocLen;

void * SysInf_MemAlloc(uint32_t size)
{
    uint8_t *retPtr = NULL;
    size = (size + 3) & (~0x3);   
	//UserAssert(size < freeLen);  
	freeLen -= size;
	retPtr = freePtr;
	freePtr += size;
    return retPtr;
}

uint16_t CRC16_Clac(uint8_t *updata,uint32_t len)
{
    uint16_t crc = 0xFFFF;
    uint8_t i, j;

    for ( j=0; j < len; j++)
    {
        crc = (uint16_t)(crc^*updata++);
        for (i = 0; i < 8; i++)
        {
            if((crc & 0x0001) > 0)
            {
                crc = crc >> 1;
                crc = crc ^ 0xa001;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }

    return (crc);
}

void SysInf_EnterCritical()
{

}

void SysInf_ExitCritical()
{

}
