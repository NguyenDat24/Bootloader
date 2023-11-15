#include "Flash.h"

/*Flash Write function*/
void Flash_WriteLongWord( uint32_t Address, uint32_t Data)
{
  /*Wait CCIF = 1 - Previous command complete*/
  while( (FTFA->FSTAT & (1 << 7)) == 0 );
  
  /*Access Error and Protection Violation Check - Clear error of previous command*/
  if ( ((FTFA->FSTAT & (1 << 4)) == 1) || ((FTFA->FSTAT & (1 << 5)) == 1) )
  {
    FTFA->FSTAT = 0x30;
  }
  
  /*Write to the FCCOB Register*/
  /*Setup Command*/
  FTFA->FCCOB0 = 0x06u;
  
  /*Write Address to FCCOB1, FCCOB2, FCCOB3*/
  FTFA->FCCOB1 = (uint8_t)(Address >> 16);
  FTFA->FCCOB2 = (uint8_t)(Address >> 8);
  FTFA->FCCOB3 = (uint8_t)(Address);
  
  /*Write Data to FCCOB4, FCCOB5, FCCOB6, FCCOB7, FCCOB8, FCCOB9, FCCOBA, FCCOBB*/
  FTFA->FCCOB7 = (uint8_t)(Data);
  FTFA->FCCOB6 = (uint8_t)(Data >> 8);
  FTFA->FCCOB5 = (uint8_t)(Data >> 16);
  FTFA->FCCOB4 = (uint8_t)(Data >> 24);
  
  /*Start command*/
  FTFA->FSTAT = 0x80u;
  
  /*Wait cmd finish*/
  while (FTFA->FSTAT == 0x00);
}

/*Flash Erase function*/
void Flash_EraseSector( uint32_t Address)
{
  /*Wait CCIF = 1 - Previous command complete*/
  while( (FTFA->FSTAT & (1 << 7)) == 0 );
  
  /*Access Error and Protection Violation Check - Clear error of previous command*/
  if ( ((FTFA->FSTAT & (1 << 4)) == 1) || ((FTFA->FSTAT & (1 << 5)) == 1) )
  {
    FTFA->FSTAT = 0x30;
  }
  
  /*Write to the FCCOB Register*/
  //Setup Command
  FTFA->FCCOB0 = 0x9u;
  
  /*Write Address to FCCOB1, FCCOB2, FCCOB3*/
  FTFA->FCCOB1 = (uint8_t)(Address >> 16);
  FTFA->FCCOB2 = (uint8_t)(Address >> 8);
  FTFA->FCCOB3 = (uint8_t)(Address);
  
  /*Start command*/
  FTFA->FSTAT = 0x80u;
  
  /*Wait cmd finish*/
  while (FTFA->FSTAT == 0x00);
}
