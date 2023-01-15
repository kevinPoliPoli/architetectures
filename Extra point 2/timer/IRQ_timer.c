/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  02-01-2023
** Last Version:        V1.00
** Descriptions:        functions to manage T0, T1, T2 and T3 interrupts
** Creator: 						Kevin Cardinale
** Correlated files:    timer.h, RIT.h, GLCD.h, LPC17xx.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../music/music.h"


/******************************************************************************
**                            PROTOTYPES
******************************************************************************/
void write_clean_cuddles(uint16_t color);
void write_idle_1(uint16_t base_y);
void write_idle_2(uint16_t base_y);
void write_idle_3(uint16_t base_y);
void clean_idle_1(uint16_t base_y);
void write_walk_1(uint16_t base_y);
void write_walk_2(uint16_t base_y);
void write_walk_3(uint16_t base_y);
void clean_walk_1(uint16_t base_y);
void clean_walk_2(uint16_t base_y);
void clean_walk_3(uint16_t base_y);
void write_or_clean_walk(uint16_t base_y, uint16_t x_offset, uint16_t color);
void write_or_clean_walk_cry(uint16_t base_y, uint16_t x_offset, uint16_t color, uint16_t color2);
void write_eat(void);
void clean_eat(void);
void show_animation(int animation);
void write_big_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t color);
void refresh_LCD(int* hr, int* min, int* sec);
void increase_happiness(void);
void increase_satiety(void);
void decrease_happiness(void);
void decrease_satitety(void);
void write_batteries(void);
void write_happiness(uint8_t happiness);
void write_satiety(uint8_t satiery);
void check_end(void);
void clean_food(void);
void clean_walk(void);

/******************************************************************************
**                            EXTERN PROTOTYPES
******************************************************************************/
extern void setup(void);							/* defined in sample.c */ 
extern void end(void);								/* defined in sample.c */ 

/******************************************************************************
**                            GLOBAL VARIBALES
******************************************************************************/
uint8_t happiness;										/* happiness scale [1,4] */ 
uint8_t satiety; 											/* satiety scale [1,4] */ 
uint16_t base_y = 264; 								/* base of generation of the pet*/ 
int animation;												/* to handle the basic animation  */ 
int hr, min, sec;											/* time measures */ 
int eating; 													/* 1 if eating procedure has begun 0 otherwise */ 
int position=0;												/* to handle the eat animation */ 
int run_away=0;												/* to handle the run away animation */
int sw_count=0;
int cuddles=0;
int i=0;
uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

/******************************************************************************
**                            EXTERN VARIABLES
******************************************************************************/
extern int food;											/* defined in IRQ_RIT.c */ 
extern int end_game;									/* defined in IRQ_RIT.c */ 
extern int runaway_sound;
extern int cuddles_sound;


/******************************************************************************
**                            TIMER HANDLERS
******************************************************************************/

