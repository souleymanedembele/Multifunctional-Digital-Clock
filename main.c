/**
 * File              : main.c
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.03.2023
 * Last Modified Date: 12.16.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */

#include "PLL.h"
#include "adc.h"
#include "alarm.h"
#include "dfplayer_lib.h"
#include "gpio.h"
#include "horse_anim.h"
#include "media.h"
#include "rtc.h"
#include "settings.h"
#include "ssd1306.h"
#include "temp.h"
#include "timer.h"
#include "uart.h"
#include <stdint.h>

uint8_t main_nav_select = 2; // settings screen is the default selection
uint8_t enter_pressed = 0;
int temperature = 0;
uint32_t alarm_time = -1;
uint32_t current_time = 0;
uint8_t setup_mode = 0;
uint32_t SW1, SW2;
int32_t addition = 0;
int time_controler = 0;

void timer0A_delayMs(int ttime);
static void run_horse_animation(void);
static void run_menu_nav_bar(uint8_t select);
static void WelcomeScreen(void);
static void display_rtc_time(void);
static void display_temp(void);
static void display_alarm(void);
static void setting_screen(uint32_t current_time);
void OutCRLF(void); // for testing

uint32_t timeElapse =
    0; // global variable to keep track of time elapsed in main loop cycle (1ms)
uint8_t button_pressed = 0;

DateTime dt;

// debug code
int main(void) {
  dt.second = 0;
  dt.minute = 30;
  dt.hour = 8;    // This is in 12-hour format
  dt.am_pm = 'P'; // 'A' for AM, 'P' for PM
  dt.day = 15;
  dt.month = 12;
  dt.year = 2023;
  char i;
  char string[20]; // global to assist in debugging
  uint32_t n;

  PLL_Init(); // set system clock to 50 MHz

  // get rtc time and check if it greater that dt
  if (rtc_get() > date_to_seconds(dt)) {
    setup_mode = 1;
  }
  // RTC is setup in setup mode
  ADC_Init();

  GPIOPortFCE_Init();
  UART_Init(); // initialize UART
               // initialize the display
  ssd1306_init();

  /*Update screen*/
  for (int j = 0; j < 2; j++) {
    run_horse_animation();
  }

  WelcomeScreen();

  // timer0_InIt(); // initialize the timer
  EnableInterrupts();

  dfplayer_init();

  while (1) {
    if (setup_mode == 0) {
      while (setup_mode == 0) {
        setting_screen(current_time);
      }
    }
    ADC0_PSSI_R |= 0x08; /* start a conversion sequence 3 */
    // ADC1_PSSI_R = 0x08;  // Initiate SS3
    ssd1306_clear();
    current_time = rtc_get();

    while ((ADC0_RIS_R & 0x08) == 0) {
      if ((current_time) >= alarm_time) {
        if (alarm_time != 0) {
          alarm_time = 0;
          dfplayer_play_first_track();
          // OutCRLF();
          // UART_OutString("Alarm on");
          // OutCRLF();
        }
      }
      // OutCRLF();
      // display_rtc_time();
      run_menu_nav_bar(main_nav_select);
    }
    /* wait for conversion to complete */
    temperature = ((ADC0_SSFIFO3_R * 190 / 2) /
                   4096); // (ADC0_SSFIFO3_R-500)/10 ; // ((ADC0_SSFIFO3_R *
                          // 190/2)/4096); //; // 190 = range of temp -40 to
    if ((current_time) >= alarm_time) {
      if (alarm_time != 0) {
        alarm_time = 0;
        dfplayer_play_first_track();
        // OutCRLF();
        // UART_OutString("Alarm on");
        // OutCRLF();
      }
    }
    ADC0_ISC_R = 0x08; /* clear completion flag */
    run_menu_nav_bar(main_nav_select);
    //  WaitForInterrupt();
  }
}

static void run_horse_animation(void) {
  /*Clear display*/
  ssd1306_clear();
  /*draw first horse*/
  ssd1306_draw_bmp(0, 0, horse1, 128, 64, 1);
  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse2, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse3, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse4, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse5, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse6, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse7, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse8, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse9, 128, 64, 1);
  ssd1306_update_screen();

  ssd1306_clear();
  ssd1306_draw_bmp(0, 0, horse10, 128, 64, 1);
  ssd1306_update_screen();
}

