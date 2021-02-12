#include "sysInit.h"

//--------------------------------------------------------------------------------------------------------------------//
static __IO uint32_t msTicks;
uint32_t upTime;


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
  LED_OFF;
  
  return status;
}


//--------------------------------------------------------------------------------------------------------------------//