/*******************************************************************************
* Function Name  : TIMER0_IRQHandler
* Description    : walk near the food and eat it functionalities
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIMER0_IRQHandler (void){
	
	if(food>0){ //eat routine
		switch(position){
		case 0:
			disable_RIT();
			position++;
			break;
		case 1:
			write_or_clean_walk(base_y, 0, White);
			write_or_clean_walk(base_y, 8, Black);
			position++;
			break;
		case 2:
			write_or_clean_walk(base_y, 8, White);
			write_or_clean_walk(base_y, 16, Black);
			position++;
			break;
		case 3:
			write_or_clean_walk(base_y, 16, White);
			write_or_clean_walk(base_y, 24, Black);
			position++;
			break;
		case 4:
			write_or_clean_walk(base_y, 24, White);
			write_eat();
			position++;
			break;
		case 5:
			clean_eat();
			write_or_clean_walk(base_y, 24, Black);
			position++;
			break;
		case 6:
			write_or_clean_walk(base_y, 24, White);
			write_eat();
			position++;
			break;
		case 7:
			clean_eat();
			write_or_clean_walk(base_y, 24, Black);
			position++;
			break;
		case 8:
			write_or_clean_walk(base_y, 24, White);
			write_eat();
			position++;
			break;
		case 9:
			clean_eat();
			write_or_clean_walk(base_y, 24, Black);
			clean_food();
			position++;
			break;
		case 10:
			if(food==1){
				increase_happiness();
				write_happiness(happiness);
			
			}else{
				increase_satiety();
				write_satiety(satiety);
			}
			position++;
			break;
		case 11:
			position++; //wait
			break;
		default:
			disable_timer(0);
			enable_RIT();
			base_y=264;
			write_or_clean_walk(base_y, 24, White);
			write_or_clean_walk(base_y, 0, Black); //return to starting position
			position=0;
		  eating = 0;
			break;
		}
	}else if(happiness==0 || satiety==0){ //run away routine
		switch(run_away){
		case 0:
			runaway_sound=1;
			end_game = 1;				
			disable_RIT();
			disable_timer(1);
			clean_food();
			clean_walk();
			break;
		case 1:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 2:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 3:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 4:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 5:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 6:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 7:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 8:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 9:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 10:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 11:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 12:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 13:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 14:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 15:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 16:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 17:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 18:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 19:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 20:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 21:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 22:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 23:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		case 24:
			write_or_clean_walk_cry(base_y, (run_away-1)*8, White, White);
			write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
			break;
		
		default:
			disable_timer(0);
			end();
			break;
	}
	
	run_away++;
	
	}

	
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}






/*******************************************************************************
* Function Name  : TIMER1_IRQHandler
* Description    : check if end condition, handle time flow
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIMER1_IRQHandler (void){
	
	refresh_LCD(&hr, &min, &sec);
	
	
	
	sw_count++;
	//to decrease happiness and satiety
	if(sw_count==5){
		decrease_happiness();
		decrease_satitety();
		write_happiness(happiness);
		write_satiety(satiety);
		sw_count=0;
		check_end();
	}
	
	if(!eating){
		animation=!animation;
		show_animation(animation);
	}
	
	if(cuddles==3){
		increase_happiness();
		write_happiness(happiness);
		write_clean_cuddles(White);
		cuddles=0;
	}else if(cuddles==2){
		//wait
		cuddles++;
	}
	if(cuddles==1){
		//wait
		cuddles++;
	}
	
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}



/*******************************************************************************
* Function Name  : TIMER2_IRQHandler
* Description    : handle run away animation
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIMER2_IRQHandler (void){
	
	disable_timer(3);
	
  LPC_TIM2->IR = 1;			// clear interrupt flag 
  return;
}

/*******************************************************************************
* Function Name  : TIMER3_IRQHandler
* Description    : touch panel functionalities
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIMER3_IRQHandler (void){
	
	static int sineticks=0;
	// DAC management
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;
	
  LPC_TIM3->IR = 1;			// clear interrupt flag 
  return;
}
/******************************************************************************
**                         FUNCTIONS TO WRITE SPRITES/PIXELS
******************************************************************************/


/*******************************************************************************
* Function Name  : write_clean_cuddles_1
* Description    : write sprite to cuddle the pet
* Input          : color: black to write, white to clean
* Output         : None
* Return         : None
*******************************************************************************/
void write_clean_cuddles(uint16_t color){
	//1
	write_big_pixel(144, 152, color);
	//1.5
	write_big_pixel(140, 148, color);
	write_big_pixel(148, 148, color);
	//2
	write_big_pixel(136, 144, color);
	write_big_pixel(144, 144, color);
	write_big_pixel(152, 144, color);
	//2.5
	write_big_pixel(132, 140, color);
	write_big_pixel(156, 140, color);
	//3
	write_big_pixel(128, 136, color);
	write_big_pixel(136, 136, color);
	write_big_pixel(144, 136, color);
	write_big_pixel(152, 136, color);
	write_big_pixel(160, 136, color);
	//3.5
	write_big_pixel(138, 132, color);
	write_big_pixel(150, 132, color);
	//4
	write_big_pixel(128, 128, color);
	write_big_pixel(136, 128, color);
	write_big_pixel(152, 128, color);
	write_big_pixel(160, 128, color);
	//5
	write_big_pixel(130, 124, color);
	write_big_pixel(134, 124, color);
	write_big_pixel(154, 124, color);
	write_big_pixel(158, 124, color);



}




