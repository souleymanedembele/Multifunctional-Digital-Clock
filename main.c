/**
 * File              : main.c
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.03.2023
 * Last Modified Date: 12.04.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */
// #include "TM4C123.h"  // Device header
#include <stdio.h>
#include <stdint.h> // C99 variable types
#include "uart.h"
#include "PLL.h"

int main(void)
{
     
	PLL_Init();       // 50  MHz
  Init_UART_0();  // init uart

  while (1)
  {

    printString("Hello World from TM4C123\n\r");
		UART_OutChar('A');
		UART_OutChar('B');
		UART_OutChar('C');
		UART_OutChar('\n');
		UART_OutChar('\r');
		//Delay();                      // wait 0.1 sec
  }
  // return 0;
}
