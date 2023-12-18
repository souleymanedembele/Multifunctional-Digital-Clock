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
#define UART_IM_RTIM \
  0x00000040                    // UART Receive Time-Out Interrupt
                                // Mask
#define UART_IM_TXIM 0x00000020 // UART Transmit Interrupt Mask
#define UART_IM_RXIM 0x00000010 // UART Receive Interrupt Mask
#define UART_RIS_RTRIS \
  0x00000040 // UART Receive Time-Out Raw
             // Interrupt Status
#define UART_RIS_TXRIS \
  0x00000020 // UART Transmit Raw Interrupt
             // Status
#define UART_RIS_RXRIS \
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

void UART_Init(void);

char UART_InChar(void);

void UART_OutChar(char data);

void UART_OutString(char *pt);

uint32_t UART_InUDec(void);

void UART_OutUDec(uint32_t n);

uint32_t UART_InUHex(void);

void UART_OutUHex(uint32_t number);

void UART_InString(char *bufPt, uint16_t max);
#endif
