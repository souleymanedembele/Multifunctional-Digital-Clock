/**
 * File              : main.c
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.03.2023
 * Last Modified Date: 12.09.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */
// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include "PLL.h"
#include <stdint.h>
#include "uart.h"
#include "dfplayer_lib.h"
#include "rtc.h"
#include "timer.h"
#include "ssd1306.h"
#include "horse_anim.h"
#include "settings.h"
#include "temp.h"
#include "media.h"
#include "gpio.h"
#include "adc.h"
#include "alarm.h"

uint8_t main_nav_select = 2; // settings screen is the default selection
uint8_t enter_pressed = 0;
int temperature = 0;
uint32_t alarm_time = -1;
uint32_t current_time = 0;
uint8_t setup_mode = 0;
// uint8_t SW1, SW2;
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

uint32_t timeElapse = 0; // global variable to keep track of time elapsed in main loop cycle (1ms)
uint8_t button_pressed = 0;

DateTime dt;

// debug code
int main(void)
{
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
  if (rtc_get() > date_to_seconds(dt))
  {
    setup_mode = 1;
  }
  // timeElapse = date_to_seconds(dt);
  // rtc_init();
  // rtc_set(date_to_seconds(dt));
  // rtc_enable();
  ADC_Init();
  // ADC1_Init();
  // alarm_time = date_to_seconds(dt) + 50; // For example, set an alarm for 60 seconds later
  GPIOPortFCE_Init();
  UART_Init(); // initialize UART
               // initialize the display
  ssd1306_init();

  /*Update screen*/
  for (int j = 0; j < 2; j++)
  {
    run_horse_animation();
  }

  WelcomeScreen();

  // timer0_InIt(); // initialize the timer
  EnableInterrupts();

  dfplayer_init();

  while (1)
  {
    if (setup_mode == 0)
    {
      while (setup_mode == 0)
      {
        setting_screen(current_time);
      }
    }
    ADC0_PSSI_R |= 0x08; /* start a conversion sequence 3 */
    // ADC1_PSSI_R = 0x08;  // Initiate SS3
    ssd1306_clear();
    current_time = rtc_get();

    while ((ADC0_RIS_R & 0x08) == 0)
    {
      if ((current_time) >= alarm_time)
      {
        if (alarm_time != 0)
        {
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
    temperature = ((ADC0_SSFIFO3_R * 190 / 2) / 4096); // (ADC0_SSFIFO3_R-500)/10 ; // ((ADC0_SSFIFO3_R * 190/2)/4096); //; // 190 = range of temp -40 to 150, 2 is voltage range 0.1-2v, 4096 is 12 bit sampling  ((ADC0_SSFIFO3_R * 190/2) / 4096);

    // while ((ADC1_RIS_R & 0x08) == 0)
    // {
    //   if ((current_time +2) >= alarm_time)
    //   {
    //     if (alarm_time != 0)
    //     {
    //       alarm_time = 0;
    //       dfplayer_play_first_track();
    //       // OutCRLF();
    //       // UART_OutString("Alarm on");
    //       // OutCRLF();
    //     }
    //   }
    //   // OutCRLF();
    //   // display_rtc_time();
    //   run_menu_nav_bar(main_nav_select);
    // }
    /* wait for conversion to complete */
    // time_controler = ADC1_SSFIFO3_R; // 12 bit so 2^12 = 4096 20 is the range of time 0-20

    if ((current_time) >= alarm_time)
    {
      if (alarm_time != 0)
      {
        alarm_time = 0;
        dfplayer_play_first_track();
        // OutCRLF();
        // UART_OutString("Alarm on");
        // OutCRLF();
      }
    }
    ADC0_ISC_R = 0x08; /* clear completion flag */
    // OutCRLF();
    // display_rtc_time();
    run_menu_nav_bar(main_nav_select);
    // timer0A_delayMs(500); // delay 1s
    //  timer0A_delayMs(10);
    //  WaitForInterrupt();
  }
  // UART_Init();              // initialize UART
  // OutCRLF();
  // for (i = 'A'; i <= 'Z'; i = i + 1) { // print the uppercase alphabet
  //   UART_OutChar(i);
  // }
  // OutCRLF();
  // UART_OutChar(' ');
  // for (i = 'a'; i <= 'z'; i = i + 1) { // print the lowercase alphabet
  //   UART_OutChar(i);
  // }
  // OutCRLF();
  // UART_OutChar('-');
  // UART_OutChar('-');
  // UART_OutChar('>');
  // while (1) {
  //   UART_OutString("InString: ");
  //   UART_InString(string, 19);
  //   UART_OutString(" OutString=");
  //   UART_OutString(string);
  //   OutCRLF();

  //   UART_OutString("InUDec: ");
  //   n = UART_InUDec();
  //   UART_OutString(" OutUDec=");
  //   UART_OutUDec(n);
  //   OutCRLF();

  //   UART_OutString("InUHex: ");
  //   n = UART_InUHex();
  //   UART_OutString(" OutUHex=");
  //   UART_OutUHex(n);
  //   OutCRLF();
  // }
}

static void run_horse_animation(void)
{
  /*Clear display*/
  // ssd1306_clear();

  /*draw first horse*/
  ssd1306_draw_bmp(0, 0, horse1, 128, 64, 1);

  /*Update display*/
  //  ssd1306_update_screen();

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

static void run_menu_nav_bar(uint8_t select)
{
  int i = 0;
  // Clear display
  ssd1306_clear();
  if (alarm_time == 0)
  {
    int num_alarm = sizeof(alarm) / sizeof(alarm[0]);
    for (i = 0; i < num_alarm; i++)
    {
      ssd1306_clear();
      ssd1306_draw_bmp(32, 0, alarm[i], 64, 64, 1);
      ssd1306_update_screen();
    }
  }
  else
  {

    // Run animation for the first selection
    int num_settings = sizeof(settings) / sizeof(settings[0]);
    int num_temps = sizeof(temps) / sizeof(temps[0]);
    int num_media = sizeof(media) / sizeof(media[0]);

    // Depending on the selection, run specific animations
    switch (select)
    {
    case 1:

      for (i = 0; i < num_media; i++)
      {
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
      for (i = 0; i < num_settings; i++)
      {
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

      for (i = 0; i < num_temps; i++)
      {
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

static void setting_screen(uint32_t current_time)
{

  uint32_t new_time = date_to_seconds(dt) + addition;

  DateTime current_date = timestamp_to_date(new_time);
  alarm_time = date_to_seconds(dt) + 50 + addition;
  DateTime current_alarm = timestamp_to_date(alarm_time);
  // hour is HH:MM:SS AM/PM
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour, current_date.minute, current_date.second, current_date.am_pm);

  // print a string
  char new_date_str[20]; // Format: "MM-DD-YYYY"
  sprintf(new_date_str, "Date: %02d-%02d-%04d", current_date.month, current_date.day, current_date.year);

  // hour is HH:MM:SS AM/PM
  char alarm_str[20]; // Format: "HH:MM:SS"
  sprintf(alarm_str, "Alarm: %02d:%02d:%02d %cM", current_alarm.hour, current_alarm.minute, current_alarm.second, current_alarm.am_pm);

  ssd1306_clear();
  // ssd1306_draw_bmp(0, 8, settings[i], 16, 16, 1);
  // ssd1306_clear();
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
  // ssd1306_put_str(time_str, 1);
  ssd1306_update_screen();
  // while (enter_pressed == 1)
  // {
  //   /* code */
  // }
}

static void WelcomeScreen(void)
{
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

static void display_rtc_time(void)
{
  DateTime current_date;
  current_time = rtc_get();
  current_date = timestamp_to_date(rtc_get());

  // hour is HH:MM:SS AM/PM
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour, current_date.minute, current_date.second, current_date.am_pm);

  // print a string
  char new_date_str[20]; // Format: "MM-DD-YYYY"
  sprintf(new_date_str, "Date: %02d-%02d-%04d", current_date.month, current_date.day, current_date.year);

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

static void display_temp(void)
{
  DateTime current_date;
  uint32_t current_time = rtc_get();
  current_date = timestamp_to_date(rtc_get());

  // hour is HH:MM:SS AM/PM
  char temperature_str[20]; // Format: "HH:MM:SS"
  sprintf(temperature_str, "Temperature: %02d C", temperature);

  // print a string
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour, current_date.minute, current_date.second, current_date.am_pm);
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

static void display_alarm(void)
{
  DateTime current_date;
  uint32_t current_time = rtc_get();
  current_date = timestamp_to_date(current_time);
  DateTime current_alarm = timestamp_to_date(alarm_time);

  // hour is HH:MM:SS AM/PM
  char alarm_str[20]; // Format: "HH:MM:SS"
  sprintf(alarm_str, "Alarm: %02d:%02d:%02d %cM", current_alarm.hour, current_alarm.minute, current_alarm.second, current_alarm.am_pm);

  // print a string
  char time_str[20]; // Format: "HH:MM:SS"
  sprintf(time_str, "Time: %02d:%02d:%02d %cM", current_date.hour, current_date.minute, current_date.second, current_date.am_pm);
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

void GPIOE_Handler(void)
{
  volatile int readbackE;

  GPIO_PORTE_ICR_R = 0x04; /* clear PE2 int */
  button_pressed = 1;
  GPIO_PORTF_DATA_R = 0x04; /* toggle Blue LED */
  Delay();
  GPIO_PORTF_DATA_R = 0x00; /* toggle Blue LED */
  Delay();
  if (setup_mode == 0)
  {

    uint32_t new_time = date_to_seconds(dt) + addition;
    rtc_disable();
    rtc_init();
    rtc_set(new_time);
    rtc_enable();
    rtc_set_alarm(alarm_time);
    setup_mode = 1;
  }
  else
  {

    if (alarm_time == 0)
    {
      alarm_time = rtc_get() + 50;
      dfplayer_init();
    }
    else
    {
      switch (main_nav_select)
      {
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
      readbackE = GPIO_PORTE_ICR_R; /* a read to force clearing of interrupt flag */
                                    // GPIO_PORTF_ICR_R = 0x10;        /* clear PC4 int */
      readbackE = readbackE;
    }
  }

  readbackE = GPIO_PORTE_ICR_R; /* a read to force clearing of interrupt flag */
                                // GPIO_PORTF_ICR_R = 0x10;        /* clear PC4 int */
  readbackE = readbackE;
}

void GPIOF_Handler(void)
{
  volatile int readback;
  SW2 = GPIO_PORTF_DATA_R & 0x01; // read PF0 into SW2
  SW1 = GPIO_PORTF_DATA_R & 0x10; // read PF0 into SW2

  GPIO_PORTF_ICR_R = 0x11; /* clear PF4 int */

  if (SW2 == 0x00)
  {
    if (setup_mode == 0)
    {
      addition++;
    }
    else
    {
      // nav right
      if (main_nav_select == 3)
      {
        main_nav_select = 1;
      }
      else
      {
        main_nav_select++;
      }
    }
  }

  if (SW1 == 0x00)
  {
    if (setup_mode == 0)
    {
      addition--;
    }
    else
    {
      // nav left
      if (main_nav_select == 1)
      {
        main_nav_select = 3;
      }
      else
      {
        main_nav_select--;
      }
    }
  }
  if (alarm_time == 0)
  {

    alarm_time = rtc_get() + 50;
    dfplayer_init();
  }
  // Call run_menu_nav_bar function
  // ssd1306_clear();
  // run_menu_nav_bar(main_nav_select);

  GPIO_PORTF_DATA_R = 0x08; /* toggle Green LED */
  Delay();
  GPIO_PORTF_DATA_R = 0x00; /* toggle Blue LED */
  Delay();

  // back to green
  // GPIO_PORTF_DATA_R = 0x08;    /* toggle Green LED */
  readback = GPIO_PORTF_ICR_R; /* a read to force clearing of interrupt flag */
                               // GPIO_PORTF_ICR_R = 0x10;        /* clear PF4 int */
  readback = readback;
}

void timer0A_delayMs(int ttime)
{
  timer0_InIt();
  int i;

  for (i = 0; i < ttime; i++)
  {
    while ((TIMER0_RIS_R & 0x01) == 0)
      ;                  /* wait for TimerA timeout flag */
    TIMER0_ICR_R = 0x01; /* clear the TimerA timeout flag */
                         // add time elapsed after each loop cycle to keep track of time passed in main loop cycle (1ms)
  }
  timeElapse++;
}

void HIBERNATE_Handler(void)
{
  // if (HIB_MIS_R & HIB_MIS_RTCALT0) {
  // Handle the alarm event
  // ...
  OutCRLF();
  // Action to be performed on alarm
  UART_OutString("Alarm");
  OutCRLF();
  // dfplayer_play_next_track();
  // dfplayer_play_next_track();
  // }
}

void OutCRLF(void)
{
  UART_OutChar(CR);
  UART_OutChar(LF);
}