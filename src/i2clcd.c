/*****************************************************************************
 
 i2clcd.c - LCD over I2C library 
                Designed for HD44870 based LCDs with I2C expander PCF8574X
                on Atmels AVR MCUs
 
 Copyright (C) 2006 Nico Eichelmann and Thomas Eichelmann
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 
 You can contact the authors at info@computerheld.de
 
*****************************************************************************/

/*
        Version 0.1
        Requires I2C-Library from Peter Fleury http://jump.to/fleury
        
        See i2clcd.h for description and example.
*/
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "i2clcd.h"
#include "i2cmaster.h"


int backlight = OFF;
//-     Display initialization sequence
void lcd_init(void)
{       
        lcd_wait_ms(20);                //-     Wait for more than 15ms after VDD rises to 4.5V
        lcd_write(LCD_D5 | LCD_D4);     //-     Set interface to 8-bit
        lcd_wait_ms(10);                 //-     Wait for more than 4.1ms
        lcd_write(LCD_D5 | LCD_D4);     //-     Set interface to 8-bit
        lcd_wait_ms(10);               //-     Wait for more than 100us
        lcd_write(LCD_D5 | LCD_D4);     //-     Set interface to 8-bit
        lcd_wait_ms(10);
        lcd_write(LCD_D5);              //-     Set interface to 4-bit
        //- From now on in 4-bit-Mode

        lcd_command(LCD_4BIT | LCD_2LINE | LCD_5X7);            //-     2-Lines, 5x7-Matrix
		lcd_command(LCD_DISPLAYOFF);                            //-     Display off
		lcd_command(LCD_CLEAR);                                 //-     Clear Screen
		lcd_command(LCD_INCREASE | LCD_DISPLAYSHIFTOFF);        //-     Entrymode (Display Shift: off, Increment Address Counter)
		lcd_command(LCD_DISPLAYON);                             //-     Display on

}

//-     Write data to i2c
void lcd_write_i2c(unsigned char value)
{
        i2c_start_wait(LCD_I2C_DEVICE+I2C_WRITE);
		
	if (backlight) {
		value |= (1 << LCD_BL_PIN);
	} else {
		value &= ~(1 << LCD_BL_PIN);
	}	
		
        i2c_write(value);
        i2c_stop();
}

//-     Write byte to display with toggle of enable-bit
void lcd_write(unsigned char value)
{
        lcd_write_i2c(value | LCD_E);           //-     Set enable to high
        lcd_write_i2c(value | LCD_E);           //-     Send data, keep enable high
        lcd_write_i2c(value & (!LCD_E));        //-     Set enable to low
}

//-     Print string to cursor position
void lcd_print(unsigned char *string)
{
        unsigned char i = 0;
        while(string[i] != 0x00)
        {
                lcd_putchar(string[i]);
                i++;
        }
}

//-     Put char to cursor position
void lcd_putchar(unsigned char value)
{
        unsigned char lcddata;
        lcddata = value;
        lcddata &= 0xf0;
        lcddata |= LCD_RS;
        lcd_write(lcddata);
        lcddata = value;
        lcddata <<= 4;
        lcddata &= 0xf0;
        lcddata |= LCD_RS;
        lcd_write(lcddata);
}

//-     Put char to position
bool lcd_putcharlr(unsigned char line, unsigned char row, unsigned char value)
{
        if(!lcd_gotolr(line, row)) return false;
        lcd_putchar(value);
        
        return true;
}

//-     Issue a command to the display (use the defined commands above)
void lcd_command(unsigned char command)
{
        unsigned char lcddata;
        lcddata = command;
        lcddata &= 0xf0;
        lcd_write(lcddata);
        lcddata = command;
        lcddata <<= 4;
        lcddata &= 0xf0;
        lcd_write(lcddata);     
}

//-     Print string to position (If string is longer than LCD_ROWS overwrite first chars)(line, row, string)
bool lcd_printlc(unsigned char line, unsigned char row, unsigned char *string)
{
        unsigned char i;
                
        for(i = 0; string[i] != 0x00; i++)
        {
                if (!lcd_putcharlr(line, row, string[i])) return false;
                row++;
                if(row > LCD_ROWS)
                {
                        row = 1;
                }
        }
        return true;
}

