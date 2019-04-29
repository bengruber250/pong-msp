#include <msp430g2553.h>
#include "pots.h"

int pot_vals[2];

void init_pots(void)
{
    /* Repeat seq. of channels, starting at A2, SMCLK / 2. */
    ADC10CTL1 = CONSEQ_1 | INCH_2 | ADC10SSEL_3 | ADC10DIV_1;
    /* 16 cycle sample, multi sample, interrupt enable. */
    ADC10CTL0 = ADC10SHT_2 | ADC10IE | MSC | ADC10ON;
    /* Enable channels A0-A2. */
    ADC10AE0 = BIT1 | BIT2;
    /* Enable data transfer controller in continous, 2 block mode. */
//    ADC10DTC0 = ADC10CT;
    /* 3 transfers per block:  transfers 1 and 2 are channels A2 and A1. */
    ADC10DTC1 = 0x02;
    /* Start address for transfers */
    ADC10SA = (int)pot_vals;
}

void get_pots(void)
{
    ADC10CTL0 |= ADC10SC | ENC;
    __bis_SR_register(CPUOFF+GIE);
    ADC10DTC1 = 0x02;
    ADC10SA = (int)pot_vals;
}

#pragma vector=ADC10_VECTOR
__interrupt void adc10_isr(void)
{
    __bic_SR_register_on_exit(CPUOFF);
}

