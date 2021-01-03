#ifndef BOILER_H
#define BOILER_H

//--------------------------------------------------------------------------------------------------------------------//
#include "stm32f10x.h"
#include "sysInit.h"

// Define ------------------------------------------------------------------------------------------------------------//
#define BOILER_T_OFF                                  GPIOB->BSRR = GPIO_BSRR_BR0  // BOILER_T
#define BOILER_T_ON                                   GPIOB->BSRR = GPIO_BSRR_BS0  // BOILER_T
#define BOILER_R_OFF                                  GPIOB->BSRR = GPIO_BSRR_BR1  // BOILER_R
#define BOILER_R_ON                                   GPIOB->BSRR = GPIO_BSRR_BS1  // BOILER_R
#define BOILER_ON                                     GPIOB->IDR & GPIO_IDR_IDR4   // BOILER_On

//--------------------------------------------------------------------------------------------------------------------//
struct boilerInitTypeDef{
  _Bool   eroor;
  _Bool   job;
  _Bool   oldJob;
  _Bool   JobTimer;
  uint8_t step;                 // шаг включения выключения
  uint8_t timer;                // таймер ошибки
};

extern struct boilerInitTypeDef boiler;
//--------------------------------------------------------------------------------------------------------------------//
_Bool BoilerInit(void);

#endif /* BOILER_H */
