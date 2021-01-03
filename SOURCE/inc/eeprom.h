#ifndef _EEPROM_H
#define _EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Define --------------------------------------------------------------------*/
#define EEPROM_CS_LOW      GPIOA->BSRR = GPIO_BSRR_BR4
#define EEPROM_CS_HIGHT    GPIOA->BSRR = GPIO_BSRR_BS4

#define CMD_W_STATUS_1  0x01
#define CMD_W_STATUS_2  0x31
#define CMD_W_STATUS_3  0x11

#define CMD_R_STATUS_1  0x05
#define CMD_R_STATUS_2  0x35
#define CMD_R_STATUS_3  0x15

#define CMD_WRITE       0x02
#define CMD_READ        0x03
#define CMD_FAST_READ   0x0B

#define CMD_WRITE_ON    0x06
#define CMD_WRITE_OFF   0x04

#define CMD_ON_RESET    0x66
#define CMD_RESET       0x99

#define CMD_DEV_ID      0x90
#define CMD_JEDEC_ID    0x9F

#define CMD_ERASE_CHIP  0x60 //0xC7 //0x60
#define CMD_ERASE_4K    0x20
#define CMD_ERASE_64K   0xD8

struct eepromInitTypeDef{
  uint8_t id;
  uint8_t blocks;
  char     *name;
};

extern struct eepromInitTypeDef eeprom;

uint8_t eepromWriteRead(uint8_t byte);
void eepromErase(void);
void eepromEraseBlocks(void);
void eepromReadPage(uint16_t page, uint8_t *buff);
void eepromWritePage(uint16_t page, uint8_t *buff);
void eepromReadTable(void);
void eepromInit(void);
//void eepromDMAInit(void);

void GuiLoadImg(uint16_t x, uint16_t y, uint8_t numb);
#endif /* _EEPROM_H */
