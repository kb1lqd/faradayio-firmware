/*
 * spi.c
 *
 *  Created on: Feb 16, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "spi.h"

unsigned char spi_transmit_byte(transmitData){
    //USCI_A0 TX buffer ready?
    while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
               USCI_B_SPI_TRANSMIT_INTERRUPT)) ;

    //Transmit Data to slave
    USCI_B_SPI_transmitData(USCI_B0_BASE, transmitData);
}
