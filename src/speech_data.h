#ifndef __LPC_DATA__
#define __LPC_DATA__

#include <stddef.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include "fix.h"

// 0 - 737 bytes - THE_QUICK_BROWN_FOX_JUMPS_OVER_THE_LAZY_DOG
const fix16_t LPC_0_A[268] PROGMEM = {
    0x008D6, -0x0095, -0x0058, -0x081D,
    0x00626, 0x00122, 0x00177, -0x0AE5,
    -0x03F0, -0x0A46, -0x02CD, -0x0E1D,
    -0x044C, -0x0ACB, -0x0340, -0x0E57,
    -0x0440, -0x0AC5, -0x0340, -0x0E5C,
    -0x0440, -0x0AC6, -0x0343, -0x0E5C,
    -0x08F6, -0x0263, -0x02EC, -0x00F8,
    0x0110C, -0x007C, -0x0D45, 0x004B9,
    0x00EE9, -0x0B7A, 0x00190, -0x05C1,
    -0x0580, 0x008EF, -0x09AA, -0x0BE9,
    -0x0404, 0x008FF, -0x0BCB, -0x09B4,
    0x000C7, 0x005DD, -0x0C16, -0x051D,
    -0x0392, 0x003A3, -0x0630, -0x03A9,
    0x00087, -0x0713, 0x0030A, -0x0428,
    0x002AA, -0x161A, 0x00252, -0x0CB2,
    -0x0C5B, -0x1B3F, -0x0B61, -0x0DB6,
    -0x0545, -0x196B, -0x0488, -0x0CDD,
    0x00FC1, -0x1DCA, 0x00EA6, -0x0DBA,
    0x006D3, -0x0771, -0x0398, -0x01A1,
    0x00128, 0x0070B, -0x0AB3, -0x0613,
    -0x001A, 0x0076D, -0x09E5, -0x077F,
    -0x007E, -0x04A4, -0x0239, 0x00322,
    0x0094B, -0x1E1C, 0x008AC, -0x0DC0,
    0x00C4C, -0x201A, 0x00BD6, -0x0EB9,
    0x00C9F, -0x1F21, 0x00BCA, -0x0DF3,
    -0x05B1, -0x053D, -0x0522, 0x0003C,
    -0x08F6, -0x04BB, -0x0600, -0x01B2,
    -0x18C1, -0x1A7F, -0x1463, -0x0AE6,
    -0x0871, 0x001E8, 0x004BB, -0x00EA,
    -0x03E8, -0x0528, -0x0382, -0x0BF9,
    -0x028E, -0x12BE, -0x01D0, -0x0CE8,
    -0x0113, -0x1520, -0x0089, -0x0DAC,
    0x01095, -0x1331, 0x011AF, -0x0927,
    0x0146C, -0x1593, 0x01480, -0x0B4D,
    -0x050F, 0x00106, -0x045C, -0x0861,
    -0x037B, -0x0453, -0x0763, -0x0335,
    -0x1679, -0x1665, -0x113E, -0x091A,
    -0x0403, -0x0CDB, -0x030E, -0x0CE6,
    -0x03F0, -0x1083, -0x02E1, -0x0DEB,
    -0x0381, -0x1015, -0x0275, -0x0E03,
    0x006B3, -0x0D33, 0x005EA, -0x0D82,
    0x012AD, -0x17F9, 0x00EA7, -0x0B79,
    0x0066C, -0x0B9A, 0x006E4, -0x0A4F,
    -0x0248, -0x0D96, -0x00F5, -0x0D8E,
    -0x043D, -0x0ACA, -0x0335, -0x0E59,
    -0x043A, -0x0AAD, -0x0335, -0x0E58,
    0x00410, -0x00C5, 0x00338, -0x0BF1,
    -0x043F, -0x0AB6, -0x0338, -0x0E56,
    -0x0447, -0x0AC4, -0x033E, -0x0E5A,
    -0x03AF, -0x0A09, -0x029D, -0x0E32,
    -0x00A6, -0x0557, -0x0127, -0x0D95,
    -0x0ED0, -0x0885, -0x0E3A, -0x0C78,
    -0x0B98, -0x00D4, -0x0DAA, -0x0B89,
    0x00B05, 0x00424, -0x1086, 0x00443,
    0x00155, 0x00AE3, -0x0506, -0x08D6,
    -0x0074, 0x009BB, -0x09CF, -0x0883,
    0x000A3, 0x008C6, -0x0CF8, -0x0609,
    0x00095, 0x008CF, -0x0CF8, -0x0613,
    0x000E7, 0x008A7, -0x0D20, -0x05B8,
    0x00307, -0x0994, 0x0032F, -0x04E9,
    0x005CF, -0x1B9F, 0x00553, -0x0DAD,
    0x00BCB, -0x1FB2, 0x00B43, -0x0E89,
    0x00E00, -0x2072, 0x00D5A, -0x0E70,
    0x00BFA, -0x1828, 0x00ADC, -0x0AAA,
    -0x02D7, 0x00415, 0x0011C, 0x00064,
    0x006E9, 0x00AF7, -0x04B1, -0x061C,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_0_PS[67] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x28, 0x29, 0x29, 0x00, 0x00, 0x00, 0x00, 0x02, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x04, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x00, 0x00, 0x27, 0x28, 0x27, 0x27, 0x00, 0x00, 0x00, 0x28, 0x00, 0x06, 0x28, 0x28
};

const fix16_t LPC_0_G[67] PROGMEM = {
    0x0000A, 0x00044, 0x001A3, 0x0049C, 0x00429, 0x00140, 0x00086, 0x00086, 0x00198, 0x003C8, 0x001D7, 0x00033, 0x0002F, 0x0011F, 0x00305, 0x00535, 0x004C6, 0x00254, 0x001EA, 0x000E4, 0x00073, 0x0003C, 0x00137, 0x00269, 0x0017F, 0x00047, 0x00034, 0x00014, 0x0001B, 0x0012C, 0x00667, 0x0034A, 0x00091, 0x0003F, 0x00046, 0x00029, 0x0001D, 0x0005C, 0x00448, 0x0050D, 0x0026C, 0x0009B, 0x00056, 0x00336, 0x004A6, 0x0022E, 0x00064, 0x002CB, 0x004A8, 0x0025B, 0x001F5, 0x00582, 0x00306, 0x000B5, 0x0008A, 0x0008F, 0x0019E, 0x001DF, 0x00100, 0x000FB, 0x002AD, 0x002AD, 0x00200, 0x000FF, 0x0007A, 0x00009, 0x00000
};

