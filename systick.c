#include "regdef.h"
#include <stdint.h>
#include <stdio.h>

volatile long system_time;


void init_systick(uint8_t frames_per_second)
{
    system_time = 0;
 
    TIM1_PSCRH = 0x3e;
    TIM1_PSCRL = 0x80;
    TIM1_IER = 0x01;

    TIM1_CR1 = 0x94;

    TIM1_ARRH = 0x0;

    if (frames_per_second > 1 && frames_per_second < 12)
        TIM1_ARRL = 1000 / frames_per_second;
    else
        TIM1_ARRL = 0xC8;
    

    TIM1_EGR = 0x1;
    TIM1_EGR = 0x1;

    TIM1_CR1 |= 0x01;
}
long now()
{
    return system_time;
}

