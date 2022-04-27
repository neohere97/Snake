#include <stdint.h>
#include <stdio.h>
#include "regdef.h"
#include "joystick.h"
#include "spi.h"
#include "display.h"
#include "systick.h"
#include "beeper_motor.h"
#include "game.h"
#include "uart.h"


#define FRAMES_PER_SECOND 5


void init_button_interrupt();

// void pca_interrupt() __interrupt(3);



uint8_t interrupt_flag = 0;

extern volatile long system_time;


// ------------------------------------------------init-interrupt------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_button_interrupt()
{
    PC_DDR = 0x0;
    PC_CR1 = 0x8;
    PC_CR2 = 0x8;
    EXTI_CR1 |= 0x20;
}

void button_interrupt_handler (void) __interrupt(5)
{
    interrupt_flag = 1;
}

void timer1_interrupt(void) __interrupt(11)
{
    TIM1_SR1 = 0;
    system_time++;
    update_frame();
}

// ------------------------------------------------main-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void main(void)
{
    //enabling interrupts
    __asm__("rim");

    init_uart();    
    init_adc1();   
    start_adc();
    start_adc();
    init_button_interrupt();
    init_spi();
    init_beeper();
    init_vibmotor();  
    init_display();
    init_systick(FRAMES_PER_SECOND);
    init_game();

    while(1){
        __asm__("nop");
    }
}
// ------------------------------------------------End-------------------------------------------------