// 1 - 253 bytes - THE_TIME_IS
const fix16_t LPC_1_A[92] PROGMEM = {
    0x0071B, 0x003CB, -0x051C, -0x05AF,
    0x00601, 0x00131, 0x0017B, -0x0B05,
    -0x03CB, -0x0A10, -0x029D, -0x0E05,
    -0x044B, -0x0ACA, -0x0340, -0x0E57,
    -0x0441, -0x0AC5, -0x0340, -0x0E5C,
    -0x0441, -0x0AC8, -0x0345, -0x0E5E,
    -0x0CF5, -0x0FAB, -0x0921, -0x05B3,
    -0x0FA3, -0x1AB7, -0x0E67, -0x0CA9,
    -0x0994, -0x1C11, -0x08CD, -0x0D4F,
    -0x09C0, -0x1CBE, -0x08F7, -0x0D74,
    -0x0DEA, -0x1A87, -0x0C85, -0x0C7A,
    -0x0B3F, -0x04FA, -0x0D04, -0x0974,
    0x0004E, 0x007D3, -0x0CC8, -0x0572,
    -0x005B, -0x052C, 0x00217, 0x00438,
    0x01119, -0x1336, 0x0123A, -0x08EF,
    0x0070E, -0x08F5, 0x00788, 0x001AF,
    -0x0473, 0x0098A, -0x0B7E, -0x0A6B,
    -0x057F, 0x009A8, -0x0B2D, -0x0B64,
    -0x05E1, 0x009DB, -0x0ABA, -0x0BF0,
    0x0001D, 0x0089B, -0x0371, -0x0A70,
    0x00475, 0x005EC, 0x001BD, -0x0B34,
    -0x08FA, 0x0016E, 0x0017E, -0x0376,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_1_PS[23] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x00, 0x27
};

const fix16_t LPC_1_G[23] PROGMEM = {
    0x0000A, 0x0004B, 0x001AA, 0x0051C, 0x004CE, 0x001A6, 0x0004D, 0x00338, 0x0052A, 0x00499, 0x004E7, 0x003F7, 0x00152, 0x001DB, 0x0009C, 0x000D3, 0x00175, 0x00320, 0x0020F, 0x000C4, 0x00051, 0x0000E, 0x00000
};

// 2 - 132 bytes - PAST
const fix16_t LPC_2_A[48] PROGMEM = {
    0x00000, 0x00000, 0x00000, 0x00000,
    -0x0393, -0x0528, -0x034E, 0x000D0,
    -0x0788, -0x1A8F, -0x0648, -0x0BA5,
    -0x0A8F, -0x1BAD, -0x0976, -0x0CD4,
    -0x0AD9, -0x1BA9, -0x09BF, -0x0CDF,
    -0x0AFB, -0x1BAF, -0x09DC, -0x0CD7,
    -0x0B37, -0x1B74, -0x09EF, -0x0C86,
    -0x0E61, -0x19B7, -0x0CDC, -0x0B38,
    -0x13C7, -0x118B, -0x0DB6, -0x05B2,
    -0x061D, -0x0076, -0x0822, -0x028D,
    -0x0815, -0x00FE, -0x06A6, -0x0344,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_2_PS[12] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x27, 0x27, 0x27, 0x27, 0x27, 0x00
};

const fix16_t LPC_2_G[12] PROGMEM = {
    0x00000, 0x0004C, 0x001F4, 0x00531, 0x00584, 0x004DC, 0x003A4, 0x001C4, 0x0003A, 0x00046, 0x00022, 0x00000
};

// 3 - 132 bytes - A_M_
const fix16_t LPC_3_A[48] PROGMEM = {
    -0x112E, -0x0A7F, -0x10C0, -0x0C73,
    -0x111E, -0x0A3D, -0x10AB, -0x0C80,
    -0x0B8B, -0x00AC, -0x0DB6, -0x0B85,
    -0x0604, 0x004FB, -0x0ABA, -0x0A3B,
    -0x0E03, -0x061B, -0x0E84, -0x0CBA,
    -0x0FF4, -0x096C, -0x1036, -0x0CC1,
    -0x0FEF, -0x096E, -0x102F, -0x0CC0,
    -0x09A0, -0x08BC, -0x08F0, -0x05A8,
    0x01168, -0x1390, 0x01283, -0x095A,
    0x01444, -0x156B, 0x0149F, -0x0B37,
    0x0166A, -0x175A, 0x01606, -0x0C46,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_3_PS[12] PROGMEM = {
    0x00, 0x00, 0x00, 0x05, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27, 0x27
};

const fix16_t LPC_3_G[12] PROGMEM = {
    0x0025C, 0x00868, 0x004F1, 0x00336, 0x00714, 0x0095D, 0x00685, 0x003DA, 0x000D8, 0x00096, 0x00035, 0x00001
};

// 4 - 143 bytes - P_M_
const fix16_t LPC_4_A[52] PROGMEM = {
    -0x05CE, -0x0094, -0x066B, 0x00277,
    0x004D9, 0x003CC, -0x0796, -0x028F,
    0x0163B, -0x015C, -0x126E, 0x009E1,
    0x01522, 0x00241, -0x1605, 0x00B34,
    -0x03BC, 0x0004A, -0x0D35, -0x0484,
    -0x0F8A, -0x08C5, -0x0FFD, -0x0C85,
    -0x1003, -0x097E, -0x103E, -0x0CC6,
    -0x0D6B, -0x0876, -0x0D8F, -0x0AE2,
    0x009DC, -0x0DC5, 0x00C46, -0x0254,
    0x00F32, -0x1106, 0x01043, -0x0688,
    0x00F63, -0x0FEE, 0x00EBF, -0x0571,
    0x0005E, 0x00248, -0x0344, 0x00A60,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_4_PS[13] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27
};

const fix16_t LPC_4_G[13] PROGMEM = {
    0x0001C, 0x0013C, 0x000EC, 0x0009D, 0x004E4, 0x00A64, 0x00853, 0x0047C, 0x00164, 0x000FD, 0x00094, 0x00007, 0x00000
};

// 5 - 176 bytes - ZERO
const fix16_t LPC_5_A[64] PROGMEM = {
    0x00436, 0x00621, -0x00DC, -0x0794,
    0x00563, 0x004F3, 0x00288, -0x0B77,
    0x0007B, 0x00BA8, -0x0544, -0x0A30,
    0x010E9, 0x00238, -0x1355, 0x00975,
    0x00FF3, 0x001E2, -0x1262, 0x008B0,
    0x00AFC, 0x00459, -0x1128, 0x0048F,
    -0x0646, 0x0057E, -0x0CD1, -0x09E6,
    -0x0A2B, 0x00119, -0x0AF9, -0x0E04,
    -0x0589, -0x035C, -0x059B, -0x0DA0,
    0x0068E, -0x0DB0, 0x006CA, -0x0CCB,
    0x0056D, -0x1115, 0x0059B, -0x0D1A,
    -0x0467, -0x1208, -0x0340, -0x0DEF,
    -0x00B7, -0x0C75, -0x0048, -0x0E77,
    0x00A71, -0x1084, 0x0095F, -0x0DD3,
    0x01613, -0x1E3F, 0x014C5, -0x0E72,
    0x00935, 0x0035A, -0x0CC0, 0x008CF,
};

const uint8_t LPC_5_PS[16] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27
};

