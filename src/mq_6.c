#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "i2clcd.h"

const uint8_t PROGMEM bar1[] = {
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00
};
const uint8_t PROGMEM bar2[] = {
        0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00
};
const uint8_t PROGMEM bar3[] = {
        0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00
};
const uint8_t PROGMEM bar4[] = {
        0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x00
};
const uint8_t PROGMEM bar5[] = {
        0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00
};

#define PROGRESSPIXELS_PER_CHAR 6
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80

#define ALARM (60)
#define output_low(port,pin) port &= ~_BV(pin)
#define output_high(port,pin) port |= _BV(pin)
#define set_input(portdir,pin) portdir &= ~_BV(pin)
#define set_output(portdir,pin) portdir |= _BV(pin)

void setup_adc() {
    ADMUX |= (_BV(ADLAR) | _BV(MUX0));
    ADCSRA |= (_BV(ADEN) | _BV(ADPS2) | _BV(ADPS1));
    DIDR0 |= _BV(ADC1D);
}

unsigned char get_adc()  {
    ADCSRA |= _BV(ADSC);

    while(ADCSRA & _BV(ADSC));

    return ADCH;

}

unsigned char *bin2dec3(unsigned char val) {
    static unsigned char str[] = "DDD";
    str[2] = '0' + (val % 10);
    val /= 10;
    str[1] = '0' + (val % 10);
    val /= 10;
    str[0] = '0' + (val % 10);
    if (str[0] == '0') {
            str[0] = ' ';
            if (str[1] == '0') {
                    str[1] = ' ';
            }
    }
    return str;
}

void lcd_data(uint8_t data) {
    lcd_putchar(data);
    // execution time 37+4 us
    _delay_us(100);
}

void lcd_customchar_P(uint8_t number, const uint8_t * PROGMEM data) {
    lcd_command(LCD_SET_CGRAM_ADDR|number<<3);
    uint8_t bytes=8;

    uint8_t i;
    for (i=0;i<bytes;i++) {
            lcd_data(pgm_read_byte(data));
            data++;
    }

    lcd_command(LCD_SET_DDRAM_ADDR|0);
}

void progressBar(uint8_t x, uint8_t y, uint8_t progress,
                 uint8_t maxprogress, uint8_t length) {
    uint8_t i;
    uint16_t pixelprogress;
    uint8_t c;
    char buff[17];

    pixelprogress = ((progress*(length*PROGRESSPIXELS_PER_CHAR))/maxprogress);

    // print exactly "length" characters
    for(i=0; i<length; i++)
        {
            // check if this is a full block, or partial or empty
            // (u16) cast is needed to avoid sign comparison warning
            if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)+5) > pixelprogress )
                {
                    // this is a partial or empty block
                    if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)) >= pixelprogress )
                        {
                            // this is an empty block
                            // use space character?
                            c = ' ';
                        }
                    else
                        {
                            // this is a partial block
                            c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
                        }
                }
            else
                {
                    // this is a full block
                    c = 5;
                }

            // write character to display
            buff[i] = c;
        }
    buff[length] = '\0';
    lcd_gotolr(y, x);
    lcd_print(buff);

}

static void setup() {
    set_output(DDRB, PB1);
    output_low(PORTB, PB1);

    setup_adc();
    lcd_backlight(ON);
    lcd_init();

    lcd_customchar_P(1, bar1);
    lcd_customchar_P(2, bar2);
    lcd_customchar_P(3, bar3);
    lcd_customchar_P(4, bar4);
    lcd_customchar_P(5, bar5);
}

int main(void) {

    setup();

    for(;;)
        {
            unsigned char v = get_adc();
            lcd_gotolr(1,0);
            lcd_print(bin2dec3(v));
            progressBar(0, 0, v, 255, 16);
            if(v > ALARM)
                output_high(PORTB, PB1);
            _delay_ms(10);
            output_low(PORTB, PB1);
        }
}
