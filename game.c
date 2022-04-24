#include "spi.h"
#include "game.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "regdef.h"
#include "display.h"
#include "joystick.h"
#include <time.h>

void game_compute() __critical;
void game_render();
void create_new_pixel(uint8_t pos_xy, uint8_t fruit_eaten, uint8_t speed);
void generate_fruit();
void game_over();
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

void init_game()
{
    long m;
    long i, j;
    uint8_t pos, xpos, ypos;

    a[0] = "IDLE";
    a[1] = "UP";
    a[2] = "RIGHT";
    a[3] = "DOWN";
    a[4] = "LEFT";

    clear_display();
    total_pixels = 0;
    create_new_pixel(0x61, 0, 0);
    generate_fruit();
    game_render();
    while (1)
    {

        for (i = 0; i < 400000; i++)
        {
        };

        pos = get_joystick_pos();
        xpos = (pixels_buffer[0].pos_xy & 0xF0) >> 4;
        ypos = pixels_buffer[0].pos_xy & 0xF;

        if (pos == UP && current_snake_dir != DOWN)
        {
            pixels_buffer[0].next_speedvector = UP;
            current_snake_dir = UP;
        }
        else if (pos == DOWN && current_snake_dir != UP)
        {

            pixels_buffer[0].next_speedvector = DOWN;
            current_snake_dir = DOWN;
        }
        else if (pos == LEFT && current_snake_dir != RIGHT)
        {

            pixels_buffer[0].next_speedvector = LEFT;
            current_snake_dir = LEFT;
        }
        else if (pos == RIGHT && current_snake_dir != LEFT)
        {

            pixels_buffer[0].next_speedvector = RIGHT;
            current_snake_dir = RIGHT;
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

    //Making the head pixel
}

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

            check_body_crash(next_pos);

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
            
            check_body_crash(next_pos);

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

            check_body_crash(next_pos);

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

            check_body_crash(next_pos);

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

    for (i = 1; i < total_pixels; i++)
    {
        pixels_buffer[i].next_speedvector = pixels_buffer[i - 1].current_speedvector;
    }
}

void game_over()
{
    clear_display();
    init_game();
}

void generate_fruit()
{

    volatile uint8_t random_posxy;
    volatile uint8_t random_times = (rand() % 6) + 3;

    for (uint8_t i = 0; i < random_times; i++)
    {
        srand(i + 255);
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
    }

    interrupt_flag = 0;
}

void check_body_crash(uint8_t next_pos)
{
    for (int j = 0; j < total_pixels; j++)
    {
        if (next_pos == pixels_buffer[j].pos_xy)
            game_over();
    }
}

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