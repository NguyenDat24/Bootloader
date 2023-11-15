#ifndef _FLASH_H_
#define _FLASH_H_

#include "MKL46Z4.h"

/*Function Prototype*/
/*Write to Flash function prototype*/
void Flash_WriteLongWord( uint32_t Address, uint32_t Data);

/*Erase Flash function prototype*/
void Flash_EraseSector( uint32_t Address);

#endif