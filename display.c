#include "display.h"
#include "regdef.h"
#include "spi.h"

void clear_display()
{
    spi_write_16(0x0001);
    spi_write_16(0x0002);
    spi_write_16(0x0003);
    spi_write_16(0x0004);
    spi_write_16(0x0005);
    spi_write_16(0x0006);
    spi_write_16(0x0007);
    spi_write_16(0x0008);
}

void init_display()
{
    spi_write_16(0x070B);
    spi_write_16(0x0009);
    spi_write_16(0x010C);
    spi_write_16(0x040A);
    spi_write_16(0x000F);
    
    clear_display();
}