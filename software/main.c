/*
 * pong.c
 *
 *  Created on: Apr 28, 2019
 *      Author: root
 */
#include <msp430g2553.h>
#include "lib_lcd.h"
#include "strings.h"

#define LEFT 1
#define RIGHT 2


static int score_left = 0;
static int score_right = 0;

int main()
{
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1=CALBC1_16MHZ;
    DCOCTL=CALDCO_16MHZ;

    int winner;

    init_display();
    init_pots();
    init_buttons();

    while (1) {
       init_game();
       display_start();
       while(!(winner = check_game_over())){
           play_pong_round();
           display_score();
       }
       display_game_over(winner);
    }
}

/*
 * Initializes the display.
 */
void init_display()
{
    init_ports();
    init_USCI();    // 4-wire SPI.
    init_LCD();
}

/*
 * Initializes the potentiometers.
 */
void init_pots()
{

}

/*
 * Initializes the buttons.
 */
void init_buttons()
{

}

/*
 * Initializes the game state.
 */
void init_game()
{
    score_left = 0;
    score_right = 0;
}

/*
 * Displays a starting screen.
 */
void display_start()
{
    fill_display(lcd_width,lcd_height,0x00); // Clear display.
    write_string(2,3,str_title,2); // Write title.
    write_string(2,6,str_subtitle,1); // Write subtitle.
}

/*
 * winner must be either LEFT or RIGHT.
 * Displays a message to celebrate the winner.
 */
void display_game_over(int winner)
{

}

/*
 * Returns 0 if the game is not over,
 * LEFT if the left player has won, and
 * RIGHT if the right player has won.
 */
int check_game_over()
{

}

/*
 * Plays a single round of pong, incrementing either
 * score_left or score_right depending on the winner.
 */
void play_pong_round()
{

}

/*
 * Displays the current score.
 */
void display_score()
{

}



