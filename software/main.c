/*
 * main.c
 *
 */
#include <msp430g2553.h>
#include "buttons.h"
#include "lib_lcd.h"
#include "pong.h"
#include "pots.h"
#include "strings.h"



static int score_left = 0;
static int score_right = 0;

static void display_score();
static void init_display();
static void init_game();
static void display_start();
static void display_game_over(int winner);
static int check_game_over();
static void display_score();

int main()
{
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1=CALBC1_16MHZ;
//    BCSCTL2 |= DIVS_3;
    DCOCTL=CALDCO_16MHZ;

    int winner;

    init_display();
    init_pots();
    init_buttons();

    while (1) {
       init_game();
       display_start();
       wait_for_button_press();

       while(!(winner = check_game_over())){
           play_pong_round();
           display_score();
           wait_for_button_press();
       }
       display_game_over(winner);
    }
}

/*
 * Initializes the display.
 */
static void init_display()
{
    init_ports();
    init_USCI();    // 4-wire SPI.
    init_LCD();
}

/*
 * Initializes the game state.
 */
static void init_game()
{
    score_left = 0;
    score_right = 0;
}

/*
 * Displays a starting screen.
 */
static void display_start()
{
    fill_display(lcd_width,lcd_height,0x00); // Clear display.
    write_string(2,3,str_title,2); // Write title.
    write_string(2,6,str_subtitle,1); // Write subtitle.
}

/*
 * winner must be either LEFT or RIGHT.
 * Displays a message to celebrate the winner.
 */
static void display_game_over(int winner)
{
    char *to_display;
    if (winner == LEFT)
        to_display = str_left_winner;
    else if (winner == RIGHT)
        to_display = str_right_winner;

    fill_display(lcd_width,lcd_height,0x00); // Clear display.
    write_string(2,3,to_display,1); // Write winner string.
}

/*
 * Returns 0 if the game is not over,
 * LEFT if the left player has won, and
 * RIGHT if the right player has won.
 */
static int check_game_over()
{
    if (score_left == WINNING_SCORE)
        return LEFT;
    else if (score_right == WINNING_SCORE)
        return RIGHT;
    else
        return 0;
}

/*
 * Displays the current score.
 * Can only display one digit scores.
 */
static void display_score()
{
    static char score_str[] = "0-0";

    score_str[0] = score_left + '0';
    score_str[2] = score_right + '0';

    fill_display(lcd_width,lcd_height,0x00); // Clear display.
    write_string(2,3,score_str,3); // Write title.
}



