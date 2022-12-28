/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons and manage the bouncing effect
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect. 	
  * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "RIT/RIT.h"

#include <string.h>
#include <math.h>
#include <stdlib.h> 


/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in funct_led								*/
#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif



/*----------------------------------------------------------------------------
  Prototypes 
 *----------------------------------------------------------------------------*/
void read_message(char[]);
int* decimalToBinary(int num);
extern int translate_morse(char* vett_input, int vet_input_lenght, char* vett_output, int vet_output_lenght, char change_symbol, char space, char sentence_end);

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/

int main (void) {
	
	char message[] = "1112000020100201301002013112001210210021113011112001114";
	
	int length = strlen(message);
	char translated[100];
	volatile int res;
	
	/* SYSTEM INITIALIZATION */
 	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       */
	
	/* INITIALIZATION FUNCTIONALITIES */
	//press key1
	all_LED_On();
	
	/* STEP 1 */
	read_message(message); // put it in rit handler
	
	/* STEP 2 */
	all_LED_On();
	
	/* STEP 3 */
	//press key2
	//disable int0 and key1
	res = translate_morse(message, length, translated, 100, '2', '3', '4');
	
	/* STEP 4 */
	//leds shows the total number of converted symbols
	//int0 and key1 enabled
	//restart with int0
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");												//wait for interrupt
  }

}

void read_message(char message[]){ //spit the message string into multiple chars
	
	//to split the string
	int init_size = strlen(message);
	
	//to handle the number of words read and turn on the leds
	int words = 0;
	int *converted;
	int k;
	
	int i;
	for(i = 0; i<init_size; i++) 
	
		if(message[i]=='4'){ //end of the message
			
			words ++;
			
			converted = decimalToBinary(words);
	
			all_LED_Off(); //reset previous states of the LEDs
		
			// print binary rapresentation of the current number of read words
			// turning on the leds 
			for(k=0;k<8;k++)
				if(*(converted+k)==1)
					LED_On(k);
				
			
		} 
		
		else if (message[i]=='2'){ 
			words ++;
			
			converted = decimalToBinary(words);
	
			all_LED_Off(); //reset previous states of the LEDs
		
			// print binary rapresentation of the current number of read words
			// turning on the leds 
			for(k=0;k<8;k++)
				if(*(converted+k)==1)
					LED_On(k);
		}
		
		else if (message[i]=='3'){
			words +=2;
			
			converted = decimalToBinary(words);
	
			all_LED_Off(); //reset previous states of the LEDs
		
			// print binary rapresentation of the current number of read words
			// turning on the leds 
			for(k=0;k<8;k++)
				if(*(converted+k)==1)
					LED_On(k);
		}
			
		
		else 
			continue;

}

int* decimalToBinary(int num) {   
   
   // Stores binary representation of number.
   static int binaryNum[8]; // assume 8 bit integer.
   int i=0;
   
   for ( ;num > 0; ){
      binaryNum[i++] = num % 2;
      num /= 2;
   }
	 
	 return binaryNum;
}
