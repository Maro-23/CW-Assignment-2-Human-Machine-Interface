#include <util/delay.h>
#include "Lcd.h"
#include "Adc_LED.h"
#include "dio.h"
#include "bit_control.h"
#include <math.h>

int main()
{
	LCD_Init();			//initialize LCD
  Adc_Init();     //initialize ADC
  DIO_init();     //initialize DIO

  unsigned long adc_reading;       //declare adc_reading
  unsigned long adc_compare = 5000;//declare adc_compare measurement
  
  unsigned char reading_buffer[10];// reading buffer for printing to LCD
  unsigned char limit_buffer[10]; //limit buffer for printing to LCD

  int prevButton_state1 = DIO_GetPinState('d', 2); //whole block of code is to allow for edge detection during button presses for button 1 and button 2
  int Button_state1;
  int prevButton_state2 = DIO_GetPinState('d', 3);
  int Button_state2;

  while(1){

  
  adc_reading = Adc_ReadChannel(1); //put adc reading from channel 1 into variable
  Button_state1 = DIO_GetPinState('d',2); //get current button reading for edge detection
  Button_state2 = DIO_GetPinState('d',3); //get current button reading for edge detection

  unsigned long display_reading = ceil(((10000*adc_reading)/1023));   //transform the steps value to a value relative to the resistance of the potentiometer

  if(Button_state1 == HIGH && Button_state1 != prevButton_state1){ //if button 1 is high and rising edge, then increment adc_compare value
    if(adc_compare<10000){ //only increment it if it is below the limit of 1023
      adc_compare += 100;//increment the adc_compare value
    }
  }

  if(Button_state2 == HIGH && Button_state2 != prevButton_state2){ //if button 1 is high and rising edge, then decrement adc_compare value
    if(adc_compare>0){ //only decrement it if it is above the limit of 0
      adc_compare -= 100; //decrement the adc_compare value
    }
  }



  if(display_reading >= adc_compare){ //compare the reading from the adc channel to the value of the adc_compare, if it is higher, then execute
    DIO_SetPinState('b', 3 , 'h'); //turn on the red LED (WARNING)
    DIO_SetPinState('b', 4 , 'l'); //turn off the green LED (NO SAFE)
  }else {
    DIO_SetPinState('b', 3 , 'l'); //turn off the red LED (NO WARNING)
    DIO_SetPinState('b', 4 , 'h'); //turn on the green LED (SAFE)
  }

  IntToStr(display_reading, reading_buffer); //change the value to be displayed on the screen from an integer to a string to display properly

  LCD_Command(0x01); //clear the display each loop
  LCD_Command(0x80); //move cursor to first line
	LCD_String("VALUE = ");	  //write "reading = " on the first row
  LCD_String(reading_buffer); //display the value in reading buffer next to reading
  LCD_Command(0xC0);		      //move cursor to second row
  LCD_String("LIMIT = ");     //write "limit = " on the second row
  LCD_String(limit_buffer);   //display the value in limit buffer next to the limit

  prevButton_state1  = Button_state1;  //move current button state to previous button state to allow for edge detection
  prevButton_state2  = Button_state2;   //move current button state to previous button state to allow for edge detection
  
  _delay_ms(200); //delay for screen visability
  }

	return 0;
}
