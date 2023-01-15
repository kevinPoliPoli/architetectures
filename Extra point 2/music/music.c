#include "music.h"
#include "../timer/timer.h"
#include "lpc17xx.h"

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */

int click_sound = 0; 
int eating_sound = 0;
int runaway_sound = 0;
int cuddles_sound = 0;

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(3);
		init_timer(3, note.freq);
		enable_timer(3);
	}
	reset_timer(2);
	init_timer(2, note.duration);
	enable_timer(2);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM2->TCR != 0) || (LPC_TIM3->TCR != 0));
}
