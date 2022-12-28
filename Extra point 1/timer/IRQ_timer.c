/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h" 

/******************************************************************************
**                            PROTOTYPES
******************************************************************************/
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
void refresh_LCD(uint8_t* hr, uint8_t* min, uint8_t* sec);
void increase_happiness(void);
void increase_satiety(void);
void decrease_happiness(void);
void decrease_satitety(void);
void write_batteries(void);
void write_happiness(uint8_t happiness);
void write_satiety(uint8_t satiery);
void check_end(void);
void clean_food(void);
/******************************************************************************
**                            EXTERN PROTOTYPES
******************************************************************************/
extern void setup(void);
extern void end(void);
/******************************************************************************
**                            GLOBAL VARIBALES
******************************************************************************/
uint8_t happiness; // [1,4]
uint8_t satiety; // [1,4]
uint16_t base_y = 264;
int animation;
uint8_t hr, min, sec;
int eating; 
int position=0;
int run_away=0;
/******************************************************************************
**                            EXTERN VARIABLES
******************************************************************************/
extern int food;


/******************************************************************************
**                            TIMER HANDLERS
******************************************************************************/

//eat animation
void TIMER0_IRQHandler (void){

	switch(position){
		case 0:
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
			//enable_timer(1);
			enable_RIT();
			base_y=264;
			write_or_clean_walk(base_y, 24, White);
			write_or_clean_walk(base_y, 0, Black); //return to starting position
			position=0;
			break;
	}
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}




//decrease and check exit condition
void TIMER1_IRQHandler (void){
	
	//to decrease happiness and satiety
	if(sec%5==0){
		decrease_happiness();
		decrease_satitety();
		write_happiness(happiness);
		write_satiety(satiety);
	}
	
	refresh_LCD(&hr, &min, &sec);
	
	check_end();
	
	
	//animation=!animation;
	//show_animation(animation);
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

//run away animation
void TIMER2_IRQHandler (void){
	disable_RIT();
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	
	clean_food();

	while(run_away<24){
		write_or_clean_walk_cry(base_y, run_away*8, White, White);
		run_away++;
		write_or_clean_walk_cry(base_y, run_away*8, Black, Blue2);
	}

	end();
	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
	
  return;
}







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


	

//IDLE FUNCTIONS
void write_idle_1(uint16_t base_y){ // ok
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

void write_idle_2(uint16_t base_y){ // ok
	write_big_pixel(92, base_y-120, White);
}
void write_idle_3(uint16_t base_y){ // ok
	write_big_pixel(92, base_y-120, Black);
}

void clean_idle_1(uint16_t base_y){ // ok 
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


//EAT FUNCTIONS
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
void increase_happiness(void){
	if(happiness<4){
		happiness+=1;	
	}
}
void increase_satiety(void){
	if(satiety<4){
		satiety+=1;	
	}
}
void decrease_happiness(void){
	happiness-=1;
}
void decrease_satitety(void){
	satiety-=1;
}

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


//GENERIC FUNCTIONS
void show_animation(int animation){
	if(animation==0){
		write_idle_2(base_y);
	}else{
		write_idle_3(base_y);
	}
}
void write_big_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t color){
	uint16_t i, j;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			LCD_SetPoint(Xpos+i,Ypos+j,color);
		}
	}
}
void write_small_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t color){
	uint16_t i, j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			LCD_SetPoint(Xpos+i,Ypos+j,color);
		}
	}
}



void refresh_LCD(uint8_t* hr, uint8_t* min, uint8_t* sec){
	
	char time_in_char[48] = "";
	
	(*sec)++;
	if((*sec)==(uint8_t)60){
		(*sec)=0;
		(*min)++;
		if(*min==(uint8_t)60){
			(*min)=0;
			(*hr)++;
		}
	}
	
	sprintf(time_in_char, "      Age: %02d : %02d : %02d         ", *hr, *min, (*sec)-1);
	GUI_Text(0, 10, (uint8_t *) time_in_char, Black, White);

	return;
}





void write_batteries(void){
	//5
	write_big_pixel(30, 54, Black);
	write_big_pixel(38, 54, Black);
	write_big_pixel(46, 54, Black);
	write_big_pixel(54, 54, Black);
	write_big_pixel(62, 54, Black);
	write_big_pixel(70, 54, Black);
	write_big_pixel(78, 54, Black);
	write_big_pixel(84, 54, Black);
	
	write_big_pixel(150, 54, Black);
	write_big_pixel(158, 54, Black);
	write_big_pixel(166, 54, Black);
	write_big_pixel(174, 54, Black);
	write_big_pixel(182, 54, Black);
	write_big_pixel(190, 54, Black);
	write_big_pixel(198, 54, Black);
	write_big_pixel(206, 54, Black);
	//4
	write_big_pixel(30, 62, Black);
	write_big_pixel(84, 62, Black);
	write_big_pixel(92, 62, Black);
	write_big_pixel(150, 62, Black);
	write_big_pixel(206, 62, Black);
	write_big_pixel(212, 62, Black);
	//3
	write_big_pixel(30, 70, Black);
	write_big_pixel(84, 70, Black);
	write_big_pixel(92, 70, Black);
	write_big_pixel(150, 70, Black);
	write_big_pixel(206, 70, Black);
	write_big_pixel(212, 70, Black);
	//2
	write_big_pixel(30, 78, Black);
	write_big_pixel(84, 78, Black);
	write_big_pixel(92, 78, Black);
	write_big_pixel(150, 78, Black);
	write_big_pixel(206, 78, Black);
	write_big_pixel(212, 78, Black);
	//1
	write_big_pixel(30, 86, Black);
	write_big_pixel(38, 86, Black);
	write_big_pixel(46, 86, Black);
	write_big_pixel(54, 86, Black);
	write_big_pixel(62, 86, Black);
	write_big_pixel(70, 86, Black);
	write_big_pixel(78, 86, Black);
	write_big_pixel(84, 86, Black);
	
	write_big_pixel(150, 86, Black);
	write_big_pixel(158, 86, Black);
	write_big_pixel(166, 86, Black);
	write_big_pixel(174, 86, Black);
	write_big_pixel(182, 86, Black);
	write_big_pixel(190, 86, Black);
	write_big_pixel(198, 86, Black);
	write_big_pixel(206, 86, Black);
	
}

void check_end(void){
	if(happiness==0 || satiety==0){
		enable_timer(2);
	}
	return;
}
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


/******************************************************************************
**                            End Of File
******************************************************************************/
