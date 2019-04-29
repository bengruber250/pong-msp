#include <msp430g2553.h>
#include "init_SSD1306.h"
#include "lib_lcd.h"
#include "lib_math.h"
#include "delay.h"
#include "fuck.h"


void init_adc(void);

void main(void)
{
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1=CALBC1_16MHZ;
    DCOCTL=CALDCO_16MHZ;


    init_ports();
    init_USCI();    // for 4-wire SPI, comment out for 8080
    init_LCD();




}


void init_adc(void)
{

}
