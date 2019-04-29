#include <msp430g2553.h>
//#include "init_SSD1306.h"
//#include "lib_lcd.h"
//#include "lib_math.h"
//#include "delay.h"
//#include "fuck.h"
#include "pots.h"

void main(void)
{
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1=CALBC1_16MHZ;
    BCSCTL2 = DIVS_3;
    DCOCTL=CALDCO_16MHZ;
    __bis_SR_register(GIE);

    init_pots();
    while (1) {
        get_pots();
    }
}




