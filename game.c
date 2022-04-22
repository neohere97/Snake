#include "spi.h"
#include "game.h"
#include <stdint.h>
#include <stdio.h>
#include "regdef.h"
#include "display.h"
#include "joystick.h"

void game_compute() __critical;
void game_render();
void create_new_pixel(uint8_t pos_xy);

const char *a[5];
struct pixel_struct
{
    uint8_t pos_xy;
    uint8_t current_speedvector;
    uint8_t next_speedvector;
};
struct pixel_struct pixels_buffer[64];
uint8_t total_pixels = 0;
volatile uint8_t current_snake_dir = UP;

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
    create_new_pixel(0x61);
    create_new_pixel(0x71);
    create_new_pixel(0x81);
    game_render();
    while (1)
    {

        for (i = 0; i < 250000; i++)
        {
        };

        pos = get_joystick_pos();
        xpos = (pixels_buffer[0].pos_xy & 0xF0) >> 4;
        ypos = pixels_buffer[0].pos_xy & 0xF;

        if (pos == UP && current_snake_dir != DOWN && xpos > 1)
        {            
            pixels_buffer[0].next_speedvector = UP;
            current_snake_dir = UP;       
            
        }
        else if (pos == DOWN && current_snake_dir != UP && xpos < 8)
        {

            pixels_buffer[0].next_speedvector = DOWN;
            current_snake_dir = DOWN;           
         }
        else if (pos == LEFT && current_snake_dir != RIGHT && ypos > 1)
        {

            pixels_buffer[0].next_speedvector = LEFT;
            current_snake_dir = LEFT;           
           }
        else if (pos == RIGHT && current_snake_dir != LEFT && ypos < 8)
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
    }

    //Making the head pixel
}

void game_compute() __critical
{
    

    uint8_t pixel_speed, xpos, ypos, i;

    for (i = 0; i < total_pixels; i++)
    {
        pixel_speed = pixels_buffer[i].next_speedvector;
        xpos = (pixels_buffer[i].pos_xy & 0xF0) >> 4;
        ypos = pixels_buffer[i].pos_xy & 0xF;

        if (pixel_speed == UP)
        {
            pixels_buffer[i].pos_xy = ((xpos - 1) << 4) | ypos;
            pixels_buffer[i].current_speedvector = pixel_speed;           

        }
        if (pixel_speed == DOWN)
        {

            pixels_buffer[i].pos_xy = ((xpos + 1) << 4) | ypos;
            pixels_buffer[i].current_speedvector = pixel_speed;
            
        }
        if (pixel_speed == LEFT)
        {

            pixels_buffer[i].pos_xy = ((xpos) << 4) | (ypos - 1);
            pixels_buffer[i].current_speedvector = pixel_speed;
            
        }
        if (pixel_speed == RIGHT)
        {
            pixels_buffer[i].pos_xy = ((xpos) << 4) | (ypos + 1);
            pixels_buffer[i].current_speedvector = pixel_speed;
            
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

void game_render(){
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
    // clear_display();

    for (i = 0; i < 8; i++)
    {
        spi_write_16(cols_data[i]);
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
    }
}


void create_new_pixel(uint8_t pos_xy) __critical
{
    struct pixel_struct temp;
    temp.pos_xy = pos_xy;

    if (total_pixels == 0)
    {
        temp.current_speedvector = UP;
        temp.next_speedvector = UP;
    }

    else
    {
        temp.current_speedvector = UP;
        temp.next_speedvector = pixels_buffer[total_pixels - 1].current_speedvector;
    }

    pixels_buffer[total_pixels] = temp;
    total_pixels++;
}