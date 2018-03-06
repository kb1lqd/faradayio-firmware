/*
 * uarttorfbytebridge.c
 *
 *  Created on: Feb 26, 2018
 *      Author: KB1LQ
 */



#include "uarttorfbytebridge.h"
#include "fifo.h"
#include "radio.h"

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

volatile unsigned char uartrxbuffer[253];
volatile unsigned char rfrxbuffer[253];
volatile unsigned char bufferbyte;



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
    if(!uarttobytebridgetimeoutflag){
        StartByteBridgeTimeoutTimer();
    }
    else{
        //__no_operation();
    }
}

void bridgeRfReceiveISR(unsigned char *buffer, unsigned char length){
    unsigned char status;
    unsigned char i;
    for(i=0; i<length; i++){
        status = put_fifo(&uarttorfbridge_rx_state_machine, &uarttorfbridge_rx_fifo_buffer, &buffer[i]);
        if(!uarttobytebridgetimeoutflag){
            StartByteBridgeTimeoutTimer();
        }
        else{
            //__no_operation();
        }
    }
    __no_operation();
}

void uarttorfbridgemainloop(void){
    //UART to RF Main Loop
    if((uarttorfbridge_state_machine.inwaiting>=253) | uarttobytebridgetimeoutflag ){
        unsigned char status;
        unsigned i;


        //If larger than 253 bytes to then fragment
        if(uarttorfbridge_state_machine.inwaiting>253){
            __no_operation();
            for(i=0; i<253; i++){
                get_fifo(&uarttorfbridge_state_machine, &uarttorfbridge_fifo_buffer, &uartrxbuffer[i]);
                __no_operation();
            }
            __no_operation();
        }
        //Smaller than 253 bytes, grab all bytes for RF transmission
        else{
            unsigned char bytestoreceive;
            bytestoreceive = uarttorfbridge_state_machine.inwaiting;
            for(i=0; i<bytestoreceive; i++){
                get_fifo(&uarttorfbridge_state_machine, &uarttorfbridge_fifo_buffer, &uartrxbuffer[i]);
                    __no_operation();
            }
            __no_operation();
        }

        //Send packet over RF
        TransmitData(uartrxbuffer, 253 ); // I am worried that multiple packets from UART fast will overwrite the global buffer?

        if(uarttorfbridge_state_machine.inwaiting){
            uarttobytebridgetimeoutflag= 0;
            StartByteBridgeTimeoutTimer();
        }
        else{
            StopByteBridgeTimeoutTimer();
            uarttobytebridgetimeoutflag = 0;
        }
    }

    //RF to UART Main Loop
        if((uarttorfbridge_rx_state_machine.inwaiting>=253) | uarttobytebridgetimeoutflag ){
            unsigned char status;
            unsigned i;


            //If larger than 253 bytes to then fragment
            if(uarttorfbridge_rx_state_machine.inwaiting>253){
                __no_operation();
                for(i=0; i<253; i++){
                    get_fifo(&uarttorfbridge_rx_state_machine, &uarttorfbridge_rx_fifo_buffer, &rfrxbuffer[i]);
                    __no_operation();
                }
                __no_operation();
            }
            //Smaller than 253 bytes, grab all bytes for RF transmission
            else{
                unsigned char bytestoreceive;
                bytestoreceive = uarttorfbridge_rx_state_machine.inwaiting;
                for(i=0; i<bytestoreceive; i++){
                    get_fifo(&uarttorfbridge_rx_state_machine, &uarttorfbridge_rx_fifo_buffer, &rfrxbuffer[i]);
                        __no_operation();
                }
                __no_operation();
            }

            //Send packet over RF
            //TransmitData(rfrxbuffer, 253 ); // I am worried that multiple packets from UART fast will overwrite the global buffer?

            for(i=0; i<253; i++){
                uarttransmitbyte(rfrxbuffer[i]);
            }
            if(uarttorfbridge_state_machine.inwaiting){
                uarttobytebridgetimeoutflag= 0;
                StartByteBridgeTimeoutTimer();
            }
            else{
                StopByteBridgeTimeoutTimer();
                uarttobytebridgetimeoutflag = 0;
            }
        }

            __no_operation();
}


