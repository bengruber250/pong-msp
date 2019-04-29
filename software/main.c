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

void main(void)
{
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1=CALBC1_16MHZ;
    DCOCTL=CALDCO_16MHZ;


    init_ports();
    init_USCI();    // for 4-wire SPI, comment out for 8080
    init_LCD();
    init_adc10();

    


}


void init_adc10(void)
{
    ADC10CTL0
    ADC10CTL1
    ADC10AE0
    
    // SHTx config so that t_sample is 8 clk cycles when driven by 
    // SMCLK at 1 MHz
    // Or 64 when driven by internal 5 MHz clk
    

}
