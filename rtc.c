#include "rtc.h"

void hibernate_write_complete(void) {
    // while (HIB_CTL_R & HIB_CTL_WRC) {
    // } // wait for RTC write to complete
       while (!(HIB_CTL_R & HIB_CTL_WRC)) {
        // wait for WRC bit to be set
    }
}

void configure_rtc_wakeup(uint32_t seconds) {
    HIB_CTL_R |= HIB_CTL_RTCWEN; // enable RTC write
    hibernate_write_complete();
    HIB_RTCSS_R = 0; // set RTC sub seconds value to 0
    hibernate_write_complete();
    HIB_RTCLD_R = seconds; // set RTC load value to seconds
    hibernate_write_complete();
    HIB_CTL_R &= ~HIB_CTL_RTCWEN; // disable RTC write
}

void configure_rtc_match(uint32_t matchValue, uint16_t subSecondsMatchValue) {
    HIB_CTL_R |= HIB_CTL_RTCWEN; // enable RTC write
    hibernate_write_complete();
    HIB_RTCSS_R = subSecondsMatchValue << HIB_RTCSS_RTCSSM_S; // set RTC sub seconds match value
    hibernate_write_complete();
    HIB_RTCM0_R = matchValue; // set RTC match value
    hibernate_write_complete();
    HIB_CTL_R &= ~HIB_CTL_RTCWEN; // disable RTC write
}

void enter_hibernate_mode(void) {
    HIB_CTL_R |= HIB_CTL_HIBREQ; // request hibernate mode
    while ((HIB_CTL_R & HIB_CTL_WRC) == 0) {
    } // wait for RTC write to complete
    __asm("    wfi"); // wait for interrupt
}

void request_hibernate_mode(void) {
    HIB_CTL_R |= HIB_CTL_HIBREQ; // request hibernate mode
    while ((HIB_CTL_R & HIB_CTL_WRC) == 0) {
    } // wait for RTC write to complete
}

void HIBERNATE_Handler(void) {
    HIB_IC_R |= HIB_IC_RTCALT0; // clear RTC interrupt flag
   // rtc_interrupt_fired = 1;
}

// initialize real time clock on tm4c123gh6pm
void rtc_init(void){
  //   SYSCTL_RCGCHIB_R |= SYSCTL_RCGCHIB_R0; // enable clock to HIB module
  //  // wait for HIB module to be ready
  //   while ((SYSCTL_PRHIB_R & SYSCTL_PRHIB_R0) == 0) {
  //   }
   
  //   //HIB_CTL_R &= ~HIB_CTL_CLK32EN; // disable 32kHz clock source
  //   HIB_CTL_R |= HIB_CTL_CLK32EN; // enable 32kHz clock source
  //   HIB_CTL_R |= HIB_CTL_OSCDRV; // select 12pF internal load capacitance
  //   HIB_CTL_R &= ~HIB_CTL_RTCEN; // disable RTC
  //   HIB_CTL_R |= HIB_CTL_RTCEN; // enable RTC
  //   HIB_IC_R |= HIB_IC_RTCALT0; // clear RTC interrupt flag
  //   HIB_IM_R |= HIB_IM_RTCALT0; // enable RTC interrupt
  //   NVIC_EN1_R |= 1 << (INT_HIBERNATE-16-32); // enable interrupt 43 in NVIC
  //   HIB_CTL_R |= HIB_CTL_RTCWEN; // enable RTC write
  //   hibernate_write_complete();
  //   HIB_RTCLD_R = 0; // set RTC load value to 0
  //   hibernate_write_complete();
  //   HIB_CTL_R &= ~HIB_CTL_RTCWEN; // disable RTC write
  //   // configure_rtc_wakeup(0);
  //   // configure_rtc_match(0, 5);
  //   // enter_hibernate_mode();
  SYSCTL_RCGCHIB_R |= SYSCTL_RCGCHIB_R0; // enable clock to HIB module
    // wait for HIB module to be ready
    while ((SYSCTL_PRHIB_R & SYSCTL_PRHIB_R0) == 0) {
    }
   
    HIB_CTL_R |= HIB_CTL_CLK32EN; // enable 32kHz clock source
    HIB_CTL_R |= HIB_CTL_OSCDRV; // select 12pF internal load capacitance
    HIB_CTL_R &= ~HIB_CTL_RTCEN; // disable RTC
    HIB_CTL_R |= HIB_CTL_RTCEN; // enable RTC
    HIB_IC_R |= HIB_IC_RTCALT0; // clear RTC interrupt flag
    HIB_IM_R |= HIB_IM_RTCALT0; // enable RTC interrupt
    NVIC_EN1_R |= 1 << (INT_HIBERNATE-16-32); // enable interrupt 43 in NVIC

    HIB_CTL_R |= HIB_CTL_RTCWEN; // enable RTC write
    hibernate_write_complete();
    // HIB_RTCLD_R = 0; // set RTC load value to 0
    hibernate_write_complete();
    HIB_CTL_R &= ~HIB_CTL_RTCWEN; // disable RTC write
    // request_hibernate_mode();
}

