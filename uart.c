// #include "uart.h"
// #include <stdio.h>
// // #include "tm4c123gh6pm.h"
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTA_PUR_R (*((volatile uint32_t *)0x40004510))
#define UART0_IFLS_R (*((volatile uint32_t *)0x4000C034))
#define NVIC_EN0_INT5 0x00000020 // Interrupt 5 enable


#include <stdint.h>
// #include "inc/tm4c123gh6pm.h"

#include "FIFO.h"
#include "uart.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
// long StartCritical(void);     // previous I bit, disable interrupts
// void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void); // low power mode
#define FIFOSIZE 16          // size of the FIFOs (must be power of 2)
#define FIFOSUCCESS 1        // return value on success
#define FIFOFAIL                                                               \
  0 // return value on failure
    // create index implementation FIFO (see FIFO.h)
    // AddIndexFifo(Rx, FIFOSIZE, char, FIFOSUCCESS, FIFOFAIL)
    // AddIndexFifo(Tx, FIFOSIZE, char, FIFOSUCCESS, FIFOFAIL)

// Initialize UART0 and UART1
// Baud rate is 115200 bits/sec
void UART_Init(void) {
  // SYSCTL_RCGCUART_R |= 0x01; // activate UART0
  // SYSCTL_RCGCGPIO_R |= 0x01; // activate port A
  // RxFifo_Init();             // initialize empty FIFOs
  // TxFifo_Init();
  // UART0_CTL_R &= ~UART_CTL_UARTEN; // disable UART
  // UART0_IBRD_R = 27;               // IBRD = int(50,000,000 / (16 * 115,200))
  // = int(27.1267) UART0_FBRD_R = 8;                // FBRD = int(0.1267 * 64 +
  // 0.5) = 8
  //                                  // 8 bit word length (no parity bits, one
  //                                  stop bit, FIFOs)
  // UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
  // UART0_IFLS_R &= ~0x3F; // clear TX and RX interrupt FIFO level fields
  //                        // configure interrupt for TX FIFO <= 1/8 full
  //                        // configure interrupt for RX FIFO >= 1/8 full
  // UART0_IFLS_R += (UART_IFLS_TX1_8 | UART_IFLS_RX1_8);
  // // enable TX and RX FIFO interrupts and RX time-out interrupt
  // UART0_IM_R |= (UART_IM_RXIM | UART_IM_TXIM | UART_IM_RTIM);
  // UART0_CTL_R |= UART_CTL_UARTEN; // enable UART
  // GPIO_PORTA_AFSEL_R |= 0x03;     // enable alt funct on PA1-0
  // GPIO_PORTA_DEN_R |= 0x03;       // enable digital I/O on PA1-0
  //                                 // configure PA1-0 as UART
  // GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
  // GPIO_PORTA_AMSEL_R = 0;                                // disable analog
  // functionality on PA
  //                                                        // UART0=priority 2
  // NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFF00FF) | 0x00004000; // bits 13-15
  // NVIC_EN0_R = NVIC_EN0_INT5;                            // enable interrupt
  // 5 in NVIC

  // Connect PC4 and PC5 to UART1
  // Activate UART1 Port C
  SYSCTL_RCGCUART_R |= 0x02; // activate UART1
  SYSCTL_RCGCGPIO_R |= 0x04; // activate port C
  RxFifo_Init();             // initialize empty FIFOs
  TxFifo_Init();
  UART1_CTL_R &= ~UART_CTL_UARTEN; // disable UART
  UART1_IBRD_R = 325; // IBRD = int(50,000,000 / (16 * 9600)) = int(325.5208))
  UART1_FBRD_R = 33;  // FBRD = int(0.5208 * 64 + 0.5) = 33
                      // 8 bit word length (no parity bits, one stop bit, FIFOs)

  UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
  UART1_IFLS_R &= ~0x3F; // clear TX and RX interrupt FIFO level fields
                         // configure interrupt for TX FIFO <= 1/8 full
                         // configure interrupt for RX FIFO >= 1/8 full
  UART1_IFLS_R += (UART_IFLS_TX1_8 | UART_IFLS_RX1_8);
  UART1_IM_R |= (UART_IM_RXIM | UART_IM_TXIM | UART_IM_RTIM);
  UART1_CTL_R |= UART_CTL_UARTEN; // enable UART
  GPIO_PORTC_AFSEL_R |= 0x30;     // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;       // enable digital I/O on PC5-4
                                  // configure PC5-4 as UART
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF) + 0x00220000;
  GPIO_PORTC_AMSEL_R = 0; // disable analog functionality on PC
                          // UART1=priority 2
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF00FFFF) | 0x00400000; // bits 21-23
  NVIC_EN0_R = NVIC_EN0_INT6; // enable interrupt 6 in NVIC

  EnableInterrupts();
}

