/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


/******************************************************************************
**                            PROTOTYPES
******************************************************************************/
void setup(void);
void end(void);
void init_timers(void);
/******************************************************************************
**                            EXTERN PROTOTYPES
******************************************************************************/
extern void write_idle_1(uint16_t base_y);
extern void refresh_LCD(uint8_t* hr, uint8_t* min, uint8_t* sec);
extern void write_batteries(void);
extern void write_happiness(uint8_t happiness);
extern void write_satiety(uint8_t satiety);
extern void write_or_clean_walk_cry(uint16_t base_y, uint16_t x_offset, uint16_t color, uint16_t color2);

/******************************************************************************
**                            GLOBAL VARIABLES
******************************************************************************/
char time_in_char[48] = "";

/******************************************************************************
**                            EXTERN VARIABLES
******************************************************************************/
extern int position;
extern int food;
extern uint8_t hr, min, sec;
extern uint16_t base_y;
extern uint8_t happiness; // scale: [1,4]
extern uint8_t satiety; // scale: [1,4]
extern int animation;
extern uint8_t hr, min, sec;




int main(void){

  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();									/* LCD Initialization         			  */
	joystick_init();											/* Joystick Initialization            */
	
	init_timers();												/* Initialize/reset all timers        */
	
	setup();															/* start/restart routine	            */
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	{
		__ASM("wfi");
  }
}


void setup(void){
	
	//reset variables
	happiness = 2; // [1,4]
	satiety = 1; // [1,4]
	animation = 0;
	hr=0, min=0, sec=0;
	position=0;
	food=0;
	
	//reset timers
	enable_RIT();													/* RIT enabled												*/
	enable_timer(1);											/* 1 second                           */
		
	LCD_Clear(White);
	refresh_LCD(&hr, &min, &sec);
	//labels
	GUI_Text(30, 30, (uint8_t *) "Happiness", Black, White);
	GUI_Text(150, 30, (uint8_t *) "Satiety", Black, White);
	write_batteries();
	write_happiness(happiness);
	write_satiety(satiety);
	write_idle_1(base_y);
	
	//meal button
	LCD_DrawLine(10, 280, 60, 280, Black);
	LCD_DrawLine(10, 280, 10, 310, Black);
	LCD_DrawLine(60, 280, 60, 310, Black);
	LCD_DrawLine(10, 310, 60, 310, Black);

	//snack button
	LCD_DrawLine(174, 280, 224, 280, Black);
	LCD_DrawLine(174, 280, 174, 310, Black);
	LCD_DrawLine(224, 280, 224, 310, Black);
	LCD_DrawLine(174, 310, 224, 310, Black);
	
	GUI_Text(20, 290, (uint8_t *) "Meal", Black, White);
	GUI_Text(180, 290, (uint8_t *) "Snack", Black, White);
	
	return;
}

void init_timers(void){
	//joystick functionalites
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	//walk/eat animation
	init_timer(0, 0x367EE4); 						    /* 1/4 second */
	//timer to handle animations, gui updating, time updating 
	init_timer(1, 0x17D7840); 						    /* 100 * 200 * 500us * 25MHz = 0x17D7840 */
	//to handle exit condition
	init_timer(2, 0x367EE4); 						    /* 1/7 second */
}


void end(void){
	
	//reset variables
	food=0;
	
	//clean screen
	LCD_Clear(Black);
	
	//time
	reset_timer(1);
	sprintf(time_in_char, "      Age: %02d : %02d : %02d         ", hr, min, sec-1);
	GUI_Text(0, 10, (uint8_t *) time_in_char, White, Black);
	
	//message
	GUI_Text(10, 50, (uint8_t *) "    Your pet ran away :(", White, Black);
	GUI_Text(10, 70, (uint8_t *) "  Press select to restart", White, Black);
	
	//pet
	write_or_clean_walk_cry(base_y-8, 0, White, Blue2);
	
	//restart button
	LCD_DrawLine(0, 280, 240, 280, Red);
	LCD_DrawLine(0, 279, 240, 279, Red);
	LCD_DrawLine(0, 280, 0, 320, Red);
	LCD_DrawLine(1, 280, 1, 320, Red);
	LCD_DrawLine(0, 319, 240, 319, Red);
	LCD_DrawLine(0, 318, 240, 318, Red);
	LCD_DrawLine(239, 319, 239, 280, Red);
	LCD_DrawLine(238, 318, 238, 280, Red);
	GUI_Text(90, 290, (uint8_t *) "Restart", White, Black);
	
	enable_RIT();
}

/******************************************************************************
**                            End Of File
******************************************************************************/
