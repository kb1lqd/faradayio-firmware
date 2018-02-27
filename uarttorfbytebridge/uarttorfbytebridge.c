/*
 * uarttorfbytebridge.c
 *
 *  Created on: Feb 26, 2018
 *      Author: KB1LQ
 */


#include "driverlib.h"
#include "uarttorfbytebridge.h"
#include "../fifo.h"
#include "../radio.h"

/** @name UART to RF Bridge FIFO Variables
*
*   Variables used during the UART to RF Bridge functionality.
*
*
@{**/
volatile fifo_state_machine uarttorfbridge_state_machine; /**< Structure for the self-test FIFO state machine */
volatile unsigned char uarttorfbridge_fifo_buffer[TRANSMIT_BUFFER_SIZE]; /**< Self-Test FIFO buffer */
volatile fifo_state_machine uarttorfbridge_rx_state_machine; /**< Structure for the self-test FIFO state machine */
volatile unsigned char uarttorfbridge_rx_fifo_buffer[TRANSMIT_BUFFER_SIZE]; /**< Self-Test FIFO buffer */
/** @}*/

volatile unsigned char uartrxbuffer[255];
volatile unsigned char bufferbyte;

unsigned char rxtimerenabled;
unsigned char rxtimercnt;

void initbridgefifo(void){
    //Application FIFO
    uarttorfbridge_state_machine.debug = 0;
    uarttorfbridge_state_machine.element_size = 1;
    uarttorfbridge_state_machine.head = 0;
    uarttorfbridge_state_machine.inwaiting = 0;
    //rf_datalink_tx_fifo_state_machine.length = 0;
    uarttorfbridge_state_machine.max_inwait = 0;
    uarttorfbridge_state_machine.tail = 0;
    uarttorfbridge_state_machine.buffer_size = TRANSMIT_BUFFER_SIZE;

    uarttorfbridge_rx_state_machine.debug = 0;
    uarttorfbridge_rx_state_machine.element_size = 1;
    uarttorfbridge_rx_state_machine.head = 0;
    uarttorfbridge_rx_state_machine.inwaiting = 0;
    //rf_datalink_tx_fifo_state_machine.length = 0;
    uarttorfbridge_rx_state_machine.max_inwait = 0;
    uarttorfbridge_rx_state_machine.tail = 0;
    uarttorfbridge_rx_state_machine.buffer_size = TRANSMIT_BUFFER_SIZE;
}

void bridgeUartReceiveISR(unsigned char rxbyte){
    unsigned char status;
    status = put_fifo(&uarttorfbridge_state_machine, &uarttorfbridge_fifo_buffer, &rxbyte);
}

void bridgeRfReceiveISR(unsigned char *buffer, unsigned char length){
    unsigned char status;
    unsigned char i;
    for(i=0; i<length; i++){
        status = put_fifo(&uarttorfbridge_rx_state_machine, &uarttorfbridge_rx_fifo_buffer, &buffer[i]);
    }
    __no_operation();
}

void uarttorfbridgemainloop(void){
    if(uarttorfbridge_state_machine.inwaiting>0){
        unsigned char status;
        unsigned i;

        //If larger than 255 bytes to then fragment
        if(uarttorfbridge_state_machine.inwaiting>255){
            __no_operation();
        }
        //Smaller than 255 bytes, grab all bytes for RF transmission
        else{
            unsigned char bytestoreceive;
            bytestoreceive = uarttorfbridge_state_machine.inwaiting;
            for(i=0; i<bytestoreceive; i++){
                get_fifo(&uarttorfbridge_state_machine, &uarttorfbridge_fifo_buffer, &uartrxbuffer[i]);
                    __no_operation();
            }
            __no_operation();
        }
        //Get packet from queue
        status = get_fifo(&uarttorfbridge_state_machine, &uarttorfbridge_fifo_buffer, bufferbyte);

        //Send packet over RF
        TransmitData(bufferbyte);
    }

    __no_operation();

    //Check rx timer count
    if(checkRxTimerCount()>30){
        __no_operation();
    }
}

void enableRxTimer(void){
    rxtimerenabled = 1;
    rxtimercnt = 0;

    TA0CCR2   = (32768/10);              // x cycles * 1/32768 = y us
    TA0CCTL2 |= CCIE;
    TA0CTL   |= MC_2 + TACLR;                 // Start the timer- continuous mode
}

unsigned char checkRxTimerEnabled(void){
    return rxtimerenabled;
}

void disableRxTimer(void){
    TA0CCTL2 &= ~CCIE;
    rxtimerenabled = 0;
    rxtimercnt = 0;
}

void incrementRxTimerCount(void){
    rxtimercnt++;
}

unsigned char checkRxTimerCount(void){
    return rxtimercnt;
}

void resetRxTimerCount(void){
    rxtimercnt = 0;
}
