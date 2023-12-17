/**
 * File              : rtc.h
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.14.2023
 * Last Modified Date: 12.14.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */

#ifndef RTC_H
#define RTC_H
#define SYSCTL_RCGCHIB_R (*((volatile uint32_t *)0x400FE614))
#define HIB_CTL_R (*((volatile uint32_t *)0x400FC010))
#define HIB_IC_R (*((volatile uint32_t *)0x400FC020))
#define HIB_IM_R (*((volatile uint32_t *)0x400FC014))
#define NVIC_EN1_R (*((volatile uint32_t *)0xE000E104))
#define HIB_RTCLD_R (*((volatile uint32_t *)0x400FC00C))
#define HIB_RTCC_R (*((volatile uint32_t *)0x400FC000))
#define SYSCTL_PRHIB_R (*((volatile uint32_t *)0x400FEA14))
#define NVIC_PRI11_R (*((volatile uint32_t *)0xE000E42C))
#define HIB_RTCSS_R (*((volatile uint32_t *)0x400FC028))
#define HIB_RTCM0_R (*((volatile uint32_t *)0x400FC004))
#define NVIC_PRI5_R (*((volatile uint32_t *)0xE000E414))

#define SYSCTL_RCGCHIB_R0 0x00000001 // Hibernation Module Run Mode
#define HIB_CTL_CLK32EN 0x00000040   // Clocking Enable
#define HIB_CTL_OSCDRV 0x00020000    // Oscillator Drive Capability
#define HIB_CTL_RTCEN 0x00000001     // RTC Timer Enable
#define HIB_IC_RTCALT0 0x00000001    // RTC Alert0 Masked Interrupt
#define HIB_IM_RTCALT0 0x00000001    // RTC Alert 0 Interrupt Mask
#define INT_HIBERNATE 59             // Hibernation Module
#define HIB_CTL_RTCWEN 0x00000008    // RTC Wake-up Enable
#define HIB_CTL_WRC 0x80000000       // Write Complete/Capable
#define SYSCTL_PRHIB_R0 0x00000001   // Hibernation Module Peripheral
#define HIB_CTL_HIBREQ 0x00000002    // Hibernation Request
#define HIB_RTCSS_RTCSSM_S 16
#define HIB_RTCSS_RTCSSM_M 0x7FFF0000 // RTC Sub Seconds Match
#define HIB_MIS_RTCALT0 0x00000001    // RTC Alert 0 Masked Interrupt
#define HIB_MIS_R (*((volatile uint32_t *)0x400FC01C))
// #include "tm4c123gh6pm.h"
#include <stdint.h>
typedef struct
{
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
    char am_pm; // 'A' for AM, 'P' for PM
} DateTime;

void rtc_init(void);
void rtc_set(uint32_t time);
uint32_t rtc_get(void);
void rtc_enable(void);
void rtc_disable(void);
void rtc_reset(void);
uint32_t date_to_seconds(DateTime dt);
void configure_rtc_wakeup(uint32_t seconds);
DateTime timestamp_to_date(uint32_t timestamp);
void configure_rtc_match(uint32_t matchValue, uint16_t subSecondsMatchValue);
void enter_hibernate_mode(void);
void rtc_set_alarm(uint32_t futureTime);
#endif
