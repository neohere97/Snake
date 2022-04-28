/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References:  STM8 Manuals
 ***************************************************************************/

#include "regdef.h"
#include <stdint.h>
#include <stdio.h>
#include "joystick.h"

#define JOYSTICK_YDIR (0x3) // Channel 3, pin D2
#define JOYSTICK_XDIR (0x4) // Channel 4, pin D3

#define ADC_CONT_MODE (0x2)
#define ADC_START (0x1)
// ------------------------------------------------init-adc1-------------------------------------------------
/***********************************************************************************
 * function : configure the ADC peripheral on the STM8 microcontroller
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_adc1()
{
    ADC_CSR |= JOYSTICK_YDIR; //Selecting channel 3
    ADC_CR1 |= ADC_CONT_MODE; //Setting continuous Mode
}
// ------------------------------------------------start-adc-------------------------------------------------
/***********************************************************************************
 * function : starts the ADC
 * parameters : none
 * return : none
 ***********************************************************************************/
void start_adc()
{
    ADC_CR1 |= ADC_START; //Turning on the ADC
}
// ------------------------------------------------get-adcval-------------------------------------------------
/***********************************************************************************
 * function : Samples the ADC Channel and returns the value
 * parameters : channel_no -> ADC channel which needs to be sampled
 * return : Measured RAW ADC value
 ***********************************************************************************/
uint16_t get_adcval(uint8_t channel_no)
{
    uint16_t data;

    //ADC channel is sampled twice for consistency and noise avoidance
    for (int i = 0; i < 2; i++)
    {
        data = 0;

        //setting the channel
        ADC_CSR = channel_no;

        while (!(ADC_CSR & 0x80))
            ;

        //10 Bits of data is joined together
        data |= (ADC_DRH << 2);
        data |= ADC_DRL;

        ADC_CSR = channel_no;
    }
    // Return the sampled data
    return data;
}
// ------------------------------------------------get-joystick-pos------------------------------------------------
/***********************************************************************************
 * function : samples ADC channels and determines the joystick position 
 * parameters : none
 * return : Returns an enum with joystick position
 ***********************************************************************************/
enum joystickpos get_joystick_pos()
{
    uint16_t xpos, ypos;

    // Get RAW ADC data
    xpos = get_adcval(JOYSTICK_XDIR);
    ypos = get_adcval(JOYSTICK_YDIR);

    //Classify the data into directions for the snake

    if (ypos > 600 && xpos > 400 && xpos < 600)
        return UP;
    else if (xpos > 600 && ypos > 400 && ypos < 600)
        return RIGHT;
    else if (ypos < 300 && xpos > 400 && xpos < 600)
        return DOWN;
    else if (xpos < 300 && ypos > 400 && ypos < 600)
        return LEFT;
    else
        return IDLE;
}
// ------------------------------------------------End-------------------------------------------------
