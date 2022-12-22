
#include "Grove_LCD_RGB_Backlight.h"
#include "stm32f0xx_hal.h"
char _displayfunction;
char _displaycontrol;

I2C_HandleTypeDef hi2c;

/*This function is used to turn on the display on the LCD.  It sends the 
hexadecimal code for the display on setting to the device in order to 
turn on the display*/

void displayOn() 
{
    _displaycontrol |= LCD_DISPLAYON;
    sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*This function is used to clear display on the LCD.  It sends the 
hexadecimal code for the clear instruction to the device in order to 
clear the display*/
void clear()
{
    sendCommand(LCD_CLEARDISPLAY);        
    HAL_Delay(2);
}


/*This function is used to set the backlight color of the dispaly.  
It writes the provided r, g, and b values to the registers for the red
value, the green value, and the blue value respectively.*/
void setRGB(char r, char g, char b)
{
   
    setReg(RED_REG, r);
    setReg(GREEN_REG, g);
    setReg(BLUE_REG, b);  
    
}

/*This function is used to write to one of the registers for the backlight
of the LCD display.  The function takes in the address of which register to
write to and the value to be written to that register and then sends it to the
LCD display via the mbed I2C library*/
void setReg(char addr, char val)
{
    char data[2];
    data[0] = addr;
    data[1] = val;
    HAL_I2C_Master_Transmit(&hi2c, RGB_ADDRESS, data, 2, HAL_MAX_DELAY);

}

/*This function is used to write to the LCD screen.  It takes in a string of
characters and writes them to the 0x40 register of the display.*/
void print(char *str)
{   
    char data[2];
    data[0] = 0x40;
    while(*str)
    {
            data[1] = *str;
            HAL_I2C_Master_Transmit(&hi2c, LCD_ADDRESS, data, 2, HAL_MAX_DELAY);
            str++;
            
    }


}


/*This function sets where on the screen the text will be written next.  It 
takes in two values which indicate the row and column on the display that 
the cursor should be moved to*/
void locate(char col, char row)
{
    if(row == 0)
    {
        col = col | 0x80;
    }
    else
    {   
        col = col | 0xc0;
    }
 
    char data[2];
    data[0] = 0x80;
    data[1] = col;
    HAL_I2C_Master_Transmit(&hi2c, LCD_ADDRESS, data, 2, HAL_MAX_DELAY);
}

/*This function sends an instruction to the LCD display using the
 built in instruction codes from the device  datasheet using the mbed
 I2C library*/
void sendCommand(char value)
{
    char data[2] = {0x80, value};
    HAL_I2C_Master_Transmit(&hi2c, LCD_ADDRESS, data, 2, HAL_MAX_DELAY);

}

void initLCD(I2C_HandleTypeDef i2c)
{   
	 hi2c=i2c;
    //Initialize displayfunction parameter for setting up LCD display
   _displayfunction |= LCD_2LINE;
   _displayfunction |= LCD_5x10DOTS;
 
   //Wait for more than 30 ms after power rises above 4.5V per the data sheet
   HAL_Delay(50);


    // Send first function set command. Wait longer that 39 us per the data sheet
    sendCommand(LCD_FUNCTIONSET | _displayfunction);
    HAL_Delay(1);
    
    // turn the display on
    displayOn();

    // clear the display
    clear();
    
    // Initialize backlight
    setReg(0, 0);
    setReg(1, 0);
    setReg(0x08, 0xAA);   
    


    

}

