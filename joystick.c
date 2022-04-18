/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: SDCC Documentation & STM8 Manuals
 ***************************************************************************/

#include "regdef.h"
#include <stdint.h>
#include <stdio.h>
#include "joystick.h"

// ------------------------------------------------init-adc1-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_adc1()
{
    ADC_CSR |= 0x3; //Selecting channel 3
    ADC_CR1 |= 0x2; //Setting continuous Mode
}
// ------------------------------------------------start-adc-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void start_adc()
{
    ADC_CR1 |= 0x1; //Turning on the ADC
}
// ------------------------------------------------get-adcval-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
uint16_t get_adcval(uint8_t channel_no)
{
    uint16_t data;
    for (int i = 0; i < 2; i++)
    {
        data = 0;
        ADC_CSR = channel_no;
        while (!(ADC_CSR & 0x80))
            ;

        data |= (ADC_DRH << 2);
        data |= ADC_DRL;

        ADC_CSR = channel_no;
    }
    return data;
}
// ------------------------------------------------get-joystick-pos------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
 enum joystickpos get_joystick_pos(){
     uint16_t xpos, ypos;

     xpos = get_adcval(JOYSTICK_XDIR);
     ypos = get_adcval(JOYSTICK_YDIR);

     if(ypos > 600 && xpos > 400 && xpos < 600)
     return UP;
     else if(xpos > 600 && ypos > 400 && ypos < 600)
     return RIGHT;
     else if(ypos < 300 && xpos > 400 && xpos < 600)
     return DOWN;
     else if(xpos < 300 && ypos > 400 && ypos < 600)
     return LEFT;
     else
     return IDLE;

 }
// ------------------------------------------------End-------------------------------------------------
