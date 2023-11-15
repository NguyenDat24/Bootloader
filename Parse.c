#include "Parse.h"
#include "Flash.h"

/*Declare macro function*/
#define Convert_2byte(a, b)                               ((a << 4) | b ) << 1
#define Convert_8byte(e0, e1, e2, e3, e4, e5, e6, e7)     ((e6 << 28) | (e7 << 24)) | ((e4 << 20) | (e5 << 16)) | ((e2 << 12) | (e3 << 8)) | ((e0 << 4) | (e1 << 0))
static uint8_t App_count = 0;

/*Char_to_Dec function*/
uint32_t ChartoDec(uint8_t Data)
{
  uint32_t result = 0;

  /*If char in [0 - 9]*/
  if ( (47 < Data) && (Data < 58) ) 
  {
    result = Data - 48;
  }
  /*If char in [A - F]*/
 else if ( (64 < Data) && (Data < 71) )
  {
    result = Data - 55;
  }

  /*Return decimal number*/
  return result;
}

void Handle_File(uint8_t *Ptr_Arr)
{
  uint32_t len_data = 0;
  uint32_t Address_sum = 0;
  uint32_t Data_sum = 0;
  uint32_t temp_address = 1;
  uint8_t index = 0;
  uint8_t odd_byte = 0;
  uint8_t mode_write = 0;
  uint8_t start_index = 0;
  uint8_t end_index = 0;
  
  /*Check if 2 APP have been loaded*/
  if (Ptr_Arr[1] == '7' || Ptr_Arr[1] == '8' || Ptr_Arr[1] == '9')
  {
    App_count++;
    if (App_count == 2)
    {
      __disable_irq();
      Flash_WriteLongWord(0x30020, 0x12345678);
    }
  }
  
  if ( (Ptr_Arr[0] == 'S') && (Ptr_Arr[1] != '0') && Ptr_Arr[1] != '7' && Ptr_Arr[1] != '8' && Ptr_Arr[1] != '9')
  {
/**********CHECK SRECORD TYPES: S1/S2/S3*********/    
    /*S1 : 2 bytes Address - Conculate start index to parse Data and Address*/
    if (Ptr_Arr[1] == '1')
    {
      len_data = (Convert_2byte(ChartoDec(Ptr_Arr[2]), ChartoDec(Ptr_Arr[3]))) - 6;
      start_index = 7;
    }
    /*S2 : 3 bytes Address - Conculate start index to parse Data and Address*/
    else if (Ptr_Arr[1] == '2')
    {
      len_data = (Convert_2byte(ChartoDec(Ptr_Arr[2]), ChartoDec(Ptr_Arr[3]))) - 8;
      start_index = 9;
    }
    /*S3 : 4 bytes Address - Conculate start index to parse Data and Address*/
    else if (Ptr_Arr[1] == '3')
    {
      len_data = (Convert_2byte(ChartoDec(Ptr_Arr[2]), ChartoDec(Ptr_Arr[3]))) - 10;
      start_index = 11;
    }

/**********SET MODE_WRITE(0,1,2,3) - CHECK ODD BYTES(1,2,3) - END_INDEX(4n BYTES)*********/   
    
    if ( (len_data % 8) == 0)
    {
      mode_write = 0;
      odd_byte = 0;
      end_index = len_data + ((start_index + 1) - odd_byte);
    }
    /*1 Odd Byte */
    else if ( ((len_data % 2) == 0) && ((len_data % 4) != 0) && (((len_data + 2) % 8) != 0))
    {
      mode_write = 1;
      odd_byte = 2;
      end_index = len_data + ((start_index + 1) - odd_byte);
    }
    /*2 Odd Byte */
    else if (((len_data % 4) == 0) && ((len_data % 8) != 0))
    {
      mode_write = 2;
      odd_byte = 4;
      end_index = len_data + ((start_index + 1) - odd_byte);
    }
    /*3 Odd Byte */
    else if ( ((len_data + 2) % 8) == 0 )
    {
      mode_write = 3;
      odd_byte = 6;
      end_index = len_data + ((start_index + 1) - odd_byte);
    }

/*********PARSING DATA AND ADDRESS TO WRITE TO FLASH*********/
    /*Parsing Address*/
    for (index = start_index; index > 3; index--)
    {
      Address_sum += (ChartoDec(Ptr_Arr[index]) * temp_address);
      temp_address = temp_address << 4;
    }
    /*Parsing 4n Byte Data*/
    for (index = (start_index + 1); index < end_index; index += 8)
    {
      Data_sum = Convert_8byte(ChartoDec(Ptr_Arr[index]), ChartoDec(Ptr_Arr[index+1]), ChartoDec(Ptr_Arr[index+2]), ChartoDec(Ptr_Arr[index+3]), ChartoDec(Ptr_Arr[index+4]), ChartoDec(Ptr_Arr[index+5]), ChartoDec(Ptr_Arr[index+6]), ChartoDec(Ptr_Arr[index+7]));
      __disable_irq();
      Flash_WriteLongWord( Address_sum, Data_sum);
      __enable_irq();
      Address_sum += 4;
      Data_sum = 0;
    }
    
/*********Parsing Odd Byte Data*********/
    /*1 Odd Byte*/
    if (mode_write == 1)
    {
      Data_sum = Convert_8byte(ChartoDec(Ptr_Arr[end_index]), ChartoDec(Ptr_Arr[end_index + 1]), 0, 0, 0, 0, 0, 0);
      __disable_irq();
      Flash_WriteLongWord( Address_sum, Data_sum);
      __enable_irq();
    }
    /*2 Odd Byte*/
    else if (mode_write == 2)
    {
      Data_sum = Convert_8byte(ChartoDec(Ptr_Arr[end_index]), ChartoDec(Ptr_Arr[end_index+1]), ChartoDec(Ptr_Arr[end_index+2]), ChartoDec(Ptr_Arr[end_index + 3]), 0, 0, 0, 0);
      __disable_irq();
      Flash_WriteLongWord( Address_sum, Data_sum);
      __enable_irq();
    }
    /*3 Odd Byte*/
    else if (mode_write == 3)
    {
      Data_sum = Convert_8byte(ChartoDec(Ptr_Arr[end_index]), ChartoDec(Ptr_Arr[end_index+1]), ChartoDec(Ptr_Arr[end_index+2]), ChartoDec(Ptr_Arr[end_index+3]), ChartoDec(Ptr_Arr[end_index+4]), ChartoDec(Ptr_Arr[end_index + 5]), 0, 0);
      __disable_irq();
      Flash_WriteLongWord( Address_sum, Data_sum);
      __enable_irq();
    }    
  }
}
