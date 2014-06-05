#include <avr/io.h>
#include <util/delay.h>

#include "i2clcd.h"

int main(void) {

    lcd_init();
    lcd_gotolr(1,4);
    lcd_print((unsigned char *)"Hi World");

    lcd_command(LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKINGON);
    lcd_backlight(0);

    int b = ON;

    for(;;)
        {
            _delay_ms(1000);
            lcd_backlight(b);

            lcd_gotolr(0,1);
            lcd_putchar(0x30+b);

            b^= ON;
        }
}
