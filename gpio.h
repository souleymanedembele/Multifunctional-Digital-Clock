#ifndef GPIO_H
#define GPIO_H

// C Program by Souleymane Dembele on 11/13/2023
// lab5.c
// Add interrupt with priority 3 on Port E
#include <stdint.h>
// #include "tm4c123gh6pm.h"

#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTF_CR_R (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R (*((volatile unsigned long *)0x4002552C))

#define GPIO_PORTF_IS_R (*((volatile unsigned long *)0x40025404))
#define GPIO_PORTF_IBE_R (*((volatile unsigned long *)0x40025408))
#define GPIO_PORTF_IEV_R (*((volatile unsigned long *)0x4002540C))
#define GPIO_PORTF_IM_R (*((volatile unsigned long *)0x40025410))
#define GPIO_PORTF_ICR_R (*((volatile unsigned long *)0x4002541C))
#define NVIC_EN0_R (*((volatile unsigned long *)0xE000E100))
#define NVIC_PRI7_R (*((volatile unsigned long *)0xE000E41C))

#define GPIO_PORTC_DATA_R (*((volatile unsigned long *)0x400063FC))
#define GPIO_PORTC_DIR_R (*((volatile unsigned long *)0x40006400))
#define GPIO_PORTC_AFSEL_R (*((volatile unsigned long *)0x40006420))
#define GPIO_PORTC_PUR_R (*((volatile unsigned long *)0x40006510))
#define GPIO_PORTC_DEN_R (*((volatile unsigned long *)0x4000651C))
#define GPIO_PORTC_LOCK_R (*((volatile unsigned long *)0x40006520))
#define GPIO_PORTC_CR_R (*((volatile unsigned long *)0x40006524))
#define GPIO_PORTC_AMSEL_R (*((volatile unsigned long *)0x40006528))
#define GPIO_PORTC_PCTL_R (*((volatile unsigned long *)0x4000652C))

// #define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_PRI0_R (*((volatile unsigned long *)0xE000E400)) // IRQ 0 to 3 Priority Register
#define GPIO_PORTC_IS_R (*((volatile unsigned long *)0x40006404))
#define GPIO_PORTC_IBE_R (*((volatile unsigned long *)0x40006408))
#define GPIO_PORTC_IEV_R (*((volatile unsigned long *)0x4000640C))
#define GPIO_PORTC_IM_R (*((volatile unsigned long *)0x40006410))
#define GPIO_PORTC_ICR_R (*((volatile unsigned long *)0x4000641C))
#define GPIO_PORTF_MIS_R (*((volatile uint32_t *)0x40025418))

// a. Define for gpio port e
#define GPIO_PORTE_CR_R (*((volatile unsigned long *)0x40024524))    // GPIO Port E Commit Register for pins 0 to 5 to enable changes to pins 0 to 5
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x400243FC))  // GPIO Port E Data Register for pins 0 to 5 to read or write a value to pins 0 to 5
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x40024400))   // GPIO Port E Direction Register for pins 0 to 5 to set pins 0 to 5 as input or output
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420)) // GPIO Port E Alternate Function Select Register for pins 0 to 5 to enable alternate function on pins 0 to 5
#define GPIO_PORTE_PUR_R (*((volatile unsigned long *)0x40024510))   // GPIO Port E Pull-Up Select Register for pins 0 to 5 to enable pull-up on pins 0 to 5
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))   // GPIO Port E Digital Enable Register for pins 0 to 5 to enable digital function on pins 0 to 5
#define GPIO_PORTE_LOCK_R (*((volatile unsigned long *)0x40024520))  // GPIO Port E Lock Register for pins 0 to 5 to lock pins 0 to 5
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528)) // GPIO Port E Analog Mode Select Register for pins 0 to 5 to enable analog function on pins 0 to 5
#define GPIO_PORTE_PCTL_R (*((volatile unsigned long *)0x4002452C))  // GPIO Port E Port Control Register for pins 0 to 5 to enable alternate function on pins 0 to 5

#define GPIO_PORTE_IS_R (*((volatile unsigned long *)0x40024404))  // GPIO Port E Interrupt Sense Register for pins 0 to 5 to set pins 0 to 5 as edge or level sensitive
#define GPIO_PORTE_IBE_R (*((volatile unsigned long *)0x40024408)) // GPIO Port E Interrupt Both Edges Register for pins 0 to 5 to set pins 0 to 5 as both edges
#define GPIO_PORTE_IEV_R (*((volatile unsigned long *)0x4002440C)) // GPIO Port E Interrupt Event Register for pins 0 to 5 to set pins 0 to 5 as rising or falling edge
#define GPIO_PORTE_IM_R (*((volatile unsigned long *)0x40024410))  // GPIO Port E Interrupt Mask Register for pins 0 to 5 to mask pins 0 to 5
#define GPIO_PORTE_ICR_R (*((volatile unsigned long *)0x4002441C)) // GPIO Port E Interrupt Clear Register for pins 0 to 5 to clear pins 0 to 5
#define NVIC_PRI1_R (*((volatile unsigned long *)0xE000E404))      // IRQ 5(4) to 7 Priority Register
#define NVIC_EN1_R (*((volatile unsigned long *)0xE000E104))       // IRQ 32 to 63 Set Enable Register