static void run_menu_nav_bar(uint8_t select) {
  int i = 0;
  // Clear display
  ssd1306_clear();
  if (alarm_time == 0) {
    int num_alarm = sizeof(alarm) / sizeof(alarm[0]);
    for (i = 0; i < num_alarm; i++) {
      ssd1306_clear();
      ssd1306_draw_bmp(32, 0, alarm[i], 64, 64, 1);
      ssd1306_update_screen();
    }
  } else {

    // Run animation for the first selection
    int num_settings = sizeof(settings) / sizeof(settings[0]);
    int num_temps = sizeof(temps) / sizeof(temps[0]);
    int num_media = sizeof(media) / sizeof(media[0]);

    // Depending on the selection, run specific animations
    switch (select) {
    case 1:

      for (i = 0; i < num_media; i++) {
        ssd1306_clear();
        ssd1306_draw_bmp(50, 34, settings[0], 32, 32, 1);
        ssd1306_draw_bmp(98, 32, temps1, 32, 32, 1);
        ssd1306_draw_bmp(0, 32, media[i], 32, 32, 1);
        display_alarm();
        ssd1306_update_screen();
      }
      break;
    case 2:

      // Draw settings animation frames
      for (i = 0; i < num_settings; i++) {
        ssd1306_clear();
        ssd1306_draw_bmp(50, 34, settings[i], 32, 32, 1);
        ssd1306_draw_bmp(98, 32, temps1, 32, 32, 1);
        ssd1306_draw_bmp(0, 32, media1, 32, 32, 1);
        display_rtc_time();
        ssd1306_update_screen();
      }
      // Delay 42ms
      // timer0A_delayMs(42);
      break;
    case 3:

      for (i = 0; i < num_temps; i++) {
        ssd1306_clear();
        ssd1306_draw_bmp(50, 34, settings1, 32, 32, 1);
        ssd1306_draw_bmp(98, 32, temps[i], 32, 32, 1);
        ssd1306_draw_bmp(0, 32, media1, 32, 32, 1);
        display_temp();
        ssd1306_update_screen();
      }
      break;
    default:
      // If no selection, display static images
      ssd1306_clear();
      ssd1306_draw_bmp(49, 32, settings1, 32, 32, 1);
      ssd1306_draw_bmp(98, 32, temps1, 32, 32, 1);
      ssd1306_draw_bmp(0, 32, media1, 32, 32, 1);
      display_rtc_time();
      ssd1306_update_screen();
      break;
    }
  }
}

static void setting_screen(uint32_t current_time) {

  uint32_t new_time = date_to_seconds(dt) + addition;

  DateTime current_date = timestamp_to_date(new_time);
  alarm_time = date_to_seconds(dt) + 50 + addition;
  DateTime current_alarm = timestamp_to_date(alarm_time);
  // hour is HH:MM:SS AM/PM
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour,
          current_date.minute, current_date.second, current_date.am_pm);

  // print a string
  char new_date_str[20]; // Format: "MM-DD-YYYY"
  sprintf(new_date_str, "Date: %02d-%02d-%04d", current_date.month,
          current_date.day, current_date.year);

  // hour is HH:MM:SS AM/PM
  char alarm_str[20]; // Format: "HH:MM:SS"
  sprintf(alarm_str, "Alarm: %02d:%02d:%02d %cM", current_alarm.hour,
          current_alarm.minute, current_alarm.second, current_alarm.am_pm);

  ssd1306_clear();
  // set XY
  ssd1306_gotoxy(0, 2);
  ssd1306_put_str("Settings", 1);
  ssd1306_gotoxy(0, 16);

  // // print a string
  ssd1306_put_str(new_date_str, 1);

  // set XY
  ssd1306_gotoxy(0, 25);
  // print a string
  ssd1306_put_str(time_str, 1);

  // set XY
  ssd1306_gotoxy(0, 34);
  // print a string
  ssd1306_put_str(alarm_str, 1);
  ssd1306_update_screen();
}

static void WelcomeScreen(void) {
  ssd1306_clear();
  // set XY
  ssd1306_gotoxy(0, 2);
  // print a string
  ssd1306_put_str("Welcome, TimeSync!", 1);
  // set XY
  ssd1306_gotoxy(0, 16);
  // print a string
  ssd1306_put_str("TCES 430 Project", 1);
  // set XY
  ssd1306_gotoxy(0, 25);
  // print a string
  ssd1306_put_str("Final Report", 1);
  ssd1306_gotoxy(0, 34);
  // print a string
  ssd1306_put_str("===================", 1);
  ssd1306_gotoxy(0, 43);
  // print a string
  ssd1306_put_str("By Souleymane.", 1);
  ssd1306_gotoxy(0, 52);
  // print a string
  ssd1306_put_str("===================", 1);
  ssd1306_update_screen();
}

static void display_rtc_time(void) {
  DateTime current_date;
  current_time = rtc_get();
  current_date = timestamp_to_date(rtc_get());

  // hour is HH:MM:SS AM/PM
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour,
          current_date.minute, current_date.second, current_date.am_pm);

  // print a string
  char new_date_str[20]; // Format: "MM-DD-YYYY"
  sprintf(new_date_str, "Date: %02d-%02d-%04d", current_date.month,
          current_date.day, current_date.year);

  // ssd1306_clear();
  // set XY
  ssd1306_gotoxy(0, 2);
  ssd1306_put_str(new_date_str, 1);
  ssd1306_gotoxy(0, 16);

  // print a string
  // ssd1306_put_str(time_str, 1);
  ssd1306_put_str(time_str, 1);
  // set XY
  // ssd1306_update_screen();
}

