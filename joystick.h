/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References:  & STM8 Manuals
 ***************************************************************************/

#ifndef ADC_1_
#define ADC_1_

// snake direction enum declarations
enum joystick_pos
{
    IDLE,
    UP,
    RIGHT,
    DOWN,
    LEFT,
    UP_FAST,
    DOWN_FAST,
    RIGHT_FAST,
    LEFT_FAST
};

//function declarations, please see the c file for descriptions
void init_adc1();
void start_adc();
uint16_t get_adcval(uint8_t channel_no);
enum joystickpos get_joystick_pos();

#endif