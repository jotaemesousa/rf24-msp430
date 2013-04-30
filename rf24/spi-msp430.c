/*
 * spi.c
 *
 *  Created on: Apr 21, 2013
 *      Author: bgouveia
 */

#include <msp430.h>

#include "spi.h"


//#define CS_PIN_BASE P1OUT
#define CS_PIN 		BIT4
#define SIMO_PIN    BIT7
#define SOMI_PIN    BIT6
#define SCLK_PIN    BIT5

//#define CE_PIN_BASE P1OUT
#define CE_PIN 		BIT0
#define CE_PORT		P2OUT
#define CE_DIR		P2DIR



void spi_init(unsigned long bitrate,unsigned long datawidth){
	//SSI


	UCB0CTL1 = UCSWRST;

    P1DIR  |= CS_PIN;
    P1OUT  |= CS_PIN;
    CE_DIR |= CE_PIN;
    CE_PORT|= CE_PIN;


  	P1SEL  |= SOMI_PIN + SIMO_PIN + SCLK_PIN;
  	P1SEL2 |= SOMI_PIN + SIMO_PIN + SCLK_PIN;

    // 3-pin, 8-bit SPI master
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
	UCB0CTL1 |= UCSSEL_2;   // SMCLK

	UCB0CTL1 &= ~UCSWRST;


}

void spi_cs_low()
{
	P1OUT &= ~CS_PIN;

}

void spi_cs_high()
{
	P1OUT |= CS_PIN;
}

void spi_ce_low()
{
	CE_PORT &= ~CE_PIN;

}

void spi_ce_high()
{
	CE_PORT |= CE_PIN;
}


uint8_t spi_transferByte(uint8_t data)
{

	    UCB0TXBUF = data;

		// wait for TX
		while (!(IFG2 & UCB0TXIFG));

		return UCB0RXBUF;

}

/*void delay(unsigned long msec)
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
}*/
