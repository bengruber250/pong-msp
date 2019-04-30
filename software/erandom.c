/*
 * erandom.c
 *
 *  Created on: Mar 25, 2019
 *      Author: pugsley
 */
#include <msp430.h>
#include <stdint.h>
#include <erandom.h>

uint16_t lfsr = 0xACE1;    /* Default seed */

void srand(int seed)
{
    lfsr = seed;
}

void init_rand_adc_seed(void)
{
    // ADC Config
    ADC10CTL0 = SREF_1 + ADC10SHT_0 + ADC10ON;      // V_ref-Vss, hold for 4 cycles, adc on
    ADC10CTL0 |= ADC10IE + REFON + REF2_5V;         // interrupt enable, reference at 2.5 V
    ADC10CTL0 |= ADC10SR;                           // less current usage by reducing reference buffer drive
    ADC10CTL1 = INCH_2 + ADC10DIV_0 + ADC10SSEL_1;  // input P1.0, clock divider 0, driven by ACLK
    ADC10AE0 |= BIT0;                               // Enable ADC function on P1.0

    ADC10CTL0 |= ENC + ADC10SC;                     // Sampling and conversion start
    while(ADC10CTL1 & ADC10BUSY);
    uint16_t temp;
    temp = ADC10MEM;                                // Get ADC val.
    if (temp != 0)
        srand((temp << 5) | 0x00C8);
    else
        srand(0xACE1);
}

inline unsigned int rand()
{
    /*
     * Compute new LSB of shift register:
     * lsb = bit0 + bit10 + bit30 + bit31
     * which are the taps for the polynomial:
     * x^32 + x^22 + x^2 + x^1 + 1
     *  (these are for 32 bit LFSR)
     */
    uint16_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5));
    /* Shift the register and OR in the new bit. */
    lfsr = (lfsr >> 1) | (bit << 15);
    /* Return only the bits that matter according to RAND_MAX */
    return RAND_MAX & lfsr;
}


// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __bic_SR_register_on_exit(LPM3_bits);           // Exit LPM3
}




