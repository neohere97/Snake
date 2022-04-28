/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References:  & STM8 Manuals
 ***************************************************************************/

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

//Game FPS to be rendered, between 2 & 12
#define FRAMES_PER_SECOND 5

// GPIO definitions 
#define GPIO3_MASK (0x8)
#define GPIO3_DIR_IN (0x0)
#define EXTI_FALLING_ED (0x20)

void init_button_interrupt();

// system time defined in systick
extern volatile long system_time;


// ------------------------------------------------init-interrupt------------------------------------------------
/***********************************************************************************
 * function : initializes the button interrupt on the joystick
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_button_interrupt()
{
    PC_DDR = GPIO3_DIR_IN;
    PC_CR1 = GPIO3_MASK;
    PC_CR2 = GPIO3_MASK;
    EXTI_CR1 |= EXTI_FALLING_ED;
}

// ------------------------------------------------button-interrupt-handler----------------------------------------------
/***********************************************************************************
 * function : handles button interrupt from the joystick
 * parameters : none
 * return : none
 ***********************************************************************************/
void button_interrupt_handler (void) __interrupt(5)
{
    generate_fruit();
}

// ------------------------------------------------timer1-interrupt------------------------------------------------
/***********************************************************************************
 * function : Acknowledges the timer flag and counts up the system time and renders a new frame
 * parameters : none
 * return : none
 ***********************************************************************************/
void timer1_interrupt(void) __interrupt(11)
{
    TIM1_SR1 = 0;
    // update system time
    system_time++;
    // generate a new frame 
    update_frame();
}

// ------------------------------------------------main-------------------------------------------------
/***********************************************************************************
 * function : initializes all peripherals and the game
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

    // All the game frames are rendered on timer interrupts, hence main loop
    // sits in a 'nop' loop

    // For future upgrades, microcontroller can be made to sleep inbetween timer interrupts
    while(1){
        __asm__("nop");
    }
}
// ------------------------------------------------End-------------------------------------------------
