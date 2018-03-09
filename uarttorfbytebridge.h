/*
 * uarttorfbytebridge.h
 *
 *  Created on: Feb 26, 2018
 *      Author: KB1LQ
 */

#ifndef UARTTORFBYTEBRIDGE_UARTTORFBYTEBRIDGE_H_
#define UARTTORFBYTEBRIDGE_UARTTORFBYTEBRIDGE_H_


/**
 * Transmit buffer FIFO size in bytes.
 */
#define TRANSMIT_BUFFER_SIZE 256

/**
 * FIFO Size for improved IP packet passthrough
 */
#define FIFO_MIN_MTU (1400)

unsigned char uarttobytebridgetimeoutflag;

void initbridgefifo(void);
void bridgeUartReceiveISR(unsigned char rxbyte);
void bridgeRfReceiveISR(unsigned char *buffer, unsigned char length);
void uarttorfbridgemainloop(void);

#endif /* UARTTORFBYTEBRIDGE_UARTTORFBYTEBRIDGE_H_ */
