# ELEC 327 S19 
## pong-msp
## Welcome to our MSP430 Pong Github page.
Here you'll find all of our source code, design files, and report files for our little game.
Below are two videos on our project and an excerpt of our report. Enjoy!

## Videos

#### Demonstration
https://www.youtube.com/watch?v=Z2DToJ7af1M

#### Explanation
https://www.youtube.com/embed/VBxOD-HLzUo



## Motivation 
Throughout the semester we've b een exploring the ins and outs of designing and implementing emb edded systems. We've written several drivers for the MSP430's modules and integrated them with software logic to make interesting devices that interact with the real world while meeting tight timing considerations. This semester, the culmination of all of our labs was the Simon project. We put together all of our code from the semester and wrote complicated logic to implement a fun game! This was the source of our motivation to build another game. We enjoyed writing the Simon software and wanted to dive deeper into games running on embedded sy stems. An other driving force was the goal of integrating a graphic display panel into our design. We explored several ideas and finally decided on making a Pong replica.

## System Specification
The Pong game is the arcade classic with two paddles on either end of the screen controlled by users and a ball that is bounced between the paddles and the top and bottom of the screen. If the ball makes it to either end of the screen, a point is scored. The paddles were originally controlled with knobs. Since we wanted to make a close replica of the game we ended up choosing potentiometers for paddle control, a black and white OLED panel (to get a CRT-esque feeling), and a couple of buttons for basic interaction. For software this means that we need ed to implement drivers for GPIO , ADC, and the screen. The game logic required that we implement (but not limit ourselves to) collision checking, bouncing, scoring, ball movement, and detecting the winner of each round.

### Mau (smg10) and Ben (bpg1)
