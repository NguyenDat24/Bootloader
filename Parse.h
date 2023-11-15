#ifndef _PARSE_H_
#define _PARSE_H_

#include "MKL46Z4.h"

/*Function Prototype*/
/*Convert char to decimal function*/
uint32_t ChartoDec(uint8_t Data);

/*Handle Data receive from UART0 and Write to Flash */
void Handle_File(uint8_t *Ptr_Arr);

#endif