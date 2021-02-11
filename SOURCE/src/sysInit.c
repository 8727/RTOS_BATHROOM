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
_Bool Sysinit(void){
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
  
  GPIOB->CRL &= ~GPIO_CRL_CNF5; //LED
  GPIOB->CRL |= GPIO_CRL_MODE5;
  
  return status;
}


//--------------------------------------------------------------------------------------------------------------------//
_Bool FlashConfigRead(void){
  
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
  
  
  
  return false;
}
