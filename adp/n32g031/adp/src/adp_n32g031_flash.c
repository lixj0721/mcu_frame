#include "n32g031_conf.h"

#define PAGE_LENGTH_IN_BYTE 512

int32_t AdpFlashErase(uint32_t addr, uint32_t lenInByte)
{
    /*需要做地址的边界确认*/
    uint8_t retry = 0;
    addr = (addr / PAGE_LENGTH_IN_BYTE) * PAGE_LENGTH_IN_BYTE;/*起始地址对齐*/
    FLASH_Unlock();
    for(uint32_t i=0;i<lenInByte;i+=PAGE_LENGTH_IN_BYTE)
    {
        while(FLASH_EraseOnePage(addr + i)!=FLASH_COMPL)
        {
            if(retry++ >= 10)return -1;/*多次尝试失败返回错误值*/
        }
    }
    FLASH_Lock();

    return 0;
}

int32_t AdpFlashWrite(uint32_t addr, uint32_t *buf, uint32_t lenInWord)
{
    for(int i=0;i<lenInWord;i++)
    {
        FLASH_ProgramWord(addr+i*4, buf[i]);
    }
    return 0;
}

int32_t AdpFlashRead(uint32_t addr, uint32_t *buf, uint32_t len)
{
    return 0;
}
