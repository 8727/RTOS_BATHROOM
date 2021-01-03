#include "main.h"

int main(void){
  Setting();
  
  
  //xTaskCreate(osTimer10Hz,"Timer10Hz", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
  
  
  vTaskStartScheduler();
  while(true){
    DelayMs(0x01);
  }
}
