/*
 * led.c
 *
 *  Created on: Mar 22, 2018
 *      Author: Brent
 */


#include "driverlib.h"
#include "led.h"

void enableGreenLed(void){
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P3,
        GPIO_PIN6
        );
}

void disableGreenLed(void){
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P3,
        GPIO_PIN6
        );
}

void enableRedLed(void){
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P3,
        GPIO_PIN7
        );
}

void disableRedLed(void){
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P3,
        GPIO_PIN7
        );
}
