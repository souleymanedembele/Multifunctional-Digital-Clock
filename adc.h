#ifndef ADC_H
#define ADC_H
// Thanks to the lab 7 for
/// ADC
#define SYSCTL_RCGCADC_R (*((volatile unsigned long *)0x400FE638))
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420)) // GPIO Port E Alternate Function Select Register for pins 0 to 5 to enable alternate function on pins 0 to 5
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))   // GPIO Port E Digital Enable Register for pins 0 to 5 to enable digital function on pins 0 to 5
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528)) // GPIO Port E Analog Mode Select Register for pins 0 to 5 to enable analog function on pins 0 to 5

#define ADC0_ACTSS_R (*((volatile unsigned long *)0x40038000))
#define ADC0_EMUX_R (*((volatile unsigned long *)0x40038014))
#define ADC0_SSMUX3_R (*((volatile unsigned long *)0x400380A0))
#define ADC0_SSCTL3_R (*((volatile unsigned long *)0x400380A4))
#define ADC0_PSSI_R (*((volatile unsigned long *)0x40038028))
#define ADC0_RIS_R (*((volatile unsigned long *)0x40038004))
#define ADC0_SSFIFO3_R (*((volatile unsigned long *)0x400380A8))
#define ADC0_ISC_R (*((volatile unsigned long *)0x4003800C))

#define TIMER0_CFG_R (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R (*((volatile unsigned long *)0x4003000C))
#define TIMER0_RIS_R (*((volatile unsigned long *)0x4003001C))
#define TIMER0_ICR_R (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R (*((volatile unsigned long *)0x40030038))
#define SYSCTL_RCGCTIMER_R (*((volatile unsigned long *)0x400FE604))

#define SYSCTL_RCGCADC_R (*((volatile unsigned long *)0x400FE638))
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528))

#define ADC1_ACTSS_R (*((volatile unsigned long *)0x40039000))
#define ADC1_EMUX_R (*((volatile unsigned long *)0x40039014))
#define ADC1_SSMUX3_R (*((volatile unsigned long *)0x400390A0))
#define ADC1_SSCTL3_R (*((volatile unsigned long *)0x400390A4))
#define ADC1_PSSI_R (*((volatile unsigned long *)0x40039028))
#define ADC1_RIS_R (*((volatile unsigned long *)0x40039004))
#define ADC1_SSFIFO3_R (*((volatile unsigned long *)0x400390A8))
#define ADC1_ISC_R (*((volatile unsigned long *)0x4003900C))

void ADC_Init(void);
void ADC1_Init(void);
#endif // ADC_H