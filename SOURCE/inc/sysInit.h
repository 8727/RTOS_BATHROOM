#ifndef SYSINIT_H
#define SYSINIT_H

//--------------------------------------------------------------------------------------------------------------------//
#include "stm32f10x.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "setting.h"

#if defined ACTIVE_SWO
  #include "stdio.h"
#endif

//--------------------------------------------------------------------------------------------------------------------//
#define CONFIG_MAGIC_KEY                0x87278727

#define INT_FLASH_KEY1                  ((uint32_t)0x45670123)
#define INT_FLASH_KEY2                  ((uint32_t)0xCDEF89AB)
#define CONFIG_MEMORY_START             ((uint32_t)0x0801F800)  // адрес, с которого будет начинаться запись во флеш (с начала 126-ой страницы F103)
#define CONFIG_MEMORY_SIZE              ((uint16_t)0X0400)      //1024 объем флеша (F103)
#define CONFIG_MEMORY_SECTOR            ((uint16_t)0X0100)      //256 размер сектора (F103)
#define CONFIG_MEMORY_N_SECTOR          (CONFIG_MEMORY_SIZE / CONFIG_MEMORY_SECTOR)
#define CONFIG_MEMORY_BUFF              (CONFIG_MEMORY_SECTOR / CONFIG_MEMORY_N_SECTOR)

#define IDCODE_1                        (*(uint32_t *)0x1FFFF7E8)
#define IDCODE_2                        (*(uint32_t *)0x1FFFF7EC)
#define IDCODE_3                        (*(uint32_t *)0x1FFFF7F0)

//--------------------------------------------------------------------------------------------------------------------//
typedef struct{
  uint32_t magicKEY;
  uint32_t dateBuild;
  uint32_t hwBuild;
  uint32_t swBuild;
  
  uint8_t  deviceType;
  uint8_t  deviceNumber;
  uint8_t  timeZone;
  uint8_t  rtcCalib;
  
  uint32_t canSpeed;
  uint16_t rs485Speed;
  
  uint16_t nRF24L01Addr;
  uint8_t  nRF24L01Prim;
  uint8_t  nRF24L01Speed;
  uint8_t  nRF24L01Power;
  uint8_t  nRF24L01Ch;
  uint8_t  rf24TypeOn;
  uint8_t  rf24TypeSend1;
  uint8_t  rf24TypeAddr1;
  uint8_t  rf24TypeSend2;
  uint8_t  rf24TypeAddr2;
  uint8_t  rf24TypeSend3;
  uint8_t  rf24TypeAddr3;
  uint8_t  rf24TypeSend4;
  uint8_t  rf24TypeAddr4;

}config_t;

struct flashSector{
  uint8_t data8 [CONFIG_MEMORY_SECTOR - 0x10];
  uint8_t  securityStartOn;
  uint8_t  securityTimeOn;
  uint16_t securityStart;
  uint32_t securityTime;
  uint32_t checkSum;
  uint32_t nWrite;
};

union configFlash{
  config_t settings;
  struct flashSector sector;
  uint32_t data32[CONFIG_MEMORY_SECTOR];
};

//--------------------------------------------------------------------------------------------------------------------//
extern union configFlash conf;
extern uint32_t upTime;

//--------------------------------------------------------------------------------------------------------------------//
void DelayMs(uint32_t ms);
_Bool InitCPU(void);
void FlashAllErase(void);
void FlashConfigUpdate(void);
_Bool FlashConfigRead(void);


#endif /* SYSINIT_H */
