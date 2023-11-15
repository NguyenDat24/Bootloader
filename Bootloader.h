#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include "MKL46Z4.h"

/* Config_CoreClock Function Prototype */
void Config_CoreClock();

/* SysTick_Init Function Prototype */
void SysTick_Init();

/* Sys_Delay Function Prototype */
void Sys_Delay(uint32_t timems);

/*JumptoApplication function*/
void JumptoApplication();

/*Button_Init function prototype */
void Button_Init();


#endif