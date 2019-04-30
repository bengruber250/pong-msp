/*
 * buttons.h
 *
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

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



#endif /* BUTTONS_H_ */
