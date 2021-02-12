#include "setting.h"

configFlash conf;
void Setting(void){

  RtcInit();
  I2cInit();
  Nrf24Init();
//  CanInit();
//  Rs485Init();
  
  #if defined DEBUG
    printf("\n\r\t\tStart setting\n\r\n");
  #endif
  printf("\tHardware : %s\tSoftware : %s\r\n",HW_BUILD ,SW_BUILD);

}
_Bool FlashConfigRead(void){
  
  RtcTypeDef dateBuild;
  dateBuild.year  = BUILD_YEAR;
  dateBuild.month = BUILD_MONTH;
  dateBuild.day   = BUILD_DAY;
  dateBuild.hour  = BUILD_TIME_H;
  dateBuild.min   = BUILD_TIME_M;
  dateBuild.sec   = BUILD_TIME_S;
  return false;
}
