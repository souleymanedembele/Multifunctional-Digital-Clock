#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdlib.h>

#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTA_LOCK_R (*((volatile unsigned long *)0x40004520))

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

void Init_UART_0(uint32_t baudRate);
char UART_InChar(void);
void UART_OutChar(char data);
void printString(char *s);

#endif
