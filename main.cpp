#include <msp430.h>
#include "rf24/RF24.h"
#include <math.h>

extern "C"
{
#include "rf24/spi.h"
#include "conio/conio.h"
#include "serial/serial.h"
#include "msp430/timer_msp.h"
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT~
	BCSCTL1 = CALBC1_1MHZ;            // Set DCO to 1MHz
	DCOCTL = CALDCO_1MHZ;

	default_timer();

	__bis_SR_register(GIE);       // Enter LPM0, interrupts enabled

	serial_init(9600);

	RF24 radio = RF24();
	// Radio pipe addresses for the 2 nodes to communicate.
	const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

	// The debug-friendly names of those roles
	const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

	// The various roles supported by this sketch
	typedef enum { role_ping_out = 1, role_pong_back } role_e;

	// The role of the current running sketch
	role_e role;
	role = role_ping_out;

	cio_printf("\n\rRF24/examples/pingpair/\n\r");
	cio_printf("ROLE: %s\n\r",role_friendly_name[role]);

	//
	// Setup and configure rf radio
	//
	radio.begin();

	// optionally, increase the delay between retries & # of retries
	radio.setRetries(15,15);

	// optionally, reduce the payload size.  seems to
	// improve reliability
	radio.setPayloadSize(8);

	radio.setDataRate(RF24_2MBPS);

	//
	// Open pipes to other nodes for communication
	//

	// This simple sketch opens two pipes for these two nodes to communicate
	// back and forth.
	// Open 'our' pipe for writing
	// Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

	if ( role == role_ping_out )
	{
		radio.openWritingPipe(pipes[0]);
		radio.openReadingPipe(1,pipes[1]);
	}
	else
	{
		radio.openWritingPipe(pipes[1]);
		radio.openReadingPipe(1,pipes[0]);
	}

	//
	// Start listening
	//

	radio.startListening();

	//
	// Dump the configuration of the rf unit for debugging
	//

	radio.printDetails();
	P1DIR |= BIT0;

	while (1)
	{
		//
		// Ping out role.  Repeatedly send the current time
		//

		if (role == role_ping_out)
		{
			// First, stop listening so we can talk.
			radio.stopListening();

			// Take the time, and send it.  This will block until complete
			unsigned long time = millis();
			cio_printf("Now sending %u...",time);
			bool ok = radio.write( &time, sizeof(unsigned long) );

			if (ok)
				cio_printf("ok...");
			else
				cio_printf("failed.\n\r");

			// Now, continue listening
			radio.startListening();

			// Wait here until we get a response, or timeout (250ms)
			unsigned long started_waiting_at = millis();
			bool timeout = false;
			while ( ! radio.available() && ! timeout )
				if (millis() - started_waiting_at > 200 )
					timeout = true;

			// Describe the results
			if ( timeout )
			{
				cio_printf("Failed, response timed out.\n\r");
			}
			else
			{
				// Grab the response, compare, and send to debugging spew
				unsigned long got_time;
				radio.read( &got_time, sizeof(unsigned long) );

				// Spew it
				cio_printf("Got response %u, round-trip delay: %u\n\r",got_time,millis()-got_time);
			}

			// Try again 1s later
			delay(1000);
			P1OUT ^= BIT0;
		}

		//
		// Pong back role.  Receive each packet, dump it out, and send it back
		//

		if ( role == role_pong_back )
		{
			// if there is data ready
			if ( radio.available() )
			{
				// Dump the payloads until we've gotten everything
				unsigned long got_time;
				bool done = false;
				while (!done)
				{
					// Fetch the payload, and see if this was the last one.
					done = radio.read( &got_time, sizeof(unsigned long) );

					// Spew it
					cio_printf("Got payload %u...",got_time);

					// Delay just a little bit to let the other unit
					// make the transition to receiver
					delay(20);
				}

				// First, stop listening so we can talk
				radio.stopListening();

				// Send the final one back.
				radio.write( &got_time, sizeof(unsigned long) );
				cio_printf("Sent response.\n\r");

				// Now, resume listening so we catch the next packets.
				radio.startListening();
			}
		}

	}

}
