/*
 * buttons.c
 *
 */

#include <msp430g2553.h>
#include <stdint.h>
#include "buttons.h"

#define WAITING 1
#define NOT_WAITING 0
#define DEBOUNCE_CNTS 1000

static int button_state = NOT_WAITING;

void init_buttons()
{
    /*
     * P 2.5 is the left button.
     * P 2.4 is the right button.
     * Both buttons are active low.
     * We therefore trigger on all falling edges.
     */
    right = 0;
    left = 0;
    P2DIR &= ~(BIT4 | BIT5); // Clear only bits 4 and 5. (Input mode)
    P2REN |= (BIT4 | BIT5); // Enable pull up/down resistors.
    P2IES |= (BIT4 | BIT5); // Falling edge interrupts.
    P2IE |= (BIT4 | BIT5); // Enable interrupts.
    P2IFG &= ~(BIT4 | BIT5); // Clear interrupt bits.
}

void wait_for_button_press()
{
    while(!(P2IN & BIT5) || !(P2IN & BIT4));
    __delay_cycles(100000);
    button_state = WAITING; // Set waiting flag.
    // Enter LPM until ISR wakes and clears waiting flag.
    __bis_SR_register(LPM0_bits + GIE);
}

void wait_for_select_press()
{
    while(!(P2IN & BIT5) || !(P2IN & BIT4));
    __delay_cycles(100000);
    button_state = NOT_WAITING; // Set waiting flag.
    __bis_SR_register(LPM0_bits + GIE);
}

inline void wait_for_bounce()
{
    volatile uint16_t reading;
    volatile uint16_t counts = 0;
    volatile uint16_t prev_state;

    while (1) {
        if (counts >= DEBOUNCE_CNTS)
            return;

        reading = P2IN & (BIT4 | BIT5);

        if (reading == prev_state)
            counts++;
        else if (reading != prev_state)
            counts = 0;

        prev_state = reading;
    }
}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
    wait_for_bounce();
    if(button_state == WAITING) {
        button_state = NOT_WAITING;

    }
    if (P2IFG & BIT4) {
        right = 1;
        P2IFG &= ~BIT4;
    }
    if (P2IFG & BIT5) {
        left = 1;
        P2IFG &= ~BIT5;
    }
    P2IFG = 0;
    __bic_SR_register_on_exit(LPM0_bits);
}
