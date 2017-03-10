/*
 *  CAB202 Assignment 2: Falling Faces
 *	main.c
 *
 *	Michael Smallcombe (n9188207), October 2015
 */

/*
* INCLUDE FILES
*/
#include "game.h"

void init_hardware(void);
void title_overlay(void);
/*
 * Main
 */
int main() {
	//SETUP MICROCONTROLLER
    set_clock_speed(CPU_8MHz);
    //Setup the hardware
    init_hardware();
	
	//TITLE
	title_overlay();

	//GAME LOOP
	game_loop();

	//END PROGRAM
    return 0;
}
/*
 * Initiate Hardware - Setup Teensy - Requires game struct
 * (B.Talbot, Queensland University of Technology, September 2015)
 */
void init_hardware(void){
    //Initialise LCD Screen
    LCDInitialise(LCD_DEFAULT_CONTRAST);

    //Initialise LEDs as outputs
    DDRB |= ((1 << PB2) | (1 << PB3));

	//POTENTIOMETER - (CAB202 Week 11 Tutorial, 2015)
	//Configure for Teensy
	//Set AVcc as reference (01), 0 left adjust (0), ADC0 as input (00000)
    ADMUX = 0b01000000;

	//Clock Speed = 8000000
	//Prescaler = 128
	//Clock Speed/Prescaler = Sampling Frequency (between 50kHz -> 200kHz)
    //8000000/128 = 62.5kHz
	//Enabled ADC = ADEN, Set Prescaler to 128 (largest) = ADPS2, ADPS1, ADPS0 (all enabled)
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

	//Initialise USB
	usb_init();
}
/*
* Display Game and Student Details
*/
void title_overlay(void){
	//TITLE
	clear_screen();
	//display details for 2 seconds
	draw_title(0, "CAB202 - A2");
	draw_title(10, "Falling Faces");
	draw_title(20, "Michael");
	draw_title(30, "Smallcombe");
	draw_title(40, "(N9188207)");
	show_screen();
	flash_LED(3, 100);
	//delay for 2 seconds
	_delay_ms(2000);
}