/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stm8flash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: STM8 datasheet & Reference Manuals
 ***************************************************************************/

#ifndef BEEPER_H
#define BEEPER_H

//function declarations, please see the c file for function descriptions
void init_beeper();
void enable_beeper();
void disable_beeper();
void init_vibmotor();
void enable_motor();
void disable_motor();

#endif
