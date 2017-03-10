/*
 *  CAB202 Assignment 2: Falling Faces
 *	player.h
 *
 *	Michael Smallcombe (n9188207), October 2015
 */
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "game.h"
#endif
void setup_player(Game * game);
void spawn_player(Game * game);
unsigned char update_player(Game * game);
void move_player(Game * game);
void draw_player(Game * game);
uint16_t adc_read(void);