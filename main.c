#include <stdint.h>
#include <stdio.h>
#include "regdef.h"

void init_uart();
void init_adc1();
void start_adc();
uint16_t get_adcval(uint8_t channel_no);

#define UART_CR2_TEN (1 << 3)
#define UART_CR3_STOP2 (1 << 5)
#define UART_CR3_STOP1 (1 << 4)
#define UART_SR_TXE (1 << 7)

void init_uart()
{

    CLK_DIVR = 0x00;    // Set the frequency to 16 MHz
    CLK_PCKENR1 = 0xFF; // Enable peripherals

    UART1_CR2 = UART_CR2_TEN;                        // Allow TX and RX
    UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
    UART1_BRR2 = 0x0B;
    UART1_BRR1 = 0x08; // 9600 baud
}

void init_adc1()
{
    ADC_CSR |= 0x3; //Selecting channel 3
    ADC_CR1 |= 0x2; //Setting continuous Mode
}

void start_adc()
{
    ADC_CR1 |= 0x1; //Turning on the ADC
}

uint16_t get_adcval(uint8_t channel_no)
{
    uint16_t data;
    ADC_CSR = channel_no;
    while (!(ADC_CSR & 0x80))
        ;

    data |= (ADC_DRH << 2);
    data |= ADC_DRL;

    ADC_CSR = channel_no;

    return data;
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
    init_uart();
    init_adc1();
    unsigned long i = 0;
    start_adc();
    start_adc();
    for (;;)
    {
        printf(" X -> %d ", get_adcval(0x3));
        printf(" Y -> %d \n", get_adcval(0x4));
        putchar('\r');
        for (i = 0; i < 147456; i++)
            ; // Sleep
    }
}