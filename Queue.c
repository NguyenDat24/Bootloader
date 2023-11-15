#include "Queue.h"

//Define Queue
#define MAX_SIZE 1028
static uint8_t QUEUE_ARR[MAX_SIZE] = {0};

static CircurlarQueue_Types Queue = {
  -1, -1, MAX_SIZE,
  &QUEUE_ARR[0]
};

//Check full of Queue
uint8_t CircurlarQueue_IsFull()
{
  uint8_t Key = FALSE;
  if ((Queue.Front == Queue.Rear + 1) || (Queue.Front == 0 && Queue.Rear == Queue.Size -1))
  {
    Key = TRUE;
  }
   return Key;
}

//Check empty of Queue
uint8_t CircurlarQueue_IsEmpty()
{
  uint8_t Key = FALSE;
  if (Queue.Front == -1)
  {
    Key = TRUE;
  }
  return Key;
}
//Handling push data in queue
void CircurlarQueue_PushData(uint8_t Input_Data)
{
  if(!CircurlarQueue_IsFull())
  {
    if (Queue.Front == -1)
    {
      Queue.Front = 0;
    }
    Queue.Rear = (Queue.Rear + 1) % Queue.Size;
    Queue.QueueArr[Queue.Rear] = Input_Data;
    
  }
}
 //Handling pop data in queue
uint8_t CircurlarQueue_PopData()
{
  uint8_t DataOut;
  if(!CircurlarQueue_IsEmpty())
  {
    DataOut = Queue.QueueArr[Queue.Front];
    if (Queue.Front == Queue.Rear)
    {
      Queue.Front = -1;
      Queue.Rear = -1;
    }
    else
    {
      Queue.Front = (Queue.Front + 1) % Queue.Size;
    }
  }
  return DataOut;
  }

