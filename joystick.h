#ifndef ADC_1_
#define ADC_1_

#define JOYSTICK_YDIR 0x3 // Channel 3, pin D2
#define JOYSTICK_XDIR 0x4 // Channel 4, pin D3    

enum joystick_pos {IDLE,UP,RIGHT,DOWN,LEFT};

void init_adc1();
void start_adc();
uint16_t get_adcval(uint8_t channel_no);
enum joystickpos get_joystick_pos();

#endif