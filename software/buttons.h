/*
 * buttons.h
 *
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

int left;
int right;

/*
 * Initializes buttons for pong.
 */
void init_buttons();
/*
 * Waits until a button is pressed.
 */
void wait_for_button_press();
/*
 * Debounces the port
 */
inline void wait_for_bounce();

/*
 * Waits until a button is pressed, sets global vars for individual buttons.
 */
void wait_for_select_press();



#endif /* BUTTONS_H_ */
