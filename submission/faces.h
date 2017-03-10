/*
 *  CAB202 Assignment 2: Falling Faces
 *	faces.h
 *
 *	Michael Smallcombe (n9188207), October 2015
 */
#ifndef __FACES_H__
#define __FACES_H__
#include "game.h"
#endif
void setup_faces(Game * game);
void spawn_faces(Game * game);
void spawn_face(Game * game, int faceNum);
void random_spawns(Game * game);
void random_spawn(Game * game, int faceNum);
unsigned char update_faces(Game * game);
unsigned char check_respawn(Game * game);
void draw_faces(Game * game);
unsigned char check_collision(Game * game);
void collision_player(Game * game, int faceNum);
void bounce_face(Game * game, int faceNum, int surface);