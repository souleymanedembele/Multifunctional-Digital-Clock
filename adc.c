#include "adc.h"

void ADC_Init(void)
{
    SYSCTL_RCGC2_R |= 0x10;
    SYSCTL_RCGCADC_R |= 0x01;  // Enable clock ADC0
    // 
    GPIO_PORTE_AFSEL_R |= 0x08; // Enable alternate function on PE3
    GPIO_PORTE_DEN_R &= ~0x08;  // Disable digital I/O on PE3
    GPIO_PORTE_AMSEL_R |= 0x08; // Enable analog function on PE3
    // 
    /* initialize ADC0 */
    ADC0_ACTSS_R &= ~0x8;   /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000; /* software trigger conversion */
    // ADC0_SSMUX3_R = 0x7;    /* get input from channel 0 */
    ADC0_SSMUX3_R = 0x0;    /* get input from channel 0 */
    ADC0_SSCTL3_R |= 0x6;   /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 0x8;    /* enable ADC0 sequencer 3 */
}

void ADC1_Init(void) {
    SYSCTL_RCGC2_R |= 0x10;       // Enable clock for Port E
    SYSCTL_RCGCADC_R |= 0x02;     // Enable clock for ADC1

    // Configure PE1 for ADC input
    GPIO_PORTE_AFSEL_R |= 0x02;   // Enable alternate function on PE1
    GPIO_PORTE_DEN_R &= ~0x02;    // Disable digital I/O on PE1
    GPIO_PORTE_AMSEL_R |= 0x02;   // Enable analog function on PE1

    // Initialize ADC1
    ADC1_ACTSS_R &= ~0x08;        // Disable SS3 during configuration
    ADC1_EMUX_R &= ~0xF000;       // Software trigger conversion
    ADC1_SSMUX3_R = 0x0;          // Set input from channel 0 (PE1)
    ADC1_SSCTL3_R |= 0x06;        // Take one sample at a time, set flag at 1st sample
    ADC1_ACTSS_R |= 0x08;         // Enable ADC1 sequencer 3
}