/*
 * sram.h
 *
 *  Created on: Feb 15, 2018
 *      Author: KB1LQ
 */

#ifndef SRAM_SRAM_H_
#define SRAM_SRAM_H_

#define SPI_BYTE_CYCLES 2064

unsigned char sram_enable_chip_select(void);
unsigned char sram_disable_chip_select(void);
unsigned char sram_Toggle_CS(void);
unsigned char sram_Read_Settings(void);



#endif /* SRAM_SRAM_H_ */
