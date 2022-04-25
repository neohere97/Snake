#include "regdef.h"
#include "beeper_motor.h"
#include <stdint.h>

void init_beeper(){
    BEEP_CSR = 0x4A;
}

void enable_beeper(){
    BEEP_CSR |= 0x20;
}

void disable_beeper(){
    BEEP_CSR = 0x4A; 
}

void init_vibmotor(){
    PC_DDR |= 0x10;    
    PC_CR1 |= 0x10;
  }

void enable_motor(){
    
    PC_ODR |= 0x10;
}

void disable_motor(){
    PC_ODR &= 0xEF;
}