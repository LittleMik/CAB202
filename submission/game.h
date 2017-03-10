/*
 *  CAB202 Assignment 2: Falling Faces
 *	game.h
 *
 *	Michael Smallcombe (n9188207), October 2015
 */
#ifndef __GAME_H__
#define __GAME_H__
/*
* Standard Library
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*
* AVR Library
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
/*
* Teensy Library
*/
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"
/*
* USB Serial Library
*/
#include "usb_serial.h"
/*
* Constants
*/
//Timer
#define FREQUENCY 8000000.0
#define PRESCALER 1024.0
#define TIMER_SPEED = 1/(FREQUENCY/PRESCALER)
//Panel
#define PANEL_HEIGHT 8
//Misc Constants
#define CHAR_HEIGHT 6
#define CHAR_WIDTH 5
#define OUTPUT_SIZE 32
//Faces
#define NUM_FACES 3
#define FACE_WIDTH 10
#define FACE_HEIGHT 10
#define FACE_GAP 7
//Player
#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 6
//Speeds
#define SPEED_SLOW 0.25
#define SPEED_MEDIUM 0.5
#define SPEED_FAST 1
//Surfaces
#define EAST 0
#define NORTH 1
#define WEST 2
#define SOUTH 3
/*
* Game Struct
*/
typedef struct Game {
	//---Game Variables---//
	unsigned char over;
	int score;
	int level;
	int lives;
	int randSeed;
	//--------------------//

	//---Faces---//
	Sprite faces[NUM_FACES];
	int spawn_x[3];
	int spawn_y[3];
	int face_angle[3];
	double speed;
	//-----------//

	//---Player---//
	Sprite player;
	//------------//

} Game;

#endif
void game_loop(void);
void setup_game(Game * game);
void draw_panel(Game * game);
void draw_title(int y, char* string);
void display_menu(Game * game);
void draw_all(Game * game);
void check_end_game(Game * game);
void display_end_game(unsigned char win);
void flash_LED(int flashes, int delay);
void usb_output(char *output);