#include "spi.h"
#include "game.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "regdef.h"
#include "display.h"
#include "joystick.h"
#include "beeper_motor.h"
#include "systick.h"
#include <time.h>

void game_compute() __critical;
void game_render();
void create_new_pixel(uint8_t pos_xy, uint8_t fruit_eaten, uint8_t speed);
void generate_fruit();
void game_over();
void init_event_timer();
void event_handler(uint8_t buzzer, uint8_t vibmotor, long loop_iter);
void error_vibration();
void check_body_crash(uint8_t next_pos);

const char *a[5];
struct pixel_struct
{
    uint8_t pos_xy;
    uint8_t current_speedvector;
    uint8_t next_speedvector;
};

struct pixel_struct pixels_buffer[64];

volatile uint8_t total_pixels = 0;
volatile uint8_t current_snake_dir = UP;

extern uint8_t interrupt_flag;

// ------------------------------------------------putchar-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_game()
{
    clear_display();
    total_pixels = 0;
    current_snake_dir = UP;
    create_new_pixel(0x61, 0, 0);
    generate_fruit();
    game_render();
}

void update_frame()
{
    uint8_t pos, xpos, ypos;

    pos = get_joystick_pos();
    xpos = (pixels_buffer[0].pos_xy & 0xF0) >> 4;
    ypos = pixels_buffer[0].pos_xy & 0xF;

    if (pos == UP)
    {
        if (current_snake_dir == DOWN)
        {
            error_vibration();
        }
        else
        {

            pixels_buffer[0].next_speedvector = UP;
            current_snake_dir = UP;
        }
    }
    else if (pos == DOWN)
    {
        if (current_snake_dir == UP)
        {
            error_vibration();
        }
        else
        {
            pixels_buffer[0].next_speedvector = DOWN;
            current_snake_dir = DOWN;
        }
    }
    else if (pos == LEFT)
    {
        if (current_snake_dir == RIGHT)
        {
            error_vibration();
        }
        else
        {
            pixels_buffer[0].next_speedvector = LEFT;
            current_snake_dir = LEFT;
        }
    }
    else if (pos == RIGHT)
    {
        if (current_snake_dir == LEFT)
        {
            error_vibration();
        }
        else
        {
            pixels_buffer[0].next_speedvector = RIGHT;
            current_snake_dir = RIGHT;
        }
    }
    else
    {
        pixels_buffer[0].next_speedvector = pixels_buffer[0].current_speedvector;
    }
    game_compute();
    game_render();
    if (interrupt_flag)
        generate_fruit();
}

// ------------------------------------------------putchar-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void game_compute() __critical
{

    uint8_t pixel_speed, xpos, ypos, i, next_pos;

    for (i = 0; i < total_pixels; i++)
    {
        pixel_speed = pixels_buffer[i].next_speedvector;
        xpos = (pixels_buffer[i].pos_xy & 0xF0) >> 4;
        ypos = pixels_buffer[i].pos_xy & 0xF;

        if (pixel_speed == UP)
        {

            if (xpos > 1)
                next_pos = ((xpos - 1) << 4) | ypos;
            else
                next_pos = ((8) << 4) | ypos;

            if (next_pos == pixels_buffer[63].pos_xy && i == 0)
            {
                create_new_pixel(next_pos, 1, UP);
                generate_fruit();
                break;
            }
            else
            {
                pixels_buffer[i].pos_xy = next_pos;
                pixels_buffer[i].current_speedvector = pixel_speed;
            }
        }
        if (pixel_speed == DOWN)
        {

            if (xpos < 8)
                next_pos = ((xpos + 1) << 4) | ypos;
            else
                next_pos = ((1) << 4) | ypos;

            if (next_pos == pixels_buffer[63].pos_xy && i == 0)
            {
                create_new_pixel(next_pos, 1, DOWN);
                generate_fruit();
                break;
            }
            else
            {
                pixels_buffer[i].pos_xy = next_pos;
                pixels_buffer[i].current_speedvector = pixel_speed;
            }
        }
        if (pixel_speed == LEFT)
        {
            if (ypos > 1)
                next_pos = ((xpos) << 4) | (ypos - 1);
            else
                next_pos = ((xpos) << 4) | (8);

            if (next_pos == pixels_buffer[63].pos_xy && i == 0)
            {
                create_new_pixel(next_pos, 1, LEFT);
                generate_fruit();
                break;
            }
            else
            {
                pixels_buffer[i].pos_xy = next_pos;
                pixels_buffer[i].current_speedvector = pixel_speed;
            }
        }
        if (pixel_speed == RIGHT)
        {

            if (ypos < 8)
                next_pos = ((xpos) << 4) | (ypos + 1);
            else
                next_pos = ((xpos) << 4) | (1);

            if (next_pos == pixels_buffer[63].pos_xy && i == 0)
            {
                create_new_pixel(next_pos, 1, RIGHT);
                break;
            }
            else
            {
                pixels_buffer[i].pos_xy = next_pos;
                pixels_buffer[i].current_speedvector = pixel_speed;
            }
        }
        if (pixel_speed == IDLE)
        {
            pixels_buffer[i].current_speedvector = pixel_speed;
        }
    }
    check_body_crash(pixels_buffer[0].pos_xy);

    for (i = 1; i < total_pixels; i++)
    {
        pixels_buffer[i].next_speedvector = pixels_buffer[i - 1].current_speedvector;
    }
}

