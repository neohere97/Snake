#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

void init_systick(uint8_t frames_per_second);
long now();
#endif
