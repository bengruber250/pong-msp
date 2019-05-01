/*
 * pong.c
 *
 */

#include "pong.h"
#include "pots.h"
#include "lib_lcd.h"
#include "erandom.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

static int ai_enable = 0;
// From center of screen towards left, how far away can the AI see the ball
static int ai_fov = 37;
// how old is the ball data that the ai has access to
static int ai_lag = 0;
// When AI can see ball, how fast can it move (number of times to divide movement adjustment by 2)
static int ai_diff = 1;
// From right, region in which the AI exactly moves to match it's last known ball data.
static int ai_ultrainstinct = 20;

static int ball_x;
static int ball_y;
static int ball_vx;
static int ball_vy;

static int paddle_left_y;
static int paddle_right_y;

static int check_vertical_collisions();
static int check_horizontal_collisions();
static int ball_in_paddle_range(int side);
static unsigned int map(unsigned int x, unsigned int in_min, unsigned int in_max, unsigned int out_min, unsigned int out_max);
static void update_paddle_positions();
static void update_ball_position();
static void clear_display();
static void draw_paddles();
static void draw_ball(int in_range);
static void draw_ball_and_paddles(int in_range);

int tickno = 0;
int ball_yold;
static int tick()
{
    int side;
    int in_range = 0;
    // Check for collisions and update ball velocities.
    update_paddle_positions();
    update_ball_position(); //(Maybe every nth tick so paddle can move faster than ball?)
    if (check_vertical_collisions()) {
        // Update ball vertical velocity
        // Don't forget to redraw things.
        ball_vy = -ball_vy;
    }
    if (side = check_horizontal_collisions()) {
        if (in_range = ball_in_paddle_range(side)) {
            // Bounce and impart velocity of paddle.
            // Don't forget to redraw things.
            ball_vx = -ball_vx;
            ball_vy = ball_vy + (int)rand();
        } else {
            clear_display();
            draw_ball_and_paddles(in_range); //Both visible even if clipping.
            return side;
        }
    }


    clear_display();
    draw_paddles();
    draw_ball(in_range);

    if ((tickno++) == ai_lag) {
        ball_yold = ball_y;
        tickno = 0;
    }

    return 0;

}

/*
 * Returns 0 if no vertical collision and 1 otherwise.
 */
static int check_vertical_collisions()
{
    if (ball_y == 0 || ball_y == (LCD_HEIGHT - 1))
        return 1;
    else
        return 0;
}

int play_pong_round(int prev_loser)
{
    /* Right loses then left serves. */
    ball_y = 30;
    ball_vy = 2;
    if (prev_loser == RIGHT) {
        ball_x = 7;
        ball_vx = 4;
    } else {
        ball_x = lcd_width - 7;
        ball_vx = -4;
    }
    int loser;
    while(!(loser = tick())) {
        __delay_cycles(500000);
    }
    return loser;
}

void pong_init_ai(int ai_difficulty)
{
    ai_enable = 1;
    switch (ai_difficulty) {
    case 5:
        // init vals are highest diff.
        break;
    case 4:
        ai_ultrainstinct = 4;
        break;
    case 3:
        // THIS IS FINE
        ai_ultrainstinct = 0;
        ai_fov = 30;
        ai_lag = 1;
        ai_diff = 1;
        break;
    case 2:
        ai_ultrainstinct = 0;
        ai_fov = 27;
        ai_lag = 2;
        ai_diff = 1;
        break;
    case 1:
        ai_ultrainstinct = 1;
        ai_fov = 30;
        ai_lag = 4;
        ai_diff = 1;
        break;

    }
}

/*
 * Returns LEFT if left horizontal collision,
 * RIGHT if right, and 0 else.
 */
static int check_horizontal_collisions()
{
    if (ball_x == 0)
        return LEFT;
    else if (ball_x == LCD_WIDTH - 1)
        return RIGHT;
    else
        return 0;
}

static int ball_in_paddle_range(int side)
{
    int hit_margin = 1;
    if (side == LEFT) {
        return ((ball_y >= (paddle_left_y - hit_margin)) && (ball_y <= (paddle_left_y + PADDLE_HEIGHT + hit_margin)));
    }
    else if (side == RIGHT) {
        return ((ball_y >= (paddle_right_y - hit_margin)) && (ball_y <= (paddle_right_y + PADDLE_HEIGHT + hit_margin)));
    }
    return -1;
}

static void update_paddle_positions()
{
    static int ai_mov = 0;
    get_pots();
    paddle_left_y = map(constrain(pot_vals[1], 0, 1020), 0, 1020, 0, LCD_HEIGHT - PADDLE_HEIGHT );
    if (ai_enable) {
        if (ball_x > LCD_WIDTH - ai_ultrainstinct) {
            ai_mov = ball_yold - (paddle_right_y + (PADDLE_HEIGHT >> 1));
            paddle_right_y = constrain(paddle_right_y + ai_mov, 0, LCD_HEIGHT - PADDLE_HEIGHT);
        } else if (ball_x > ((LCD_WIDTH / 2) - ai_fov)) {
            ai_mov = ball_yold - (paddle_right_y + (PADDLE_HEIGHT >> 1));
            ai_mov >>= ai_diff;
            paddle_right_y = constrain(paddle_right_y + ai_mov, 0, LCD_HEIGHT - PADDLE_HEIGHT);
        }
    } else {
        paddle_right_y = map(constrain(pot_vals[0], 0, 1020), 0, 1020, 0, LCD_HEIGHT - PADDLE_HEIGHT );
    }
    return;
}

static void update_ball_position()
{

    ball_x = constrain(ball_x + ball_vx, 0, LCD_WIDTH - 1);
    ball_y = constrain(ball_y + ball_vy, 0, LCD_HEIGHT - 1);
}

static void clear_display()
{
    fill_display(lcd_width,lcd_height,0x00);
}

static void draw_paddles()
{
    draw_rect(0, paddle_left_y, 1, PADDLE_HEIGHT);
    draw_rect(LCD_WIDTH - 1, paddle_right_y, 1, PADDLE_HEIGHT);
}

static void draw_ball(int in_range)
{
    if (in_range)
        return;
    draw_rect(ball_x, ball_y, BALL_SIZE, BALL_SIZE);
}

static void draw_ball_and_paddles(int in_range)
{
    int i = 10000;
    while(--i) {
        if (i & 1) {
            draw_paddles();
            draw_ball(in_range);
        } else {
            draw_ball(in_range);
            draw_paddles();
        }

    }
}


static unsigned int map(unsigned int x, unsigned int in_min, unsigned int in_max, unsigned int out_min, unsigned int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