// ------------------------------------------------putchar-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/

void game_over()
{
    long i;
    clear_display();
    enable_motor();
    enable_beeper();
    for (i = 0; i < 300000; i++)
        ;
    disable_motor();
    disable_beeper();
    BEEP_CSR = 0x4C;
    enable_beeper();
    for (i = 0; i < 300000; i++)
        ;

    enable_motor();
    for (i = 0; i < 300000; i++)
        ;
    disable_motor();
    disable_beeper();
    BEEP_CSR = 0x4E;
    enable_beeper();
    for (i = 0; i < 300000; i++)
        ;
    enable_motor();
    for (i = 0; i < 300000; i++)
        disable_motor();
    disable_beeper();
    BEEP_CSR = 0x4A;

    init_game();
}

void error_vibration()
{
    enable_motor();
    for (long i = 0; i < 200000; i++)
        ; // Sleep
    disable_motor();
}

void generate_fruit()
{

    volatile uint8_t random_posxy;
    srand(now() + 77);
repeat:
    random_posxy = ((rand() % (8) + 1) << 4) | (rand() % (8) + 1);

    for (int i = 0; i < total_pixels; i++)
    {
        if (random_posxy == pixels_buffer[i].pos_xy)
            goto repeat;
    }

    pixels_buffer[63].pos_xy = random_posxy;
    pixels_buffer[63].current_speedvector = IDLE;
    pixels_buffer[63].next_speedvector = IDLE;

    interrupt_flag = 0;
}
// ------------------------------------------------putchar-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void check_body_crash(uint8_t next_pos)
{
    for (int j = 1; j < total_pixels; j++)
    {
        if (next_pos == pixels_buffer[j].pos_xy)
            game_over();
    }
}
// ------------------------------------------------game_render-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void game_render()
{
    uint16_t cols_data[8] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008};
    uint8_t pos, speed_vector, temp, temp2;
    int i;
    for (i = 0; i < total_pixels; i++)
    {
        //conversion of pixel data to display format data
        pos = pixels_buffer[i].pos_xy;
        temp = (uint8_t)(pos & 0xF) - 1;
        temp2 = 1 << (((pos & 0xF0) >> 4) - 1);
        cols_data[temp] |= temp2 << 8;
    }
    // render the fruit at the end of the array
    pos = pixels_buffer[63].pos_xy;
    temp = (uint8_t)(pos & 0xF) - 1;
    temp2 = 1 << (((pos & 0xF0) >> 4) - 1);
    cols_data[temp] |= temp2 << 8;

    for (i = 0; i < 8; i++)
    {
        spi_write_16(cols_data[i]);
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
    }
}
// ------------------------------------------------create_new_pixel-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void create_new_pixel(uint8_t pos_xy, uint8_t fruit_eaten, uint8_t speed) __critical
{
    struct pixel_struct temp;
    temp.pos_xy = pos_xy;

    if (total_pixels == 0 && !fruit_eaten)
    {
        temp.current_speedvector = UP;
        temp.next_speedvector = UP;
        pixels_buffer[total_pixels] = temp;
    }

    if (total_pixels > 0 && !fruit_eaten)
    {
        temp.current_speedvector = UP;
        temp.next_speedvector = pixels_buffer[total_pixels - 1].current_speedvector;
        pixels_buffer[total_pixels] = temp;
    }

    if (fruit_eaten)
    {
        long i;
        enable_beeper();
        for (i = 0; i < 100000; i++)
            ; // Sleep
        disable_beeper();

        for (uint8_t i = total_pixels + 1; i > 0; i--)
        {
            pixels_buffer[i] = pixels_buffer[i - 1];
        }
        temp.current_speedvector = speed;
        temp.next_speedvector = speed;
        pixels_buffer[0] = temp;
        generate_fruit();
        game_render();
    }
    total_pixels++;
}

// ------------------------------------------------End-------------------------------------------------
