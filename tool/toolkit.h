#ifndef __TOOLKIT_H__
#define __TOOLKIT_H__

#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#define MAX_BUFFER_LENGTH 384

#define ADD_2_MAX(a,max)	do{if((a) < (max))a++;}while(0)
#define MINUS_2_MIN(a,min)	do{if((a) > (min))a--;}while(0)
	
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ENDIAN_TYPE_CONVERT_16(num)  ((((uint16_t)(num) & 0xff00U) >> 8) | \
                                      (((uint16_t)(num) & 0x00ffU) << 8))
#define ENDIAN_TYPE_CONVERT_32(num)  ((((uint32_t)(num) & 0xff000000U) >> 24) | \
                                      (((uint32_t)(num) & 0x00ff0000U) >> 8) | \
                                      (((uint32_t)(num) & 0x0000ff00U) << 8) | \
                                      (((uint32_t)(num) & 0x000000ffU) << 24))

enum{ENDIAN_TYPE_LITTLE=0,ENDIAN_TYPE_BIG};

#ifndef HOST_ENDIAN_TYPE
#define HOST_ENDIAN_TYPE ENDIAN_TYPE_LITTLE
#endif

#define ENDIAN_TYPE_SWITCH_16(type,num) ((type==HOST_ENDIAN_TYPE)?num:((((uint16_t)(num) & 0xff00U) >> 8) | \
                                      (((uint16_t)(num) & 0x00ffU) << 8)))
#define ENDIAN_TYPE_SWITCH_32(type,num) ((type==HOST_ENDIAN_TYPE)?num:((((uint32_t)(num) & 0xff000000U) >> 24) | \
                                      (((uint32_t)(num) & 0x00ff0000U) >> 8) | \
                                      (((uint32_t)(num) & 0x0000ff00U) << 8) | \
                                      (((uint32_t)(num) & 0x000000ffU) << 24)))

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef TRUE
enum{FALSE=0, TRUE=!FALSE};
#endif

#define ARRAY_SIZE(A)	(sizeof(A)/sizeof(A[0]))
#define Ls(i)	(1 << (i))
#define Rs(i)	(1 >> (i))

typedef struct RingBuffer * FifoInf_Handel;

#define USER_ASSERT(in) do{if(!in)while(1);}while(0)

FifoInf_Handel FifoInf_Init(uint32_t size, uint32_t unitSize);
int32_t  FifoInf_DeInit(FifoInf_Handel fifo);
int32_t FifoInf_Add(FifoInf_Handel fifo,void *data, uint32_t len);
int32_t FifoInf_AddForce(FifoInf_Handel fifo,void *data, uint32_t len);
int32_t FifoInf_Del(FifoInf_Handel fifo,void *data, uint32_t len);
int32_t FifoInf_Get(FifoInf_Handel fifo,void *data, uint32_t len);
uint32_t FifoInf_Size(FifoInf_Handel fifo);
uint32_t FifoInf_Len(FifoInf_Handel fifo);
void FifoInf_Reset(FifoInf_Handel fifo);
void * SysInf_MemAlloc(uint32_t size);

uint16_t Modbus_CRC16(uint8_t *updata,uint32_t len);

int32_t small_vsprintf(char *buf, const char *format, va_list args);
int32_t small_vsnprintf(char *buf,  uint32_t size,const char *fmt, va_list args);

#define my_vsprintf small_vsprintf
#define my_vsnprintf small_vsnprintf

#endif  //__TOOLKIT_H__