const fix16_t LPC_5_G[16] PROGMEM = {
    0x00014, 0x00062, 0x00094, 0x000AB, 0x00103, 0x0014B, 0x002DD, 0x002B4, 0x0024C, 0x00216, 0x005B3, 0x00798, 0x00495, 0x00261, 0x0007E, 0x00002
};

// 6 - 110 bytes - ONE
const fix16_t LPC_6_A[40] PROGMEM = {
    0x02060, -0x1CA9, 0x009D3, -0x01C9,
    0x02E4E, -0x402F, 0x02CDA, -0x0F04,
    0x01F70, -0x2C31, 0x01DB6, -0x0E4F,
    0x00F18, -0x20E1, 0x00E73, -0x0E84,
    0x00C33, -0x1FF2, 0x00BB2, -0x0EA1,
    0x00E2D, -0x1F36, 0x00D16, -0x0D6D,
    0x00358, 0x0022E, -0x0924, -0x0297,
    0x00111, 0x006E2, -0x0AD9, -0x0606,
    -0x0031, 0x00774, -0x09D7, -0x0799,
    0x00024, 0x00781, -0x09BC, -0x074C,
};

const uint8_t LPC_6_PS[10] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27
};

const fix16_t LPC_6_G[10] PROGMEM = {
    0x00019, 0x00016, 0x0009A, 0x001FD, 0x0020E, 0x00178, 0x00120, 0x000B2, 0x0005D, 0x00002
};

// 7 - 110 bytes - TWO
const fix16_t LPC_7_A[40] PROGMEM = {
    0x00000, 0x00000, 0x00000, 0x00000,
    -0x0A0A, -0x080F, -0x0296, -0x0262,
    -0x0444, 0x007E0, -0x0527, -0x0E25,
    -0x0297, 0x0048C, -0x01AD, -0x0E90,
    -0x01AE, 0x003FC, -0x0169, -0x0EF4,
    0x0013A, 0x001AC, 0x00155, -0x0EAC,
    0x007E9, -0x0619, 0x00813, -0x0E3B,
    0x010FB, -0x12B7, 0x0108A, -0x0E38,
    0x01702, -0x1B6C, 0x01659, -0x0EA7,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_7_PS[10] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27
};

const fix16_t LPC_7_G[10] PROGMEM = {
    0x00000, 0x00021, 0x0015F, 0x0052C, 0x0052E, 0x003C8, 0x00242, 0x00192, 0x00060, 0x00001
};

// 8 - 99 bytes - THREE
const fix16_t LPC_8_A[36] PROGMEM = {
    -0x08C0, 0x00295, -0x095E, -0x050C,
    -0x0C9D, -0x0447, -0x05B0, -0x0079,
    0x00A1D, -0x0EEF, 0x00A9A, -0x0C1D,
    0x00204, -0x017C, 0x001A7, -0x0BCD,
    -0x0180, 0x00D86, -0x0738, -0x0B8C,
    0x01BE4, -0x0C2B, -0x0991, 0x00770,
    0x01F27, -0x136A, -0x02DC, 0x00512,
    0x01854, -0x03FA, -0x1028, 0x008C3,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_8_PS[9] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28
};

const fix16_t LPC_8_G[9] PROGMEM = {
    0x0000A, 0x00026, 0x001AC, 0x00677, 0x0038A, 0x00089, 0x0004B, 0x00026, 0x00001
};

// 9 - 121 bytes - FOUR
const fix16_t LPC_9_A[44] PROGMEM = {
    -0x0133, 0x008CE, 0x001F1, -0x009E,
    -0x01EE, 0x0022B, -0x005A, -0x02CE,
    0x01388, -0x1E0E, 0x00FBD, -0x0A8A,
    0x017A8, -0x2496, 0x015E8, -0x0D9A,
    0x016D3, -0x22D5, 0x01406, -0x0C5A,
    0x0187B, -0x2487, 0x015BB, -0x0CAD,
    0x01A2C, -0x274E, 0x01861, -0x0DDF,
    0x016DB, -0x2516, 0x015B4, -0x0E5E,
    0x01336, -0x2169, 0x01257, -0x0E3C,
    0x01110, -0x1F37, 0x0106F, -0x0E3D,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_9_PS[11] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27, 0x27
};

const fix16_t LPC_9_G[11] PROGMEM = {
    0x00005, 0x00024, 0x00101, 0x001E1, 0x0028F, 0x00220, 0x0015C, 0x000CA, 0x000C8, 0x00054, 0x00001
};

// 10 - 132 bytes - FIVE
const fix16_t LPC_10_A[48] PROGMEM = {
    -0x02FD, 0x006EA, 0x00107, -0x0225,
    -0x0549, -0x0C77, -0x00F6, -0x00A1,
    -0x092C, -0x1C4E, -0x0877, -0x0D90,
    -0x08D3, -0x1C23, -0x0820, -0x0D86,
    -0x0964, -0x1C9D, -0x08A6, -0x0D90,
    -0x0CFA, -0x1BFE, -0x0BCB, -0x0CF5,
    -0x0D58, -0x0AE3, -0x0D82, -0x09BE,
    -0x00A5, 0x0065A, -0x0D1D, -0x0559,
    -0x0305, 0x0055F, -0x05B4, -0x0B47,
    0x0013B, 0x0039B, 0x0008A, -0x07F7,
    0x00282, 0x00207, 0x00103, -0x077E,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_10_PS[12] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27, 0x27, 0x27
};

