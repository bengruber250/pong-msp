/*
 * pong.c
 *
 *  Created on: Apr 28, 2019
 *      Author: root
 */
int main()
{
    init_display();
    init_pots();
    init_buttons();
    while (1) {
       init_game();
       display_start();
       while(!game_over()){
           play_pong_round();
           // Maybe clear display here.
       }
    }
}

void init_display() {

}



