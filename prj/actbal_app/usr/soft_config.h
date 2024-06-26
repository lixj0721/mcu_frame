#ifndef __SOFT_CONFIG_H__
#define __SOFT_CONFIG_H__

#include "toolkit.h"

#define RUN_LED_NUM 2
#define RUN_LED_CONFIG_MODE 1/*预留配置运行灯模式*/

#define UART_INF_MSG_FIFO MAX_BUFFER_LENGTH

#define DEBUG_METHOD_UART /*配置打印方式*/
#define DEBUG_PORT_INDEX    UARTCh_UPPER


/*bootloader相关*/
#pragma pack(1)
struct BinInfo_t{
    uint32_t binLen;
    uint32_t binCrc;
};
#pragma pack()

#define BIN_FILE_INFO_TOTAL_LENGTH 64/*预留的程序内容长度为64字节*/

#define MCU_FLASH_START_ADDR 0x08000000
#define MCU_FLASH_START_END  0x08008000
#define MCU_FLASH_START_SIZE  (MCU_FLASH_START_END - MCU_FLASH_START_ADDR)

#define BOOTLOADER_FLASH_BOOT_SIZE 0x00000C00

#define BOOTLOADER_FLASH_APP_START_ADDR (BOOTLOADER_FLASH_BOOT_SIZE+MCU_FLASH_START_ADDR)
#define BOOTLOADER_FLASH_APP_BIN_INFO_ADDR BOOTLOADER_FLASH_APP_START_ADDR
#define BOOTLOADER_FLASH_APP_SIZE     ((MCU_FLASH_START_SIZE - BOOTLOADER_FLASH_BOOT_SIZE) / 2)

#define BOOTLOADER_FLASH_BACK_START_ADDR (BOOTLOADER_FLASH_APP_START_ADDR + BOOTLOADER_FLASH_APP_SIZE)
#define BOOTLOADER_FLASH_BACK_BIN_INFO_ADDR BOOTLOADER_FLASH_BACK_START_ADDR
#define BOOTLOADER_FLASH_BACK_SIZE  BOOTLOADER_FLASH_APP_SIZE
/*end of bootloader*/

#endif //__SOFT_CONFIG_H__


