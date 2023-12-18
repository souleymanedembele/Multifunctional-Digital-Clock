#include "gpio.h"

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
