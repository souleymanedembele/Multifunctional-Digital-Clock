#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTA_LOCK_R (*((volatile unsigned long *)0x40004520))
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define UART0_DR_R (*((volatile unsigned long *)0x4000C000))
#define UART0_FR_R (*((volatile unsigned long *)0x4000C018))
#define SYSCTL_RCGCUART_R (*((volatile uint32_t *)0x400FE618))
#define SYSCTL_PRUART_R (*((volatile uint32_t *)0x400FEA18))
#define SYSCTL_PRGPIO_R (*((volatile uint32_t *)0x400FEA08))
#define UART0_CTL_R (*((volatile uint32_t *)0x4000C030))
#define UART0_IBRD_R (*((volatile uint32_t *)0x4000C024))
#define UART0_FBRD_R (*((volatile uint32_t *)0x4000C028))
#define UART0_LCRH_R (*((volatile uint32_t *)0x4000C02C))
#define GPIO_PORTA_PCTL_R (*((volatile uint32_t *)0x4000452C))
#define GPIO_PORTA_AMSEL_R (*((volatile uint32_t *)0x40004528))
#define GPIO_PORTA_AFSEL_R (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_DEN_R (*((volatile uint32_t *)0x4000451C))
#define UART0_IM_R (*((volatile uint32_t *)0x4000C038))
#define NVIC_PRI1_R (*((volatile uint32_t *)0xE000E404))
#define NVIC_EN0_R (*((volatile uint32_t *)0xE000E100))
#define UART0_RIS_R (*((volatile uint32_t *)0x4000C03C))
#define UART0_ICR_R (*((volatile uint32_t *)0x4000C044))

#define UART1_CTL_R (*((volatile uint32_t *)0x4000D030))
#define UART1_FR_R (*((volatile uint32_t *)0x4000D018))
#define UART1_ILPR_R (*((volatile uint32_t *)0x4000D020))
#define UART1_IBRD_R (*((volatile uint32_t *)0x4000D024))
#define UART1_FBRD_R (*((volatile uint32_t *)0x4000D028))
#define UART1_LCRH_R (*((volatile uint32_t *)0x4000D02C))
#define UART1_CTL_R (*((volatile uint32_t *)0x4000D030))
#define UART1_IFLS_R (*((volatile uint32_t *)0x4000D034))
#define UART1_IM_R (*((volatile uint32_t *)0x4000D038))
#define UART1_RIS_R (*((volatile uint32_t *)0x4000D03C))
#define UART1_MIS_R (*((volatile uint32_t *)0x4000D040))
#define UART1_ICR_R (*((volatile uint32_t *)0x4000D044))

#define GPIO_PORTC_ICR_R (*((volatile uint32_t *)0x4000641C))
#define GPIO_PORTC_AFSEL_R (*((volatile uint32_t *)0x40006420))
#define GPIO_PORTC_DEN_R (*((volatile uint32_t *)0x4000651C))
#define GPIO_PORTC_PCTL_R (*((volatile uint32_t *)0x4000652C))
#define GPIO_PORTC_AMSEL_R (*((volatile uint32_t *)0x40006528))

#define NVIC_EN0_INT5 0x00000020 // Interrupt 5 enable
#define NVIC_EN0_INT6 0x00000040 // Interrupt 6 enable

#define UART_FR_RXFF 0x00000040     // UART Receive FIFO Full
#define UART_FR_TXFF 0x00000020     // UART Transmit FIFO Full
#define UART_FR_RXFE 0x00000010     // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8 0x00000060 // 8 bit word length
#define UART_LCRH_FEN 0x00000010    // UART Enable FIFOs
#define UART_CTL_UARTEN 0x00000001  // UART Enable
#define UART_IFLS_RX1_8 0x00000000  // RX FIFO >= 1/8 full
#define UART_IFLS_TX1_8 0x00000000  // TX FIFO <= 1/8 full
#define UART_IM_RTIM                                                           \
  0x00000040                    // UART Receive Time-Out Interrupt
                                // Mask
#define UART_IM_TXIM 0x00000020 // UART Transmit Interrupt Mask
#define UART_IM_RXIM 0x00000010 // UART Receive Interrupt Mask
#define UART_RIS_RTRIS                                                         \
  0x00000040 // UART Receive Time-Out Raw
             // Interrupt Status
#define UART_RIS_TXRIS                                                         \
  0x00000020 // UART Transmit Raw Interrupt
             // Status
#define UART_RIS_RXRIS                                                         \
  0x00000010                     // UART Receive Raw Interrupt
                                 // Status
#define UART_ICR_RTIC 0x00000040 // Receive Time-Out Interrupt Clear
#define UART_ICR_TXIC 0x00000020 // Transmit Interrupt Clear
#define UART_ICR_RXIC 0x00000010 // Receive Interrupt Clear
#define UART1_DR_R (*((volatile uint32_t *)0x4000D000))
// standard ASCII symbols
#define CR 0x0D
#define LF 0x0A
#define BS 0x08
#define ESC 0x1B
#define SP 0x20
#define DEL 0x7F

//------------UART_Init------------
// Initialize the UART for 115,200 baud rate (assuming 50 MHz clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void);

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
char UART_InChar(void);

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data);

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt);

//------------UART_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART_InUDec(void);

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_OutUDec(uint32_t n);

//---------------------UART_InUHex----------------------------------------
// Accepts ASCII input in unsigned hexadecimal (base 16) format
// Input: none
// Output: 32-bit unsigned number
// No '$' or '0x' need be entered, just the 1 to 8 hex digits
// It will convert lower case a-f to uppercase A-F
//     and converts to a 16 bit unsigned number
//     value range is 0 to FFFFFFFF
// If you enter a number above FFFFFFFF, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART_InUHex(void);

//--------------------------UART_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void UART_OutUHex(uint32_t number);

//------------UART_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void UART_InString(char *bufPt, uint16_t max);
#endif
