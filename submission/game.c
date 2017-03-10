/*
 *  CAB202 Assignment 2: Falling Faces
 *	game.c
 *
 *	Michael Smallcombe (n9188207), October 2015
 */

/*
* INCLUDE FILES
*/
#include "game.h"
#include "faces.h"
#include "player.h"

/*
* Game Loop - void
*/
void game_loop(void){
	unsigned char updatePlayer, updateFaces;
	Game game;
	//MENU
	display_menu(&game);
	//LEVEL SETUP
	setup_game(&game);
	setup_faces(&game);
	setup_player(&game);
	draw_all(&game);
	//GAME LOOP
	while(!game.over){
		//MOVE PLAYER
		move_player(&game);
		updatePlayer = update_player(&game);
		updateFaces = update_faces(&game);
		//CHECK UPDATE NEEDED
		if(updatePlayer || updateFaces == 1){
			draw_all(&game);
		}
		check_end_game(&game);
	}
	_delay_ms(5000);
	flash_LED(3, 100);
	game_loop();
}
/*
* Setup Game - requires game struct
*/
void setup_game(Game * game){
	game->score = 0;
	game->lives = 3;
	game->over = 0;
	char output[OUTPUT_SIZE];
	if(game->level == 3){
		//wait for serial connection
		while(!usb_serial_get_control()||!usb_configured()){
			clear_screen();
			draw_title(10, "Waiting for");
			draw_title(20, "USB Connection");
			show_screen();
		}
		clear_screen();
		draw_title(10, "Connected");
		sprintf(output, "%s", "Teensy Connection Successful");
		usb_output(output);
		show_screen();
	}
}
/*
* Draw All - requires game struct
*/
void draw_all(Game * game){
	clear_screen();
	draw_panel(game);
	draw_faces(game);
	draw_player(game);
	show_screen();	
}
/*
* Draw Status Panel - requires game struct
*/
void draw_panel(Game * game){
	char status[20];
	sprintf(status, "%s%i%s%i%s%i", "L:", game->lives, ", S:", game->score, ", Lvl:", game->level);
	draw_string(0, 0, status);
	draw_line(0, PANEL_HEIGHT, LCD_X, PANEL_HEIGHT);
}


/*
* Draw Centered Title - Requires y position and string
*/
void draw_title(int y, char* string){
	//find string length
	int x, i = 0, length = 0;
	//increase length until end of string reached
	//(B.Talbot, Queensland University of Technology, September 2015)
	while(string[i] != '\0'){
        i++;
		length++;
	}
	//adjust x
	x = (LCD_X/2) - ((length*CHAR_WIDTH)/2);
	draw_string(x, y, string);
}

/*
* Display Menu - Requires game struct
*/
void display_menu(Game * game){
	clear_screen();
	unsigned char inMenu;
	inMenu = 1;
	game->level = 1;
	int RB_pressed_confidence_level = 0;
	int LB_pressed_confidence_level = 0;
	int RB_pressed = 0;
	int LB_pressed = 0;
	while(inMenu){
		game->randSeed++;
		draw_title(0, "Menu");
		draw_title(10, "Select Level: RB");
		draw_title(20, "Start Level: LB");
		draw_title(40, "Level: 1");
		switch(game->level){
		case 1:
			draw_title(40, "Level: 1");
			break;
		case 2:
			draw_title(40, "Level: 2");
			break;
		case 3:
			draw_title(40, "Level: 3");
			break;
		}
		//Right Button Press
		if(!bit_is_clear(PINB, 1)){
			RB_pressed_confidence_level++;
			if(RB_pressed_confidence_level > 3 && RB_pressed == 0){
				//button pressed
				RB_pressed = 1;
				//cycle level
				if(game->level < 3){
					game->level++;
				}else{
					game->level = 1;
				}
				//reset press confidence level
				RB_pressed_confidence_level = 0;
			}
		}else{
			RB_pressed = 0;
			RB_pressed_confidence_level = 0;
		}
		//Left Button Press
		if(!bit_is_clear(PINB, 0)){
			LB_pressed_confidence_level++;
			if(LB_pressed_confidence_level > 3 && LB_pressed == 0){
				//button pressed
				inMenu = 0;
				//reset press confidence level
				LB_pressed_confidence_level = 0;
			}
		}else{
			LB_pressed = 0;
			LB_pressed_confidence_level = 0;
		}
		show_screen();
	}
}
/*
* Check End Game - requires game struct
*/
void check_end_game(Game * game){
	if(game->score >= 20){
		game->over = 1;
		//winning overlay
		display_end_game(1);
	}else if(game->lives <= 0){
		game->over = 1;
		//game over overlay
		display_end_game(0);
	}else{
		game->over = 0;
	}
}
/*
* End Game Overlay - requires game struct and bool win
*/
void display_end_game(unsigned char win){
	clear_screen();
	if(win == 1){
		draw_title(20, "You Win!");
		show_screen();
		flash_LED(5, 100);
	}else{
		draw_title(20, "Game Over");
		show_screen();
	}
}
/*
* Flash LEDs - requires number of flashes and delay in milliseconds
*/
void flash_LED(int flashes, int delay){
	for(int i = 0; i < flashes*2; i++){
		PORTB ^= (1 << PB2);
		PORTB ^= (1 << PB3);
		_delay_ms(delay);
	}
}
/*
* USB Output - requires output string
*/
void usb_output(char *output){
	int i = 0;
	//send string to putty
	//B.Talbot, September 2015
	//send each character in string until end of string is reached
	while(output[i] != '\0'){
		usb_serial_putchar(output[i]);
		i++;
	}
	usb_serial_putchar('\n');
	usb_serial_putchar('\r');
}