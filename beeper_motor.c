/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stm8flash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References:STM8 datasheet & Reference Manuals
 ***************************************************************************/

#include "regdef.h"
#include "beeper_motor.h"
#include <stdint.h>

// Register Defines
#define BEEPER_INIT   (0x4A)
#define ENABLE_BEEPER (0x20)
#define GPIO4_MASK    (0x10)

// ------------------------------------------------init-beeper-------------------------------------------------
/***********************************************************************************
 * function : Initializes the beeper peripheral with default frequency
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_beeper()
{
    BEEP_CSR = BEEPER_INIT;
}

// ------------------------------------------------enable-beeper------------------------------------------------
/***********************************************************************************
 * function : Turns on the beeper by setting the enable bit
 * parameters : none
 * return : none
 ***********************************************************************************/
void enable_beeper()
{
    BEEP_CSR |= ENABLE_BEEPER;
}

// ------------------------------------------------diable-beeper------------------------------------------------
/***********************************************************************************
 * function : Turns off the beeper by resetting to initial state
 * parameters : none
 * return : none
 ***********************************************************************************/
void disable_beeper()
{
    BEEP_CSR = BEEPER_INIT;
}

// ------------------------------------------------init-vibmotor-------------------------------------------------
/***********************************************************************************
 * function : Initializes the GPIO for the vibration motor
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_vibmotor()
{
    PC_DDR |= GPIO4_MASK; // Data Direction Out
    PC_CR1 |= GPIO4_MASK; // Push Pull configuration
}

// ------------------------------------------------enable-motor------------------------------------------------
/***********************************************************************************
 * function : Sets the motor GPIO to high and turns on the motor
 * parameters : none
 * return : none
 ***********************************************************************************/
void enable_motor()
{
    PC_ODR |= GPIO4_MASK;
}

// ------------------------------------------------disable-motor------------------------------------------------
/***********************************************************************************
 * function : Sets the GPIO to low and turns off the motor
 * parameters : none
 * return : none
 ***********************************************************************************/
void disable_motor()
{
    PC_ODR &= ~GPIO4_MASK;
}
// ------------------------------------------------End-------------------------------------------------
