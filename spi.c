#include "regdef.h"
#include "spi.h"


void init_spi()
{
    //Initializing the slave select pin
    PA_DDR = 0x8;
    PA_CR2 = 0x8;
    PA_CR1 = 0x8;
    PA_ODR = 0x8;

    SPI_CR2 = 0x3;
    SPI_CR1 = 0x44;
}

void spi_write_16(uint16_t data)
{
    while (!(SPI_SR & 0x2) && (SPI_SR & 0x80))
        ;
    PA_ODR = 0;
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    SPI_DR = (uint8_t)(data & 0xF);
    while (!(SPI_SR & 0x2) && (SPI_SR & 0x80))
        ;
    SPI_DR = (uint8_t)(data >> 8);
    while (!(SPI_SR & 0x2) && (SPI_SR & 0x80))
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
    PA_ODR = 0x8;
}