void rtc_set(uint32_t time){
    HIB_CTL_R |= HIB_CTL_RTCWEN; // enable RTC write
    hibernate_write_complete();
    HIB_RTCLD_R = time; // set RTC load value to time
    hibernate_write_complete();
    HIB_CTL_R &= ~HIB_CTL_RTCWEN; // disable RTC write
    request_hibernate_mode();
}

uint32_t rtc_get(void){
    return HIB_RTCC_R; // return the current RTC value
}

void rtc_enable(void){
    HIB_CTL_R |= HIB_CTL_RTCEN; // enable RTC
    while (!(HIB_CTL_R & HIB_CTL_WRC)) {
    } // wait for RTC write to complete
}

void rtc_disable(void){
    HIB_CTL_R &= ~HIB_CTL_RTCEN; // disable RTC
}

void rtc_reset(void){
    HIB_CTL_R |= HIB_CTL_RTCWEN; // enable RTC write
    HIB_RTCLD_R = 0; // reset RTC load value to 0
    HIB_CTL_R &= ~HIB_CTL_RTCWEN; // disable RTC write
}

DateTime timestamp_to_date(uint32_t timestamp) {
    DateTime dt;

   dt.second = timestamp % 60;
    timestamp /= 60;
    dt.minute = timestamp % 60;
    timestamp /= 60;
    dt.hour = timestamp % 24;
    timestamp /= 24;

    if (dt.hour < 12) {
        dt.am_pm = 'A';
    } else {
        dt.am_pm = 'P';
        dt.hour -= 12;
    }
    if (dt.hour == 0) {
        dt.hour = 12;
    }

    uint32_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    dt.year = 1970;

    while (1) {
        uint32_t leap = dt.year % 4 == 0 && (dt.year % 100 != 0 || dt.year % 400 == 0);
        uint32_t days_in_year = 365 + leap;
        if (timestamp < days_in_year)
            break;
        dt.year++;
        timestamp -= days_in_year;
    }

    if (dt.year % 4 == 0 && (dt.year % 100 != 0 || dt.year % 400 == 0))
        days_in_month[1] = 29;
    else
        days_in_month[1] = 28;

    dt.month = 0;
    while (1) {
        if (timestamp < days_in_month[dt.month])
            break;
        timestamp -= days_in_month[dt.month];
        dt.month++;
    }

    dt.day = timestamp + 1;
    dt.month++; // Months are 1-based

    return dt;
}

uint32_t date_to_seconds(DateTime dt) {
    uint32_t days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint32_t timestamp = 0;
    uint32_t month, year;

    // Convert hour from 12-hour format to 24-hour format
    if (dt.am_pm == 'P' && dt.hour < 12) {
        dt.hour += 12;
    } else if (dt.am_pm == 'A' && dt.hour == 12) {
        dt.hour = 0;
    }

    // Adjust for leap year
    for (year = 1970; year < dt.year; year++) {
        uint32_t leap = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
        timestamp += 365 + leap;
    }

    // Adjust for leap year in current year
    if (dt.year % 4 == 0 && (dt.year % 100 != 0 || dt.year % 400 == 0))
        days_in_month[2] = 29;

    for (month = 1; month < dt.month; month++) {
        timestamp += days_in_month[month];
    }

    timestamp += dt.day - 1;
    timestamp = timestamp * 24 + dt.hour;
    timestamp = timestamp * 60 + dt.minute;
    timestamp = timestamp * 60 + dt.second;

    return timestamp;
}


// void HIBERNATE_Handler(void) {
//     HIB_IC_R |= HIB_IC_RTCALT0; // clear RTC interrupt flag
//    // rtc_interrupt_fired = 1;
// }
// // Function to read the current time from the RTC
// uint32_t rtc_read(void){
//     return HIB_RTCC_R;
// }

// // Function to set the current time of the RTC
// void rtc_set(uint32_t time){
//     HIB_CTL_R |= HIB_CTL_RTCWEN; // enable RTC write
//     HIB_RTCLD_R = time; // set RTC load value to time
//     HIB_CTL_R &= ~HIB_CTL_RTCWEN; // disable RTC write
// }