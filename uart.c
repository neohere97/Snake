#include "regdef.h"
#include <stdint.h>


#define UART_CR2_TEN (0x8)
#define UART_CR3_STOP2 (0x20)
#define UART_CR3_STOP1 (0x10)
#define UART_SR_TXE (0x80)

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