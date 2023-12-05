#include "uart.h"
// #include "tm4c123gh6pm.h"
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
// PA0 -> RX
// PA1 -> TX
void Init_UART_0(uint32_t baudRate) {

  // Enable clock access to gpio a
  SYSCTL_RCGC2_R |= 0x01;
  //  SYSCTL_RCGCGPIO_R |= 0x01;
  SYSCTL_RCGCUART_R |= 0x01;
  // unlock port a
  GPIO_PORTA_LOCK_R = 0x4C4F434B;

  // wait for peripheral
  while ((SYSCTL_PRUART_R & 0x01) == 0) {
  }
  while ((SYSCTL_PRGPIO_R & 0x01) == 0) {
  }

  // disable uart
  UART0_CTL_R &= ~0x01;
  UART0_IBRD_R = 27;    // Set integer part of baud rate
  UART0_FBRD_R = 8;     // Set fractional part of baud rate
  UART0_LCRH_R = 0x60;  // 8-bit, no parity, 1-stop bit, FIFOs
  UART0_CTL_R |= 0x301; // Enable UART0 with TXE, RXE and UARTEN bits
  // set mode to alternate function
  GPIO_PORTA_PCTL_R |= 0x00000011; // UART
  GPIO_PORTA_AMSEL_R &= ~0x03;     // disable analog function on PA1-0
  GPIO_PORTA_AFSEL_R |= 0x03;      // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;        // enable digital I/O on PA1-0
}
char UART_InChar(void) {
  // Wait until RXFE is 0
  while ((UART0_FR_R & 0x10) != 0) {
  }
  return ((char)(UART0_DR_R & 0xFF));
}
// Wait for the buffer to be not full, then output
void UART_OutChar(char data) {
  while ((UART0_FR_R & 0x20) != 0) {
  }
  UART0_DR_R = data;
}
void printString(char *s) {
  if (s == NULL)
    return; // Check for null pointer
  while (*s != '\0') {
    UART_OutChar(*(s++));
  }
}
