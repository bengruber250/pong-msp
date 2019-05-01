# ELEC 327 S19 
## pong-msp
## Welcome to our MSP430 Pong Github page.
Here you'll find all of our source code, design files, and report files for our little game.
Below is an excerpt of our report. Enjoy!

## Motivation 
Throughout the semester we've b een exploring the ins and outs of designing and implementing emb edded systems. We've written several drivers for the MSP430's m o d u les an d integrated th em w ith software logic to m ake interestin g d ev ices that that interact with the real world while meeting tight timing considerations. T h is sem ester, th e cu lm in ation of all of ou r lab s was th e S im on p ro ject. We put together all of our co de from the sem ester and w rote com plicated logic to implement a fun game! This was the source of our motivation to build another gam e. We enjoyed writing the Sim on software and wanted to dive deep er into gam es ru n n in g on emb ed d ed sy stem s. A n oth er d riv in g force was th e goal of integrating a graphic display panel into our design. We explored several ideas and nally decided on making a Pong replica

## System Specification
The Pong game is the arcade classic with two paddles on either end of the screen controlled by users and a ball that is b ounced b etween the paddles and the top and b ottom of the screen. If the ball makes it to either end of the screen, a p oint is scored. The paddles were originally controlled with knobs. Since we wanted to make a close replica of the game we ended up cho osing p otentiom eters for p ad d le control, a b lack an d w h ite O L E D p an el (to get a CRT-esque feeling), and a couple of buttons for basic interaction. For software th is m eant th at we n eed ed to im p lem ent d rivers for G P IO , ADC, and the screen. The game logic required that we implement (but not limit ourselves to) collision checking, b ouncing, scoring, ball movement, and detecting winners

### Mau (smg10) and Ben (bpg1)
