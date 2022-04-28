/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References:  & STM8 Manuals
 ***************************************************************************/


#include "regdef.h"
#include <stdint.h>
#include <stdio.h>

#define TIM1_EN (0x1)
#define UPDATE_EVENT (0x1)
#define TIM1_INTERRUPT (0x1)
#define TIM1_CONFIG_DOWNCOUNT (0x94)

//Global system time
volatile long system_time;

// ------------------------------------------------init-systick------------------------------------------------
/***********************************************************************************
 * function : initializes the systick, which is also used as a game tick timer
 * parameters : frames_per_second -> FPS to be rendered 
 * return : none
 ***********************************************************************************/
void init_systick(uint8_t frames_per_second)
{
    system_time = 0;
 
    //Set the prescalar to produce 1000 ticks per second
    TIM1_PSCRH = 0x3e;
    TIM1_PSCRL = 0x80;

    TIM1_IER = TIM1_INTERRUPT;

    TIM1_CR1 = TIM1_CONFIG_DOWNCOUNT;

    //Set higher byte of auto reload register to 0
    TIM1_ARRH = 0x0;

    //Calculate the lower byte of reload register based on the FPS 
    // requirements
    if (frames_per_second > 1 && frames_per_second < 12)
        TIM1_ARRL = 1000 / frames_per_second;
    else
        TIM1_ARRL = 0xC8;
    
    // Update event to update all timer registers
    TIM1_EGR = UPDATE_EVENT;
    TIM1_EGR = UPDATE_EVENT;

    //Enable timer 1
    TIM1_CR1 |= TIM1_EN;
}

// ------------------------------------------------now-------------------------------------------------
/***********************************************************************************
 * function : Returns current system ticks
 * parameters : none
 * return : current system ticks/time
 ***********************************************************************************/
long now()
{
    return system_time;
}

// ------------------------------------------------End------------------------------------------------

