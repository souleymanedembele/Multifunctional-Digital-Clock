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

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void OutCRLF(void){
 UART_OutChar(CR);
 UART_OutChar(LF);
}

uint32_t timeElapse = 0; // global variable to keep track of time elapsed in main loop cycle (1ms)
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


// debug code
int main(void) {

  char i;
  char string[20]; // global to assist in debugging
  uint32_t n;

  PLL_Init(); // set system clock to 50 MHz
DateTime dt;
dt.second = 50;
dt.minute = 59;
dt.hour = 11; // This is in 12-hour format
dt.am_pm = 'P'; // 'A' for AM, 'P' for PM
dt.day = 15;
dt.month = 12;
dt.year = 2023;

    timeElapse = date_to_seconds(dt);
  rtc_init();
  rtc_set(date_to_seconds(dt));
  rtc_enable();
  UART_Init(); // initialize UART
 // timer0_InIt(); // initialize the timer
  EnableInterrupts();



 // dfplayer_init();
 // dfplayer_play_first_track();
 // display rtc time 
 DateTime current_date;
 uint32_t current_time = rtc_get();
//  UART_OutString("Current Date: ");
//   current_date = timestamp_to_date(rtc_get());
//   UART_OutUDec(current_date.year);
//   UART_OutChar('/');
//   UART_OutUDec(current_date.month);
//   UART_OutChar('/');
//   UART_OutUDec(current_date.day);
//   UART_OutChar(' ');
//   UART_OutUDec(current_date.hour);
//   UART_OutChar(':');
//   UART_OutUDec(current_date.minute);
//   UART_OutChar(':');
//   UART_OutUDec(current_date.second);
//   OutCRLF();

  while (1) {
    current_time = rtc_get();
    current_date = timestamp_to_date(current_time);
    UART_OutString("Current Date: ");
    UART_OutUDec(current_date.month);
    UART_OutChar('/');
    UART_OutUDec(current_date.day);
    UART_OutChar('/');
    UART_OutUDec(current_date.year);
    UART_OutChar(' ');
    UART_OutUDec(current_date.hour);
    UART_OutChar(':');
    UART_OutUDec(current_date.minute);
    UART_OutChar(':');
    UART_OutUDec(current_date.second);
    UART_OutChar(' ');
    if(current_date.am_pm == 'A') {
      UART_OutString("AM");
    } else {
      UART_OutString("PM");
    }

    OutCRLF();
    timer0A_delayMs(500); // delay 1s
    //timer0A_delayMs(10);
   //WaitForInterrupt();
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