/*******************************************************************************
* Function Name  : write_eat
* Description    : sprite of the pet eating
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void write_eat(void){

	//CLEAN WALK 3 SPRITE
	base_y = 264;
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	base_y -=8;
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	base_y -=8;
	write_big_pixel(100-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	base_y -=8;
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	base_y -=8;
	write_big_pixel(84-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(156-24, base_y, White);
	base_y -=8;
	write_big_pixel(76-24, base_y, White);
	write_big_pixel(164-24, base_y, White);
	base_y -=8;
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	write_big_pixel(172-24, base_y, White);
	base_y -=8;
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(172-24, base_y, White);
	base_y -=8;
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	write_big_pixel(44-24, base_y, White);
	write_big_pixel(52-24, base_y, White);
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	write_big_pixel(36-24, base_y, White);
	write_big_pixel(76-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(156-24, base_y, White);
	write_big_pixel(164-24, base_y, White);
	write_big_pixel(172-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	write_big_pixel(44-24, base_y, White);
	write_big_pixel(52-24, base_y, White);
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	base_y -=8;
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	base_y -=8;
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	base_y -=8;
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	base_y -=8;
	write_big_pixel(76-24, base_y, White);
	write_big_pixel(84-24, base_y, White);
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	//WRITE EAT SPRITE
	base_y = 264;
	//1
	write_big_pixel(92-24, base_y, Black);
	write_big_pixel(108-24, base_y, Black);
	write_big_pixel(124-24, base_y, Black);
	write_big_pixel(140-24, base_y, Black);
	base_y -=8;
	//2
	write_big_pixel(132-24, base_y, Black);
	write_big_pixel(100-24, base_y, Black);
	base_y -=8;
	//3
	write_big_pixel(100-24, base_y, Black);
	write_big_pixel(132-24, base_y, Black);
	base_y -=8;
	//4
	write_big_pixel(92-24, base_y, Black);
	write_big_pixel(100-24, base_y, Black);
	write_big_pixel(108-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	write_big_pixel(124-24, base_y, Black);
	write_big_pixel(132-24, base_y, Black);
	write_big_pixel(140-24, base_y, Black);
	base_y -=8;
	//5
	write_big_pixel(84-24, base_y, Black);
	write_big_pixel(148-24, base_y, Black);
	write_big_pixel(156-24, base_y, Black);
	base_y -=8;
	//6
	write_big_pixel(76-24, base_y, Black);
	write_big_pixel(164-24, base_y, Black);
	base_y -=8;
	//7
	write_big_pixel(68-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	write_big_pixel(124-24, base_y, Black);
	write_big_pixel(132-24, base_y, Black);
	write_big_pixel(172-24, base_y, Black);
	base_y -=8;
	//8
	write_big_pixel(68-24, base_y, Black);
	write_big_pixel(140-24, base_y, Black);
	write_big_pixel(172-24, base_y, Black);
	base_y -=8;
	//9
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(148-24, base_y, Black);
	write_big_pixel(180-24, base_y, Black);
	base_y -=8;
	//10
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(148-24, base_y, Black);
	write_big_pixel(180-24, base_y, Black);
	base_y -=8;
	//11
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	write_big_pixel(124-24, base_y, Black);
	write_big_pixel(148-24, base_y, Black);
	write_big_pixel(180-24, base_y, Black);
	base_y -=8;
	//12
	write_big_pixel(44-24, base_y, Black);
	write_big_pixel(52-24, base_y, Black);
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(108-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	write_big_pixel(124-24, base_y, Black);
	write_big_pixel(132-24, base_y, Black);
	write_big_pixel(140-24, base_y, Black);
	write_big_pixel(180-24, base_y, Black);
	base_y -=8;
	//13
	write_big_pixel(44-24, base_y, Black);
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(68-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	write_big_pixel(140-24, base_y, Black);
	write_big_pixel(148-24, base_y, Black);
	write_big_pixel(180-24, base_y, Black);
	base_y -=8;
	//14
	write_big_pixel(52-24, base_y, Black);
	write_big_pixel(76-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	write_big_pixel(156-24, base_y, Black);
	write_big_pixel(164-24, base_y, Black);
	write_big_pixel(172-24, base_y, Black);
	write_big_pixel(180-24, base_y, Black);
	base_y -=8;
	//15
	write_big_pixel(44-24, base_y, Black);
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(68-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	write_big_pixel(92-24, base_y, Black);
	write_big_pixel(100-24, base_y, Black);
	base_y -=8;
	//16
	write_big_pixel(44-24, base_y, Black);
	write_big_pixel(52-24, base_y, Black);
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(92-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	base_y -=8;
	//17
	write_big_pixel(60-24, base_y, Black);
	write_big_pixel(116-24, base_y, Black);
	base_y -=8;
	//18
	write_big_pixel(68-24, base_y, Black);
	write_big_pixel(108-24, base_y, Black);
	base_y -=8;
	//19
	write_big_pixel(76-24, base_y, Black);
	write_big_pixel(84-24, base_y, Black);
	write_big_pixel(92-24, base_y, Black);
	write_big_pixel(100-24, base_y, Black);
	base_y=264;
}



/*******************************************************************************
* Function Name  : clean_eat
* Description    : sprite of the pet not eating near the food
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void clean_eat(void){
//CLEAN EAT SPRITE
	base_y = 264;
	//1
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	base_y -=8;
	//2
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	base_y -=8;
	//3
	write_big_pixel(100-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	base_y -=8;
	//4
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	base_y -=8;
	//5
	write_big_pixel(84-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(156-24, base_y, White);
	base_y -=8;
	//6
	write_big_pixel(76-24, base_y, White);
	write_big_pixel(164-24, base_y, White);
	base_y -=8;
	//7
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(172-24, base_y, White);
	base_y -=8;
	//8
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	write_big_pixel(172-24, base_y, White);
	base_y -=8;
	//9
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	//10
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	//11
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	//12
	write_big_pixel(44-24, base_y, White);
	write_big_pixel(52-24, base_y, White);
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(124-24, base_y, White);
	write_big_pixel(132-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	//13
	write_big_pixel(44-24, base_y, White);
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(140-24, base_y, White);
	write_big_pixel(148-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	//14
	write_big_pixel(52-24, base_y, White);
	write_big_pixel(76-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(156-24, base_y, White);
	write_big_pixel(164-24, base_y, White);
	write_big_pixel(172-24, base_y, White);
	write_big_pixel(180-24, base_y, White);
	base_y -=8;
	//15
	write_big_pixel(44-24, base_y, White);
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	base_y -=8;
	//16
	write_big_pixel(44-24, base_y, White);
	write_big_pixel(52-24, base_y, White);
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	base_y -=8;
	//17
	write_big_pixel(60-24, base_y, White);
	write_big_pixel(116-24, base_y, White);
	base_y -=8;
	//18
	write_big_pixel(68-24, base_y, White);
	write_big_pixel(108-24, base_y, White);
	base_y -=8;
	//19
	write_big_pixel(76-24, base_y, White);
	write_big_pixel(84-24, base_y, White);
	write_big_pixel(92-24, base_y, White);
	write_big_pixel(100-24, base_y, White);
	base_y=264;
}


	




/*******************************************************************************
* Function Name  : write_idle_1
* Description    : base idle sprite of the pet centerend in the screen
* Input          : base_y
* Output         : None
* Return         : None
*******************************************************************************/
void write_idle_1(uint16_t base_y){  
	//1
	write_big_pixel(92, base_y, Black);
	write_big_pixel(108, base_y, Black);
	write_big_pixel(124, base_y, Black);
	write_big_pixel(140, base_y, Black);
	base_y -=8;
	//2
	write_big_pixel(132, base_y, Black);
	write_big_pixel(100, base_y, Black);
	base_y -=8;
	//3
	write_big_pixel(100, base_y, Black);
	write_big_pixel(132, base_y, Black);
	base_y -=8;
	//4
	write_big_pixel(92, base_y, Black);
	write_big_pixel(100, base_y, Black);
	write_big_pixel(108, base_y, Black);
	write_big_pixel(116, base_y, Black);
	write_big_pixel(124, base_y, Black);
	write_big_pixel(132, base_y, Black);
	write_big_pixel(140, base_y, Black);
	base_y -=8;
	//5
	write_big_pixel(84, base_y, Black);
	write_big_pixel(148, base_y, Black);
	write_big_pixel(156, base_y, Black);
	base_y -=8;
	//6
	write_big_pixel(76, base_y, Black);
	write_big_pixel(164, base_y, Black);
	base_y -=8;
	//7
	write_big_pixel(68, base_y, Black);
	write_big_pixel(116, base_y, Black);
	write_big_pixel(124, base_y, Black);
	write_big_pixel(132, base_y, Black);
	write_big_pixel(140, base_y, Black);
	write_big_pixel(172, base_y, Black);
	base_y -=8;
	//8
	write_big_pixel(68, base_y, Black);
	write_big_pixel(148, base_y, Black);
	write_big_pixel(172, base_y, Black);
	base_y -=8;
	//9
	write_big_pixel(60, base_y, Black);
	write_big_pixel(148, base_y, Black);
	write_big_pixel(180, base_y, Black);
	base_y -=8;
	//10
	write_big_pixel(60, base_y, Black);
	write_big_pixel(148, base_y, Black);
	write_big_pixel(180, base_y, Black);
	base_y -=8;
	//11
	write_big_pixel(60, base_y, Black);
	write_big_pixel(116, base_y, Black);
	write_big_pixel(124, base_y, Black);
	write_big_pixel(132, base_y, Black);
	write_big_pixel(140, base_y, Black);
	write_big_pixel(180, base_y, Black);
	base_y -=8;
	//12
	write_big_pixel(60, base_y, Black);
	write_big_pixel(108, base_y, Black);
	write_big_pixel(116, base_y, Black);
	write_big_pixel(124, base_y, Black);
	write_big_pixel(132, base_y, Black);
	write_big_pixel(180, base_y, Black);
	base_y -=8;
	//13
	write_big_pixel(44, base_y, Black);
	write_big_pixel(52, base_y, Black);
	write_big_pixel(60, base_y, Black);
	write_big_pixel(68, base_y, Black);
	write_big_pixel(116, base_y, Black);
	write_big_pixel(140, base_y, Black);
	write_big_pixel(148, base_y, Black);
	write_big_pixel(180, base_y, Black);
	base_y -=8;
	//14
	write_big_pixel(36, base_y, Black);
	write_big_pixel(76, base_y, Black);
	write_big_pixel(116, base_y, Black);
	write_big_pixel(156, base_y, Black);
	write_big_pixel(164, base_y, Black);
	write_big_pixel(172, base_y, Black);
	write_big_pixel(180, base_y, Black);
	base_y -=8;
	//15
	write_big_pixel(44, base_y, Black);
	write_big_pixel(52, base_y, Black);
	write_big_pixel(60, base_y, Black);
	write_big_pixel(68, base_y, Black);
	write_big_pixel(116, base_y, Black);
	write_big_pixel(92, base_y, Black);
	write_big_pixel(100, base_y, Black);
	base_y -=8;
	//16
	write_big_pixel(60, base_y, Black);
	write_big_pixel(92, base_y, Black);
	write_big_pixel(116, base_y, Black);
	base_y -=8;
	//17
	write_big_pixel(60, base_y, Black);
	write_big_pixel(116, base_y, Black);
	base_y -=8;
	//18
	write_big_pixel(68, base_y, Black);
	write_big_pixel(108, base_y, Black);
	base_y -=8;
	//19
	write_big_pixel(76, base_y, Black);
	write_big_pixel(84, base_y, Black);
	write_big_pixel(92, base_y, Black);
	write_big_pixel(100, base_y, Black);
	base_y = 264;
}


