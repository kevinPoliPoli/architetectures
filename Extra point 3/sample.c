/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            start and restart of tamagotchi functionalities
** Modified by:             Kevin Cardinale
** Modified date:           02-01-2023
** Version:                 v1.0
** Descriptions:            tamagotchi implementation
** Correlated files: 				timer.h, RIT.h, joystick.h, GLCD.h, LPC17xx.h
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include <stdio.h>
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
extern void write_idle_1(uint16_t base_y); 									/* defined in IRQ_timer.c */ 
extern void refresh_LCD(int* hr, int* min, int* sec); 			/* defined in IRQ_timer.c */ 
extern void write_batteries(void); 													/* defined in IRQ_timer.c */ 
extern void write_happiness(uint8_t happiness); 						/* defined in IRQ_timer.c */ 
extern void write_satiety(uint8_t satiety); 								/* defined in IRQ_timer.c */ 
extern void write_or_clean_walk_cry(uint16_t base_y, uint16_t x_offset, uint16_t color, uint16_t color2); 		/* defined in IRQ_timer.c */ 

/******************************************************************************
**                            GLOBAL VARIABLES
******************************************************************************/
char time_in_char[48] = ""; /* used to write time in the LCD */ 

/******************************************************************************
**                            EXTERN VARIABLES
******************************************************************************/
extern int position; 									/* defined in IRQ_timer.c */ 
extern int food;  										/* defined in IRQ_RIT.c  */ 
extern int hr, min, sec; 							/* defined in IRQ_timer.c  */ 
extern uint16_t base_y; 							/* defined in IRQ_timer.c  */ 
extern uint8_t happiness;							/* defined in IRQ_timer.c  */ 
extern uint8_t satiety; 							/* defined in IRQ_timer.c */ 
extern int animation; 								/* defined in IRQ_timer.c  */ 
extern int end_game; 									/* defined in IRQ_RIT.c  */ 
extern int eating; 										/* defined in IRQ_timer.c  */ 


/*******************************************************************************
* Function Name  : main
* Description    : pheriferals, system and game initialization
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void){

  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();									/* LCD Initialization         			  */
	joystick_init();											/* Joystick Initialization            */
	
	setup();															/* start/restart routine	            */
		
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	{
		__ASM("wfi");
  }
}






/******************************************************************************
**                            PROTOTYPES'S IMPLEMENTATION
******************************************************************************/

/*******************************************************************************
* Function Name  : setup
* Description    : used to start a new game, generate part of the GUI, start timers
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void setup(void){
	
	char time_in_char[48] = "";
	
	//reset variables
	happiness = 1; // [1,4]
	satiety = 1; // [1,4]
	animation = 0;
	hr=0, min=0, sec=4;
	position=0;
	food=0;
	end_game=0;
	eating = 0;
	
	init_timers();												/* Initialize/reset all timers        */
	
	//reset timers
	enable_RIT();													/* RIT enabled												*/
	enable_timer(1);											/* 1 second                           */
	LCD_Clear(White);
	sprintf(time_in_char, "      Age: 00 : 00 : 00        ");
	GUI_Text(0, 10, (uint8_t *) time_in_char, Black, White);

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




/*******************************************************************************
* Function Name  : init_timers
* Description    : initialization of RIT, timer 0, timer 1, timer 2
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void init_timers(void){
	//joystick functionalites
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	//walk/eat animation
	init_timer(0, 0x367EE4); 						    /* 1/7 second */
	//timer to handle animations, gui updating, time updating 
	init_timer(1, 0x17D7840); 						    /* 1s * 25MHz = 0x17D7840 */
	//to handle exit condition
	init_timer(2, 0x367EE4); 						    /* 1/7 second */
}




/*******************************************************************************
* Function Name  : end
* Description    : generate the GUI of the game when a game ends, resets some timers 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void end(void){
	
	//reset variables
	food=0;
	
	//clean screen
	LCD_Clear(Black);
	
	//time
	reset_timer(1);
	sprintf(time_in_char, "      Age: %02d : %02d : %02d         ", hr, min, sec);
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
