/*
 * pong.h
 *
 */

#ifndef PONG_H_
#define PONG_H_

#define LEFT 1
#define RIGHT 2
#define WINNING_SCORE 9
#define BALL_SIZE 1

#define PADDLE_HEIGHT 10
#define LCD_HEIGHT 64
#define LCD_WIDTH 128

int play_pong_round(int prev_winner);
void pong_init_ai(int ai_difficulty);



#endif /* PONG_H_ */
