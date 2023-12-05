#include "uart.h"
#include <stdio.h>
//#include "tm4c123gh6pm.h"
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTA_PUR_R (*((volatile uint32_t *)0x40004510))
// PA0 -> RX
// PA1 -> TX
void Init_UART_0(void) {

  // Enable clock access to gpio a
  SYSCTL_RCGC2_R |= 0x01;
  //  SYSCTL_RCGCGPIO_R |= 0x01;
  SYSCTL_RCGCUART_R |= 0x01;
  // unlock port a
  GPIO_PORTA_LOCK_R = 0x4C4F434B;

  // wait for peripheral
  // while ((SYSCTL_PRUART_R & 0x01) == 0) {
  // }
  // while ((SYSCTL_PRGPIO_R & 0x01) == 0) {
  //}

  for (int i = 0; i < 1000; i++)
    ;

  // disable uart
  UART0_CTL_R = 0x00;
  UART0_IBRD_R = 27;   // Set integer part of baud rate
  UART0_FBRD_R = 8;    // Set fractional part of baud rate

  UART0_LCRH_R = 0x60;  // 8-bit, no parity, 1-stop bit, FIFOs
  UART0_CTL_R |= 0x301; // Enable UART0 with TXE, RXE and UARTEN bits
  // set mode to alternate function
  GPIO_PORTA_PCTL_R |= 0x00000011; // UART
  GPIO_PORTA_AMSEL_R = 0x00;     // disable analog function on PA1-0
  GPIO_PORTA_AFSEL_R |= 0x03;      // enable alt funct on PA1-0
  GPIO_PORTA_PUR_R = 0x11;         // enable pullup resistors on PF4,PF0
  GPIO_PORTA_DEN_R |= 0x03;        // enable digital I/O on PA1-0
}
void Delay(void){unsigned long volatile time;
  time = 727240*200/91;  // 0.1sec
  while(time){
    time--;
  }
}

char UART_InChar(void) {
  // Wait until RXFE is 0
  while ((UART0_FR_R & 0x10) != 0);
  return ((char)(UART0_DR_R & 0xFF));
}
// Wait for the buffer to be not full, then output
void UART_OutChar(char data) {
  while ((UART0_FR_R & 0x0020)) {};
  UART0_DR_R = data;
}

// Print a character to UART.
int fputc(int ch, FILE *f){
  if((ch == 10) || (ch == 13) || (ch == 27)){
    UART_OutChar(13);
    UART_OutChar(10);
    return 1;
  }
  UART_OutChar(ch);
  return 1;
}
// Get input from UART, echo
int fgetc (FILE *f){
  char ch = UART_InChar();  // receive from keyboard
  UART_OutChar(ch);            // echo
  return ch;
}
// Function called when file error occurs.
int ferror(FILE *f){
  /* Your implementation of ferror */
  return EOF;
}

void printString(const char *str) {
  while (*str != '\0') {
    UART_OutChar(*str++);
	//	Delay();
  }
}
