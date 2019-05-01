/*
 * erandom.h
 *
 *  Created on: Mar 25, 2019
 *      Author: pugsley
 */

#ifndef ERANDOM_H_
#define ERANDOM_H_

//#define RAND_MAX 1
#define RAND_MAX 6
//#define RAND_MAX 7
//#define RAND_MAX 15
//#define RAND_MAX 31
//#define RAND_MAX 63
//#define RAND_MAX 127
//#define RAND_MAX 255
//#define RAND_MAX 32767
//#define RAND_MAX 65536

void init_rand_adc_seed(void);
void srand(int seed);
inline unsigned int rand(void);


#endif /* ERANDOM_H_ */
