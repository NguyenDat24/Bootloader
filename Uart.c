#include "Uart.h"

/*Typedef for the function pointer*/
typedef void (*fun_call_back) (uint8_t);
fun_call_back func_call = 0;

/*Enable Clock for PORTA (1,2), UART0*/
void Clock_Init()
{
  /*Enable Clock for UART0 // Cap clock cho module UART0: Doc, ghi thanh ghi cua module*/
  SIM->SCGC4 |= (1 << 10);
  
  /*MCGIR Clock active*/
  MCG->C1 |= (1 << 1);
  
  /*Enable Clock for transmit and receive - MCGIRCLOCK*/
  SIM->SOPT2 |= (3 << 26);
  
  /*Enable Clock for Port A*/
  SIM->SCGC5 |= (1 << 9);
  
  /*Fast internal reference clock selected*/
  MCG ->C2 |= (1 << 0);
  
  /*FCRDIV: Divide 1*/
  MCG->SC &= ~MCG_SC_FCRDIV_MASK;
}

/*Config Port pin: PA1, PA2 - mode : Alternative2 / Pull Up*/
void Port_UART0_Init()
{
  /*Mode: Alternative2*/
  PORTA->PCR[1] |= (2 << 8);
  PORTA->PCR[2] |= (2 << 8);
  
  /*Set Pull Up for Pin*/
  PORTA->PCR[1] |= (3 << 0);
  PORTA->PCR[2] |= (3 << 0);
}

/*Config Baudrate UART*/
void UART0_Set_Baudrate(Uart0_Config *Uart0_Config)
{
  uint32_t expression = Uart0_Config->Baudrate;
  switch (expression)
  {
    case 600:
    {
      UART0->C4 = ((UART0->C4) & ~(0x1F)) | 0x4;
      UART0->BDL = 53;
      UART0->BDH = 5;
      break;
    }
    case 1200:
    {
      UART0->C4 = ((UART0->C4) & ~(0x1F)) | 0x4;
      UART0->BDL = 154;
      UART0->BDH = 2;
      break;
    }
    case 2400:
    {
      UART0->C4 = ((UART0->C4) & ~(0x1F)) | 0x6;
      UART0->BDL = 238;
      break;
    }
    case 4800:
    {
      UART0->C4 = ((UART0->C4) & ~(0x1F)) | 0x6;
      UART0->BDL = 119;
      break;
    }
    case 9600:
    {
      UART0->C4 = ((UART0->C4) & ~(0x1F)) | 0x3;
      UART0->BDL = 104;
      break;
    }
    case 14400:
    {
      UART0->C4 = ((UART0->C4) & ~(0x1F)) | 0x3;
      UART0->BDL = 69;
      break;
    }
  }
}

/*Config UART - Baudrate - Frame - Interrupt*/
void UART0_Init(fun_call_back func_ptr)
{
  func_call = func_ptr;
  
  /*Frame: 8 bit Data, 1 bit stop, Parity = none*/
  UART0->C1 &= ~(1 << 4);
  UART0->BDH &= ~(1 << 5);
  UART0->C1 &= ~(1 << 1);
  
  /*Receiver samples input data using the rising and falling edge of the baud rate clock*/
  UART0->C5 |= (1 << 1);
  
  /*Enable Transmit*/
  UART0->C2 |= (1 << 3);
  
  /*Enable Receive*/
  UART0->C2 |= (1 << 2);
  
  /*Enable Receive Interrupt & NVIC*/
  UART0->C2 |= (1 << 5);
  NVIC_EnableIRQ(UART0_IRQn);
}

/*UART_IRQ handler*/
void UART0_IRQHandler(void)
{
  /*If RDRF Flag set to 1 -> Read data*/
  if ( (UART0->S1 & (1 << 5)) != 0)
  {
    /*Call back execution*/
    func_call(UART0->D);
  }
}