void Delay(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
void WaitForInterrupt(void);
void GPIOPortFCE_Init(void); // add E to the gpio port
void GPIOF_Handler(void);
// void GPIOC_Handler(void);
void GPIOE_Handler(void);

unsigned long SW1, SW2;

void GPIOPortFCE_Init(void)
{

    SYSCTL_RCGC2_R |= 0x00000034;   /* 1) activate clock for Port F, C and E*/
                                    /////////// Initialize PORT F  ///////////
    GPIO_PORTF_LOCK_R = 0x4C4F434B; /* 2) unlock GPIO Port F */
    GPIO_PORTF_CR_R = 0x1F;         /* allow changes to PF4-0 */
    GPIO_PORTF_AMSEL_R = 0x00;      /* 3) disable analog on PF */
    GPIO_PORTF_PCTL_R = 0x00000000; /* 4) PCTL GPIO on PF4-0 */
    GPIO_PORTF_DIR_R = 0x0E;        /* 5) PF4,PF0 in, PF3-1 out */
    GPIO_PORTF_AFSEL_R = 0x00;      /* 6) disable alt funct on PF7-0 */
    GPIO_PORTF_PUR_R = 0x11;        /* enable pull-up on PF0 and PF4 */
    GPIO_PORTF_DEN_R = 0x1F;        /* 7) enable digital I/O on PF4-0 */

    GPIO_PORTF_IS_R &= ~0x11;                              /*  PF4 is edge-sensitive */
    GPIO_PORTF_IBE_R &= ~0x11;                             /*  PF4 is not both edges */
    GPIO_PORTF_IEV_R &= ~0x11;                             /*  PF4 falling edge event */
    GPIO_PORTF_ICR_R = 0x11;                               /*  Clear flag4 */
    GPIO_PORTF_IM_R |= 0x11;                               /*  arm interrupt on PF4 */
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF0FFFFF) | 0x00A00000; /*  priority 5 */
    NVIC_EN0_R |= 0x40000000;                              /*  Enable interrupt 30 in NVIC */
    //                                                        ///////////////  Initialize PORT C  //////////
    // GPIO_PORTC_LOCK_R = 0x4C4F434B;                        /* 2) unlock GPIO Port C */
    // GPIO_PORTC_CR_R = 0x30;                                /* allow changes to PC4-5 */
    // GPIO_PORTC_AMSEL_R = 0x00;                             /* 3) disable analog on PC */
    // GPIO_PORTC_PCTL_R = 0x00001111;                        /* 4) PCTL GPIO on PC7-0 */
    // GPIO_PORTC_DIR_R = 0x20;                               /* 5) PF5 OUT, PF4 IN */
    // GPIO_PORTC_AFSEL_R = 0x00;                             /* 6) disable alt funct on PC7-0 */
    // GPIO_PORTC_PUR_R = 0x10;                               /* enable pull-up on PC4 */
    // GPIO_PORTC_DEN_R = 0x30;                               /* 7) enable digital I/O on PC4-5 */

    // GPIO_PORTC_IS_R &= ~0x10;                              /*  PC4 is edge-sensitive */
    // GPIO_PORTC_IBE_R &= ~0x10;                             /*  PC4 is not both edges */
    // GPIO_PORTC_IEV_R &= ~0x10;                             /*  PC4 falling edge event */
    // GPIO_PORTC_ICR_R = 0x10;                               /*  Clear flag4 */
    // GPIO_PORTC_IM_R |= 0x10;                               /*  arm interrupt on PC4 */
    // NVIC_PRI0_R = (NVIC_PRI0_R & 0xFF0FFFFF) | 0x00800000; /*  priority 4 */
    // NVIC_EN0_R |= 0x00000004;                              /*  Enable interrupt 2 in NVIC */

// Initialize GPIO Port E (PE2 as input, Interrupt priority 3)
SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E

// Unlock and commit Port E
GPIO_PORTE_LOCK_R = 0x4C4F434B; // Unlock GPIO Port E
GPIO_PORTE_CR_R |= 0x04;        // Allow changes to PE2 only

// Disable analog function on PE2
GPIO_PORTE_AMSEL_R &= ~0x04;

// Clear alternate functions on PE2
GPIO_PORTE_PCTL_R &= ~0x00000F00;

// Set PE2 as input
GPIO_PORTE_DIR_R &= ~0x04;

// Enable digital I/O on PE2
GPIO_PORTE_DEN_R |= 0x04;

// Disable pull-up on PE2
GPIO_PORTE_PUR_R &= ~0x04;

// Configure PE2 for falling edge trigger
GPIO_PORTE_IS_R &= ~0x04;
GPIO_PORTE_IBE_R &= ~0x04;
GPIO_PORTE_IEV_R &= ~0x04;

// Clear and enable interrupt for PE2
GPIO_PORTE_ICR_R = 0x04;
GPIO_PORTE_IM_R |= 0x04;

// Set priority 3 for the interrupt and enable it
NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF00) | 0x00000060;
NVIC_EN0_R |= 0x00000010;

// Enable global interrupts in main
}

#endif // GPIO_H