#include <avr/io.h>
#include <util/delay.h>

#include "i2clcd.h"

#define set_output(portdir,pin) portdir |= _BV(pin)

int main(void) {

    lcd_init();                                             //-     Display initialization
    char string[] = "Hi World";
    lcd_gotolr(1,4);
    lcd_print((unsigned char *)string);                                      //-     Print a string

    //-     Turn cursor off and activate blinking
    lcd_command(LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKINGON);

    for(;;)
        {
            _delay_ms(1000);
        }
}
