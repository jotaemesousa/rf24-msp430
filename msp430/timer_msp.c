/*
 * timer_msp.c
 *
 *  Created on: 30 de Dez de 2012
 *      Author: Asus
 */

#include "timer_msp.h"
#include "msp430g2553.h"

struct timer_msp timer0;

void default_timer(void)
{
	timer0.s = 0;
	timer0.ms = 0;
	CCR0 = 1000;
	TACTL = TASSEL_2 + MC_1;
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
}

unsigned long int millis(void)
{
	return(timer0.ms + timer0.s*1000L);
}
#ifndef DELAY_MSP430
void delay(unsigned long msec)
{
	while(msec--)
	{
		__delay_cycles(1000);
	}
}
void delayMicroseconds(unsigned long usec)
{
	while(usec--)
	{
		__delay_cycles(1);
	}
}
#endif

//===========================================================================
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	timer0.ms++;

	if(timer0.ms >= 1000)
	{
		timer0.ms = 0;
		timer0.s++;
	}



}
