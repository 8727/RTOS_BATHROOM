#ifndef RTC_H
#define RTC_H

//--------------------------------------------------------------------------------------------------------------------//
#include "stm32f10x.h"
#include "sysInit.h"

//--------------------------------------------------------------------------------------------------------------------//
//#define RTC_CCO
//#define RTC_AL
//#define RTC_OW
//#define RTC_DEBUG

#define SEC_A_DAY   86400
//                                   -12      -11    -10    -9:30     -9      -8      -7      -6      -5     -4:30    -4    -3:30     -3      -2      -1       0      +1      +2      +3     +3:30
static const uint16_t ZONE_SEC[] = {0xA8C0, 0x9AB0, 0x8CA0, 0x8598, 0x7E90, 0x7080, 0x6270, 0x5460, 0x4650, 0x3F48, 0x3840, 0x3138, 0x2A30, 0x1C20, 0x0E10, 0x0000, 0x0E10, 0x1C20, 0x2A30, 0x3138,
//                                    +4     +4:30    +5     +5:30   +5:45    +6     +6:30    +7      +8     +8:45    +9     +9:30    +10   +10:30   +11    +11:30    +12   +12:45    +13     +14
                                    0x3840, 0x3F48, 0x4650, 0x4D58, 0x50DC, 0x5460, 0x5B68, 0x6270, 0x7080, 0x7B0C, 0x7E90, 0x8598, 0x8CA0, 0x93A8, 0x9AB0, 0xA1B8, 0xA8C0, 0xB3C4, 0xB6D0, 0xC4E0};

//--------------------------------------------------------------------------------------------------------------------//
typedef struct{
  uint16_t year;    /* 1970..2106 */
  uint8_t  month;   /* 1..12 */
  uint8_t  day;     /* 1..31 */
  uint8_t  wday;    /* 0..6 (Sun..Sat) */
  uint8_t  hour;    /* 0..23 */
  uint8_t  min;     /* 0..59 */
  uint8_t  sec;     /* 0..59 */
}RtcTypeDef;

//--------------------------------------------------------------------------------------------------------------------//
uint32_t RtcTimeZoneAdjustment(uint32_t time, uint8_t time_zone);
void RtcSecondsToTime (uint32_t seconds, RtcTypeDef* unixTime);
uint32_t RtcTimeToSeconds(RtcTypeDef* unixTime);
uint32_t RtcGetSeconds(void);
void RtcSetSeconds(uint32_t seconds);
_Bool RtcInit(void);

#endif /* RTC_H */
