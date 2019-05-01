/*
 * main.c
 *
 */
#include <msp430g2553.h>
#include "buttons.h"
#include "lib_lcd.h"
#include "init_SSD1306.h"
#include "pong.h"
#include "pots.h"
#include "strings.h"
#include "erandom.h"



static int score_left = 0;
static int score_right = 0;
static int ai_difficulty = 0;


static void display_score();
static void init_display();
static void init_game();
static void display_start();
static void display_game_over(int winner);
static int check_game_over();
static void display_score();
static void update_score( int winner);
static void display_player_select();
static void display_ai_select(int ai_difficulty);

int main()
{
    WDTCTL = WDT_MDLY_8;
    IE1 |= WDTIE;
    BCSCTL1=CALBC1_16MHZ;
//    BCSCTL2 |= DIVS_3;
    DCOCTL=CALDCO_16MHZ;

    int winner;
    init_rand_adc_seed();
    ADC10CTL0 = 0x00;
    init_display();
    init_pots();
    init_buttons();


    while (1) {
       init_game();
       display_start();
       wait_for_button_press();
       display_player_select();
       wait_for_select_press();
       if (left) {
           left = 0;
           while (1) {
               display_ai_select(ai_difficulty);
               wait_for_select_press();
               if (left) {
                   left = 0;
                   ai_difficulty++;
               } else if (right) {
                   right = 0;
                   break;
               }
               if (ai_difficulty > 5)
                   ai_difficulty = 0;
           }
           pong_init_ai(ai_difficulty);
       }
       right = 0;
       int prev_loser = RIGHT;
       while(!(winner = check_game_over())) {
           int loser = play_pong_round(prev_loser);
           update_score(loser);
           display_score();
           wait_for_button_press();
           prev_loser = loser;
       }
       display_game_over(winner);
       wait_for_button_press();
       __delay_cycles(1000000);
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
    fill_display(lcd_width, lcd_height, 0x00); // Clear display.
    write_string(1, 2, str_title, 2); // Write title.
    write_small_string(1, 6, str_subtitle, 0); // Write subtitle.
    write_small_string(1, 7, str_subtitle2, 0); // Write subtitle.
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
//    write_string(2,3,to_display,1); // Write winner string.
    write_string(2, 3, to_display, 2);
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
    static char score_str[] = "0   --   0";

    score_str[0] = score_left + '0';
    score_str[9] = score_right + '0';

    fill_display(lcd_width,lcd_height,0x00); // Clear display.
    write_string(2,3,score_str,2); // Write score.

    write_small_string(1, 6, str_subtitle, 0); // pabt.
    write_small_string(1, 7, str_continue, 0); // cont.
}

static void update_score(int loser)
{
    if (loser == LEFT)
        score_right++;
    else if (loser == RIGHT)
        score_left++;
}

static void display_player_select()
{
    fill_display(lcd_width, lcd_height, 0x00); // Clear display.
    write_small_string(1, 2, str_select, 0); // Write subtitle.
    write_small_string(1, 3, str_select2, 0); // Write subtitle.
    write_small_string(1, 5, str_select3, 0); // Write subtitle.
}

static void display_ai_select(int ai_difficulty)
{
    char diff_str[] = "Difficulty: 0";
    diff_str[12] = ai_difficulty + '0';
    fill_display(lcd_width, lcd_height, 0x00); // Clear display.
    write_small_string(1, 2, str_selectai, 0); // Write subtitle.
    write_small_string(1, 3, str_selectai2, 0); // Write subtitle.
    write_small_string(1, 4, str_selectai3, 0); // Write subtitle.
    write_small_string(1, 6, diff_str, 0); // Write subtitle.
}

// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    /* Constantly shuffle the lfsr */
    rand();
}



