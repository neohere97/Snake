/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References:  & STM8 Manuals
 ***************************************************************************/

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

//function declarations, please see the c file for descriptions
void spi_write_16(uint16_t data);
void init_spi();

#endif