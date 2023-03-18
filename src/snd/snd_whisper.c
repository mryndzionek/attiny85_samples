#include "snd.h"

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include "lpc_data.h"
#include "annoyatron.h"

#define DC_BLOCK_FACTOR (0x3cc)
#define INVERSE_SPEED (2)

#define INIT_LPC(_id)                          \
    curr_a = (fix16_t const *)LPC_##_id##_A;   \
    curr_g = (fix16_t const *)LPC_##_id##_G;   \
    curr_ps = (fix16_t const *)LPC_##_id##_PS; \
    curr_len = sizeof(LPC_##_id##_PS);

const uint16_t SND_DIV = 250;

static fix16_t g_xs[LPC_ORDER];
static fix16_t g_hs[LPC_ORDER];
static uint16_t g_i = 0;
static uint16_t g_j = 0;
static uint16_t g_ps;
static fix16_t g_g;
static fix16_t g_de_y;
static fix16_t g_dc_y;
static uint16_t g_t;

static fix16_t const *curr_g;
static fix16_t const *curr_ps;
static fix16_t const *curr_a;
static uint16_t curr_len;

static void update(void)
{
    g_g = pgm_read_word(&curr_g[g_i]);
    g_ps = pgm_read_byte(&curr_ps[g_i]);

    for (uint8_t j = 0; j < LPC_ORDER; j++)
    {
        g_hs[j] = pgm_read_word(&curr_a[(g_i * LPC_ORDER) + j]);
        g_xs[j] = FIX_ZERO;
    }

    g_de_y = FIX_ZERO;
    g_dc_y = FIX_ZERO;
    g_t = 0;
}

static uint16_t _rnd(uint16_t range)
{
    static uint16_t lfsr = 0xACE1u;

    unsigned lsb = lfsr & 1;
    lfsr >>= 1;
    if (lsb)
    {
        lfsr ^= 0xB400u;
    }

    return lfsr % range;
}

static inline fix16_t exec(void)
{
    fix16_t ny = FIX_ZERO;
    fix16_t y = FIX_ZERO;

    uint32_t rnd = _rnd(0xFFFF) | ((uint32_t)_rnd(0xFFFF) << 16);
    fix16_t noise = (rnd % (2 * FIX_ONE)) - FIX_ONE;

    // unvoiced
    ny = fix_mul(g_g, noise);

    // fir convolution
    for (uint8_t k = 0; k < LPC_ORDER; k++)
    {
        ny = fix_add(ny, fix_mul(g_hs[k], g_xs[k]));
    }

    // dc-blocking
    y = fix_add(fix_add(ny, -g_de_y), fix_mul(DC_BLOCK_FACTOR, g_dc_y));
    g_de_y = ny;
    g_dc_y = y;

    for (uint8_t i = LPC_ORDER - 1; i > 0; i--)
    {
        g_xs[i] = g_xs[i - 1];
    }

    g_xs[0] = ny;
    g_t++;

    return y;
}

static void choose(void)
{
    uint8_t i = rand() % 9;
    switch (i)
    {
    case 0:
        INIT_LPC(0);
        break;

    case 1:
        INIT_LPC(1);
        break;

    case 2:
        INIT_LPC(2);
        break;

    case 3:
        INIT_LPC(3);
        break;

    case 4:
        INIT_LPC(4);
        break;

    case 5:
        INIT_LPC(5);
        break;

    case 6:
        INIT_LPC(6);
        break;

    case 7:
        INIT_LPC(7);
        break;

    case 8:
        INIT_LPC(8);
        break;
    }
}

bool snd_get_sample(uint8_t *sample)
{
    static bool start = true;

    if (start)
    {
        choose();
        start = false;
    }

    if (g_i < curr_len)
    {
        if (g_j == 0)
        {
            update();
        }

        fix16_t y = exec();
        g_j++;
        if (g_j == (LPC_FRAME_LEN * INVERSE_SPEED))
        {
            g_i++;
            g_j = 0;
        }
        *sample = ((y / 120) + 128);
        return false;
    }
    else
    {
        g_i = 0;
        start = true;
        return true;
    }
}

void snd_wait(void)
{
    annoy_deep_sleep(1);
}
