#include "snd.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "annoyatron.h"

const uint16_t SND_DIV = 62;

static const uint8_t wavtable[] PROGMEM = {
    128, 129, 127, 126, 128, 130, 127, 125, 128, 130, 127, 124, 128, 131, 127, 123,
    128, 132, 127, 123, 128, 132, 127, 122, 129, 133, 126, 122, 129, 134, 126, 121,
    129, 134, 125, 121, 130, 134, 125, 121, 130, 134, 125, 121, 131, 135, 124, 120,
    131, 135, 124, 121, 131, 135, 124, 121, 131, 134, 124, 121, 132, 134, 123, 121,
    132, 134, 123, 122, 132, 133, 123, 122, 132, 133, 123, 123, 132, 132, 123, 123,
    131, 131, 124, 124, 131, 131, 124, 125, 130, 130, 125, 126, 130, 129, 126, 126,
    129, 129, 126, 127, 128, 128, 128, 128, 127, 127, 129, 128, 126, 126, 130, 129,
    125, 126, 131, 130, 123, 125, 133, 130, 122, 125, 134, 130, 120, 124, 136, 131,
    118, 124, 137, 131, 117, 124, 139, 131, 115, 124, 141, 131, 113, 124, 143, 131,
    112, 124, 144, 131, 110, 125, 146, 130, 108, 125, 147, 130, 107, 126, 149, 129,
    105, 126, 151, 128, 104, 127, 152, 127, 102, 128, 153, 126, 101, 129, 155, 125,
    100, 130, 156, 124, 99, 131, 156, 123, 98, 133, 157, 122, 97, 134, 158, 120,
    97, 135, 158, 119, 97, 137, 158, 118, 97, 138, 158, 117, 97, 139, 158, 115,
    97, 140, 158, 114, 98, 141, 157, 113, 98, 142, 156, 112, 99, 143, 155, 111,
    100, 144, 154, 111, 101, 145, 153, 110, 103, 145, 152, 110, 104, 146, 150, 109,
    106, 146, 149, 109, 107, 146, 147, 109, 109, 146, 145, 110, 110, 145, 144, 110,
    112, 145, 142, 111, 114, 144, 140, 112, 115, 143, 139, 113, 117, 142, 137, 114,
    119, 140, 135, 115, 120, 139, 134, 117, 122, 137, 133, 118, 123, 136, 131, 120,
    124, 134, 130, 122, 125, 132, 129, 124, 126, 130, 128, 126, 127, 128, 128, 128,
    128, 126, 127, 130, 128, 124, 127, 132, 129, 122, 126, 134, 129, 120, 126, 136,
    129, 118, 126, 138, 128, 116, 127, 140, 128, 114, 127, 142, 128, 113, 128, 143,
    127, 111, 128, 145, 126, 110, 129, 146, 126, 108, 130, 147, 125, 107, 131, 148,
    124, 106, 132, 149, 123, 106, 133, 150, 122, 105, 134, 150, 121, 105, 135, 150,
    120, 105, 136, 150, 119, 105, 137, 150, 118, 105, 138, 150, 117, 105, 139, 150,
    116, 106, 139, 149, 115, 106, 140, 148, 115, 107, 141, 147, 114, 108, 141, 147,
    114, 109, 141, 145, 114, 110, 141, 144, 114, 111, 142, 143, 114, 113, 142, 142,
    114, 114, 141, 140, 114, 115, 141, 139, 114, 116, 140, 138, 115, 118, 140, 137,
    115, 119, 139, 136, 116, 120, 138, 135, 117, 121, 138, 133, 118, 122, 137, 132,
    119, 123, 136, 132, 120, 124, 135, 131, 121, 125, 134, 130, 122, 125, 133, 129,
    123, 126, 132, 129, 123, 126, 131, 129, 124, 127, 130, 128, 125, 127, 130, 128,
    126, 127, 129, 128, 126, 127, 128, 128, 127, 127, 128, 128, 127, 128, 128, 128,
    127, 128, 127, 128, 127, 128, 128, 127, 128, 127, 128, 127, 127, 127, 128, 127,
    127, 128, 128, 127, 126, 128, 129, 127, 126, 128, 130, 127, 125, 128, 130, 127,
    124, 129, 131, 126, 123, 129, 132, 126, 122, 129, 133, 125, 121, 130, 135, 124,
    120, 131, 136, 124, 119, 132, 137, 122, 118, 133, 138, 121, 116, 134, 139, 120,
    115, 136, 140, 119, 114, 137, 142, 117, 113, 139, 143, 115, 112, 141, 144, 114,
    111, 142, 145, 112, 110, 144, 145, 110, 109, 146, 146, 108, 109, 148, 146, 106,
    109, 150, 146, 104, 109, 152, 146, 102, 109, 154, 146, 100, 109, 156, 146, 98,
    109, 158, 145, 97, 110, 159, 145, 95, 111, 161, 144, 94, 112, 162, 143, 93,
    113, 163, 142, 92, 114, 164, 140, 91, 115, 164, 139, 91, 117, 164, 138, 91,
    118, 164, 136, 91, 120, 164, 135, 91, 121, 163, 133, 92, 123, 162, 132, 93,
    124, 161, 130, 95, 125, 160, 129, 96, 127, 158, 128, 98, 128, 155, 127, 101,
    129, 153, 126, 103, 130, 150, 125, 107, 130, 147, 125, 110, 130, 144, 125, 113,
    130, 140, 125, 117, 130, 136, 125, 121, 129, 132, 126, 124, 128, 128, 127, 129,
    127, 125, 129, 133, 125, 120, 131, 137, 123, 116, 133, 141, 120, 112, 136, 145,
    118, 109, 139, 148, 114, 105, 142, 152, 111, 101, 146, 156, 107, 98, 150, 159,
    103, 95, 154, 162, 99, 92, 159, 164, 94, 90, 163, 167, 89, 88, 168, 168,
    85, 86, 173, 170, 80, 85, 178, 171, 75, 84, 183, 172, 70, 83, 188, 172,
    65, 83, 193, 172, 60, 84, 197, 171, 55, 85, 202, 170, 51, 86, 206, 168,
    47, 88, 210, 166, 43, 90, 214, 164, 40, 92, 217, 162, 37, 95, 220, 159,
    34, 98, 222, 155, 32, 101, 224, 152, 30, 105, 225, 148, 29, 108, 226, 145,
    29, 112, 226, 141, 29, 116, 226, 137, 29, 120, 225, 133, 31, 124, 223, 130,
    33, 127, 221, 126, 35, 131, 219, 123, 38, 134, 215, 120, 42, 137, 212, 117,
    46, 140, 207, 114, 50, 142, 202, 112, 55, 144, 197, 111, 61, 145, 192, 109,
    66, 146, 186, 109, 72, 146, 179, 109, 79, 146, 173, 109, 85, 145, 167, 110,
    92, 144, 160, 112, 98, 142, 153, 114, 105, 140, 147, 117, 112, 137, 140, 120,
    118, 133, 134, 124, 124, 129, 128, 129, 130, 124, 122, 134, 136, 119, 116, 139,
    142, 113, 111, 145, 146, 107, 106, 151, 151, 101, 102, 157, 155, 94, 98, 164,
    158, 87, 95, 171, 161, 81, 93, 178, 163, 74, 91, 185, 165, 67, 89, 192,
    166, 60, 89, 199, 167, 52, 89, 206, 166, 46, 89, 212, 166, 39, 90, 219,
    164, 33, 92, 225, 162, 27, 94, 230, 160, 22, 97, 236, 157, 17, 100, 240,
    154, 13, 103, 244, 150, 9, 107, 248, 146, 6, 111, 250, 142, 3, 116, 253,
    137, 1, 121, 254, 132, 0, 125, 255, 127, 0, 130, 255, 123, 0, 135, 254,
    118, 1, 140, 253, 113, 3, 144, 251, 109, 5, 148, 248, 105, 8, 153, 245,
    101, 12, 156, 241, 97, 16, 160, 236, 94, 21, 163, 231, 91, 26, 165, 226,
    89, 32, 167, 220, 87, 38, 168, 214, 86, 44, 169, 207, 86, 51, 169, 201,
    86, 58, 169, 194, 87, 65, 168, 187, 88, 72, 166, 180, 90, 79, 164, 173,
    92, 86, 161, 166, 95, 92, 158, 159, 99, 99, 154, 153, 103, 105, 149, 147,
    108, 111, 145, 141, 113, 117, 139, 136, 119, 122, 133, 131, 124, 126, 128, 126,
    131, 131, 121, 123, 137, 134, 115, 119, 144, 137, 108, 116, 150, 140, 101, 114,
    157, 142, 95, 113, 163, 143, 88, 112, 170, 144, 82, 111, 177, 144, 75, 111,
    183, 144, 69, 112, 189, 143, 64, 113, 194, 142, 58, 114, 200, 140, 53, 116,
    204, 138, 48, 119, 209, 135, 44, 121, 213, 132, 41, 125, 216, 129, 37, 128,
    219, 125, 35, 131, 221, 122, 33, 135, 223, 118, 32, 139, 224, 114, 31, 143,
    224, 111, 31, 146, 224, 107, 31, 150, 224, 103, 32, 154, 222, 100, 34, 157,
    221, 96, 35, 160, 218, 93, 38, 163, 216, 91, 41, 166, 212, 88, 44, 168,
    209, 86, 48, 170, 205, 84, 52, 171, 201, 83, 56, 172, 197, 82, 61, 173,
    192, 82, 65, 173, 187, 82, 70, 173, 182, 82, 75, 172, 178, 83, 80, 171,
    173, 84, 85, 170, 168, 86, 90, 168, 163, 88, 94, 166, 159, 90, 99, 163,
    154, 93, 103, 160, 150, 96, 107, 157, 146, 100, 110, 154, 143, 103, 114, 150,
    139, 107, 117, 146, 136, 111, 120, 143, 134, 114, 122, 139, 132, 119, 124, 135,
    130, 122, 126, 130, 128, 126, 128, 126, 127, 130, 129, 123, 126, 134, 129, 119,
    126, 138, 130, 115, 125, 141, 130, 112, 125, 145, 129, 109, 126, 148, 129, 106,
    127, 151, 128, 103, 127, 153, 127, 101, 128, 156, 126, 98, 129, 157, 125, 97,
    131, 159, 124, 95, 132, 160, 122, 94, 133, 161, 121, 93, 135, 162, 119, 93,
    136, 163, 118, 93, 138, 163, 116, 93, 139, 162, 115, 93, 141, 162, 114, 94,
    142, 161, 113, 94, 143, 160, 112, 96, 144, 159, 111, 97, 145, 158, 110, 98,
    145, 156, 109, 100, 146, 154, 109, 102, 146, 153, 109, 103, 146, 151, 109, 105,
    146, 149, 109, 107, 146, 147, 109, 109, 145, 145, 110, 111, 145, 143, 111, 113,
    144, 141, 111, 114, 143, 140, 112, 116, 142, 138, 113, 118, 141, 137, 114, 119,
    140, 135, 115, 120, 139, 134, 117, 122, 138, 133, 118, 123, 137, 132, 119, 124,
    135, 131, 120, 125, 134, 130, 121, 125, 133, 129, 122, 126, 132, 129, 123, 127,
    131, 128, 124, 127, 130, 128, 125, 127, 129, 128, 126, 127, 129, 128, 127, 127,
    128, 128, 127, 127, 128, 127, 127, 127, 128, 127, 128, 127, 128, 127, 128, 127,
    128, 127, 127, 128, 128, 128, 127, 128, 128, 128, 127, 128, 128, 127, 126, 128,
    129, 127, 126, 128, 130, 127, 125, 128, 131, 127, 124, 129, 131, 126, 123, 129,
    132, 126, 122, 130, 133, 125, 122, 130, 134, 124, 121, 131, 135, 124, 120, 132,
    136, 123, 119, 133, 137, 122, 118, 134, 138, 121, 117, 135, 139, 120, 116, 136,
    139, 119, 115, 137, 140, 117, 115, 138, 141, 116, 115, 140, 141, 115, 114, 141,
    141, 113, 114, 142, 141, 112, 114, 144, 141, 111, 114, 145, 141, 110, 114, 146,
    141, 108, 114, 147, 141, 107, 115, 148, 140, 106, 115, 149, 140, 106, 116, 150,
    139, 105, 116, 150, 138, 105, 117, 151, 137, 104, 118, 151, 136, 104, 119, 151,
    136, 104, 120, 151, 134, 104, 121, 151, 133, 104, 122, 150, 132, 105, 123, 150,
    131, 106, 124, 149, 130, 106, 125, 148, 130, 108, 126, 147, 129, 109, 127, 145,
    128, 110, 128, 144, 127, 112, 128, 142, 127, 113, 129, 141, 126, 115, 129, 139,
    126, 117, 129, 137, 126, 119, 129, 135, 126, 121, 129, 133, 126, 123, 129, 131,
    127, 125, 128, 129, 127, 127, 128, 127, 128, 129, 127, 125, 129, 131, 126, 123,
    130, 133, 125, 121, 131, 135, 123, 120, 132, 136, 122, 118, 134, 138, 121, 116,
    135, 139, 119, 115, 137, 141, 118, 114, 138, 142, 116, 113, 140, 143, 114, 112,
    141, 143, 113, 111, 143, 144, 111, 111, 145, 144, 109, 111, 147, 144, 108, 111,
    148, 145, 106, 111, 150, 144, 105, 111, 151, 144, 103, 111, 152, 143, 102, 112,
    154, 143, 101, 112, 155, 142, 100, 113, 156, 141, 99, 114, 156, 140, 99, 115,
    157, 139, 98, 116, 157, 138, 98, 118, 157, 137, 98, 119, 157, 136, 98, 120,
    157, 134, 98, 121, 157, 133, 99, 123, 156, 132, 99, 124, 155, 131, 100, 125,
    155, 130, 101, 126, 154, 128, 102, 127, 153, 127, 103, 128, 151, 127, 104, 129,
    150, 126, 106, 130, 148, 125, 107, 130, 147, 125, 109, 131, 145, 124, 111, 131,
    144, 124, 112, 131, 142, 124, 114, 131, 140, 124, 116, 131, 139, 124, 117, 131,
    137, 124, 119, 131, 136, 124, 120, 131, 134, 125, 122, 130, 133, 125, 123, 130,
    131, 126, 124, 129, 130, 126, 125, 129, 129, 127, 127, 128, 128, 127, 128, 127,
    127, 128, 128, 127, 126, 129, 129, 126, 126, 129, 130, 125, 125, 130, 130, 125,
    125, 131, 130, 124, 124, 131, 131, 124, 124, 132, 131, 123, 124, 132, 131, 123,
    124, 132, 131, 122, 124};

static uint16_t rnd(uint16_t range)
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

bool snd_get_sample(uint8_t *sample)
{
    static uint16_t p = 0;

    if (p == sizeof(wavtable))
    {
        p = 0;
        return true;
    }
    else
    {
        *sample = pgm_read_byte(&wavtable[p++]) >> 5;
        return false;
    }
}

void snd_wait(void)
{
    static uint8_t count = 15;
    static uint8_t c = 0;

    if (++c == count)
    {
        c = 0;
        count = 15 + rnd(10);
        annoy_deep_sleep(4 + rnd(6));
    }
}
