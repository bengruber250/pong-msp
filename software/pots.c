#include <msp430g2553.h>
#include "pots.h"

int pot_vals[2];

void init_pots(void)
{
    /* Repeat seq. of channels, starting at A2, SMCLK / 2. */
    ADC10CTL1 = CONSEQ_1 | INCH_2 | ADC10SSEL_3 | ADC10DIV_1;
    /* 16 cycle sample, multi sample*/
    ADC10CTL0 = ADC10SHT_2 | MSC | ADC10ON;
    /* Enable channels A1-A2. */
    ADC10AE0 = BIT1 | BIT2;
    /* 2 transfers per block. */
    ADC10DTC1 = 0x02;
    /* Start address for transfers */
    ADC10SA = (int)pot_vals;
}

void get_pots(void)
{
    ADC10CTL0 |= ADC10SC | ENC;
//    __bis_SR_register(CPUOFF+GIE);
    while(!(ADC10CTL0 & ADC10IFG));
    ADC10DTC1 = 0x02;
    ADC10SA = (int)pot_vals;
}

//#pragma vector=ADC10_VECTOR
//__interrupt void adc10_isr(void)
//{
//    __bic_SR_register_on_exit(CPUOFF);
//}

