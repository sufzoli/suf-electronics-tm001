/*
 * pcf8576_lcd35.c
 *
 *  Created on: 2013.04.20.
 *      Author: zoli
 */

#include "i2c_usi.h"
#include "textconv.h"
#include "7segment.h"
#include "pcf8576_lcd35.h"

unsigned char _lcd_buff[4];

void lcd35_init(unsigned char initialize_i2c, char pullup_i2c)
{
//	unsigned char i2c_command = 0x49;
	unsigned char i2c_command = 0x59;
	LCD1D = 0;
	LCD2D = 0;
	LCD3D = 0;
	LCDFL = 0;
	if(initialize_i2c)
	{
		i2c_init(pullup_i2c);
	}
	i2c_transmit(PCF8576_ADDR, 1, &i2c_command);
}

void lcd35_send()
{
	unsigned char i2c_command = 0x49;
	unsigned char buff[5];
	i2c_transmit(PCF8576_ADDR, 1, &i2c_command);
	buff[0] = 0;
	buff[1] = LCD3D;
	buff[2] = LCD2D;
	buff[3] = LCD1D;
	buff[4] = LCDFL;
	i2c_transmit(PCF8576_ADDR, 5, &buff[0]);
	i2c_command = 0x59;
	i2c_transmit(PCF8576_ADDR, 1, &i2c_command);
}

/*
void lcd35_wlong(unsigned long value)
{
	unsigned char disp[5];
	if(value < 2000)
	{
	    LongToBCD(4,disp,value,0);
	    LCD3D = segments[disp[0]];
	    LCD2D = segments[disp[1]];
	    LCD1D = segments[disp[2]];
	    if(disp[3] == 1)
	    {
	    	LCDFL |= 0x40;
	    }
	    else
	    {
	    	LCDFL &= ~0x40;
	    }
	}
}
*/

void lcd35_write(unsigned int value)
{
	unsigned char disp[5];
	if(value < 2000)
	{
//	    LongToBCD(4,disp,value,0);
	    IntToBCD(4,disp,value,0);
	    LCD3D = segments[disp[0]];
	    LCD2D = segments[disp[1]];
	    LCD1D = segments[disp[2]];
	    if(disp[3] == 1)	// Switch on the half digit
	    {
	    	LCDFL |= LCD_Z;
	    }
	    else
	    {
	    	LCDFL &= ~LCD_Z;
	    }
	}
}
