#include "sysInit.h"

//--------------------------------------------------------------------------------------------------------------------//
static __IO uint32_t msTicks;
uint32_t upTime;
union configFlash conf;

//--------------------------------------------------------------------------------------------------------------------//
#if defined ACTIVE_SWO
  struct __FILE { int handle; };
  FILE __stdout;
  int fputc(int ch, FILE *f){ if(DEMCR & TRCENA){ while(ITM_Port32(0x00) == 0x00);
    ITM_Port8(0x00) = ch; }
    return(ch);
  }
#endif

//--------------------------------------------------------------------------------------------------------------------//
void SysTick_Handler(void){ msTicks++; }

//--------------------------------------------------------------------------------------------------------------------//
void DelayMs(uint32_t ms){ uint32_t tickStart = msTicks;
  while((msTicks - tickStart) < ms){}
}

//--------------------------------------------------------------------------------------------------------------------//
_Bool InitCPU(void){
  uint8_t status = true;
  status = SysTick_Config(SystemCoreClock / 1000);   //1ms
  
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
  
  return status;
}

//--------------------------------------------------------------------------------------------------------------------//
void FlashAllErase(void){
  FLASH->KEYR = INT_FLASH_KEY1;
  FLASH->KEYR = INT_FLASH_KEY2;
  FLASH->CR |= FLASH_CR_MER;
  FLASH->CR |= FLASH_CR_STRT;
  FLASH->CR |= FLASH_CR_LOCK;
}

//--------------------------------------------------------------------------------------------------------------------//
void FlashConfigsErase(void){
  FLASH->KEYR = INT_FLASH_KEY1;
  FLASH->KEYR = INT_FLASH_KEY2;
  while(FLASH->SR & FLASH_SR_BSY);
  if(FLASH->SR & FLASH_SR_EOP) FLASH->SR = FLASH_SR_EOP;
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = CONFIG_MEMORY_START;
  FLASH->CR |= FLASH_CR_STRT;
  while(!(FLASH->SR & FLASH_SR_EOP));
  FLASH->SR = FLASH_SR_EOP;
  FLASH->CR &= ~FLASH_CR_PER;
  FLASH->CR |= FLASH_CR_LOCK;
}

//--------------------------------------------------------------------------------------------------------------------//
void FlashConfigsWrite(uint32_t* buff, uint8_t sector){
  uint32_t adr = sector * CONFIG_MEMORY_SECTOR;
  uint32_t data;
  if(adr >= CONFIG_MEMORY_SIZE) adr = 0x00;
  adr += CONFIG_MEMORY_START;
  FLASH->KEYR = INT_FLASH_KEY1;
  FLASH->KEYR = INT_FLASH_KEY2;
  while(FLASH->SR & FLASH_SR_BSY);
  if(FLASH->SR & FLASH_SR_EOP) FLASH->SR = FLASH_SR_EOP;
  FLASH->CR |= FLASH_CR_PG;
  for(uint16_t i = 0x00; i < CONFIG_MEMORY_BUFF; i++){
    data = buff[i];
    *(__IO uint16_t*)adr = (uint16_t)data;
    while(!(FLASH->SR & FLASH_SR_EOP));
    FLASH->SR = FLASH_SR_EOP;
    adr += 0x02;
    data >>= 0x10;
    *(__IO uint16_t*)adr = (uint16_t)data;
    while(!(FLASH->SR & FLASH_SR_EOP));
    FLASH->SR = FLASH_SR_EOP;
    adr += 0x02;
  }
  FLASH->CR &= ~(FLASH_CR_PG);
  FLASH->CR |= FLASH_CR_LOCK;
}

//--------------------------------------------------------------------------------------------------------------------//
void FlashConfigsRead(uint32_t* buff, uint8_t sector){
  uint32_t adr = sector * CONFIG_MEMORY_SECTOR;
  if(adr >= CONFIG_MEMORY_SIZE) adr = 0x00;
  adr += CONFIG_MEMORY_START;
  for(uint16_t i = 0x00; i < CONFIG_MEMORY_BUFF; i++){
    buff[i] = *(uint32_t*)adr;
    adr += 0x04;
  }
}

//--------------------------------------------------------------------------------------------------------------------//
uint32_t FlashConfigCheckSum(void){
  uint32_t checkSum = 0x00;
  for(uint32_t i = 0x00; i< (CONFIG_MEMORY_SECTOR - 0x10); i++){
    checkSum += conf.sector.data8[i];
  }
  return checkSum;
}

//--------------------------------------------------------------------------------------------------------------------//
void FlashConfigUpdate(void){
  conf.sector.nWrite++;
  uint8_t sector = (conf.sector.nWrite % CONFIG_MEMORY_N_SECTOR);
  conf.sector.checkSum = FlashConfigCheckSum();
  FlashConfigsErase();
  FlashConfigsWrite(conf.data32, sector);
}
//--------------------------------------------------------------------------------------------------------------------//
_Bool FlashConfigRead(void){
  for(uint8_t i = 0x00; i < CONFIG_MEMORY_N_SECTOR; i++){ 
    FlashConfigsRead(conf.data32, i);
    if(!(CONFIG_MAGIC_KEY == conf.settings.magicKEY)){
      if(FlashConfigCheckSum() == conf.sector.checkSum){
        return false;
      }
    }
  }
  memset(conf.data32, 0xFFFFFFFF, sizeof(conf.data32));
  conf.settings.magicKEY = CONFIG_MAGIC_KEY;
  
  RtcTypeDef dateBuild;
  dateBuild.year  = BUILD_YEAR;
  dateBuild.month = BUILD_MONTH;
  dateBuild.day   = BUILD_DAY;
  dateBuild.hour  = BUILD_TIME_H;
  dateBuild.min   = BUILD_TIME_M;
  dateBuild.sec   = BUILD_TIME_S;
  conf.settings.dateBuild = RtcTimeZoneAdjustment(RtcTimeToSeconds(&dateBuild), TIME_ZONE);
  
  conf.settings.hwBuild = (uint32_t)HW_BUILD;
  conf.settings.swBuild = (uint32_t)SW_BUILD;
  
  conf.settings.deviceType = DEVICE_TYPE;
  conf.settings.deviceNumber = DEVICE_NUMBER;
  conf.settings.timeZone = TIME_ZONE;
  conf.settings.rtcCalib = RTC_CALIBRATION;
  
  conf.settings.canSpeed = CAN_SPEED;
  conf.settings.rs485Speed = RS485_SPEED;
  
  conf.settings.nRF24L01Addr = RF24L01_ADDR;
  conf.settings.nRF24L01Prim = RF24L01_PRIM;
  conf.settings.nRF24L01Speed = RF24L01_SPEED;
  conf.settings.nRF24L01Power = RF24L01_POWER;
  conf.settings.nRF24L01Ch = NRF24L01_CH;
  
  
  
  conf.sector.checkSum = FlashConfigCheckSum();
  conf.sector.nWrite = 0x00;
  FlashConfigsWrite(conf.data32, 0x00);
  return false;
}
