#include "boiler.h"

struct boilerInitTypeDef boiler;

//--------------------------------------------------------------------------------------------------------------------//
void BoilerOnOff(void){
  if(boiler.eroor){ // проверка на ошибку
    BOILER_T_OFF; BOILER_R_OFF; // EROOR все выключаем
  }else{
    if(boiler.job){ // если включено увеличиваем, выключено уменьшаем шаг
      boiler.step++; if(boiler.step > 0x03){ boiler.step = 0x03; } //чтоб не переполнился
    }else{
      if(boiler.step != 0x00){ boiler.step--; } //чтоб не переполнился;
    }
    switch(boiler.step){
      case 0x03:  BOILER_T_OFF; BOILER_R_ON;
    break;
      case 0x02:  BOILER_T_ON; BOILER_R_ON;
    break;
      case 0x01:  BOILER_T_ON; BOILER_R_OFF;
    break;
      case 0x00:
      default:    BOILER_T_OFF; BOILER_R_OFF;
    break;
    }
  }
  
  if(oiler.JobTimer){ // Останавливаем таймер, если нет ошибки
    boiler.timer --;
    if(BOILER_ON == boiler.job){ boiler.JobTimer = False; }
  }
  
  if((boiler.timer == 0x00) & boiler.JobTimer){ // EROOR
    boiler.eroor = True;
    boiler.JobTimer = False;
  }
  
  if(boiler.job  != boiler.oldJob){ // Запускаем таймер
    boiler.oldJob = boiler.job;
    boiler.timer = 0x05;            // Количество тиков
    boiler.JobTimer = True;
  }
}

//--------------------------------------------------------------------------------------------------------------------//
_Bool BoilerInit(void){
  GPIOB->CRL = (GPIOB->CRL & (~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0))) | GPIO_CRL_MODE0; // BOILER_T
  GPIOB->CRL = (GPIOB->CRL & (~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1))) | GPIO_CRL_MODE1; // BOILER_R
  
  return false;
}
