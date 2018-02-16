/*
 * init_gpio.c
 *
 *  Created on: Feb 9, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "init_gpio.h"

unsigned char init_gpio_spi(void){
    /**
     * SCLK = P1.4
     * MISO = P1.2 (Needs pullup?)
     * MOSI = P1.3 (Needs pullup?)
     * CS0  = P5.5
     * CS1  = P3.5
     * CS2  = P1.1
     * HOLD = P5.6
     */
    // Set chip select pins as outputs
    GPIO_setAsOutputPin(
        GPIO_PORT_P5,
        GPIO_PIN5
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN5
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN1
        );

    // Set SPI HOLD pin as outputs
    GPIO_setAsOutputPin(
        GPIO_PORT_P5,
        GPIO_PIN6
        );


    // Set chip select pins HIGH
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

    // Set SPI HOLD pin HIGH
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P5,
        GPIO_PIN6
        );


    //option select MISO
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN2
        );

    //option SCLK, MOSI
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN3 + GPIO_PIN4
        );


    return 0;
}
