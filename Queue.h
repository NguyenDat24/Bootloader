#ifndef _CIRCULAR_QUEUE_H_
#define _CIRCULAR_QUEUE_H_

#include "MKL46Z4.h"
/************************************************************
*Definitions
************************************************************/
#define TRUE                    1U
#define FALSE                   0U

//Define Queue Types
typedef struct
{
  int8_t Front, Rear;
  uint32_t Size;
  uint8_t * QueueArr;
} CircurlarQueue_Types;

//Function Prototype
//Check full of Queue
uint8_t CircurlarQueue_IsFull();

//Check empty of Queue
uint8_t CircurlarQueue_IsEmpty();

//Handling push data in queue
void CircurlarQueue_PushData(uint8_t Input_Data);

//Handling pop data in queue
uint8_t CircurlarQueue_PopData();

#endif