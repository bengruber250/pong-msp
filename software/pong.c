/*
 * pong.c
 *
 */

static int ball_x;
static int ball_y;
static int ball_vx;
static int ball_vy;


static int tick()
{
    // Check for collisions and update ball velocities.
    if (check_vertical_collisions()) {
        // Update ball vertical velocity.
    }
    if (side = check_horizontal_collisions()) {
        if (ball_in_paddle_range(side)) {
            // Bounce and impart velocity of paddle.
        }
        // return LEFT/RIGHT.
    }

    update_paddle_positions();
    update_ball_position(); //(Maybe every nth tick so paddle can move faster than ball?)

    clear_display();
    draw_paddles();
    draw_ball();
    return 0;

}

int play_pong_round()
{
    int winner;
    while(!(winner = tick()));
    return winner;
}