//-     Print string to position (If string is longer than LCD_ROWS continue in next line)(line, row, string)
bool lcd_printlrc(unsigned char line, unsigned char row, unsigned char *string)
{
        unsigned char i;
                
        for(i = 0; string[i] != 0x00; i++)
        {
                if(!lcd_putcharlr(line, row, string[i])) return false;
                row++;
                if(row > LCD_ROWS)
                {
                        line++;
                        row = 1;
                }
                if(line > LCD_LINES)
                {
                        line = 1;
                }
        }
        return true;
}

//-     Print string to position (line, row, string)
bool lcd_printlr(unsigned char line, unsigned char row, unsigned char *string)
{
        if(!lcd_gotolr(line, row)) return false;
        lcd_print(string);
        
        return true;
}

//-     Go to position (line, row)
bool lcd_gotolr(unsigned char line, unsigned char row )
{
        unsigned char lcddata;
        
        if(line > LCD_LINES) return false;
        if(row > LCD_ROWS) return false;
        if((line == 0) || (row == 0) ) return false;

        unsigned char line_addr=(line%2)*0x40;
        lcd_command(0x80|(line_addr+row));
        
        return true;    
}

//-     Go to nextline (if next line > LCD_LINES return false)
bool lcd_nextline(void)
{
        unsigned char line = 0x00, row = 0x00;
        lcd_getlr(&line, &row);
        if (!lcd_gotolr(line + 1, 1)) return false;
                else return true;
}

//-     Read data from i2c
unsigned char lcd_read_i2c(void)
{
        unsigned char lcddata = 0x00;
        i2c_start_wait(LCD_I2C_DEVICE+I2C_READ);
        lcddata = i2c_readNak();
        i2c_stop();
        return lcddata;
}

//-     Read data from display over i2c (second nibble corresponds to highbyte or lowbyte)
unsigned char lcd_read(bool mode)
{
        unsigned char lcddata;
        if(mode == LCD_DATA)
                lcd_write_i2c(LCD_E | LCD_RS | LCD_RW | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
        else
                lcd_write_i2c(LCD_E | LCD_RW | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
        
        lcddata = lcd_read_i2c();
        lcd_write_i2c(0x00);
        return lcddata;
}

//-     Read one complete byte over i2c from display
unsigned char lcd_getbyte(bool mode)
{
        unsigned char hib, lob;
        hib = lcd_read(mode);
        lob = lcd_read(mode);
        return (hib << 4) + (lob & 0x0F);
}

//-     Get line and row (target byte for line, target byte for row)
bool lcd_getlr(unsigned char *line, unsigned char *row)
{
        unsigned char lcddata, tmp;
        lcddata =       lcd_getbyte(LCD_ADDRESS);
        if (lcddata & (1 << 7)) return false;
        tmp = lcddata;
        tmp &= 0xF0;
        switch(tmp)
        {
                case LCD_LINE1:
                        *line = 1;
                        break;
#if LCD_LINES>=2
                case LCD_LINE2:
                        *line = 2;
                        break;
#endif
#if LCD_LINES>=3
                case LCD_LINE3:
                        *line = 3;
                        break;
#endif
#if LCD_LINES>=4
                case LCD_LINE4:
                        *line = 4;
                        break;
#endif
                default:
                        return false;
        }
        lcddata &= 0x0F;
        *row = lcddata + 1;
        return true;
}

//-     Check if busy
bool lcd_busy(void)
{
        unsigned char state = lcd_getbyte(LCD_ADDRESS);
        if (state & (1 << 7)) return true;
                else return false;
}

//-     Wait some microseconds
void lcd_wait_us(unsigned short us)
{
        unsigned short i;
        for(i = 0; i < us; i++)
        {
                wait1us;
        }
}

//-     Wait some milliseconds
void lcd_wait_ms(unsigned short ms)
{
        unsigned short i;
        for(i = 0; i < ms; i++)
        {
                wait1ms;
        }
}


void lcd_backlight(int bl)
{
	backlight = bl;
}
