/*
 * pcf8576_lcd35.h
 *
 *  Created on: 2013.04.20.
 *      Author: zoli
 */

#ifndef PCF8576_LCD35_H_
#define PCF8576_LCD35_H_

void lcd35_init(unsigned char i2c_init, char i2c_pullup);
void lcd35_send();
void lcd35_write(unsigned int value);

#define PCF8576_ADDR	0x38

#define LCD_Y	0x80
#define LCD_Z	0x40
#define LCD_X	0x01

#define LCD_P1	0x08
#define LCD_P2	0x04
#define LCD_P3	0x02

#define LCD_COL 0x20
#define LCD_LOW_BAT	0x10

#define LCD1D	_lcd_buff[0]
#define LCD2D	_lcd_buff[1]
#define LCD3D	_lcd_buff[2]
#define LCDFL	_lcd_buff[3]

extern unsigned char _lcd_buff[4];

#endif /* PCF8576_LCD35_H_ */
