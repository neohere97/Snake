#include <stdint.h>
#include <stdio.h>
#include "regdef.h"
#include "joystick.h"

void init_uart();

#define UART_CR2_TEN (1 << 3)
#define UART_CR3_STOP2 (1 << 5)
#define UART_CR3_STOP1 (1 << 4)
#define UART_SR_TXE (1 << 7)

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

int putchar(int c)
{
    while (!(UART1_SR & UART_SR_TXE))
        ;

    UART1_DR = c;

    return (c);
}

void main(void)
{
    const char *a[5];

    a[0] = "IDLE";
    a[1] = "UP";
    a[2] = "RIGHT";
    a[3] = "DOWN";
    a[4] = "LEFT";

    init_uart();
    init_adc1();
    unsigned long i = 0;
    start_adc();
    start_adc();
    for (;;)
    {
        printf("%s\n", a[get_joystick_pos()]);
        putchar('\r');
        for (i = 0; i < 147456; i++)
            ; // Sleep
    }
}