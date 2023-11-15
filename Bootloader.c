#include "Bootloader.h"

static volatile uint32_t PROGRAM_START_ADDRESS_APP1 = 0XA000;
static volatile uint32_t PROGRAM_START_ADDRESS_APP2 = 0X20000;
static volatile uint8_t App_Select = 1;
static volatile uint8_t signal = 1;

/*Function to config 1MHz Processor Clock*/
void Config_CoreClock()
{
   //Internal reference clock
   MCG->C1 |= MCG_C1_CLKS(1);
   
   //Select fast internal(4 MHZ IRC)
   MCG->C2 &= ~MCG_C2_IRCS_MASK;
   MCG->C2 |= MCG_C2_IRCS(1);
   
   //Divede 1
   MCG->SC &= ~MCG_SC_FCRDIV_MASK;
   
   //Divide 4
   SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1_MASK;
   SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(3); 
   
   /*Update Processor Clock*/
   SystemCoreClockUpdate();
}

/*SysTick_Init function*/
void SysTick_Init()
{
  /*Config 1MHz Processor Clock*/
  Config_CoreClock();
  
  //Clear current value
  SysTick->VAL = 0u;
  
  //Set clock source - Processor Clock
  SysTick->CTRL |= (1 << 2);
}

/*Sys_Delay function - param: time(ms)*/
void Sys_Delay(uint32_t timems)
{
  //Set Reload value
  SysTick->LOAD = ((SystemCoreClock/1000) * timems);
  
  //Set Enable
  SysTick->CTRL |= (1 << 0);
  
  //Wait Flag
  while ( ((SysTick->CTRL & (1 << 16)) == 0) && signal == 1);
  
  //Set Disable
  SysTick->CTRL &= ~(1 << 0);
}

/*Initialize Button for interrupt to jump to App*/
void Button_Init()
{
  //Enable CLock for PortC
  SIM->SCGC5 |= (1 << 11);
  
  //SET PORTC12 as GPIO - Multiplexer
  PORTC->PCR[12] &= ~PORT_PCR_MUX_MASK;
  PORTC->PCR[12] |= PORT_PCR_MUX(1);
  
  //Set GPIOC_12 = Input - Direction
  GPIOC->PDDR &= ~(1 << 12);
  
  //Set pull up = default logic 1 for PORTC12
  PORTC->PCR[12] |= PORT_PCR_PE(1);
  PORTC->PCR[12] |= PORT_PCR_PS(1);
  
  //Set enable interrupt - Falling edge for PORTC12
  PORTC->PCR[12] &= ~PORT_PCR_IRQC_MASK;
  PORTC->PCR[12] |= PORT_PCR_IRQC(10);
  
  //Set enable interrupt = NVIC
  NVIC->ISER[0] |= (1u << 31);
}

/*IRQ handler*/
void PORTC_PORTD_IRQHandler(void)
{
  PORTC->ISFR |= (1 << 12);
  App_Select = 2;
  signal = 0;
}

/*Jump to APP Select*/
void JumptoApplication()
{
  Sys_Delay(5000);
  if (App_Select == 2)
  {
    /*Disable all interrupt*/
    __disable_irq();

    /*Set Main Stack Pointer*/
    __set_MSP(*((volatile uint32_t*) PROGRAM_START_ADDRESS_APP2));
    SCB->VTOR = PROGRAM_START_ADDRESS_APP2;
    
    /*Set Program Counter*/
    void (*reset_handler)(void) = (void (*)(void))(*((volatile uint32_t*) (PROGRAM_START_ADDRESS_APP2 + 4)));
    
    /*Jump to Application*/
    reset_handler();
  }
  
  else if (App_Select == 1)
  {
    /*Disable all interrupt*/
    __disable_irq();
    
    /*Set Main Stack Pointer*/
    __set_MSP(*((volatile uint32_t*) PROGRAM_START_ADDRESS_APP1));
    SCB->VTOR = PROGRAM_START_ADDRESS_APP1;
    
    /*Set Program Counter*/
    void (*reset_handler)(void) = (void (*)(void))(*((volatile uint32_t*) (PROGRAM_START_ADDRESS_APP1 + 4)));
    
    /*Jump to Application*/
    reset_handler();
  }
}