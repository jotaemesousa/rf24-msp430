/*
 * spi.h
 *
 *  Created on: Apr 21, 2013
 *      Author: bgouveia
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#ifdef MSP430
//#define CS_PIN_BASE P1OUT
#define CS_PIN 		BIT4

//#define CE_PIN_BASE P1OUT
#define CE_PIN 		BIT0
#define CE_PORT		P2OUT
#define CE_DIR		P2DIR
#define SIMO_PIN    BIT7
#define SOMI_PIN    BIT6
#define SCLK_PIN    BIT5
#endif

void spi_init(unsigned long bitrate,unsigned long datawidth);
uint8_t spi_transferByte(uint8_t data);

void spi_cs_low();
void spi_cs_high();

void spi_ce_low();
void spi_ce_high();


void delay(unsigned long msec);
void delayMicroseconds(unsigned long usec);

#endif /* SPI_H_ */
