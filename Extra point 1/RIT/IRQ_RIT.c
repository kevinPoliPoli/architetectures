/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h" 
#include "../timer/timer.h"

/******************************************************************************
**                            PROTOTYPES
******************************************************************************/
void meal(uint16_t base_y);
void snack(uint16_t base_y);

/******************************************************************************
**                            EXTERN PROTOTYPES
******************************************************************************/
extern void write_big_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t color);
extern uint16_t base_y;
extern void setup();

/******************************************************************************
**                            BLOBAL VARIABLES
******************************************************************************/
int food = 0;
int select=0;		//25
int left=0; 			//27
int right=0;			//28
//volatile int down=0;

/******************************************************************************
**                            EXTERN VARIABLES
******************************************************************************/
extern uint8_t happiness;
extern uint8_t satiety;




void RIT_IRQHandler (void)
{					
	// JOISTICK SELECT
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		/* Joystick SELECT pressed */
		select++;
		switch(select){
			case 1:
					if(food==1){
						enable_timer(0);													
						disable_RIT();
						//reset meal button
						LCD_DrawLine(10, 280, 60, 280, Black);
						LCD_DrawLine(9, 279, 61, 279, White);
						LCD_DrawLine(10, 280, 10, 310, Black);
						LCD_DrawLine(9, 280, 9, 310, White);
						LCD_DrawLine(60, 280, 60, 310, Black);
						LCD_DrawLine(61, 280, 61, 310, White);
						LCD_DrawLine(10, 310, 60, 310, Black);
						LCD_DrawLine(9, 311, 61, 311, White);
					}else if(food==2){
						enable_timer(0);
						disable_RIT();						
						//reset snack button
						LCD_DrawLine(174, 280, 224, 280, Black);
						LCD_DrawLine(173, 279, 225, 279, White);
						LCD_DrawLine(174, 280, 174, 310, Black);
						LCD_DrawLine(173, 280, 173, 310, White);
						LCD_DrawLine(224, 280, 224, 310, Black);
						LCD_DrawLine(225, 280, 225, 310, White);
						LCD_DrawLine(174, 310, 224, 310, Black);
						LCD_DrawLine(173, 311, 225, 311, White);
					}else{
						//the pet ran away, press select to restart
						setup();
					}
					
				break;
			default:
				break;
		}
	}
	else{
			select=0;
	}
	
	// JOISTICK LEFT
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joystick LEFT pressed */
		left++;
		switch(left){
			case 1:
				//set meal button
				LCD_DrawLine(10, 280, 60, 280, Red);
				LCD_DrawLine(9, 279, 61, 279, Red);
				LCD_DrawLine(10, 280, 10, 310, Red);
				LCD_DrawLine(9, 280, 9, 310, Red);
				LCD_DrawLine(60, 280, 60, 310, Red);
				LCD_DrawLine(61, 280, 61, 310, Red);
				LCD_DrawLine(10, 310, 60, 310, Red);
				LCD_DrawLine(9, 311, 61, 311, Red);
			
				//reset snack button
				LCD_DrawLine(174, 280, 224, 280, Black);
				LCD_DrawLine(173, 279, 225, 279, White);
				LCD_DrawLine(174, 280, 174, 310, Black);
				LCD_DrawLine(173, 280, 173, 310, White);
				LCD_DrawLine(224, 280, 224, 310, Black);
				LCD_DrawLine(225, 280, 225, 310, White);
				LCD_DrawLine(174, 310, 224, 310, Black);
				LCD_DrawLine(173, 311, 225, 311, White);
	
				food = 1;
				meal(base_y);
				
				break;
			default:
				break;
		}
	}
	else{
			left=0;
	}
	
	// JOISTICK RIGHT
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joystick RIGHT pressed */
		right++;
		switch(right){
			case 1:
				//reset meal button
				LCD_DrawLine(10, 280, 60, 280, Black);
				LCD_DrawLine(9, 279, 61, 279, White);
				LCD_DrawLine(10, 280, 10, 310, Black);
				LCD_DrawLine(9, 280, 9, 310, White);
				LCD_DrawLine(60, 280, 60, 310, Black);
				LCD_DrawLine(61, 280, 61, 310, White);
				LCD_DrawLine(10, 310, 60, 310, Black);
				LCD_DrawLine(9, 311, 61, 311, White);
			
				//set snack button
				LCD_DrawLine(174, 280, 224, 280, Red);
				LCD_DrawLine(173, 279, 225, 279, Red);
				LCD_DrawLine(174, 280, 174, 310, Red);
				LCD_DrawLine(173, 280, 173, 310, Red);
				LCD_DrawLine(224, 280, 224, 310, Red);
				LCD_DrawLine(225, 280, 225, 310, Red);
				LCD_DrawLine(174, 310, 224, 310, Red);
				LCD_DrawLine(173, 311, 225, 311, Red);

				food = 2;
				snack(base_y);
			
				break;
			default:
				break;
		}
	}
	else{
			right=0;
	}
	
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            PROTOTYPES'S IMPLEMENTATION
******************************************************************************/
void meal(uint16_t base_y){
	//1
	write_big_pixel(20, base_y, White);
	write_big_pixel(28, base_y, Black);
	write_big_pixel(36, base_y, Black);
	write_big_pixel(44, base_y, White);
	base_y -=8;
	//2
	write_big_pixel(20, base_y, Black);
	write_big_pixel(44, base_y, Black);
	base_y -=8;
	//3
	write_big_pixel(20, base_y, Black);
	write_big_pixel(44, base_y, Black);
	base_y -=8;
	//4
	write_big_pixel(20, base_y, White);
	write_big_pixel(28, base_y, Black);
	write_big_pixel(36, base_y, Black);
	write_big_pixel(44, base_y, White);
}
void snack(uint16_t base_y){
	//1
	write_big_pixel(20, base_y, Black);
	write_big_pixel(28, base_y, Black);
	write_big_pixel(36, base_y, Black);
	write_big_pixel(44, base_y, Black);
	base_y -=8;
	//2
	write_big_pixel(20, base_y, Black);
	write_big_pixel(44, base_y, Black);
	base_y -=8;
	//3
	write_big_pixel(20, base_y, Black);
	write_big_pixel(44, base_y, Black);
	base_y -=8;
	//4
	write_big_pixel(20, base_y, Black);
	write_big_pixel(28, base_y, Black);
	write_big_pixel(36, base_y, Black);
	write_big_pixel(44, base_y, Black);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
