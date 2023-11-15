#include "Queue.h"
#include "Uart.h"
#include "MKL46Z4.h"
#include "Flash.h"
#include "Parse.h"
#include "Bootloader.h"

/*Define macro*/
#define VECTOR_TABLE_FLASH_ADDR 0x00000000
#define VECTOR_TABLE_RAM_ADDR   0x20001000

//Declare global variable
uint32_t count = 0;
uint8_t Arr_temp[512] = {0};
uint32_t *Value_App1 = (uint32_t*)(0x30000);
uint32_t *Value_App2 = (uint32_t*)(0x30010);
uint32_t *End_Of_Srec = (uint32_t*)(0x30020);

/*Function call back execution*/
void func_push_queue(uint8_t Data)
{
  CircurlarQueue_PushData(Data);
}

/*User Config Baudrate*/
Uart0_Config User_Config_Baudrate = {
  .Baudrate = BAUDRATE_9600,
};

/*main function*/
void main()
{
  Clock_Init();
  Port_UART0_Init();
  UART0_Set_Baudrate(&User_Config_Baudrate);
  UART0_Init(&func_push_queue);
  Button_Init();
  
  /*Copy Vector Table from Flash to Ram*/
  uint32_t* source_ptr = (uint32_t*)VECTOR_TABLE_FLASH_ADDR;
  uint32_t* des_ptr = (uint32_t*)VECTOR_TABLE_RAM_ADDR;
  for (uint32_t i = 0; i < 48; i++)
  {
      *(des_ptr + i) = *(source_ptr + i);
  }
  SCB->VTOR = VECTOR_TABLE_RAM_ADDR;
  
  /*Erase before load App*/
  if (*Value_App1 != 0x12345678)
  {
    __disable_irq();
    Flash_EraseSector(0xA000);
    Flash_WriteLongWord(0x30000, 0x12345678);
    __enable_irq();
  }
  
  if (*Value_App2 != 0x12345678)
  {
    __disable_irq();
    Flash_EraseSector(0x20000);
    Flash_WriteLongWord(0x30010, 0x12345678);
    __enable_irq();
  }

  while(1)
  {
    /*If Queue is not Empty*/
    if (!CircurlarQueue_IsEmpty())
    {
      Arr_temp[count] = CircurlarQueue_PopData();
      count++;
      if (Arr_temp[count-1] == '\n')
      {
        Handle_File(&Arr_temp[0]);
        count = 0;
      }
    }

    /*Check if the file has finished loading and jump to the new app*/
    if (*End_Of_Srec == 0x12345678)
    {
      SysTick_Init();
      JumptoApplication();
    }
  }
}
