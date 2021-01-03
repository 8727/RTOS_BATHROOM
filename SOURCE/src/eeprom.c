#include "eeprom.h"

struct eepromInitTypeDef eeprom;

uint8_t eepromWriteRead(uint8_t byte){
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = byte;
  while(!(SPI1->SR & SPI_SR_RXNE));
  return SPI1->DR;
}

void eepromWriteWaitEnd(void){
  uint8_t status = 0x00;
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_R_STATUS_1);
  do{
    status = eepromWriteRead(0x00);
    #if defined DEBUG_EEPROM
      printf("EEPROM Wait...\r\n");
    #endif
  }while((status & 0x01) == 0x01);
  EEPROM_CS_HIGHT;
}

void eepromWriteOn(void){
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_WRITE_ON);
  EEPROM_CS_HIGHT;
}

void eepromWriteOff(void){
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_WRITE_OFF);
  EEPROM_CS_HIGHT;
} 

void eepromErase(void){
  eepromWriteOn();
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_ERASE_CHIP);
  EEPROM_CS_HIGHT;
  eepromWriteWaitEnd();
  eepromWriteOff();
}

void eepromEraseBlocks(void){
//  GuiEraseW25qxx();
  for(uint8_t i = 0x00; i < eeprom.blocks; i++){
    eepromWriteOn();
    EEPROM_CS_LOW;
    eepromWriteRead(CMD_ERASE_64K);
    eepromWriteRead(i);
    eepromWriteRead(0x00);
    eepromWriteRead(0x00);
    EEPROM_CS_HIGHT;
//    GuiEraseBlocks(i);
    eepromWriteWaitEnd();
    eepromWriteOff();
  }
}

void eepromReadPage(uint16_t page, uint8_t *buff){
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_FAST_READ);
  eepromWriteRead((page & 0xFFFF) >> 0x08);
  eepromWriteRead(page & 0x00FF);
  eepromWriteRead(0x00);
  eepromWriteRead(0x00);
  for(uint16_t i = 0x00; i < 0x0100; i++)
    buff[i] = eepromWriteRead(0x00);
  EEPROM_CS_HIGHT;
}

void eepromWritePage(uint16_t page, uint8_t *buff){
  eepromWriteOn();
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_WRITE);
  eepromWriteRead((page & 0xFFFF) >> 0x08);
  eepromWriteRead(page & 0x00FF);
  eepromWriteRead(0x00);
  for(uint16_t i = 0x00; i < 0x0100; i++)
    eepromWriteRead(buff[i]);
  EEPROM_CS_HIGHT;
  eepromWriteWaitEnd();
  eepromWriteOff();
}

void eepromInit(void){
  GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
  GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;
  GPIOA->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7;
  
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  SPI1->CR1 &= ~SPI_CR1_BR;
  SPI1->CR1 &= ~SPI_CR1_CPOL;
  SPI1->CR1 &= ~SPI_CR1_CPHA;
  SPI1->CR1 &= ~SPI_CR1_DFF;
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI1->CR1 |= SPI_CR1_MSTR;
  SPI1->CR1 |= SPI_CR1_SPE;     // SPI Flash
  
  eeprom.id = 0x00;
  
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_ON_RESET);
  EEPROM_CS_HIGHT;
 
  EEPROM_CS_LOW;
  eepromWriteRead(CMD_RESET);
  EEPROM_CS_HIGHT;

  EEPROM_CS_LOW;
  eepromWriteRead(CMD_JEDEC_ID);
  eepromWriteRead(0x00);
  eepromWriteRead(0x00);
  eeprom.id = eepromWriteRead(0x00);
  EEPROM_CS_HIGHT;
  
  switch (eeprom.id){
    case 0x16:  eeprom.blocks = 0x40;
                eeprom.name = "W25Q32BV";
    break;
    case 0x17:  eeprom.blocks = 0x80;
                eeprom.name = "W25Q64FV";
    break;
    case 0x18:  eeprom.blocks = 0xFF;
                eeprom.name = "W25Q128FV";
    break;
    default:    eeprom.blocks = 0x00;
                eeprom.name = "XXXXXXXXX";
    break;
  }
  
  #if defined DEBUG_EEPROM
    printf("< OK >    Initialization EEPROM\r\n");
  #endif
  #if defined DEBUG_EEPROM
    printf("< OK >    EEPROM : %s\r\n", eeprom.name);
  #endif
  eepromReadTable();
}
