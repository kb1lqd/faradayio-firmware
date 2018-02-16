/*
 * sram.h
 *
 *  Created on: Feb 15, 2018
 *      Author: KB1LQ
 */

#ifndef SRAM_SRAM_H_
#define SRAM_SRAM_H_

#define SPI_BYTE_CYCLES 2064

/** @name SPI SRAM Definitions
*   @brief Definitions for the SPI SRAM IC
*
@{**/
#define SRAM_READ 0x03
#define SRAM_WRITE 0x02
#define SRAM_RDSR 0x05
#define SRAM_WRSR 0x01
/** @}*/

unsigned char sram_enable_chip_select(void);
unsigned char sram_disable_chip_select(void);
unsigned char sram_Toggle_CS(void);
unsigned char sram_Read_Settings(void);
unsigned char Faraday_SRAM_Write_Settings(unsigned char mode);
unsigned char sram_selftest(void);



#endif /* SRAM_SRAM_H_ */
