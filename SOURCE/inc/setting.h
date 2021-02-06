#ifndef _SETTINGS_H
#define _SETTINGS_H

//--------------------------------------------------------------------------------------------------------------------//
#include "stm32f10x.h"
#include "sysInit.h"
#include "rtc.h"
#include "i2c.h"

#include "nRF24L01.h"

//--------------------------------------------------------------------------------------------------------------------//
#define INFO
//#define DEBUG

//--------------------------------------------------------------------------------------------------------------------//
#define HW_BUILD                        "1.00"
#define SW_BUILD                        "2.00"
#define CAN_SPEED                       0x001C0004  //0x00050004 - 1000 Kb //0x001C0004 - 500 Kb //0x001C0009 - 250 Kb //0x001C0013 - 125 Kb //0x001C0018 - 100 Kb
#define RS485_SPEED                     UART_SPEED_1 // (9600)
#define DEVICE_TYPE                     0x01        // Device type  0x00 = Core, 0x01 = Water, 0x02 = Power/Timer start, 0x03 = SW
#define DEVICE_NUMBER                   0x00        // Device number
#define RTC_CALIBRATION                 0x00        // RTC CalibrationPpm
#define TIME_ZONE                       19          // Moscow Standard Time, Eastern African Time

#define RF24L01_ADDR                    0x8727
#define RF24L01_PRIM                    ((DEVICE_TYPE << 0x004) | DEVICE_NUMBER)
#define RF24L01_SPEED                   0x01        // 0x00  1Mbps, 0x01  2Mbps, 0x10 250kbps
#define RF24L01_POWER                   0x03        // 0x00 -18dBm, 0x01 -12dBm, 0x02 -6dBm, 0x03 0dBm
#define NRF24L01_CH                     0x70        // 0-125 0x00-0x7D



//--------------------------------------------------------------------------------------------------------------------//
#define PRIORITY_RTC                     0x0E
#define PRIORITY_RS485                   0x01
#define PRIORITY_CAN_TX                  0x00
#define PRIORITY_CAN_RX0                 0x00
#define PRIORITY_CAN_RX1                 0x00
#define PRIORITY_CAN_ERROR               0x00
#define PRIORITY_RF24                    0x00

//--------------------------------------------------------------------------------------------------------------------//
#define CanModeNormal                   0x00
#define CanModeLoopBack                 CAN1->BTR |= CAN_BTR_LBKM
#define CanModeSilent                   CAN1->BTR |= CAN_BTR_SILM
#define CanModeSilentLoopBack           CAN1->BTR |= (CAN_BTR_SILM | CAN_BTR_LBKM)

#define SPEED_USART_1 (a)               ((SYSCLK_FREQ_72MHz + (a >> 1)) / a
#define SPEED_USART_2 (a)               (((SYSCLK_FREQ_72MHz / 0x02) + (a >> 1)) / a
#define UART_SPEED_1                    0x208D      //0x0057       //921600
                                                    //0x00A0       //500000
                                                    //0x00AE       //460800
                                                    //0x0140       //250000
                                                    //0x015B       //230400
                                                    //0x0000       //125000
                                                    //0x02B6       0x0271           //115200
                                                    //0x056D       0x04E2           //57600
                                                    //0x0823       0x0753           //38400
                                                    //0x208D       0x1D4C           //9600

#define USART_SPEED_2_1152              0x015B      //0x002B       //921600
#define USART_SPEED_2_96                0x1047      //0x0050       //500000
                                                    //0x0057       //460800
                                                    //0x00A0       //250000
                                                    //0x00AE       //230400
                                                    //0x0000       //125000
                                                    //0x015B       //115200
                                                    //0x02B6       //57600
                                                    //0x0412       //38400
                                                    //0x0823       //19200
                                                    //0x1047       //9600

//--------------------------------------------------------------------------------------------------------------------//
#if ( defined INFO || defined DEBUG)
  #define ACTIVE_SWO
#endif

#if defined ACTIVE_SWO
  #define ITM_Port8(n)                  (*((volatile unsigned char *)(0xE0000000+4*n)))
  #define ITM_Port32(n)                 (*((volatile unsigned long *)(0xE0000000+4*n)))
  #define DEMCR                         (*((volatile unsigned long *)(0xE000EDFC)))
  #define TRCENA                        0x01000000
#endif

#define true                            0x01
#define false                           0x00


//--------------------------------------------------------------------------------------------------------------------//
#define BUILD_YEAR (__DATE__[7] == '?' ? 1900 : (((__DATE__[7] - '0') * 1000 ) + (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + __DATE__[10] - '0'))
#define BUILD_MONTH (__DATE__ [2] == '?' ? 1 : __DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) : __DATE__ [2] == 'b' ? 2 : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
        : __DATE__ [2] == 'y' ? 5 : __DATE__ [2] == 'l' ? 7 : __DATE__ [2] == 'g' ? 8 : __DATE__ [2] == 'p' ? 9 : __DATE__ [2] == 't' ? 10 : __DATE__ [2] == 'v' ? 11 : 12)
#define BUILD_DAY (__DATE__[4] == '?' ? 1 : ((__DATE__[4] == ' ' ? 0 : ((__DATE__[4] - '0') * 10)) + __DATE__[5] - '0'))
#define BUILD_TIME_H (__TIME__[0] == '?' ? 1 : ((__TIME__[0] == ' ' ? 0 : ((__TIME__[0] - '0') * 10)) + __TIME__[1] - '0'))
#define BUILD_TIME_M (__TIME__[3] == '?' ? 1 : ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0'))
#define BUILD_TIME_S (__TIME__[6] == '?' ? 1 : ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0'))

//--------------------------------------------------------------------------------------------------------------------//
void Setting(void);

#endif /* _SETTINGS_H */
