/*
 * sram.c
 *
 *  Created on: Feb 15, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "sram.h"
#include "spi.h"



unsigned char sram_enable_chip_select(void){
    //Set P5.5 for slave reset
    GPIO_setOutputLowOnPin(

        GPIO_PORT_P5,
        GPIO_PIN5
        );
    GPIO_setOutputLowOnPin(

        GPIO_PORT_P3,
        GPIO_PIN5
        );
    GPIO_setOutputLowOnPin(

        GPIO_PORT_P1,
        GPIO_PIN1
        );

    return 0;
}

unsigned char sram_disable_chip_select(void){

    //USCI_A0 TX buffer ready?
    //while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
    //           USCI_B_SPI_TRANSMIT_INTERRUPT)) ;
    //Set P5.5 for slave reset
    GPIO_setOutputHighOnPin(

        GPIO_PORT_P5,
        GPIO_PIN5
        );
    GPIO_setOutputHighOnPin(

        GPIO_PORT_P3,
        GPIO_PIN5
        );

    GPIO_setOutputHighOnPin(

        GPIO_PORT_P1,
        GPIO_PIN1
        );

    return 0;
}

unsigned char sram_Toggle_CS(void){
    //This function simply turns the SRAM CS pin enabled and disabled which when used on bootup prevents the low impedance issue noted in firmware issue #80
    sram_enable_chip_select();
    __delay_cycles(SPI_BYTE_CYCLES); //Per datasheet at 3.0V CS delay is 25ns = @16MHz is 2.5 clock cycles
    sram_disable_chip_select();
    __delay_cycles(SPI_BYTE_CYCLES); //Per datasheet at 3.0V CS delay is 25ns = @16MHz is 2.5 clock cycles
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
    //sram_enable_chip_select();
    //__delay_cycles(SPI_BYTE_CYCLES*5);

    //Send the READ command
    spi_transmit_byte(SRAM_RDSR);
    //__delay_cycles(SPI_BYTE_CYCLES*5);

    //Send dummy byte to shift SPI registers out of SRAM into RX CC430
    spi_transmit_byte(0x00); //dummy
    //__delay_cycles(SPI_BYTE_CYCLES*5);

    //__delay_cycles(SPI_BYTE_CYCLES*5); //Per datasheet at 3.0V CS delay is 25ns = @16MHz is 2.5 clock cycles
    //sram_disable_chip_select();

    unsigned char test2;
    //test2 = UCB0RXBUF>>6; // Shift mode bytes down from bits 6 & 7 to LSB
    test2 = spi_receive_byte();
    test2 = test2>>6;

    return test2;
}

unsigned char Faraday_SRAM_Write_Settings(unsigned char mode){
    // 0 = Byte Mode
    // 1 = Sequential Mode
    // 2 = Page Mode
    // 4 = Reserved (Test?)

    if (mode < 4){
        unsigned char mode2 = mode<<6; //Shift mode bits to bits 6 and 7
        //Select the SRAM chip select

        //sram_enable_chip_select();
        //__delay_cycles(SPI_BYTE_CYCLES*5); //Per datasheet at 3.0V CS delay is 25ns = @16MHz is 2.5 clock cycles

        //Send the READ command
        spi_transmit_byte(SRAM_WRSR);
        //__delay_cycles(SPI_BYTE_CYCLES*5);

        //Send dummy byte to shift SPI registers out of SRAM into RX CC430
        spi_transmit_byte(mode2); //dummy

        //USCI_A0 TX buffer ready?
        while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
                   USCI_B_SPI_TRANSMIT_INTERRUPT)) ;

        //__delay_cycles(SPI_BYTE_CYCLES*5); //Per datasheet at 3.0V CS delay is 25ns = @16MHz is 2.5 clock cycles
        //sram_disable_chip_select();
    }
    else{
        __no_operation(); // ERROR!
    }
}


unsigned char sram_selftest(void){
    volatile unsigned char testbyte;

    // Mode 0
    sram_enable_chip_select();
    __no_operation();
    Faraday_SRAM_Write_Settings(0);
    __no_operation();
    testbyte = sram_Read_Settings();
    sram_disable_chip_select();
    __no_operation();

    // Mode 1
    sram_enable_chip_select();
    __no_operation();
    Faraday_SRAM_Write_Settings(1);
    __no_operation();
    testbyte = sram_Read_Settings();
    sram_disable_chip_select();
    __no_operation();

    // Mode 2
    sram_enable_chip_select();
    __no_operation();
    Faraday_SRAM_Write_Settings(2);
    __no_operation();
    testbyte = sram_Read_Settings();
    sram_disable_chip_select();
    __no_operation();

    // Mode 3
    sram_enable_chip_select();
    __no_operation();
    Faraday_SRAM_Write_Settings(3);
    __no_operation();
    testbyte = sram_Read_Settings();
    sram_disable_chip_select();
    __no_operation();


    // Return
    return 0;
}
