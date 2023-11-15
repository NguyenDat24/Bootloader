#ifndef _UART_H_
#define _UART_H_

#include "MKL46Z4.h"

/*Enum Baudrate*/
typedef enum
{
  BAUDRATE_600     = 600,
  BAUDRATE_1200    = 1200,
  BAUDRATE_2400    = 2400,
  BAUDRATE_4800    = 4800,
  BAUDRATE_9600    = 9600,
  BAUDRATE_14400   = 14400,
} UART0_BaudrateType;

typedef struct
{
  UART0_BaudrateType Baudrate;
} Uart0_Config;

/*Function Prototype*/
/*Initialize Clock*/
void Clock_Init();

/*Config Baudrate*/
void UART0_Set_Baudrate(Uart0_Config *Uart0_Config);

/*Initialize PORT for UART0*/
void Port_UART0_Init();

/*Config UART0*/
void UART0_Init();

#endif