/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References:  & STM8 Manuals
 ***************************************************************************/

#include "regdef.h"
#include "spi.h"

#define GPIO3_MASK (0x8)
#define SPI_CR2_MODE (0x3)
#define SPI_EN (0x44)
#define SPI_BUSY_FLAG (0x80)
#define SPI_TRX_FLAG (0x2)

// ------------------------------------------------init-spi------------------------------------------------
/***********************************************************************************
 * function : initializes SPI hardware on the STM8
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_spi()
{
    //Initializing the slave select pin
    PA_DDR = GPIO3_MASK;
    PA_CR2 = GPIO3_MASK;
    PA_CR1 = GPIO3_MASK;
    PA_ODR = GPIO3_MASK;

    SPI_CR2 = SPI_CR2_MODE;
    SPI_CR1 = SPI_EN;
}

// ------------------------------------------------spi-write-16----------------------------------------------
/***********************************************************************************
 * function : Write 16 Bits / 2 Bytes of data at a time
 * parameters : data  -> 2 Bytes of data to be written
 * return : none
 ***********************************************************************************/
void spi_write_16(uint16_t data)
{
    while (!(SPI_SR & SPI_TRX_FLAG) && (SPI_SR & SPI_BUSY_FLAG))
        ;
    // Make chip select pin low
    PA_ODR = 0;

    // nop's are to adjust time skew and slave select pin timings
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    // Write the first byte of data
    SPI_DR = (uint8_t)(data & 0xF);
    while (!(SPI_SR & SPI_TRX_FLAG) && (SPI_SR & SPI_BUSY_FLAG))
        ;
    //Write the second byte of data
    SPI_DR = (uint8_t)(data >> 8);
    while (!(SPI_SR & SPI_TRX_FLAG) && (SPI_SR & SPI_BUSY_FLAG))
        ;
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    // make chip select pin high
    PA_ODR = 0x8;
}
// ------------------------------------------------End------------------------------------------------
