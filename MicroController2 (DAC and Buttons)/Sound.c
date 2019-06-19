// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "tm4c123gh6pm.h"
#include "DAC.h"

#define timer_freq 80000000/11025
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);  
void SysTick_Disable(void);
uint32_t Song_Size = 0;
const uint8_t *Song_Ptr;
const uint8_t *Sound_Ptr;
uint32_t Index = 0;
uint32_t Sound_Size = 0;
uint32_t Sound_Index = 0;


// Initialize SysTick and DAC for song output
void Sound_Init(void){
  DAC_Init();
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
	NVIC_ST_RELOAD_R = timer_freq -1; // reload value
	NVIC_ST_CURRENT_R = 0; //Any value to reset clock
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000; //Set priority to 1
  NVIC_ST_CTRL_R = 0x00000007;      // enable SysTick with core clock and interrupts
}


// Change Song or Sound be playing as well as its frequency
void Sound_Play(uint32_t size, const uint8_t *Song){
	Song_Size = size;
	Song_Ptr  = Song;
	Index = 0;
	NVIC_ST_RELOAD_R = (timer_freq) - 1;
	//DAC_Out(0);
	GPIO_PORTF_DATA_R ^= 0x4;
}

//timer for sound effects
void Timer0_Init(uint32_t size, const uint8_t *Sound){ // void(*task)(void),
	uint8_t sr = 0;
	SysTick_Disable();
	Sound_Size = size;
	Sound_Ptr = Sound;
	Sound_Index = 0;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = timer_freq-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
  EndCritical(sr);
}

//disable Timer0A to play theme
void Timer0A_Disable(void){
	TIMER0_CTL_R = 0;
}

//disable SysTick for playing a sound effect
void SysTick_Disable(void){
  NVIC_ST_RELOAD_R = 0; // reload value 
}

//enable SysTick
void SysTick_Enable(void){
	NVIC_ST_RELOAD_R = timer_freq -1; // reload value
}	

//output note from song/sound
void SysTick_Handler(void){
	Index = (Index + 1) % Song_Size;
	DAC_Out(Song_Ptr[Index]);
	GPIO_PORTF_DATA_R ^= 0x4;
}


//Play Sound in DAC
void Timer0A_Handler(void){
	//Plays Background Music While game is ON
  TIMER0_ICR_R = TIMER_ICR_TATOCINT; // acknowledge TIMER0A timeout
	Sound_Index++;
	DAC_Out(Sound_Ptr[Sound_Index]);
	GPIO_PORTF_DATA_R ^= 0x4;
	if(Sound_Index >= Sound_Size){
		Timer0A_Disable();
		SysTick_Enable();
	}
}