void static copyHardwareToSoftware(void) {
  char letter;
  while (((UART1_FR_R & UART_FR_RXFE) == 0) &&
         (RxFifo_Size() < (FIFOSIZE - 1))) {
    letter = UART1_DR_R;
    RxFifo_Put(letter);
  }
}

void static copySoftwareToHardware(void) {
  char letter;
  while (((UART1_FR_R & UART_FR_TXFF) == 0) && (TxFifo_Size() > 0)) {
    TxFifo_Get(&letter);
    UART1_DR_R = letter;
  }
}

char UART_InChar(void) {
  char letter;
  while (RxFifo_Get(&letter) == FIFOFAIL) {
  };
  return (letter);
}

void UART_OutChar(char data) {
  while (TxFifo_Put(data) == FIFOFAIL) {
  };
  UART1_IM_R &= ~UART_IM_TXIM; // disable TX FIFO interrupt
  copySoftwareToHardware();
  UART1_IM_R |= UART_IM_TXIM; // enable TX FIFO interrupt
}

void UART1_Handler(void) {
  if (UART1_RIS_R & UART_RIS_TXRIS) { // hardware TX FIFO <= 2 items
    UART1_ICR_R = UART_ICR_TXIC;      // acknowledge TX FIFO
    // copy from software TX FIFO to hardware TX FIFO
    copySoftwareToHardware();
    if (TxFifo_Size() == 0) {      // software TX FIFO is empty
      UART1_IM_R &= ~UART_IM_TXIM; // disable TX FIFO interrupt
    }
  }
  if (UART1_RIS_R & UART_RIS_RXRIS) { // hardware RX FIFO >= 2 items
    UART1_ICR_R = UART_ICR_RXIC;      // acknowledge RX FIFO
    // copy from hardware RX FIFO to software RX FIFO
    copyHardwareToSoftware();
  }
  if (UART1_RIS_R & UART_RIS_RTRIS) { // receiver timed out
    UART1_ICR_R = UART_ICR_RTIC;      // acknowledge receiver time out
    // copy from hardware RX FIFO to software RX FIFO
    copyHardwareToSoftware();
  }
}

void UART_OutString(char *pt) {
  while (*pt) {
    UART_OutChar(*pt);
    pt++;
  }
}

uint32_t UART_InUDec(void) {
  uint32_t number = 0, length = 0;
  char character;
  character = UART_InChar();
  while (character != CR) { // accepts until <enter> is typed
    // The next line checks that the input is a digit, 0-9.
    // If the character is not 0-9, it is ignored and not echoed
    if ((character >= '0') && (character <= '9')) {
      number = 10 * number +
               (character - '0'); // this line overflows if above 4294967295
      length++;
      UART_OutChar(character);
    }
    // If the input is a backspace, then the return number is
    // changed and a backspace is outputted to the screen
    else if ((character == BS) && length) {
      number /= 10;
      length--;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  return number;
}

void UART_OutUDec(uint32_t n) {
  // This function uses recursion to convert decimal number
  //   of unspecified length as an ASCII string
  if (n >= 10) {
    UART_OutUDec(n / 10);
    n = n % 10;
  }
  UART_OutChar(n + '0'); /* n is between 0 and 9 */
}

uint32_t UART_InUHex(void) {
  uint32_t number = 0, digit, length = 0;
  char character;
  character = UART_InChar();
  while (character != CR) {
    digit = 0x10; // assume bad
    if ((character >= '0') && (character <= '9')) {
      digit = character - '0';
    } else if ((character >= 'A') && (character <= 'F')) {
      digit = (character - 'A') + 0xA;
    } else if ((character >= 'a') && (character <= 'f')) {
      digit = (character - 'a') + 0xA;
    }
    // If the character is not 0-9 or A-F, it is ignored and not echoed
    if (digit <= 0xF) {
      number = number * 0x10 + digit;
      length++;
      UART_OutChar(character);
    }
    // Backspace outputted and return value changed if a backspace is inputted
    else if ((character == BS) && length) {
      number /= 0x10;
      length--;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  return number;
}

void UART_OutUHex(uint32_t number) {
  if (number >= 0x10) {
    UART_OutUHex(number / 0x10);
    UART_OutUHex(number % 0x10);
  } else {
    if (number < 0xA) {
      UART_OutChar(number + '0');
    } else {
      UART_OutChar((number - 0x0A) + 'A');
    }
  }
}

void UART_InString(char *bufPt, uint16_t max) {
  int length = 0;
  char character;
  character = UART_InChar();
  while (character != CR) {
    if (character == BS) {
      if (length) {
        bufPt--;
        length--;
        UART_OutChar(BS);
      }
    } else if (length < max) {
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  *bufPt = 0;
}
