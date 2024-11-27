#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <util/atomic.h>

#include <stdbool.h>

#include "speech_synth.h"

int main(void)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        MCUSR &= ~_BV(WDRF);
    }
    sei();

    speech_synth_t *synth = speech_synth_init();

    for (;;)
    {
        for (lpc_name_e n = 0; n < lpc_name_max; n++)
        {
            speech_synth_say(synth, n);
        }
    }
}