const fix16_t LPC_10_G[12] PROGMEM = {
    0x0000E, 0x0005C, 0x0024E, 0x004CE, 0x0048F, 0x004A0, 0x0049A, 0x00164, 0x000F6, 0x0005A, 0x00021, 0x00001
};

// 11 - 121 bytes - SIX
const fix16_t LPC_11_A[44] PROGMEM = {
    -0x04B7, -0x03AF, 0x00012, 0x0051A,
    -0x0522, -0x01F6, -0x0406, -0x030A,
    -0x0496, 0x0097C, -0x0B43, -0x0AAF,
    -0x05B3, 0x009B6, -0x0B34, -0x0B90,
    -0x0598, 0x009B1, -0x0B29, -0x0B7C,
    -0x0180, 0x00823, -0x0C92, -0x0768,
    -0x0360, 0x00118, -0x0592, -0x04D1,
    -0x0A28, -0x05F6, -0x066E, -0x01A8,
    -0x1820, -0x199F, -0x1383, -0x0A93,
    -0x1735, -0x1891, -0x13B8, -0x0C49,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_11_PS[11] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x27, 0x27, 0x00, 0x07
};

const fix16_t LPC_11_G[11] PROGMEM = {
    0x0001F, 0x00088, 0x001C1, 0x00424, 0x003BB, 0x00183, 0x00072, 0x00054, 0x00021, 0x00012, 0x00001
};

// 12 - 154 bytes - SEVEN
const fix16_t LPC_12_A[56] PROGMEM = {
    -0x0619, -0x042E, -0x00F0, 0x004C7,
    -0x0A3D, -0x07E9, -0x0AE4, -0x0533,
    -0x0EF8, -0x08E5, -0x1053, -0x0B7E,
    -0x0FE6, -0x0966, -0x103C, -0x0CAC,
    -0x0FE7, -0x096D, -0x102D, -0x0CBE,
    -0x0F72, -0x091F, -0x0FBF, -0x0C90,
    -0x055E, -0x0331, -0x0679, -0x096E,
    0x0068E, -0x0D48, 0x00754, -0x0BBE,
    -0x00F4, -0x0C80, 0x00066, -0x0D92,
    -0x031C, -0x0961, -0x0202, -0x0E4F,
    -0x02B7, -0x0711, -0x018E, -0x0DFB,
    0x000E0, 0x006C2, -0x0AAB, -0x063C,
    0x00045, 0x0071A, -0x0A4C, -0x06F3,
    0x00036, 0x00737, -0x0A29, -0x06F8,
};

const uint8_t LPC_12_PS[14] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27, 0x00, 0x00, 0x27, 0x27
};

const fix16_t LPC_12_G[14] PROGMEM = {
    0x00021, 0x00082, 0x003AE, 0x0087D, 0x007AC, 0x0043C, 0x000DF, 0x00085, 0x00550, 0x006FE, 0x003C6, 0x0015A, 0x000CE, 0x00006
};

// 13 - 99 bytes - EIGHT
const fix16_t LPC_13_A[36] PROGMEM = {
    -0x112D, -0x0A80, -0x10CD, -0x0C70,
    -0x1131, -0x0A6E, -0x10BF, -0x0C77,
    -0x0F8F, -0x06C5, -0x0FD1, -0x0C96,
    -0x0939, 0x00243, -0x0D84, -0x0A9C,
    0x00AA1, 0x003DE, -0x10CF, 0x004AE,
    0x01291, 0x0018F, -0x1463, 0x00B96,
    0x00315, 0x00514, -0x0B50, -0x01EE,
    -0x0914, -0x00C5, -0x078E, -0x0432,
    -0x0278, 0x0066E, -0x00B4, 0x005B5,
};

const uint8_t LPC_13_PS[9] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27, 0x27
};

const fix16_t LPC_13_G[9] PROGMEM = {
    0x0040B, 0x009CD, 0x006C8, 0x00419, 0x0016B, 0x0008C, 0x00058, 0x00074, 0x00006
};

// 14 - 143 bytes - NINE
const fix16_t LPC_14_A[52] PROGMEM = {
    -0x014D, 0x00477, -0x05A6, -0x0B56,
    -0x011F, -0x0213, -0x083E, -0x06BE,
    -0x0E07, -0x13E5, -0x0AC5, -0x0B77,
    -0x0CA5, -0x1BCE, -0x0B9A, -0x0D63,
    -0x094E, -0x1C74, -0x0893, -0x0D81,
    -0x0A8D, -0x1CCF, -0x09B0, -0x0D66,
    -0x0EBE, -0x165A, -0x0D4A, -0x0B5B,
    -0x075C, 0x00259, -0x0C95, -0x08F8,
    0x00417, 0x0088C, -0x0D8A, -0x02EA,
    0x00196, 0x00723, -0x0B15, -0x0574,
    0x00108, 0x006FE, -0x0AC7, -0x061D,
    -0x000F, 0x00769, -0x09E9, -0x0775,
    0x0042C, 0x0071D, -0x0A9D, -0x0309,
};

const uint8_t LPC_14_PS[13] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27
};

const fix16_t LPC_14_G[13] PROGMEM = {
    0x0004F, 0x00124, 0x00270, 0x00495, 0x004D1, 0x004B6, 0x0053D, 0x00283, 0x0010C, 0x0010C, 0x000FB, 0x00080, 0x00003
};

// 15 - 110 bytes - TEN
const fix16_t LPC_15_A[40] PROGMEM = {
    0x00000, 0x00000, 0x00000, 0x00000,
    -0x0B2F, -0x096F, -0x0713, -0x03E6,
    -0x0E51, -0x08A3, -0x102A, -0x0ACC,
    -0x0FD8, -0x0962, -0x1041, -0x0C97,
    -0x0FDE, -0x0963, -0x1027, -0x0CBB,
    -0x0E8D, -0x07E1, -0x0F0E, -0x0C35,
    0x00034, 0x00658, -0x0A54, -0x06A6,
    0x000EE, 0x006F8, -0x0ABF, -0x0634,
    -0x002D, 0x0076C, -0x09D6, -0x0796,
    0x00171, 0x0090A, -0x0BDE, -0x0448,
};

const uint8_t LPC_15_PS[10] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27
};

