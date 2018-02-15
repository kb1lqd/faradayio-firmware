/*
 * sram.c
 *
 *  Created on: Feb 15, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "sram.h"




unsigned char sram_enable_chip_select(void){
    //Set P5.5 for slave reset
    GPIO_setOutputHighOnPin(

        GPIO_PORT_P5,
        GPIO_PIN5
        );

    return 0;
}

unsigned char sram_disable_chip_select(void){
    //Set P5.5 for slave reset
    GPIO_setOutputLowOnPin(

        GPIO_PORT_P5,
        GPIO_PIN5
        );

    return 0;
}

unsigned char sram_Toggle_CS(void){
    //This function simply turns the SRAM CS pin enabled and disabled which when used on bootup prevents the low impedance issue noted in firmware issue #80
    sram_enable_chip_select();
    __delay_cycles(SPI_BYTE_CYCLES); //Per datasheet at 3.0V CS delay is 25ns = @16MHz is 2.5 clock cycles
    sram_disable_chip_select();

    return 0;
}

unsigned char sram_Read_Settings(void){
    // 0 = Byte Mode
    // 1 = Page Mode
    // 2 = Sequential Mode
    // 4 = Reserved (Test?)

    //Select the SRAM chip select
    sram_enable_chip_select();
    __delay_cycles(SPI_BYTE_CYCLES*5);

    //Send the READ command
    //spi_tx(SRAM_RDSR);
    __delay_cycles(SPI_BYTE_CYCLES*5);

    //Send dummy byte to shift SPI registers out of SRAM into RX CC430
    spi_tx(0x00); //dummy
    __delay_cycles(SPI_BYTE_CYCLES*5);

    __delay_cycles(SPI_BYTE_CYCLES*5); //Per datasheet at 3.0V CS delay is 25ns = @16MHz is 2.5 clock cycles
    sram_disable_chip_select();

    unsigned char test2;
    test2 = UCB0RXBUF>>6; // Shift mode bytes down from bits 6 & 7 to LSB
    return test2;
}
