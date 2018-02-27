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

void initbridgefifo(void);
void bridgeUartReceiveISR(unsigned char rxbyte);
void bridgeRfReceiveISR(unsigned char *buffer, unsigned char length);
void uarttorfbridgemainloop(void);
void enableRxTimer(void);
unsigned char checkRxTimerEnabled(void);
void disableRxTimer(void);
void incrementRxTimerCount(void);
unsigned char checkRxTimerCount(void);
void resetRxTimerCount(void);

#endif /* UARTTORFBYTEBRIDGE_UARTTORFBYTEBRIDGE_H_ */