/*******************************************************************************
* Function Name  : write_idle_2
* Description    : function to simulate the closed eye of the pet
* Input          : base_y
* Output         : None
* Return         : None
*******************************************************************************/
void write_idle_2(uint16_t base_y){
	base_y = 264;
	write_big_pixel(92, base_y-120, White);
	base_y = 264;
}



/*******************************************************************************
* Function Name  : write_idle_3
* Description    : function to simulate the opened eye of the pet
* Input          : base_y
* Output         : None
* Return         : None
*******************************************************************************/
void write_idle_3(uint16_t base_y){  
	write_big_pixel(92, base_y-120, Black);
}




/*******************************************************************************
* Function Name  : clean_idle_1
* Description    : function to clear pixels of the pet in idle position
* Input          : base_y
* Output         : None
* Return         : None
*******************************************************************************/
void clean_idle_1(uint16_t base_y){   
	base_y=264;
	//1
	write_big_pixel(92, base_y, White);
	write_big_pixel(108, base_y, White);
	write_big_pixel(124, base_y, White);
	write_big_pixel(140, base_y, White);
	base_y -=8;
	//2
	write_big_pixel(132, base_y, White);
	write_big_pixel(100, base_y, White);
	base_y -=8;
	//3
	write_big_pixel(100, base_y, White);
	write_big_pixel(132, base_y, White);
	base_y -=8;
	//4
	write_big_pixel(92, base_y, White);
	write_big_pixel(100, base_y, White);
	write_big_pixel(108, base_y, White);
	write_big_pixel(116, base_y, White);
	write_big_pixel(124, base_y, White);
	write_big_pixel(132, base_y, White);
	write_big_pixel(140, base_y, White);
	base_y -=8;
	//5
	write_big_pixel(84, base_y, White);
	write_big_pixel(148, base_y, White);
	write_big_pixel(156, base_y, White);
	base_y -=8;
	//6
	write_big_pixel(76, base_y, White);
	write_big_pixel(164, base_y, White);
	base_y -=8;
	//7
	write_big_pixel(68, base_y, White);
	write_big_pixel(116, base_y, White);
	write_big_pixel(124, base_y, White);
	write_big_pixel(132, base_y, White);
	write_big_pixel(140, base_y, White);
	write_big_pixel(172, base_y, White);
	base_y -=8;
	//8
	write_big_pixel(68, base_y, White);
	write_big_pixel(148, base_y, White);
	write_big_pixel(172, base_y, White);
	base_y -=8;
	//9
	write_big_pixel(60, base_y, White);
	write_big_pixel(148, base_y, White);
	write_big_pixel(180, base_y, White);
	base_y -=8;
	//10
	write_big_pixel(60, base_y, White);
	write_big_pixel(148, base_y, White);
	write_big_pixel(180, base_y, White);
	base_y -=8;
	//11
	write_big_pixel(60, base_y, White);
	write_big_pixel(116, base_y, White);
	write_big_pixel(124, base_y, White);
	write_big_pixel(132, base_y, White);
	write_big_pixel(140, base_y, White);
	write_big_pixel(180, base_y, White);
	base_y -=8;
	//12
	write_big_pixel(60, base_y, White);
	write_big_pixel(108, base_y, White);
	write_big_pixel(116, base_y, White);
	write_big_pixel(124, base_y, White);
	write_big_pixel(132, base_y, White);
	write_big_pixel(180, base_y, White);
	base_y -=8;
	//13
	write_big_pixel(44, base_y, White);
	write_big_pixel(52, base_y, White);
	write_big_pixel(60, base_y, White);
	write_big_pixel(68, base_y, White);
	write_big_pixel(116, base_y, White);
	write_big_pixel(140, base_y, White);
	write_big_pixel(148, base_y, White);
	write_big_pixel(180, base_y, White);
	base_y -=8;
	//14
	write_big_pixel(36, base_y, White);
	write_big_pixel(76, base_y, White);
	write_big_pixel(116, base_y, White);
	write_big_pixel(156, base_y, White);
	write_big_pixel(164, base_y, White);
	write_big_pixel(172, base_y, White);
	write_big_pixel(180, base_y, White);
	base_y -=8;
	//15
	write_big_pixel(44, base_y, White);
	write_big_pixel(52, base_y, White);
	write_big_pixel(60, base_y, White);
	write_big_pixel(68, base_y, White);
	write_big_pixel(116, base_y, White);
	write_big_pixel(92, base_y, White);
	write_big_pixel(100, base_y, White);
	base_y -=8;
	//16
	write_big_pixel(60, base_y, White);
	write_big_pixel(92, base_y, White);
	write_big_pixel(116, base_y, White);
	base_y -=8;
	//17
	write_big_pixel(60, base_y, White);
	write_big_pixel(116, base_y, White);
	base_y -=8;
	//18
	write_big_pixel(68, base_y, White);
	write_big_pixel(108, base_y, White);
	base_y -=8;
	//19
	write_big_pixel(76, base_y, White);
	write_big_pixel(84, base_y, White);
	write_big_pixel(92, base_y, White);
	write_big_pixel(100, base_y, White);
	base_y = 264;
}





