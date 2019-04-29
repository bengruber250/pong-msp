/*
 * buttons.c
 *
 */

#include <msp430g2553.h>
#include "buttons.h"

#define WAITING 1
#define NOT_WAITING 0

static int button_state = NOT_WAITING;

void init_buttons()
{
    /*
     * P 2.5 is the left button.
     * P 2.4 is the right button.
     * Both buttons are active low.
     * We therefore trigger on all falling edges.
     */
    P2DIR &= ~(BIT4 | BIT5); // Clear only bits 4 and 5. (Input mode)
    P2REN |= (BIT4 | BIT5); // Enable pull up/down resistors.
    P2IES |= (BIT4 | BIT5); // Falling edge interrupts.
    P2IE |= (BIT4 | BIT5); // Enable interrupts.
    P2IFG &= ~(BIT4 | BIT5); // Clear interrupt bits.
}

void wait_for_button_press()
{
    button_state = WAITING; // Set waiting flag.
    // Enter LPM until ISR wakes and clears waiting flag.
    __bis_SR_register(LPM0_bits + GIE);
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
    if(button_state == WAITING) {
        button_state = NOT_WAITING;
        P2IFG = 0;
        __bic_SR_register_on_exit(LPM0_bits);
    }
    else {
        P2IFG = 0;
    }

}
