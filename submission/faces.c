/*
 *  CAB202 Assignment 2: Falling Faces
 *	faces.c
 *
 *	Michael Smallcombe (n9188207), October 2015
 */

/*
* INCLUDE FILES
*/
#include "faces.h"

unsigned char bitmap_faces[NUM_FACES][20] = {
	{
		0b11111111, 0b11000000,
		0b10000000, 0b01000000,
		0b10110011, 0b01000000,
		0b10110011, 0b01000000,
		0b10000000, 0b01000000,
		0b10100001, 0b01000000,
		0b10100001, 0b01000000,
		0b10011110, 0b01000000,
		0b10000000, 0b01000000,
		0b11111111, 0b11000000,
	},
	{
		0b11111111, 0b11000000,
		0b10000000, 0b01000000,
		0b10110000, 0b01000000,
		0b10110011, 0b01000000,
		0b10000011, 0b01000000,
		0b10100000, 0b01000000,
		0b10111111, 0b01000000,
		0b10011100, 0b01000000,
		0b10000000, 0b01000000,
		0b11111111, 0b11000000,
	},
	{
		0b11111111, 0b11000000,
		0b10000000, 0b01000000,
		0b11110011, 0b11000000,
		0b10100001, 0b01000000,
		0b10000000, 0b01000000,
		0b10000000, 0b01000000,
		0b10011110, 0b01000000,
		0b10100001, 0b01000000,
		0b10000000, 0b01000000,
		0b11111111, 0b11000000,
	},
};
/*
* Setup Faces - requires game struct
*/
void setup_faces(Game * game){
	//seed random
	srand(game->randSeed);
	//initialise sprites and set to invisible
	for(int i = 0; i < NUM_FACES; i++){
		init_sprite(&(game->faces[i]), i*FACE_WIDTH, -FACE_HEIGHT, FACE_WIDTH, FACE_HEIGHT, bitmap_faces[i]);
		game->faces[i].is_visible = 0;
	}
	game->speed = SPEED_SLOW;
	spawn_faces(game);
}
/*
* Spawn Faces - requires game struct
*/
void spawn_faces(Game * game){
	random_spawns(game);
	for(int i = 0; i < NUM_FACES; i++){
		//set spawn position
		game->faces[i].x = game->spawn_x[i];
		game->faces[i].y = game->spawn_y[i];
		//set speed
		if(game->level == 3){
			//dx = adjacent, dy = opposite, speed = hypotenuse
			//dx = cos(angle) * speed
			//dy = sin(angle) * speed
			game->faces[i].dx = cos(game->face_angle[i]);
			game->faces[i].dy = -1 * sin(game->face_angle[i]);
		}else{
			game->faces[i].dx = 0;
			game->faces[i].dy = 1;
		}
		//show sprite
		game->faces[i].is_visible = 1;
	}
}
/*
* Spawn Face - requires game struct and face number
*/
void spawn_face(Game * game, int faceNum){
	random_spawn(game, faceNum);
	//set spawn position
	game->faces[faceNum].x = game->spawn_x[faceNum];
	game->faces[faceNum].y = game->spawn_y[faceNum];
	//set speed
	if(game->level == 3){
		//dx = adjacent, dy = opposite, speed = hypotenuse
		//dx = cos(angle) * speed
		//dy = sin(angle) * speed (assume speed is 1, speed can range from 0.25 <> 1, multiplied in update_faces)
		game->faces[faceNum].dx = cos(game->face_angle[faceNum]);
		game->faces[faceNum].dy = -1 * sin(game->face_angle[faceNum]);
	}else{
		game->faces[faceNum].dx = 0;
		game->faces[faceNum].dy = 1;
	}
	//show sprite
	game->faces[faceNum].is_visible = 1;
}
/*
* Generate Random Spawn Position - requires game struct
*/
void random_spawns(Game * game){
	unsigned char canSpawn;
	//random range = rand()%(max - min) + min	
	do{
		canSpawn = 1;
		for(int i = 0; i < NUM_FACES; i++){
			//SET X SPAWN
			game->spawn_x[i] = rand()%(LCD_X - FACE_WIDTH);

			//Level Differences
			if(game->level == 3){
				//SET Y SPAWN
				game->spawn_y[i] = rand()%(LCD_Y - ((PANEL_HEIGHT + 1) + FACE_HEIGHT + PLAYER_HEIGHT)) + (PANEL_HEIGHT + 1);
				
				//SET ANGLE using dx and dy
				game->face_angle[i] = rand()%360;

				//CHECK SAFE SPAWN
				//check away from player
				if(((game->player.x < game->spawn_x[i] + FACE_WIDTH + FACE_GAP) && (game->player.x + PLAYER_WIDTH + FACE_GAP > game->spawn_x[i]))&&((game->player.y + PLAYER_HEIGHT + FACE_GAP > game->spawn_y[i]) && (game->player.y < game->spawn_y[i] + FACE_HEIGHT + FACE_GAP))){
					canSpawn = 0;
				}
				//check away from other faces
				for(int j = 0; j < NUM_FACES; j++){
					if(i != j){
						//check x and y position available
						if((game->spawn_x[j] < game->spawn_x[i] + FACE_WIDTH + FACE_GAP) && (game->spawn_x[j] + FACE_WIDTH + FACE_GAP > game->spawn_x[i])){
							if((game->spawn_y[j] + FACE_HEIGHT + FACE_GAP > game->spawn_y[i]) && (game->spawn_y[j] < game->spawn_y[i] + FACE_HEIGHT + FACE_GAP)){
								//redo random
								canSpawn = 0;
							}
						}
					}
				}
			}else{
				//SET Y SPAWN
				game->spawn_y[i] = PANEL_HEIGHT + 1;

				//CHECK SAFE SPAWN
				//check away from other faces
				for(int j = 0; j < NUM_FACES; j++){
					if(i != j){
						//check x position available
						if((game->spawn_x[j] < game->spawn_x[i] + FACE_WIDTH + FACE_GAP) && (game->spawn_x[j] + FACE_WIDTH + FACE_GAP > game->spawn_x[i])){
								//redo random
								canSpawn = 0;
						}
					}
				}
			}
		}
	}while(!canSpawn);
}
/*
* Set Random Spawn for individual face - requires game struct
*/
void random_spawn(Game * game, int faceNum){
	unsigned char canSpawn;
	do{
		canSpawn = 1;
		//SET X SPAWN
		game->spawn_x[faceNum] = rand()%(LCD_X - FACE_WIDTH);

		//SET Y SPAWN
		game->spawn_y[faceNum] = rand()%(LCD_Y - ((PANEL_HEIGHT + 1) + FACE_HEIGHT)) + (PANEL_HEIGHT + 1);
				
		//SET ANGLE
		game->face_angle[faceNum] = rand()%360;

		//CHECK SAFE SPAWN
		//check away from player
		if(((game->player.x < game->spawn_x[faceNum] + FACE_WIDTH + FACE_GAP) && (game->player.x + PLAYER_WIDTH + FACE_GAP > game->spawn_x[faceNum]))&&((game->player.y + PLAYER_HEIGHT + FACE_GAP > game->spawn_y[faceNum]) && (game->player.y < game->spawn_y[faceNum] + FACE_HEIGHT + FACE_GAP))){
			canSpawn = 0;
		}
		//check away from player
		for(int j = 0; j < NUM_FACES; j++){
			if(faceNum != j){
				if((game->faces[j].x < game->spawn_x[faceNum] + FACE_WIDTH + FACE_GAP) && (game->faces[j].x + FACE_WIDTH + FACE_GAP > game->spawn_x[faceNum])){
					if((game->faces[j].y + FACE_HEIGHT + FACE_GAP > game->spawn_y[faceNum]) && (game->faces[j].y < game->spawn_y[faceNum] + FACE_HEIGHT + FACE_GAP)){
						//redo random
						canSpawn = 0;
					}
				}
			}
		}
	}while(!canSpawn);
}
/*
* Update Faces - requires game struct
*/
unsigned char update_faces(Game * game){
	int current_x, current_y;
	unsigned char faces_updated, collision;
	faces_updated = 0;
	collision = 0;

	for(int i = 0; i < NUM_FACES; i++){
		current_x = game->faces[i].x;
		current_y = game->faces[i].y;
		game->faces[i].x += (game->faces[i].dx * game->speed);
		game->faces[i].y += (game->faces[i].dy * game->speed);
		if((current_x != game->faces[i].x)||(current_y != game->faces[i].y)){
			faces_updated = 1;
		}
	}

	if(check_collision(game) == 1){
		faces_updated = 1;
	}
	if(check_respawn(game) == 1){
		faces_updated = 1;
	}
	return faces_updated;
}
/*
* Check Face/Player Collision - requires game struct
*/
unsigned char check_collision(Game * game){
	unsigned char collision_detected = 0;
	for(int i = 0; i < NUM_FACES; i++){
		if(game->faces[i].is_visible == 1){
			//check player collision
			if(((game->faces[i].y + FACE_HEIGHT >= game->player.y)&&(game->faces[i].y <= game->player.y + PLAYER_HEIGHT))&&((game->faces[i].x + FACE_WIDTH >= game->player.x)&&(game->faces[i].x <= game->player.x + PLAYER_WIDTH))){
					collision_player(game, i);
					collision_detected = 1;
			}

			if(game->level == 3){

				//check wall collision
				if(game->faces[i].x + FACE_WIDTH >= LCD_X){
					bounce_face(game, i, EAST);
					collision_detected = 1;
				}else if(game->faces[i].y <= PANEL_HEIGHT + 1){
					bounce_face(game, i, NORTH);
					collision_detected = 1;
				}else if(game->faces[i].x <= 0){
					bounce_face(game, i, WEST);
					collision_detected = 1;
				}else if(game->faces[i].y + FACE_HEIGHT >= LCD_Y){
					bounce_face(game, i, SOUTH);
					collision_detected = 1;
				}

				//check face collision
				for(int j = 0; j < NUM_FACES; j++){
					if(i != j){
						//EAST-WEST
						if((game->faces[j].y + FACE_HEIGHT - 2 > game->faces[i].y) && (game->faces[j].y < game->faces[i].y + FACE_HEIGHT - 2)){
							if((game->faces[j].x <= game->faces[i].x + FACE_WIDTH) && (game->faces[j].x > game->faces[i].x)){
								//j bounce on east surface
								bounce_face(game, i, EAST);
								//i bounce on west surface
								bounce_face(game, j, WEST);
							}
							if((game->faces[i].x <= game->faces[j].x + FACE_WIDTH) && (game->faces[i].x > game->faces[j].x)){
								//i bounce on east surface
								bounce_face(game, j, EAST);
								//j bounce on west surface
								bounce_face(game, i, WEST);
							}

						}
						//NORTH-SOUTH
						if((game->faces[j].x < game->faces[i].x + FACE_WIDTH - 2)&&(game->faces[j].x + FACE_WIDTH - 2 > game->faces[i].x)){
							if((game->faces[j].y <= game->faces[i].y + FACE_HEIGHT) && (game->faces[j].y > game->faces[i].y)){
								//j bounce on north surface
								bounce_face(game, j, NORTH);
								//i bounce on south surface
								bounce_face(game, i, SOUTH);
							}
							if((game->faces[i].y <= game->faces[j].y + FACE_HEIGHT) && (game->faces[i].y > game->faces[j].y)){
								//i bounce on north surface
								bounce_face(game, i, NORTH);
								//j bounce on south surface
								bounce_face(game, j, SOUTH);
							}
						}
					}
				}
			}
		}
	}
	return collision_detected;
}
/*
* Player Collision Behaviour - requires game struct and faceNum
*/
void collision_player(Game * game, int faceNum){
	switch(faceNum){
	case 0:
		game->score += 2;
		break;
	case 1:
		if(game->speed == SPEED_SLOW){
			game->speed = SPEED_MEDIUM;
		}else{
			game->speed = SPEED_FAST;
		}
		break;
	case 2:
		game->lives--;
		break;
	}
	game->faces[faceNum].is_visible = 0;
}
/*
* Check Respawn Faces - requires game struct
*/
unsigned char check_respawn(Game * game){
	if(game->level != 3){
		if(game->faces[0].y + FACE_HEIGHT >= LCD_Y){
			spawn_faces(game);
			return 1;
		}
	}else{
		for(int i = 0; i < NUM_FACES; i++){
			if(game->faces[i].is_visible == 0){
				spawn_face(game, i);
				return 1;
			}
		}
	}
	return 0;
}
/*
* Draw Faces - requires game struct
*/
void draw_faces(Game * game){
	for(int i = 0; i < NUM_FACES; i++){
		if(game->faces[i].is_visible == 1){
			draw_sprite(&(game->faces[i]));
		}
	}
}
/*
* Bounce Face - requires game struct, face number and surface value
*/
void bounce_face(Game * game, int faceNum, int surface){
	//update face heading according to rebound surface
	switch(surface){
	case EAST:
		if(game->faces[faceNum].dx > 0){
			game->faces[faceNum].dx = game->faces[faceNum].dx * -1;
		}
		break;
	case NORTH:
		if(game->faces[faceNum].dy < 0){
			game->faces[faceNum].dy = game->faces[faceNum].dy * -1;
		}
		break;
	case WEST:
		if(game->faces[faceNum].dx < 0){
			game->faces[faceNum].dx = game->faces[faceNum].dx * -1;
		}
		break;
	case SOUTH:
		if(game->faces[faceNum].dy > 0){
			game->faces[faceNum].dy = game->faces[faceNum].dy * -1;
		}
		break;
	}
}