/*******************************************************************************
* Function Name  : clean_food
* Description    : function to clear pixels of the food (meal or snack)
* Input          : base_y
* Output         : None
* Return         : None
*******************************************************************************/
void clean_food(void){
	base_y = 264;
	//1
	write_big_pixel(20, base_y, White);
	write_big_pixel(28, base_y, White);
	write_big_pixel(36, base_y, White);
	write_big_pixel(44, base_y, White);
	base_y -=8;
	//2
	write_big_pixel(20, base_y, White);
	write_big_pixel(44, base_y, White);
	base_y -=8;
	//3
	write_big_pixel(20, base_y, White);
	write_big_pixel(44, base_y, White);
	base_y -=8;
	//4
	write_big_pixel(20, base_y, White);
	write_big_pixel(28, base_y, White);
	write_big_pixel(36, base_y, White);
	write_big_pixel(44, base_y, White);
	base_y = 264;
}



/*******************************************************************************
* Function Name  : write_or_clean_walk
* Description    : function used to write the pet in different position given the offset from the initial position and the color
* Input          : base_y: height of the sprite,  x_offset:offset from the initial sprite's position, color: color of the pixels
* Output         : None
* Return         : None
*******************************************************************************/
void write_or_clean_walk(uint16_t base_y, uint16_t x_offset, uint16_t color){
	//WRITE NEXT SPRITE
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(108-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(100-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	write_big_pixel(108-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(84-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(156-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(76-x_offset, base_y, color);
	write_big_pixel(164-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	write_big_pixel(172-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(172-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(108-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(44-x_offset, base_y, color);
	write_big_pixel(52-x_offset, base_y, color);
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(36-x_offset, base_y, color);
	write_big_pixel(76-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(156-x_offset, base_y, color);
	write_big_pixel(164-x_offset, base_y, color);
	write_big_pixel(172-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(44-x_offset, base_y, color);
	write_big_pixel(52-x_offset, base_y, color);
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(108-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(76-x_offset, base_y, color);
	write_big_pixel(84-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	base_y = 264;
}




/*******************************************************************************
* Function Name  : write_or_clean_walk_cry
* Description    : function used to write the crying pet in different position given the offset from the initial position and the color
* Input          : base_y: height of the sprite,  x_offset:offset from the initial sprite's position, color: color of the pixels, color2: color2 of the pixels   
* Output         : None
* Return         : None
*******************************************************************************/
void write_or_clean_walk_cry(uint16_t base_y, uint16_t x_offset, uint16_t color, uint16_t color2){
	//WRITE NEXT SPRITE
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(108-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(100-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	write_big_pixel(108-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(84-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(156-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(76-x_offset, base_y, color);
	write_big_pixel(164-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	write_big_pixel(172-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(172-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color2);
	write_big_pixel(108-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(124-x_offset, base_y, color);
	write_big_pixel(132-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(44-x_offset, base_y, color);
	write_big_pixel(52-x_offset, base_y, color);
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color2);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(140-x_offset, base_y, color);
	write_big_pixel(148-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(36-x_offset, base_y, color);
	write_big_pixel(76-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color2);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(156-x_offset, base_y, color);
	write_big_pixel(164-x_offset, base_y, color);
	write_big_pixel(172-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	write_big_pixel(180-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(44-x_offset, base_y, color);
	write_big_pixel(52-x_offset, base_y, color);
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(60-x_offset, base_y, color);
	write_big_pixel(116-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(68-x_offset, base_y, color);
	write_big_pixel(108-x_offset, base_y, color);
	base_y -=8;
	write_big_pixel(76-x_offset, base_y, color);
	write_big_pixel(84-x_offset, base_y, color);
	write_big_pixel(92-x_offset, base_y, color);
	write_big_pixel(100-x_offset, base_y, color);
	base_y = 264;
}








/*******************************************************************************
* Function Name  : write_happiness
* Description    : function to write inside the battery the level of happiness of the pet
* Input          : happiness: level of happiness of the pet
* Output         : None
* Return         : None
*******************************************************************************/
void write_happiness(uint8_t happiness){
	uint16_t Xpos = 40;
	uint16_t Ypos = 64;
	int i;
	for(i=0;i<happiness;i+=1){
		write_big_pixel(Xpos, Ypos, Black);
		write_big_pixel(Xpos, Ypos+8, Black);
		write_big_pixel(Xpos, Ypos+12, Black);
		Xpos+=10;
	}
	for(i=happiness;i<4;i+=1){
		write_big_pixel(Xpos, Ypos, White);
		write_big_pixel(Xpos, Ypos+8, White);
		write_big_pixel(Xpos, Ypos+12, White);
		Xpos+=10;
	}
}



/*******************************************************************************
* Function Name  : write_satiety
* Description    : function to write inside the battery the level of satiety of the pet
* Input          : satiety: level of satiety of the pet
* Output         : None
* Return         : None
*******************************************************************************/
void write_satiety(uint8_t satiety){
	uint16_t Xpos = 160;
	uint16_t Ypos = 64;
	int i;
	for(i=0;i<satiety;i+=1){
		write_big_pixel(Xpos, Ypos, Black);
		write_big_pixel(Xpos, Ypos+8, Black);
		write_big_pixel(Xpos, Ypos+12, Black);
		Xpos+=10;
	}
	for(i=satiety;i<4;i+=1){
		write_big_pixel(Xpos, Ypos, White);
		write_big_pixel(Xpos, Ypos+8, White);
		write_big_pixel(Xpos, Ypos+12, White);
		Xpos+=10;
	}
}







/*******************************************************************************
* Function Name  : write_batteries
* Description    : function to write the batterie containing the level of happiness/satiety of the pet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void write_batteries(void){

	//5
	write_big_pixel(30, 54, Black);
	write_big_pixel(38, 54, Black);
	write_big_pixel(46, 54, Black);
	write_big_pixel(54, 54, Black);
	write_big_pixel(62, 54, Black);
	write_big_pixel(70, 54, Black);
	write_big_pixel(78, 54, Black);
	write_big_pixel(80, 54, Black);
	
	write_big_pixel(150, 54, Black);
	write_big_pixel(158, 54, Black);
	write_big_pixel(166, 54, Black);
	write_big_pixel(174, 54, Black);
	write_big_pixel(182, 54, Black);
	write_big_pixel(190, 54, Black);
	write_big_pixel(198, 54, Black);
	write_big_pixel(200, 54, Black);
	//4
	write_big_pixel(30, 62, Black);
	write_big_pixel(80, 62, Black);
	write_big_pixel(88, 62, Black);
	write_big_pixel(150, 62, Black);
	write_big_pixel(200, 62, Black);
	write_big_pixel(208, 62, Black);
	//3
	write_big_pixel(30, 70, Black);
	write_big_pixel(80, 70, Black);
	write_big_pixel(88, 70, Black);
	write_big_pixel(150, 70, Black);
	write_big_pixel(200, 70, Black);
	write_big_pixel(208, 70, Black);
	//2
	write_big_pixel(30, 78, Black);
	write_big_pixel(80, 78, Black);
	write_big_pixel(88, 78, Black);
	write_big_pixel(150, 78, Black);
	write_big_pixel(200, 78, Black);
	write_big_pixel(208, 78, Black);
	//1
	write_big_pixel(30, 86, Black);
	write_big_pixel(38, 86, Black);
	write_big_pixel(46, 86, Black);
	write_big_pixel(54, 86, Black);
	write_big_pixel(62, 86, Black);
	write_big_pixel(70, 86, Black);
	write_big_pixel(78, 86, Black);
	write_big_pixel(80, 86, Black);
	
	write_big_pixel(150, 86, Black);
	write_big_pixel(158, 86, Black);
	write_big_pixel(166, 86, Black);
	write_big_pixel(174, 86, Black);
	write_big_pixel(182, 86, Black);
	write_big_pixel(190, 86, Black);
	write_big_pixel(198, 86, Black);
	write_big_pixel(200, 86, Black);
	
}












/******************************************************************************
**                         GENERAL FUNCTIONALITIES
******************************************************************************/


/*******************************************************************************
* Function Name  : increase_happiness
* Description    : funciton to increase the happiness of the pet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void increase_happiness(void){
	if(happiness<4){
		happiness+=1;	
	}
}



/*******************************************************************************
* Function Name  : increase_satiety
* Description    : funciton to increase the satiety of the pet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void increase_satiety(void){
	if(satiety<4){
		satiety+=1;	
	}
}



/*******************************************************************************
* Function Name  : decrease_happiness
* Description    : funciton to happiness the satiety of the pet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void decrease_happiness(void){
	happiness-=1;
}



/*******************************************************************************
* Function Name  : decrease_satitety
* Description    : funciton to decrease the satiety of the pet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void decrease_satitety(void){
	satiety-=1;
}




/*******************************************************************************
* Function Name  : show_animation
* Description    : funciton to swap between the closed and opened eye of the pet
* Input          : animation: identifier of the animation to show
* Output         : None
* Return         : None
*******************************************************************************/
void show_animation(int animation){
	if(animation==0){
		write_idle_2(base_y);
	}else{
		write_idle_3(base_y);
	}
}



/*******************************************************************************
* Function Name  : write_big_pixel
* Description    : function used to write 8x8 pixels given x, y and color
* Input          : Xpos: position x of the pixel, Ypos: position y of the pixel, color: color of the pixel
* Output         : None
* Return         : None
*******************************************************************************/
void write_big_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t color){
	uint16_t i, j;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			LCD_SetPoint(Xpos+i,Ypos+j,color);
		}
	}
}



/*******************************************************************************
* Function Name  : write_small_pixel
* Description    : function used to write 4x4 pixels given x, y and color
* Input          : Xpos: position x of the pixel, Ypos: position y of the pixel, color: color of the pixel
* Output         : None
* Return         : None
*******************************************************************************/
void write_small_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t color){
	uint16_t i, j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			LCD_SetPoint(Xpos+i,Ypos+j,color);
		}
	}
}






/*******************************************************************************
* Function Name  : refresh_LCD
* Description    : funciton used to increase the time and write it in the screen
* Input          : hr: pointer to the hours variable, min: pointer to the minutes variable, sec: pointer to the seconds variable
* Output         : None
* Return         : None
*******************************************************************************/
void refresh_LCD(int* hr, int* min, int* sec){
	
	char time_in_char[48] = "";
	
	(*sec)++;
	
	if((*sec)==60){
		(*sec)=0;
		(*min)++;
		if(*min==60){
			(*min)=0;
			(*hr)++;
		}
	}
	
	sprintf(time_in_char, "      Age: %02d : %02d : %02d         ", *hr, *min, *sec);
	GUI_Text(0, 10, (uint8_t *) time_in_char, Black, White);

	return;
}










/*******************************************************************************
* Function Name  : check_end
* Description    : function to check the exit condition of the game
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void check_end(void){
	if(happiness==0 || satiety==0){
		disable_timer(0);
		reset_timer(0);
		//enable_timer(0);
		food = 0;
		runaway_sound=1;
	}
	return;
}












/*******************************************************************************
* Function Name  : clean_walk
* Description    : function to clean pixels when the pet is eating but happiness or satiety go to zero
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void clean_walk(void){
		LCD_DrawRectangle(10, 110, 200, 168, White);
}


/******************************************************************************
**                            End Of File
******************************************************************************/
