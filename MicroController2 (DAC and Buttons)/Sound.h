// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// November 17, 2014
#include <stdint.h>

void Sound_Init(void);
void Sound_Play(uint32_t size, const uint8_t *Song);
void SysTick_Disable(void);
void Timer0A_Disable(void);
void Timer0_Init(uint32_t size, const uint8_t *Song);
void SysTick_Enable(void);

