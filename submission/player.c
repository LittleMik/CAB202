/*
 *  CAB202 Assignment 2: Falling Faces
 *	player.c
 *
 *	Michael Smallcombe (n9188207), October 2015
 */

/*
* INCLUDE FILES
*/
#include "player.h"

unsigned char bitmap_player[6] = {
	0b00100000,
	0b10101000,
	0b11011000,
	0b11011000,
	0b11111000,
	0b10101000,
};
/*
* Setup Player - requires game struct
*/
void setup_player(Game * game){
	init_sprite(&(game->player), LCD_X/2, LCD_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, bitmap_player);
	game->player.is_visible = 0;
	spawn_player(game);
	if(game->level == 3){
		usb_output("Use the 'a', 'w', 's' and 'd' keys to move");
	}
}
/*
* Spawn Player - requires game struct
*/
void spawn_player(Game * game){
	//show sprite
	game->player.is_visible = 1;
}
/*
* Update Player - requires game struct
*/
unsigned char update_player(Game * game){
	int current_x, current_y;
	unsigned char player_updated;
	if(game->level != 2){
		player_updated = 0;
		current_x = game->player.x;
		current_y = game->player.y;
		game->player.x += game->player.dx;
		game->player.y += game->player.dy;
		if((current_x != game->player.x)||(current_y != game->player.y)){
			player_updated = 1;
		}
	}else{
		player_updated = 1;
	}
	return player_updated;
}
/*
* Move Player - requires struct
*/
void move_player(Game * game){
	uint16_t adc_result;
	int16_t input;
	switch(game->level){
	case 1:
		if(!bit_is_clear(PINB, 1)){
			//right button
			if((game->player.x + PLAYER_WIDTH) < LCD_X){
				game->player.dx = 1;
			}else{
				game->player.dx = 0;
			}
			//game->player.x += game->player.dx;
		}else if(!bit_is_clear(PINB, 0)){
			//left button
			if(game->player.x > 0){
				game->player.dx = -1;
			}else{
				game->player.dx = 0;
			}
			//game->player.x += game->player.dx;
		}else{
			//idle
			game->player.dx = 0;
		}
		break;
	case 2:
		//Potentiometer Calculations
		adc_result = adc_read();
		//maximum x value for player sprite before going off screen
		int max_x = LCD_X - PLAYER_WIDTH;
		//maximum value of adc == 1023
		double max_adc = 1023.0;
		//Find adc value in a range from 0 -> 1 and multiply the result by the maximum possible x position to identify
		//the location of the player on the screen
		//To move the player right on a clockwise motion and left on an anti-clockwise motion, the result is inversed
		//by subtracting the previous result from the maximum x value
 	    game->player.x = (max_x)-((adc_result / max_adc)*(long)(max_x));
		break;
	case 3:
		input = usb_serial_getchar();
		//fush input after input recieved (stops extra input coming through from multiple key presses)
		usb_serial_flush_input();
		switch(input){
		case 'a':
			//left
			if(game->player.x > 0){
				game->player.dx = -1;
			}else{
				game->player.dx = 0;
			}
			break;
		case 'd':
			//right
			if((game->player.x + PLAYER_WIDTH) < LCD_X){
				game->player.dx = 1;
			}else{
				game->player.dx = 0;
			}
			break;
		case 'w':
			//up
			if(game->player.y > (PANEL_HEIGHT + 1)){
				game->player.dy = -1;
			}else{
				game->player.dy = 0;
			}
			break;
		case 's':
			//down
			if((game->player.y + PLAYER_HEIGHT) < LCD_Y){
				game->player.dy = 1;
			}else{
				game->player.dy = 0;
			}
			break;
		default:
			//idle
			game->player.dy = 0;
			game->player.dx = 0;
			break;
		}
		break;
	}
}
/*
* Draw Player - requires game struct
*/
void draw_player(Game * game){
	draw_sprite(&(game->player));
}
/*
* ADC Read (CAB202 Week 11 Tutorial, 2015)
*/
uint16_t adc_read(void){
	//Start Converstion = set ADSC to 1
    ADCSRA |= (1<<ADSC);
 
	//Wait until conversion finishes (When ADSC == 0)
    while(ADCSRA & (1<<ADSC));
	//return ADC value
    return (ADC);
}