const fix16_t LPC_15_G[10] PROGMEM = {
    0x00001, 0x00044, 0x00388, 0x009EF, 0x00919, 0x004C2, 0x001C9, 0x0017E, 0x000A6, 0x00002
};

// 16 - 165 bytes - ELEVEN
const fix16_t LPC_16_A[60] PROGMEM = {
    -0x015A, 0x008B6, -0x0C34, -0x07D9,
    -0x00CE, 0x007A1, -0x0C78, -0x06FB,
    0x00186, -0x0381, 0x00085, -0x0D9C,
    -0x0683, -0x06C1, -0x067C, -0x0D76,
    -0x0F46, -0x08FC, -0x0F72, -0x0CD1,
    -0x0FF8, -0x0978, -0x103A, -0x0CBE,
    -0x0F86, -0x0934, -0x0FD3, -0x0C94,
    -0x055F, -0x032A, -0x0668, -0x0966,
    0x00680, -0x0D5D, 0x00752, -0x0BB0,
    -0x00CE, -0x0CB6, 0x0008E, -0x0D84,
    -0x031B, -0x0963, -0x01FF, -0x0E4C,
    -0x02C5, -0x0734, -0x0193, -0x0DF6,
    0x00098, 0x006BB, -0x0A60, -0x0699,
    0x0005F, 0x0070F, -0x0A5E, -0x06D3,
    0x000BA, 0x007A2, -0x0AEA, -0x05E9,
};

const uint8_t LPC_16_PS[15] PROGMEM = {
    0x00, 0x00, 0x00, 0x08, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27, 0x00, 0x00, 0x27, 0x06
};

const fix16_t LPC_16_G[15] PROGMEM = {
    0x00124, 0x00320, 0x001B7, 0x00405, 0x00794, 0x006B1, 0x003C6, 0x000D4, 0x0007C, 0x004BE, 0x00652, 0x00393, 0x00141, 0x000B5, 0x00005
};

// 17 - 143 bytes - TWELVE
const fix16_t LPC_17_A[52] PROGMEM = {
    0x00000, 0x00000, 0x00000, 0x00000,
    -0x086A, -0x05FC, -0x0184, -0x01B5,
    0x00B5D, 0x003E9, -0x09BA, -0x024C,
    0x01A51, -0x2088, 0x0140D, -0x0B0B,
    -0x0128, -0x0DDE, -0x00FC, -0x0BC1,
    -0x0EFB, -0x09AC, -0x0ECE, -0x0C83,
    -0x0FDB, -0x0972, -0x1015, -0x0CAF,
    -0x02D3, -0x0BF7, -0x032C, -0x08BC,
    0x01465, -0x2067, 0x0137E, -0x0E89,
    0x01713, -0x228B, 0x0159E, -0x0E2C,
    0x00401, -0x0509, -0x042D, -0x038A,
    0x00092, 0x00245, 0x00156, -0x0726,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_17_PS[13] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27, 0x27
};

const fix16_t LPC_17_G[13] PROGMEM = {
    0x00001, 0x0003E, 0x000A8, 0x00174, 0x009EB, 0x00AC4, 0x00647, 0x004D3, 0x00117, 0x000E9, 0x000E3, 0x00022, 0x00000
};

// 18 - 176 bytes - THIRTEEN
const fix16_t LPC_18_A[64] PROGMEM = {
    -0x0603, -0x0075, -0x066A, -0x07BB,
    -0x0920, -0x05B8, -0x04FB, -0x054B,
    -0x0669, -0x026D, -0x06B5, -0x0E22,
    -0x0399, -0x07AD, -0x01D3, -0x0D70,
    -0x0149, -0x09C0, 0x0002D, -0x0E0C,
    -0x0110, -0x099E, -0x004B, -0x0EA2,
    -0x01B7, -0x09C5, -0x011B, -0x0E9C,
    -0x034C, -0x09A2, -0x0278, -0x0E24,
    -0x076D, -0x0482, -0x0772, -0x0DF5,
    0x00531, 0x0084E, -0x09FF, -0x04C7,
    0x0158B, 0x000B9, -0x14C0, 0x00AF0,
    0x020C9, -0x1872, 0x00284, 0x002DE,
    0x00992, 0x00670, -0x0CB5, 0x0001D,
    0x00142, 0x006C8, -0x0B05, -0x05C3,
    0x0005B, 0x0070A, -0x0A59, -0x06D7,
    0x00A08, 0x00592, -0x0BBD, 0x002CE,
};

const uint8_t LPC_18_PS[16] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27, 0x00, 0x00, 0x00, 0x27
};

const fix16_t LPC_18_G[16] PROGMEM = {
    0x00008, 0x0001D, 0x00196, 0x0064A, 0x00709, 0x00558, 0x00408, 0x00344, 0x000E8, 0x000B1, 0x000C2, 0x0005C, 0x00109, 0x00161, 0x000A0, 0x00002
};

// 19 - 187 bytes - FOURTEEN
const fix16_t LPC_19_A[68] PROGMEM = {
    -0x0164, 0x0083D, 0x001E1, -0x012D,
    -0x0204, 0x0006C, -0x01D2, -0x0309,
    0x01466, -0x1F69, 0x01115, -0x0B3F,
    0x017FC, -0x2514, 0x01666, -0x0DD7,
    0x017C3, -0x2422, 0x01557, -0x0CE6,
    0x0193A, -0x2578, 0x016A7, -0x0CF4,
    0x0191F, -0x26F0, 0x017CF, -0x0E56,
    0x0141C, -0x2262, 0x01327, -0x0E4A,
    0x00D56, -0x1B5D, 0x00C93, -0x0D47,
    0x006A4, -0x131E, 0x006AB, -0x0D62,
    0x00649, 0x008AA, -0x0BB0, -0x036E,
    0x015EA, 0x0003B, -0x147D, 0x00AF6,
    0x02248, -0x1C33, 0x00633, 0x0017C,
    0x00829, 0x0066D, -0x0C45, -0x0097,
    0x00135, 0x006CB, -0x0AFF, -0x05D1,
    0x00053, 0x0070C, -0x0A50, -0x06E1,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_19_PS[17] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27, 0x27, 0x00, 0x00, 0x00, 0x27
};

const fix16_t LPC_19_G[17] PROGMEM = {
    0x00007, 0x0002E, 0x00133, 0x00229, 0x002BE, 0x00242, 0x0011F, 0x000F0, 0x00124, 0x0005E, 0x00080, 0x00082, 0x00038, 0x000C8, 0x000ED, 0x0005C, 0x00001
};

