
/***************************************************************************
 * ESD Final Project, Spring 2022
 * Tools: VSCode,make,stm8flash,SDCC
 * Author: Chinmay Shalawadi
 * Institution: University of Colorado Boulder
 * Mail id: chsh1552@colorado.edu
 * References: SDCC Documentation, MAX7219 Datasheet
 ***************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spi.h"
#include "game.h"
#include "regdef.h"
#include "display.h"
#include "joystick.h"
#include "beeper_motor.h"
#include "systick.h"

#define NUM_PIXELS (64)

// local function declarations
static void game_compute() __critical;
static void game_render();
static void create_new_pixel(uint8_t pos_xy, uint8_t fruit_eaten, uint8_t speed);
static void game_over();
static void game_delay(long loop_iter);
static void error_vibration();
static void check_body_crash(uint8_t next_pos);

// Pixel structure with location and velocity information
struct pixel_struct
{
    uint8_t pos_xy;              // Stores X,Y position of the pixel on the display grid
    uint8_t current_speedvector; // Current direction of the pixel
    uint8_t next_speedvector;    // Next direction of the pixel
};

// Buffer to hold array of 64 pixels, max size of display
struct pixel_struct pixels_buffer[NUM_PIXELS];

// global variables to keep track of live pixels and snake direction
volatile uint8_t total_pixels;
volatile uint8_t current_snake_dird;

// ------------------------------------------------init-game------------------------------------------------
/***********************************************************************************
 * function : clears the display and starts the game
 * parameters : none
 * return : none
 ***********************************************************************************/
void init_game()
{
    clear_display();
    // setting default values for the game
    total_pixels = 0;
    current_snake_dir = UP;

    // creating snake body, 1 pixel at row 6, col 1
    create_new_pixel(0x61, 0, 0);

    // generating fruit for the snake at random location
    generate_fruit();

    // render current pixel data
    game_render();
}
// ------------------------------------------------update-frame------------------------------------------------
/***********************************************************************************
 * function : update the game by generating a new frame and rendering it.
 * parameters : none
 * return : none
 ***********************************************************************************/
void update_frame()
{
    uint8_t pos, xpos, ypos;

    // Get current joystick position from the joystick driver
    pos = get_joystick_pos();

    // Extract the X and Y co-ordinates of the snake head
    // Snake head is always at the 0th position in the pixel buffer
    xpos = (pixels_buffer[0].pos_xy & 0xF0) >> 4;
    ypos = pixels_buffer[0].pos_xy & 0xF;

    // Handle Joystick position UP
    if (pos == UP)
    {
        if (current_snake_dir == DOWN)
        {
            error_vibration();
        }
        else
        {
            //Update next snake head direction to be UP
            pixels_buffer[0].next_speedvector = UP;
            current_snake_dir = UP;
        }
    }
    // Handle Joystick position DOWN
    else if (pos == DOWN)
    {
        if (current_snake_dir == UP)
        {
            error_vibration();
        }
        else
        {
            //Update next snake head direction to be DOWN
            pixels_buffer[0].next_speedvector = DOWN;
            current_snake_dir = DOWN;
        }
    }
    // Handle Joystick position LEFT
    else if (pos == LEFT)
    {
        if (current_snake_dir == RIGHT)
        {
            error_vibration();
        }
        else
        {
            //Update next snake head direction to be LEFT
            pixels_buffer[0].next_speedvector = LEFT;
            current_snake_dir = LEFT;
        }
    }

    // Handle Joystick position RIGHT
    else if (pos == RIGHT)
    {
        if (current_snake_dir == LEFT)
        {
            error_vibration();
        }
        else
        {
            //Update next snake head direction to be RIGHT
            pixels_buffer[0].next_speedvector = RIGHT;
            current_snake_dir = RIGHT;
        }
    }
    else
    {
        pixels_buffer[0].next_speedvector = pixels_buffer[0].current_speedvector;
    }

    //Compute the new position of the pixels in the pixel buffer
    game_compute();
    //Render updated pixels to the display 
    game_render();
}

// ------------------------------------------------putchar-------------------------------------------------
/***********************************************************************************
 * function : Shows the eeprom menu and waits for user input
 * parameters : none
 * return : none
 ***********************************************************************************/
void game_compute() __critical
{

    uint8_t pixel_direction, xpos, ypos, i, next_pos;

    for (i = 0; i < total_pixels; i++)
    {
        pixel_direction = pixels_buffer[i].next_speedvector;
        xpos = (pixels_buffer[i].pos_xy & 0xF0) >> 4;
        ypos = pixels_buffer[i].pos_xy & 0xF;

        if (pixel_direction == UP)
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
                pixels_buffer[i].current_speedvector = pixel_direction;
            }
        }
        if (pixel_direction == DOWN)
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
                pixels_buffer[i].current_speedvector = pixel_direction;
            }
        }
        if (pixel_direction == LEFT)
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
                pixels_buffer[i].current_speedvector = pixel_direction;
            }
        }
        if (pixel_direction == RIGHT)
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
                pixels_buffer[i].current_speedvector = pixel_direction;
            }
        }
        if (pixel_direction == IDLE)
        {
            pixels_buffer[i].current_speedvector = pixel_direction;
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
    game_delay(150);
    disable_motor();
    disable_beeper();
    BEEP_CSR = 0x4C;
    enable_beeper();
    game_delay(150);
    enable_motor();
    game_delay(150);
    disable_motor();
    disable_beeper();
    BEEP_CSR = 0x4E;
    enable_beeper();
    game_delay(150);
    enable_motor();
    game_delay(300);
    disable_motor();
    disable_beeper();
    BEEP_CSR = 0x4A;

    init_game();
}

void error_vibration()
{
    enable_motor();
    game_delay(120);
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
        // conversion of pixel data to display format data
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
        game_delay(30);
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

void game_delay(long loop_iter)
{
    long i;

    for (i = 0; i < loop_iter * 1000; i++)
        ;
}
// ------------------------------------------------End-------------------------------------------------
