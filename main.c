/**
 * File              : main.c
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.03.2023
 * Last Modified Date: 12.04.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */
// #include "TM4C123.h"  // Device header
#include "uart.h"
#include "PLL.h"
int main(void)
{
     
	PLL_Init();       // 50  MHz
  Init_UART_0(9600);  // init uart
  while (1)
  {
    printString("Hello World\n\r");
  }
  // return 0;
}
