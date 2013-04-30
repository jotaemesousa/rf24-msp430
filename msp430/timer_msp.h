/*
 * timer_msp.h
 *
 *  Created on: 30 de Dez de 2012
 *      Author: Asus
 */

#ifndef TIMER_MSP_H_
#define TIMER_MSP_H_

struct timer_msp
{
	unsigned long int ms;
	unsigned long int s;
};

void default_timer(void);
unsigned long int millis(void);
#ifndef DELAY_MSP430
void delay(unsigned long msec);
void delayMicroseconds(unsigned long usec);
#endif

#endif /* TIMER_MSP_H_ */
