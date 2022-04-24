#include <stdint.h>
#include <stdio.h>
#include "regdef.h"
#include "joystick.h"
#include "spi.h"
#include "display.h"
#include "game.h"

void init_uart();
void init_interrupt();


// void pca_interrupt() __interrupt(3);

#define UART_CR2_TEN (1 << 3)
#define UART_CR3_STOP2 (1 << 5)
#define UART_CR3_STOP1 (1 << 4)
#define UART_SR_TXE (1 << 7)

uint8_t interrupt_flag = 0;

// ------------------------------------------------init-uart------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_uart()
{
    //Pin D6 Tx, D5 Rx

    CLK_DIVR = 0x00;    // Set the frequency to 16 MHz
    CLK_PCKENR1 = 0xFF; // Enable peripherals

    UART1_CR2 = UART_CR2_TEN;                        // Allow TX and RX
    UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
    UART1_BRR2 = 0x0B;
    UART1_BRR1 = 0x08; // 9600 baud
}
// ------------------------------------------------init-interrupt------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_interrupt()
{
    PC_DDR = 0x0;
    PC_CR1 = 0x8;
    PC_CR2 = 0x8;
    EXTI_CR1 |= 0x20;
}

void pca_interrupt(void) __interrupt(5)
{
    interrupt_flag = 1;
}
// ------------------------------------------------putchar-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
int putchar(int c)
{
    while (!(UART1_SR & UART_SR_TXE))
        ;

    UART1_DR = c;

    return (c);
}
// ------------------------------------------------main-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void main(void)
{

    __asm__("rim");


    init_uart();
    init_adc1();
    unsigned long i = 0;
    start_adc();
    start_adc();
    init_interrupt();
    init_spi();
  

    uint8_t col1 = 1;
    uint8_t cur_state = 0;

    init_display();   
    init_game();
    // spi_write_16(0x8001);
    for (;;)
    {
        // if(get_joystick_pos() == UP && col1 != 0x1){
        //     spi_write_16((uint16_t) ((col1 >> 1) << 8)| 0x1);
        //     col1 = col1 >> 1;
        // }else if(get_joystick_pos() == DOWN && col1 != 0x80){
        //     spi_write_16((uint16_t) ((col1 << 1) << 8)| 0x1);
        //     col1 = col1 << 1;
        // }   
        // printf("%s\n", a[get_joystick_pos()]);
        // putchar('\r');
        for (i = 0; i < 70000; i++)
            ; // Sleep
    }
}
// ------------------------------------------------End-------------------------------------------------
