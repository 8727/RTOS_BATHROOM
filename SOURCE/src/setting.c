#include "setting.h"

void Setting(void){
  
  InitCPU();
  FlashConfigRead();
  RtcInit();
  I2cInit();
  Nrf24Init();
//  CanInit();
//  Rs485Init();
  
  #if defined DEBUG_SETTING
    printf("\n\r\t\tStart setting\n\r\n");
  #endif
//  printf("\tHardware : %s\tSoftware : %s\r\n",HW_BUILD ,SW_BUILD);
//  printf("Unique ID 0x%08x 0x%08x 0x%08x\r\n",IDCODE_1, IDCODE_2, IDCODE_3);

}
