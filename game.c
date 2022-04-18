#include "spi.h"
#include "game.h"
#include <stdint.h>
#include <stdio.h>
#include "regdef.h"
#include "display.h"
#include "joystick.h"

void game_compute_render();
// void game_render();
void create_new_pixel(uint8_t pos_xy);

struct pixel_struct
{
    uint8_t pos_xy;
    uint8_t cur_next_speedvector;
};
struct pixel_struct pixels_buffer[64];
uint8_t total_pixels = 0;

void init_game()
{
    long m;
    long i, j;
    uint8_t pos,xpos,ypos;

    clear_display();
    create_new_pixel(0x11);
    game_compute_render();
    while (1)
    {

        for (i = 0; i < 60000; i++){};

        pos = get_joystick_pos();
        xpos = (pixels_buffer[0].pos_xy & 0xF0 )>> 4;
        ypos = pixels_buffer[0].pos_xy & 0xF;
        if(pos == UP && xpos > 1){
            pixels_buffer[0].pos_xy = ((xpos - 1) << 4) | ypos; 
            game_compute_render();           
        }
        if(pos == DOWN && xpos < 8){
            pixels_buffer[0].pos_xy = ((xpos + 1) << 4) | ypos; 
            game_compute_render();
        }
        if(pos == LEFT && ypos > 1){
            pixels_buffer[0].pos_xy = ((xpos) << 4) | (ypos - 1); 
            game_compute_render();
        }
        if(pos == RIGHT && ypos < 8){
            pixels_buffer[0].pos_xy = ((xpos) << 4) | (ypos + 1); 
            game_compute_render();
        }
        
        
    }

    //Making the head pixel
}

void game_compute_render() __critical
{
    uint16_t cols_data[8] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008};
    uint8_t pos, speed_vector, temp, temp2;
    int i;
    for (i = 0; i < total_pixels; i++)
    {
        pos = pixels_buffer[i].pos_xy;
        speed_vector = pixels_buffer[i].cur_next_speedvector;
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
    // if(total_pixels == 0){
    struct pixel_struct temp;
    temp.pos_xy = pos_xy;
    temp.cur_next_speedvector = 0x00;
    pixels_buffer[total_pixels] = temp;

    total_pixels++;
    // }

    // printf("posxy -> %X \n\r",pixels_buffer[0].pos_xy);
}