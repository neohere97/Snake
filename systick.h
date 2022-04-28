/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stmflash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 ***************************************************************************/

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

//function declarations, please see c file for descriptions
void init_systick(uint8_t frames_per_second);
long now();
#endif
