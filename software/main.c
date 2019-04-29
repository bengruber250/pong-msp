#include <msp430g2553.h>
#include "init_SSD1306.h"
#include "lib_lcd.h"
#include "lib_math.h"
#include "delay.h"
#include "fuck.h"

#define KNBP P1
#define KNB1 BIT1
#define KNB2 BIT2


void init_adc10(void);

int pot_vals[2];
int pot1, pot2;

void main(void)
{
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1=CALBC1_16MHZ;
    BCSCTL2 = DIVS_3;
    DCOCTL=CALDCO_16MHZ;
    __bis_SR_register(GIE);


//    init_ports();
//    init_USCI();    // for 4-wire SPI, comment out for 8080
//    init_LCD();
    init_adc10();
    while (1) {
        ADC10CTL0 |= ADC10SC | ENC;
        __bis_SR_register(CPUOFF+GIE);
        ADC10DTC1 = 0x02;
        ADC10SA = (int)pot_vals;
        pot1 = pot_vals[0];
        pot2 = pot_vals[1];
    }
}


void init_adc10(void)
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
    /* Start ADC. */
//    ADC10CTL0 |= ADC10SC | ENC;

}


#pragma vector=ADC10_VECTOR
__interrupt void adc10_isr(void)
{
    __bic_SR_register_on_exit(CPUOFF);
}