// 20 - 165 bytes - FIFTEEN
const fix16_t LPC_20_A[60] PROGMEM = {
    -0x042B, 0x00A63, -0x0014, 0x00144,
    -0x0637, 0x00649, -0x031B, -0x00B0,
    -0x0310, 0x00911, -0x0BD6, -0x0912,
    -0x0529, 0x0099B, -0x0B43, -0x0B17,
    -0x059A, 0x009AD, -0x0B3A, -0x0B75,
    -0x06CB, 0x009B8, -0x0A54, -0x0CDC,
    -0x04AC, 0x0059F, -0x06A6, -0x0C68,
    -0x049B, 0x0005E, -0x01D9, -0x01F8,
    0x00641, 0x008E1, -0x0BF8, -0x0349,
    0x015F1, 0x00031, -0x1478, 0x00AF6,
    0x02046, -0x16F6, 0x000F5, 0x00380,
    0x00812, 0x0066C, -0x0C3E, -0x00A4,
    0x00134, 0x006CC, -0x0AFF, -0x05D2,
    0x00052, 0x0070C, -0x0A50, -0x06E2,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_20_PS[15] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27
};

const fix16_t LPC_20_G[15] PROGMEM = {
    0x0000B, 0x00045, 0x0013A, 0x00381, 0x003BF, 0x00246, 0x0007E, 0x0005A, 0x000BC, 0x000BD, 0x00056, 0x00125, 0x0015A, 0x00085, 0x00001
};

// 21 - 176 bytes - SIXTEEN
const fix16_t LPC_21_A[64] PROGMEM = {
    -0x0406, 0x000D1, 0x0001A, 0x00A16,
    -0x0B04, -0x0825, -0x0480, 0x00061,
    -0x038B, 0x008FB, -0x0B6C, -0x09A6,
    -0x0566, 0x009A9, -0x0B28, -0x0B58,
    -0x05A0, 0x009B0, -0x0B38, -0x0B7C,
    -0x03C2, 0x00905, -0x0BF2, -0x0998,
    -0x019B, 0x0039C, -0x0872, -0x0605,
    -0x0C5B, -0x075E, -0x0766, -0x0130,
    -0x15E5, -0x1309, -0x0E25, -0x05AF,
    0x00747, 0x0087C, -0x0CA0, -0x026A,
    0x0165C, -0x008F, -0x13E0, 0x00AD0,
    0x02463, -0x2198, 0x00B8E, -0x008E,
    0x00744, 0x00677, -0x0C0E, -0x011E,
    0x0012A, 0x006CF, -0x0AF8, -0x05DE,
    0x0004B, 0x0070F, -0x0A4C, -0x06E8,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_21_PS[16] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x27, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x27
};

const fix16_t LPC_21_G[16] PROGMEM = {
    0x0000C, 0x0005A, 0x00139, 0x003AE, 0x003C0, 0x001BB, 0x00075, 0x00066, 0x0003C, 0x000C8, 0x000C2, 0x0004C, 0x0013C, 0x00163, 0x00077, 0x00001
};

// 22 - 209 bytes - SEVENTEEN
const fix16_t LPC_22_A[76] PROGMEM = {
    -0x02F2, 0x00192, 0x000E1, 0x00AD4,
    -0x0C25, -0x0AC3, -0x064A, -0x010B,
    -0x0DF5, -0x087E, -0x1016, -0x0A65,
    -0x0FBA, -0x0946, -0x104D, -0x0C69,
    -0x0FEE, -0x0972, -0x1034, -0x0CBE,
    -0x0FD7, -0x0965, -0x1021, -0x0CB7,
    -0x0BBA, -0x06A3, -0x0C0A, -0x0B47,
    0x000E5, -0x0D9B, 0x00247, -0x0D0B,
    -0x02C8, -0x0A10, -0x0184, -0x0DFE,
    -0x0177, -0x01E9, -0x0180, -0x0BDD,
    0x0004F, 0x006E4, -0x0A4F, -0x06DD,
    0x00392, 0x00600, -0x064D, -0x0639,
    0x01466, 0x001B7, -0x14B9, 0x00A5B,
    0x01EB0, -0x13FD, -0x0170, 0x00424,
    0x0188A, -0x0426, -0x10AD, 0x00979,
    0x00384, 0x00691, -0x0B5F, -0x03CD,
    0x000FE, 0x006EC, -0x0AD4, -0x0618,
    0x00020, 0x0071D, -0x0A2C, -0x071B,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_22_PS[19] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27, 0x00, 0x27, 0x06, 0x27, 0x27, 0x00, 0x00, 0x01, 0x27
};

const fix16_t LPC_22_G[19] PROGMEM = {
    0x00009, 0x0004C, 0x001E5, 0x00712, 0x008BF, 0x00619, 0x00257, 0x0017C, 0x00730, 0x002C7, 0x000B1, 0x00096, 0x000CA, 0x00084, 0x00052, 0x00161, 0x0014C, 0x0005A, 0x00000
};

// 23 - 154 bytes - EIGHTEEN
const fix16_t LPC_23_A[56] PROGMEM = {
    -0x1115, -0x0A72, -0x10B8, -0x0C5C,
    -0x1139, -0x0A7E, -0x10CE, -0x0C7D,
    -0x1097, -0x0911, -0x1067, -0x0C84,
    -0x0C03, -0x0060, -0x0E2E, -0x0C08,
    0x0043C, 0x004D6, -0x0EE9, -0x005A,
    0x012F8, 0x000BC, -0x13A5, 0x00B67,
    0x00933, 0x0056E, -0x0FB6, 0x0022D,
    0x005CC, 0x0089F, -0x0B13, -0x03FD,
    0x015AE, 0x000A2, -0x14CA, 0x00B06,
    0x024AD, -0x2281, 0x00C8A, -0x00F6,
    0x008B6, 0x0066B, -0x0C6C, -0x004C,
    0x0013B, 0x006CA, -0x0B02, -0x05CB,
    0x00056, 0x0070B, -0x0A54, -0x06DD,
    0x0010A, 0x00857, -0x0BE0, -0x04D7,
};

const uint8_t LPC_23_PS[14] PROGMEM = {
    0x00, 0x00, 0x00, 0x05, 0x28, 0x27, 0x27, 0x27, 0x27, 0x27, 0x00, 0x00, 0x0A, 0x27
};