static void display_temp(void) {
  DateTime current_date;
  uint32_t current_time = rtc_get();
  current_date = timestamp_to_date(rtc_get());

  // hour is HH:MM:SS AM/PM
  char temperature_str[20]; // Format: "HH:MM:SS"
  sprintf(temperature_str, "Temperature: %02d C", temperature);

  // print a string
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour,
          current_date.minute, current_date.second, current_date.am_pm);
  // ssd1306_clear();
  // set XY
  ssd1306_gotoxy(0, 2);
  ssd1306_put_str(temperature_str, 1);
  ssd1306_gotoxy(0, 16);

  // print a string
  // ssd1306_put_str(time_str, 1);
  ssd1306_put_str(time_str, 1);
  // set XY
  // ssd1306_update_screen();
}

static void display_alarm(void) {
  DateTime current_date;
  uint32_t current_time = rtc_get();
  current_date = timestamp_to_date(current_time);
  DateTime current_alarm = timestamp_to_date(alarm_time);

  // hour is HH:MM:SS AM/PM
  char alarm_str[20]; // Format: "HH:MM:SS"
  sprintf(alarm_str, "Alarm: %02d:%02d:%02d %cM", current_alarm.hour,
          current_alarm.minute, current_alarm.second, current_alarm.am_pm);

  // print a string
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour,
          current_date.minute, current_date.second, current_date.am_pm);
  // ssd1306_clear();
  // set XY
  ssd1306_gotoxy(0, 2);
  ssd1306_put_str(alarm_str, 1);
  ssd1306_gotoxy(0, 16);

  // print a string
  // ssd1306_put_str(time_str, 1);
  ssd1306_put_str(time_str, 1);
  // set XY
  // ssd1306_update_screen();
}

void GPIOE_Handler(void) {
  volatile int readbackE;

  GPIO_PORTE_ICR_R = 0x04; /* clear PE2 int */
  button_pressed = 1;
  GPIO_PORTF_DATA_R = 0x04; /* toggle Blue LED */
  Delay();
  GPIO_PORTF_DATA_R = 0x00; /* toggle Blue LED */
  Delay();
  if (setup_mode == 0) {

    uint32_t new_time = date_to_seconds(dt) + addition;
    rtc_disable();
    rtc_init();
    rtc_set(new_time);
    rtc_enable();
    rtc_set_alarm(alarm_time);
    setup_mode = 1;
  } else {

    if (alarm_time == 0) {
      alarm_time = rtc_get() + 50;
      dfplayer_init();
    } else {
      switch (main_nav_select) {
      case 1:
        /* code */
        break;
      case 2:
        /* code */
        setup_mode = 0;
        break;
      case 3:
        /* code */
        break;
      default:
        break;
      }
      readbackE =
          GPIO_PORTE_ICR_R; /* a read to force clearing of interrupt flag */
      // GPIO_PORTF_ICR_R = 0x10;        /* clear PC4 int */
      readbackE = readbackE;
    }
  }

  readbackE = GPIO_PORTE_ICR_R; /* a read to force clearing of interrupt flag */
  // GPIO_PORTF_ICR_R = 0x10;        /* clear PC4 int */
  readbackE = readbackE;
}

void GPIOF_Handler(void) {
  volatile int readback;
  SW2 = GPIO_PORTF_DATA_R & 0x01; // read PF0 into SW2
  SW1 = GPIO_PORTF_DATA_R & 0x10; // read PF0 into SW2

  GPIO_PORTF_ICR_R = 0x11; /* clear PF4 int */

  if (SW2 == 0x00) {
    if (setup_mode == 0) {
      addition++;
    } else {
      // nav right
      if (main_nav_select == 3) {
        main_nav_select = 1;
      } else {
        main_nav_select++;
      }
    }
  }

  if (SW1 == 0x00) {
    if (setup_mode == 0) {
      addition--;
    } else {
      // nav left
      if (main_nav_select == 1) {
        main_nav_select = 3;
      } else {
        main_nav_select--;
      }
    }
  }
  if (alarm_time == 0) {

    alarm_time = rtc_get() + 50;
    dfplayer_init();
  }

  GPIO_PORTF_DATA_R = 0x08; /* toggle Green LED */
  Delay();
  GPIO_PORTF_DATA_R = 0x00; /* toggle Blue LED */
  Delay();

  readback = GPIO_PORTF_ICR_R; /* a read to force clearing of interrupt flag */
  readback = readback;
}

void timer0A_delayMs(int ttime) {
  timer0_InIt();
  int i;

  for (i = 0; i < ttime; i++) {
    while ((TIMER0_RIS_R & 0x01) == 0)
      ;                  /* wait for TimerA timeout flag */
    TIMER0_ICR_R = 0x01; /* clear the TimerA timeout flag */
  }
  timeElapse++;
}

void HIBERNATE_Handler(void) {
  // Clear the interrupt flag
  // test
  // HIB_IC_R |= HIB_IC_RTCALT0;
  // OutCRLF();
  // // Action to be performed on alarm
  // UART_OutString("Alarm");
  // OutCRLF();
}

void OutCRLF(void) {
  UART_OutChar(CR);
  UART_OutChar(LF);
}
