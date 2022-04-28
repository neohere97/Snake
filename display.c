/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stm8flash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: , MAX7219 Datasheet
 ***************************************************************************/

#include "display.h"
#include "regdef.h"
#include "spi.h"

// ------------------------------------------------clear-display-------------------------------------------------
/***********************************************************************************
 * function : clears the display by turning off all the pixels
 * parameters : none
 * return : none
 ***********************************************************************************/
void clear_display()
{
    // Writing 0 to all columns to clear the display
    spi_write_16(0x0001);
    spi_write_16(0x0002);
    spi_write_16(0x0003);
    spi_write_16(0x0004);
    spi_write_16(0x0005);
    spi_write_16(0x0006);
    spi_write_16(0x0007);
    spi_write_16(0x0008);
}
// ------------------------------------------------init-display-------------------------------------------------
/***********************************************************************************
 * function : Initializes MAX7219
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_display()
{
    // Sets digit format, brightness of LED's & Turns ON the display module
    spi_write_16(0x070B);
    spi_write_16(0x0009);
    spi_write_16(0x010C);
    spi_write_16(0x040A);
    spi_write_16(0x000F);

    clear_display();
}
// ------------------------------------------------End-------------------------------------------------
