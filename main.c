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
//#include "uart.h"
#include "dfplayer_lib.h"

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
// void OutCRLF(void){
//  UART_OutChar(CR);
//  UART_OutChar(LF);
//}
// debug code
int main(void) {
  char i;
  char string[20]; // global to assist in debugging
  uint32_t n;

  PLL_Init(); // set system clock to 50 MHz
  dfplayer_init();
  dfplayer_play_first_track();
  while (1) {
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