const fix16_t LPC_23_G[14] PROGMEM = {
    0x00202, 0x00775, 0x006FB, 0x00472, 0x001EE, 0x00091, 0x00056, 0x000CA, 0x000D3, 0x00057, 0x00137, 0x00180, 0x000A1, 0x00002
};

// 24 - 198 bytes - NINETEEN
const fix16_t LPC_24_A[72] PROGMEM = {
    -0x014D, 0x0046C, -0x05AE, -0x0B4B,
    -0x015F, -0x0312, -0x083A, -0x066F,
    -0x0F34, -0x16FB, -0x0C5B, -0x0BE8,
    -0x0B08, -0x1C04, -0x0A2B, -0x0D67,
    -0x09A3, -0x1CAD, -0x08E0, -0x0D7A,
    -0x0DA4, -0x1AEB, -0x0C36, -0x0C2D,
    -0x09CE, -0x0306, -0x0C63, -0x08AC,
    0x00462, 0x007D5, -0x0DF2, -0x0236,
    0x00186, 0x006A1, -0x0B31, -0x0562,
    0x000E2, 0x00690, -0x0AC4, -0x062A,
    -0x00B5, 0x006CF, -0x0986, -0x0804,
    0x004CC, 0x00805, -0x0947, -0x0534,
    0x01545, 0x00119, -0x14F2, 0x00AEA,
    0x02009, -0x169A, 0x000B9, 0x00388,
    0x00A51, 0x00661, -0x0CFA, 0x00077,
    0x00149, 0x006C6, -0x0B08, -0x05BB,
    0x00062, 0x00709, -0x0A5F, -0x06CF,
    0x000EC, 0x006F9, -0x0A46, -0x0654,
};

const uint8_t LPC_24_PS[18] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27, 0x27, 0x00, 0x00, 0x00
};

const fix16_t LPC_24_G[18] PROGMEM = {
    0x0004F, 0x0014F, 0x002FD, 0x0059C, 0x00534, 0x00599, 0x0042B, 0x00133, 0x00123, 0x00104, 0x00066, 0x00090, 0x000A4, 0x00052, 0x000D1, 0x0012A, 0x00093, 0x00002
};

// 25 - 176 bytes - TWENTY
const fix16_t LPC_25_A[64] PROGMEM = {
    0x00000, 0x00000, 0x00000, 0x00000,
    -0x091F, -0x068C, -0x01EE, -0x01AA,
    0x00AFA, 0x003FF, -0x0985, -0x0288,
    0x015CA, -0x1B40, 0x01023, -0x0B2C,
    -0x04CF, -0x0C74, -0x0409, -0x0B42,
    -0x0FA6, -0x098F, -0x0FBC, -0x0CB1,
    -0x0FF4, -0x096E, -0x1037, -0x0CBF,
    -0x0718, -0x003A, -0x0A2E, -0x08DF,
    0x00155, 0x00669, -0x0B0E, -0x05A2,
    -0x001F, 0x006D0, -0x0A12, -0x0758,
    -0x00F6, 0x001EB, -0x0639, -0x0659,
    -0x0035, 0x0090D, -0x0C94, -0x06E3,
    0x00070, 0x008DE, -0x0CE8, -0x0639,
    0x000AB, 0x008C2, -0x0CF3, -0x0609,
    0x001C5, 0x0083D, -0x0D59, -0x04E6,
    0x00FAC, 0x003A7, -0x126A, 0x00993,
};

const uint8_t LPC_25_PS[16] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x28, 0x28, 0x27, 0x27, 0x27, 0x00, 0x00
};

const fix16_t LPC_25_G[16] PROGMEM = {
    0x00001, 0x0002E, 0x0008C, 0x0019F, 0x00972, 0x00805, 0x0055E, 0x002AB, 0x00127, 0x000B7, 0x0006B, 0x0019E, 0x002B2, 0x0020A, 0x000B9, 0x00003
};

// 26 - 165 bytes - THIRTY
const fix16_t LPC_26_A[60] PROGMEM = {
    -0x067F, 0x000D1, -0x0793, -0x0729,
    -0x0858, -0x0404, -0x0391, -0x03ED,
    -0x06A5, -0x01F1, -0x0736, -0x0E0A,
    -0x0439, -0x06A1, -0x02C1, -0x0D9C,
    -0x018A, -0x09A4, 0x0000E, -0x0DE2,
    -0x00FB, -0x09BB, -0x0015, -0x0E90,
    -0x0197, -0x09A1, -0x0102, -0x0EA6,
    -0x02AA, -0x0A0D, -0x01E4, -0x0E54,
    -0x06F2, -0x0537, -0x06C8, -0x0DE8,
    0x00009, 0x00569, -0x0981, -0x0680,
    -0x003D, 0x00918, -0x0C98, -0x06E8,
    0x00079, 0x008DA, -0x0CEA, -0x0632,
    0x000B5, 0x008BD, -0x0CF5, -0x0601,
    0x0022D, 0x0080E, -0x0D87, -0x0471,
    0x00000, 0x00000, 0x00000, 0x00000,
};

const uint8_t LPC_26_PS[15] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27, 0x00, 0x00, 0x00
};

const fix16_t LPC_26_G[15] PROGMEM = {
    0x00003, 0x00019, 0x000D8, 0x0048E, 0x0061F, 0x004DA, 0x0038F, 0x002DE, 0x00137, 0x00082, 0x001D9, 0x00290, 0x001D0, 0x0007F, 0x00001
};

// 27 - 165 bytes - FORTY
const fix16_t LPC_27_A[60] PROGMEM = {
    -0x026A, 0x00740, 0x00194, -0x01FB,
    0x00002, -0x05C9, -0x0619, -0x026B,
    0x016F3, -0x2372, 0x0150F, -0x0D53,
    0x01770, -0x242E, 0x0156E, -0x0D4A,
    0x017FC, -0x243C, 0x0156F, -0x0CCD,
    0x01A3E, -0x272D, 0x0184F, -0x0DBB,
    0x0173F, -0x2563, 0x0160D, -0x0E5A,
    0x0123D, -0x2066, 0x0115E, -0x0E1E,
    0x008DA, -0x165B, 0x00890, -0x0D15,
    -0x033F, -0x060C, -0x0235, -0x04EB,
    -0x000D, 0x008EE, -0x0C95, -0x06BE,
    0x0005C, 0x008E6, -0x0CE1, -0x064C,
    0x000A2, 0x008C7, -0x0CF2, -0x0610,
    0x00183, 0x0085D, -0x0D3F, -0x052E,
    0x00636, 0x006B9, -0x0F4A, 0x00006,
};

