/**
 * File              : timer.h
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.13.2023
 * Last Modified Date: 12.14.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */

#ifndef TIMER_H
#define TIMER_H

//#include "tm4c123gh6pm.h"
#define TIMER0_CFG_R (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R (*((volatile unsigned long *)0x4003000C))
#define TIMER0_RIS_R (*((volatile unsigned long *)0x4003001C))
#define TIMER0_ICR_R (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R (*((volatile unsigned long *)0x40030038))
#define SYSCTL_RCGCTIMER_R (*((volatile unsigned long *)0x400FE604))


#include <stdint.h>

void timer0_InIt(void);
// void timer_wait(uint32_t delay);

volatile uint32_t seconds = 0;

void timer0_InIt(void)
{
  SYSCTL_RCGCTIMER_R |= 0x01; /* enable clock to Timer0 */

  TIMER0_CTL_R = 0x00;        /* disable Timer before initialization */
  TIMER0_CFG_R = 0x04;        /* 16-bit option */
  TIMER0_TAMR_R = 0x02;       /* periodic mode and down-counter */
  TIMER0_TAILR_R = 50000 - 1; /* Timer A interval load value register */
  TIMER0_ICR_R = 0x1;         /* clear the Tim erA timeout flag*/
  TIMER0_CTL_R |= 0x01;       /* enable Timer A after initialization */
  TIMER0_TAPR_R = 1;          // Prescalar value.. Can extend the cycle time max 255 times
}


#endif /* TIMER_H */
