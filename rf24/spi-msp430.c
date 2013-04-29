/*
 * spi.c
 *
 *  Created on: Apr 21, 2013
 *      Author: bgouveia
 */

#include <msp430.h>

#include "spi.h"

static unsigned long ulClockMS=0;

void spi_init(unsigned long bitrate,unsigned long datawidth){
	//SSI

	P1OUT  |= CS_PIN;
	P1DIR  |= CS_PIN;


	P2SEL = 0x00;  // Make sure CSn works instead of crystal

	UCB0CTL1 |= UCSWRST;                      // **Disable USCI state machine**
	UCB0CTL0 |= UCMST+UCCKPH+UCMSB+UCSYNC;    // 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2;                     // SMCLK
	UCB0BR0 = 0x02;                           // UCLK/2
	UCB0BR1 = 0;


	P1SEL  |= SOMI_PIN + SIMO_PIN + SCLK_PIN;
	P1SEL2 |= SOMI_PIN + SIMO_PIN + SCLK_PIN;

	P1DIR |= SIMO_PIN | SCLK_PIN;
	                                            // SPI TXD out direction
	UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

	CE_DIR |= CE_PIN;
	spi_cs_high();
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