const uint8_t LPC_27_PS[15] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x27, 0x27, 0x27, 0x27, 0x27, 0x00, 0x00
};

const fix16_t LPC_27_G[15] PROGMEM = {
    0x0000C, 0x00045, 0x00121, 0x00266, 0x0026F, 0x001A8, 0x000E4, 0x000E5, 0x000F1, 0x0003F, 0x000EB, 0x001EE, 0x00194, 0x000B9, 0x00005
};

// 28 - 143 bytes - FIFTY
const fix16_t LPC_28_A[52] PROGMEM = {
    -0x0525, 0x008FC, -0x0007, 0x00095,
    -0x0323, 0x006BB, -0x095B, -0x0555,
    -0x03F7, 0x0095D, -0x0B8A, -0x0A05,
    -0x0564, 0x009A2, -0x0B3C, -0x0B48,
    -0x05A8, 0x009B1, -0x0B38, -0x0B82,
    -0x06C7, 0x008C5, -0x0931, -0x0D61,
    -0x04A6, 0x00446, -0x045A, -0x040C,
    -0x0733, -0x0202, -0x02A9, -0x014B,
    0x0002D, 0x008C8, -0x0CA0, -0x0684,
    0x0004C, 0x008EC, -0x0CDA, -0x065B,
    0x0009F, 0x008C9, -0x0CF2, -0x0612,
    0x0014A, 0x00878, -0x0D29, -0x056B,
    0x00403, 0x00785, -0x0E8B, -0x0234,
};

const uint8_t LPC_28_PS[13] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x27, 0x27, 0x27, 0x00, 0x00, 0x01
};

const fix16_t LPC_28_G[13] PROGMEM = {
    0x00016, 0x00061, 0x00190, 0x0035F, 0x002CD, 0x00192, 0x00054, 0x00046, 0x00116, 0x00290, 0x00232, 0x00117, 0x0000B
};

const uint16_t LPC_A_ADDRS[29] PROGMEM = {
    (uint16_t)LPC_0_A,
    (uint16_t)LPC_1_A,
    (uint16_t)LPC_2_A,
    (uint16_t)LPC_3_A,
    (uint16_t)LPC_4_A,
    (uint16_t)LPC_5_A,
    (uint16_t)LPC_6_A,
    (uint16_t)LPC_7_A,
    (uint16_t)LPC_8_A,
    (uint16_t)LPC_9_A,
    (uint16_t)LPC_10_A,
    (uint16_t)LPC_11_A,
    (uint16_t)LPC_12_A,
    (uint16_t)LPC_13_A,
    (uint16_t)LPC_14_A,
    (uint16_t)LPC_15_A,
    (uint16_t)LPC_16_A,
    (uint16_t)LPC_17_A,
    (uint16_t)LPC_18_A,
    (uint16_t)LPC_19_A,
    (uint16_t)LPC_20_A,
    (uint16_t)LPC_21_A,
    (uint16_t)LPC_22_A,
    (uint16_t)LPC_23_A,
    (uint16_t)LPC_24_A,
    (uint16_t)LPC_25_A,
    (uint16_t)LPC_26_A,
    (uint16_t)LPC_27_A,
    (uint16_t)LPC_28_A,
};

const uint16_t LPC_PS_ADDRS[29] PROGMEM = {
    (uint16_t)LPC_0_PS,
    (uint16_t)LPC_1_PS,
    (uint16_t)LPC_2_PS,
    (uint16_t)LPC_3_PS,
    (uint16_t)LPC_4_PS,
    (uint16_t)LPC_5_PS,
    (uint16_t)LPC_6_PS,
    (uint16_t)LPC_7_PS,
    (uint16_t)LPC_8_PS,
    (uint16_t)LPC_9_PS,
    (uint16_t)LPC_10_PS,
    (uint16_t)LPC_11_PS,
    (uint16_t)LPC_12_PS,
    (uint16_t)LPC_13_PS,
    (uint16_t)LPC_14_PS,
    (uint16_t)LPC_15_PS,
    (uint16_t)LPC_16_PS,
    (uint16_t)LPC_17_PS,
    (uint16_t)LPC_18_PS,
    (uint16_t)LPC_19_PS,
    (uint16_t)LPC_20_PS,
    (uint16_t)LPC_21_PS,
    (uint16_t)LPC_22_PS,
    (uint16_t)LPC_23_PS,
    (uint16_t)LPC_24_PS,
    (uint16_t)LPC_25_PS,
    (uint16_t)LPC_26_PS,
    (uint16_t)LPC_27_PS,
    (uint16_t)LPC_28_PS,
};

const uint16_t LPC_G_ADDRS[29] PROGMEM = {
    (uint16_t)LPC_0_G,
    (uint16_t)LPC_1_G,
    (uint16_t)LPC_2_G,
    (uint16_t)LPC_3_G,
    (uint16_t)LPC_4_G,
    (uint16_t)LPC_5_G,
    (uint16_t)LPC_6_G,
    (uint16_t)LPC_7_G,
    (uint16_t)LPC_8_G,
    (uint16_t)LPC_9_G,
    (uint16_t)LPC_10_G,
    (uint16_t)LPC_11_G,
    (uint16_t)LPC_12_G,
    (uint16_t)LPC_13_G,
    (uint16_t)LPC_14_G,
    (uint16_t)LPC_15_G,
    (uint16_t)LPC_16_G,
    (uint16_t)LPC_17_G,
    (uint16_t)LPC_18_G,
    (uint16_t)LPC_19_G,
    (uint16_t)LPC_20_G,
    (uint16_t)LPC_21_G,
    (uint16_t)LPC_22_G,
    (uint16_t)LPC_23_G,
    (uint16_t)LPC_24_G,
    (uint16_t)LPC_25_G,
    (uint16_t)LPC_26_G,
    (uint16_t)LPC_27_G,
    (uint16_t)LPC_28_G,
};

const uint16_t LPC_LENGTHS[29] PROGMEM = {
    67, 23, 12, 12, 13, 16, 10, 10, 9, 11, 12, 11, 14, 9, 13, 10, 15, 13, 16, 17, 15, 16, 19, 14, 18, 16, 15, 15, 13
};


#endif // __LPC_DATA__
