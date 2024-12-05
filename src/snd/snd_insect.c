#include "snd.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "annoyatron.h"

const uint16_t SND_DIV = 125;

static const uint8_t wavtable[] PROGMEM = {
    0x73, 0x74, 0x75, 0x76, 0x78, 0x77, 0x79, 0x74, 0x66, 0x6C, 0x6F, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x77, 0x79, 0x75, 0x67, 0x6B, 0x6F, 0x72, 0x73, 0x76, 0x76, 0x77, 0x77, 0x79, 0x75, 0x67, 0x6B,
    0x6F, 0x72, 0x74, 0x75, 0x76, 0x77, 0x77, 0x79, 0x75, 0x66, 0x6B, 0x6E, 0x72, 0x74, 0x75, 0x76,
    0x78, 0x77, 0x7A, 0x74, 0x66, 0x6B, 0x6E, 0x73, 0x74, 0x76, 0x76, 0x78, 0x77, 0x79, 0x72, 0x66,
    0x6C, 0x6F, 0x73, 0x74, 0x76, 0x76, 0x78, 0x77, 0x7A, 0x71, 0x66, 0x6D, 0x6F, 0x73, 0x74, 0x76,
    0x76, 0x78, 0x77, 0x7A, 0x6F, 0x66, 0x6D, 0x6F, 0x73, 0x74, 0x76, 0x77, 0x78, 0x77, 0x7A, 0x6D,
    0x66, 0x6E, 0x70, 0x74, 0x74, 0x76, 0x76, 0x78, 0x78, 0x7A, 0x6B, 0x67, 0x6E, 0x70, 0x74, 0x75,
    0x76, 0x76, 0x78, 0x78, 0x79, 0x69, 0x68, 0x6E, 0x71, 0x74, 0x75, 0x76, 0x77, 0x77, 0x78, 0x77,
    0x68, 0x6A, 0x6E, 0x72, 0x74, 0x75, 0x76, 0x77, 0x77, 0x79, 0x75, 0x67, 0x6B, 0x6F, 0x72, 0x74,
    0x75, 0x75, 0x77, 0x77, 0x79, 0x74, 0x66, 0x6C, 0x6F, 0x73, 0x74, 0x75, 0x75, 0x78, 0x77, 0x7A,
    0x71, 0x66, 0x6D, 0x70, 0x73, 0x74, 0x75, 0x75, 0x78, 0x77, 0x7A, 0x6F, 0x67, 0x6E, 0x70, 0x74,
    0x74, 0x76, 0x76, 0x78, 0x77, 0x7A, 0x6D, 0x67, 0x6E, 0x70, 0x74, 0x74, 0x76, 0x76, 0x78, 0x78,
    0x79, 0x6C, 0x68, 0x6E, 0x70, 0x74, 0x74, 0x76, 0x76, 0x78, 0x77, 0x79, 0x6B, 0x69, 0x6F, 0x71,
    0x74, 0x74, 0x75, 0x76, 0x78, 0x77, 0x79, 0x6B, 0x69, 0x6F, 0x70, 0x73, 0x74, 0x75, 0x77, 0x78,
    0x77, 0x79, 0x6B, 0x69, 0x6F, 0x71, 0x73, 0x75, 0x75, 0x76, 0x77, 0x77, 0x79, 0x6B, 0x69, 0x6F,
    0x71, 0x73, 0x74, 0x76, 0x76, 0x78, 0x77, 0x78, 0x6A, 0x69, 0x6F, 0x71, 0x74, 0x74, 0x76, 0x76,
    0x78, 0x77, 0x78, 0x6B, 0x68, 0x6F, 0x71, 0x74, 0x74, 0x76, 0x76, 0x77, 0x77, 0x79, 0x6B, 0x69,
    0x6F, 0x71, 0x74, 0x74, 0x75, 0x76, 0x77, 0x77, 0x79, 0x6B, 0x69, 0x6F, 0x71, 0x74, 0x73, 0x75,
    0x76, 0x78, 0x77, 0x79, 0x6C, 0x68, 0x6F, 0x70, 0x74, 0x74, 0x76, 0x76, 0x78, 0x77, 0x79, 0x6C,
    0x68, 0x6E, 0x70, 0x74, 0x74, 0x76, 0x76, 0x78, 0x77, 0x7A, 0x6C, 0x67, 0x6E, 0x70, 0x73, 0x74,
    0x76, 0x76, 0x78, 0x77, 0x7A, 0x6D, 0x67, 0x6E, 0x70, 0x73, 0x73, 0x76, 0x76, 0x78, 0x77, 0x7A,
    0x6E, 0x67, 0x6E, 0x6F, 0x73, 0x73, 0x76, 0x76, 0x78, 0x78, 0x7A, 0x6E, 0x66, 0x6E, 0x6F, 0x73,
    0x74, 0x76, 0x76, 0x78, 0x77, 0x7A, 0x6F, 0x66, 0x6D, 0x6F, 0x74, 0x74, 0x76, 0x76, 0x78, 0x77,
    0x7A, 0x70, 0x65, 0x6D, 0x6F, 0x73, 0x73, 0x76, 0x76, 0x78, 0x78, 0x7A, 0x71, 0x65, 0x6D, 0x6F,
    0x73, 0x73, 0x75, 0x76, 0x78, 0x78, 0x7A, 0x72, 0x65, 0x6C, 0x6F, 0x73, 0x73, 0x75, 0x76, 0x78,
    0x78, 0x7A, 0x74, 0x64, 0x6B, 0x6F, 0x73, 0x73, 0x75, 0x76, 0x78, 0x78, 0x7B, 0x74, 0x64, 0x6B,
    0x6E, 0x72, 0x73, 0x75, 0x76, 0x78, 0x78, 0x7B, 0x75, 0x63, 0x6A, 0x6E, 0x72, 0x73, 0x76, 0x77,
    0x79, 0x79, 0x7B, 0x77, 0x63, 0x69, 0x6E, 0x71, 0x74, 0x76, 0x77, 0x78, 0x79, 0x7B, 0x78, 0x64,
    0x68, 0x6D, 0x71, 0x73, 0x75, 0x77, 0x78, 0x79, 0x7A, 0x7A, 0x66, 0x66, 0x6D, 0x70, 0x74, 0x74,
    0x77, 0x78, 0x7A, 0x7A, 0x7C, 0x68, 0x64, 0x6D, 0x6F, 0x74, 0x73, 0x77, 0x77, 0x7B, 0x79, 0x7D,
    0x6B, 0x62, 0x6C, 0x6E, 0x74, 0x73, 0x77, 0x77, 0x7B, 0x79, 0x7D, 0x70, 0x60, 0x6B, 0x6D, 0x73,
    0x73, 0x77, 0x78, 0x7A, 0x7A, 0x7C, 0x77, 0x61, 0x67, 0x6D, 0x71, 0x73, 0x75, 0x78, 0x79, 0x7A,
    0x7B, 0x7C, 0x65, 0x63, 0x6C, 0x6F, 0x73, 0x74, 0x78, 0x78, 0x7B, 0x7A, 0x7F, 0x6E, 0x5F, 0x6A,
    0x6D, 0x72, 0x73, 0x77, 0x78, 0x7B, 0x7A, 0x7E, 0x78, 0x5F, 0x64, 0x6C, 0x6F, 0x73, 0x75, 0x79,
    0x7A, 0x7C, 0x7C, 0x80, 0x68, 0x5E, 0x6A, 0x6C, 0x72, 0x74, 0x79, 0x79, 0x7C, 0x7C, 0x80, 0x77,
    0x5C, 0x63, 0x6B, 0x6E, 0x73, 0x77, 0x7A, 0x7A, 0x7D, 0x7D, 0x81, 0x68, 0x5C, 0x69, 0x6B, 0x71,
    0x74, 0x79, 0x79, 0x7C, 0x7C, 0x80, 0x7A, 0x5D, 0x62, 0x6A, 0x6E, 0x74, 0x75, 0x79, 0x79, 0x7D,
    0x7C, 0x83, 0x6D, 0x5A, 0x67, 0x6A, 0x71, 0x74, 0x77, 0x79, 0x7C, 0x7D, 0x7E, 0x80, 0x62, 0x5D,
    0x69, 0x6B, 0x73, 0x74, 0x79, 0x79, 0x7D, 0x7D, 0x82, 0x78, 0x5B, 0x62, 0x69, 0x6E, 0x74, 0x75,
    0x79, 0x7A, 0x7F, 0x7D, 0x83, 0x6D, 0x59, 0x66, 0x6A, 0x71, 0x74, 0x77, 0x79, 0x7C, 0x7E, 0x7F,
    0x80, 0x61, 0x5C, 0x68, 0x6C, 0x73, 0x74, 0x7A, 0x7A, 0x7E, 0x7C, 0x82, 0x77, 0x5A, 0x62, 0x68,
    0x6F, 0x73, 0x76, 0x7A, 0x7A, 0x7E, 0x7D, 0x84, 0x6B, 0x59, 0x66, 0x6A, 0x71, 0x73, 0x79, 0x79,
    0x7C, 0x7D, 0x80, 0x7F, 0x5E, 0x5E, 0x68, 0x6C, 0x73, 0x74, 0x7B, 0x79, 0x7E, 0x7C, 0x84, 0x73,
    0x58, 0x63, 0x68, 0x70, 0x73, 0x77, 0x7A, 0x7C, 0x7F, 0x7F, 0x83, 0x64, 0x59, 0x66, 0x6B, 0x72,
    0x74, 0x7A, 0x7A, 0x7E, 0x7E, 0x83, 0x7A, 0x59, 0x5F, 0x67, 0x6E, 0x73, 0x76, 0x7B, 0x7B, 0x80,
    0x7E, 0x85, 0x6B, 0x56, 0x64, 0x69, 0x71, 0x73, 0x79, 0x7A, 0x7D, 0x7F, 0x82, 0x80, 0x5D, 0x5B,
    0x66, 0x6C, 0x72, 0x74, 0x7A, 0x7B, 0x80, 0x7F, 0x86, 0x72, 0x56, 0x62, 0x68, 0x6F, 0x72, 0x76,
    0x7A, 0x7D, 0x80, 0x82, 0x84, 0x62, 0x58, 0x65, 0x69, 0x72, 0x73, 0x79, 0x7B, 0x80, 0x7F, 0x86,
    0x79, 0x57, 0x5F, 0x66, 0x6D, 0x72, 0x75, 0x7B, 0x7D, 0x81, 0x80, 0x88, 0x69, 0x55, 0x63, 0x68,
    0x71, 0x73, 0x79, 0x7A, 0x7F, 0x7F, 0x84, 0x80, 0x59, 0x5A, 0x66, 0x6C, 0x73, 0x75, 0x7A, 0x7A,
    0x80, 0x7F, 0x89, 0x70, 0x53, 0x62, 0x67, 0x70, 0x73, 0x78, 0x7A, 0x7E, 0x80, 0x83, 0x85, 0x5C,
    0x57, 0x65, 0x69, 0x72, 0x74, 0x7B, 0x7A, 0x81, 0x80, 0x88, 0x75, 0x52, 0x60, 0x66, 0x6E, 0x73,
    0x77, 0x7B, 0x7E, 0x82, 0x83, 0x86, 0x5E, 0x56, 0x66, 0x69, 0x72, 0x73, 0x7B, 0x7A, 0x82, 0x80,
    0x89, 0x75, 0x51, 0x60, 0x66, 0x6F, 0x72, 0x78, 0x7B, 0x7D, 0x82, 0x83, 0x87, 0x5D, 0x56, 0x65,
    0x69, 0x72, 0x73, 0x7B, 0x7A, 0x82, 0x80, 0x89, 0x73, 0x51, 0x61, 0x66, 0x6F, 0x72, 0x78, 0x7A,
    0x7E, 0x82, 0x84, 0x85, 0x5A, 0x57, 0x66, 0x6A, 0x72, 0x74, 0x7B, 0x7C, 0x82, 0x81, 0x8A, 0x6E,
    0x50, 0x61, 0x66, 0x6F, 0x73, 0x79, 0x7B, 0x80, 0x81, 0x86, 0x81, 0x55, 0x59, 0x65, 0x6A, 0x73,
    0x76, 0x7B, 0x7D, 0x83, 0x81, 0x8B, 0x67, 0x50, 0x62, 0x66, 0x70, 0x74, 0x7B, 0x7B, 0x83, 0x80,
    0x88, 0x7C, 0x51, 0x5C, 0x66, 0x6B, 0x72, 0x77, 0x7C, 0x7F, 0x82, 0x83, 0x89, 0x5F, 0x52, 0x64,
    0x67, 0x71, 0x74, 0x7B, 0x7D, 0x82, 0x81, 0x8B, 0x73, 0x4F, 0x5F, 0x66, 0x6E, 0x73, 0x79, 0x7C,
    0x80, 0x82, 0x85, 0x85, 0x58, 0x55, 0x66, 0x69, 0x72, 0x74, 0x7C, 0x7D, 0x83, 0x81, 0x8B, 0x6B,
    0x4E, 0x62, 0x66, 0x70, 0x73, 0x79, 0x7C, 0x80, 0x82, 0x87, 0x7F, 0x53, 0x59, 0x66, 0x6B, 0x73,
    0x76, 0x7C, 0x7D, 0x83, 0x82, 0x8A, 0x65, 0x50, 0x64, 0x67, 0x70, 0x74, 0x7B, 0x7C, 0x82, 0x81,
    0x88, 0x7C, 0x51, 0x5B, 0x66, 0x6B, 0x73, 0x77, 0x7C, 0x7E, 0x83, 0x83, 0x8A, 0x62, 0x51, 0x64,
    0x66, 0x70, 0x73, 0x7B, 0x7C, 0x83, 0x81, 0x89, 0x7B, 0x50, 0x5C, 0x64, 0x6C, 0x73, 0x77, 0x7C,
    0x7F, 0x83, 0x82, 0x8B, 0x61, 0x51, 0x63, 0x66, 0x71, 0x73, 0x7B, 0x7C, 0x84, 0x81, 0x8A, 0x7A,
    0x4F, 0x5D, 0x64, 0x6B, 0x73, 0x77, 0x7C, 0x7F, 0x84, 0x84, 0x8A, 0x60, 0x51, 0x63, 0x66, 0x72,
    0x74, 0x7B, 0x7C, 0x83, 0x82, 0x8A, 0x7A, 0x4F, 0x5B, 0x64, 0x6C, 0x73, 0x77, 0x7B, 0x7F, 0x84,
    0x84, 0x8A, 0x60, 0x50, 0x63, 0x67, 0x70, 0x74, 0x7B, 0x7C, 0x84, 0x82, 0x8B, 0x7A, 0x4E, 0x5B,
    0x65, 0x6B, 0x72, 0x78, 0x7B, 0x80, 0x85, 0x84, 0x8B, 0x5F, 0x50, 0x63, 0x66, 0x71, 0x74, 0x7B,
    0x7C, 0x84, 0x82, 0x8B, 0x79, 0x4D, 0x5C, 0x64, 0x6C, 0x72, 0x78, 0x7C, 0x80, 0x84, 0x85, 0x89,
    0x5B, 0x51, 0x63, 0x68, 0x72, 0x74, 0x7D, 0x7C, 0x83, 0x82, 0x8C, 0x71, 0x4D, 0x5E, 0x65, 0x6E,
    0x72, 0x7A, 0x7C, 0x81, 0x83, 0x87, 0x84, 0x54, 0x55, 0x64, 0x69, 0x72, 0x76, 0x7D, 0x7E, 0x84,
    0x83, 0x8C, 0x64, 0x4D, 0x62, 0x65, 0x70, 0x74, 0x7D, 0x7C, 0x83, 0x82, 0x8C, 0x77, 0x4C, 0x5D,
    0x65, 0x6D, 0x72, 0x7A, 0x7C, 0x81, 0x83, 0x88, 0x85, 0x52, 0x55, 0x65, 0x6A, 0x71, 0x77, 0x7D,
    0x7F, 0x84, 0x84, 0x8C, 0x60, 0x4E, 0x62, 0x67, 0x71, 0x75, 0x7D, 0x7D, 0x85, 0x81, 0x8C, 0x6F,
    0x4B, 0x5F, 0x65, 0x70, 0x73, 0x7C, 0x7C, 0x82, 0x81, 0x89, 0x7E, 0x4E, 0x5A, 0x64, 0x6D, 0x72,
    0x78, 0x7C, 0x7F, 0x83, 0x86, 0x88, 0x57, 0x54, 0x63, 0x69, 0x72, 0x75, 0x7D, 0x7E, 0x84, 0x84,
    0x8C, 0x62, 0x4F, 0x62, 0x67, 0x71, 0x73, 0x7D, 0x7D, 0x83, 0x82, 0x8D, 0x6E, 0x4D, 0x60, 0x66,
    0x70, 0x72, 0x7B, 0x7C, 0x83, 0x82, 0x8C, 0x79, 0x4C, 0x5B, 0x65, 0x6F, 0x72, 0x79, 0x7D, 0x80,
    0x82, 0x89, 0x83, 0x51, 0x57, 0x64, 0x6C, 0x72, 0x76, 0x7D, 0x7F, 0x84, 0x86, 0x8A, 0x59, 0x51,
    0x64, 0x69, 0x72, 0x75, 0x7C, 0x7E, 0x85, 0x83, 0x8E, 0x64, 0x4D, 0x62, 0x66, 0x71, 0x72, 0x7C,
    0x7D, 0x84, 0x82, 0x8E, 0x72, 0x4B, 0x5F, 0x64, 0x6F, 0x71, 0x7A, 0x7D, 0x83, 0x83, 0x8B, 0x7F,
    0x4D, 0x59, 0x63, 0x6D, 0x72, 0x77, 0x7E, 0x81, 0x84, 0x87, 0x89, 0x56, 0x51, 0x63, 0x69, 0x72,
    0x74, 0x7D, 0x7E, 0x86, 0x84, 0x8E, 0x63, 0x4C, 0x62, 0x66, 0x71, 0x72, 0x7C, 0x7D, 0x85, 0x83,
    0x8F, 0x71, 0x4A, 0x5E, 0x64, 0x6F, 0x72, 0x7B, 0x7C, 0x84, 0x83, 0x8C, 0x7E, 0x4B, 0x59, 0x63,
    0x6D, 0x73, 0x79, 0x7D, 0x81, 0x84, 0x88, 0x88, 0x51, 0x53, 0x63, 0x6A, 0x73, 0x78, 0x7E, 0x7F,
    0x85, 0x85, 0x8C, 0x5A, 0x4D, 0x61, 0x67, 0x72, 0x77, 0x7E, 0x7F, 0x85, 0x83, 0x8E, 0x64, 0x4A,
    0x60, 0x66, 0x71, 0x75, 0x7E, 0x7E, 0x85, 0x82, 0x8E, 0x6E, 0x49, 0x5E, 0x64, 0x6F, 0x74, 0x7C,
    0x7E, 0x85, 0x82, 0x8D, 0x76, 0x49, 0x5B, 0x63, 0x6F, 0x74, 0x7B, 0x7E, 0x83, 0x82, 0x8B, 0x7D,
    0x4D, 0x59, 0x64, 0x6C, 0x73, 0x7A, 0x7C, 0x81, 0x83, 0x88, 0x82, 0x52, 0x57, 0x65, 0x6B, 0x73,
    0x79, 0x7C, 0x7F, 0x82, 0x84, 0x83, 0x57, 0x58, 0x66, 0x6C, 0x74, 0x77, 0x7B, 0x7D, 0x81, 0x81,
    0x84, 0x5E, 0x59, 0x68, 0x6C, 0x73, 0x75, 0x7A, 0x7B, 0x80, 0x7E, 0x83, 0x63, 0x5A, 0x68, 0x6C,
    0x73, 0x75, 0x7B, 0x7A, 0x7F, 0x7C, 0x82, 0x67, 0x5A, 0x69, 0x6C, 0x74, 0x75, 0x7A, 0x79, 0x7D,
    0x7B, 0x81, 0x6A, 0x5C, 0x69, 0x6C, 0x73, 0x74, 0x79, 0x79, 0x7C, 0x7A, 0x80, 0x6E, 0x5D, 0x6A,
    0x6C, 0x73, 0x74, 0x78, 0x78, 0x7C, 0x7A, 0x7E, 0x70, 0x5F, 0x6A, 0x6C, 0x73, 0x74, 0x77, 0x78,
    0x7A, 0x79, 0x7D, 0x73, 0x60, 0x6A, 0x6D, 0x73, 0x74, 0x76, 0x77, 0x79, 0x79, 0x7B, 0x75, 0x62,
    0x6A, 0x6D, 0x72, 0x74, 0x76, 0x77, 0x79, 0x79, 0x7A, 0x76, 0x63, 0x69, 0x6E, 0x72, 0x74, 0x76,
    0x77, 0x78, 0x79, 0x7A, 0x78, 0x65, 0x68, 0x6E, 0x71, 0x74, 0x75, 0x77, 0x77, 0x79, 0x7A, 0x79,
    0x66, 0x67, 0x6E, 0x71, 0x74, 0x75, 0x77, 0x77, 0x79, 0x79, 0x79, 0x67, 0x67, 0x6E, 0x70, 0x74,
    0x75, 0x77, 0x77, 0x78, 0x79, 0x7A, 0x69, 0x66, 0x6E, 0x70, 0x74, 0x75, 0x76, 0x77, 0x78, 0x78,
    0x7A, 0x6A, 0x65, 0x6D, 0x6F, 0x74, 0x75, 0x77, 0x77, 0x79, 0x78, 0x7B, 0x6C, 0x65, 0x6D, 0x70,
    0x73, 0x74, 0x77, 0x77, 0x79, 0x77, 0x7B, 0x6E, 0x65, 0x6D, 0x6F, 0x73, 0x74, 0x76, 0x76, 0x78,
    0x77, 0x7A, 0x71, 0x65, 0x6D, 0x6F, 0x73, 0x74, 0x76, 0x76, 0x78, 0x78, 0x7A, 0x73, 0x64, 0x6B,
    0x6F, 0x73, 0x74, 0x76, 0x76, 0x78, 0x78, 0x7A, 0x75, 0x65, 0x6B, 0x6F, 0x72, 0x73, 0x75, 0x76,
    0x78, 0x78, 0x79, 0x77, 0x66, 0x69, 0x6E, 0x71, 0x73, 0x75, 0x77, 0x77, 0x78, 0x78, 0x79, 0x67,
    0x68, 0x6E, 0x71, 0x74, 0x75, 0x76, 0x77, 0x78, 0x78, 0x7A, 0x69, 0x67, 0x6E, 0x70, 0x73, 0x74,
    0x76, 0x77, 0x79, 0x77, 0x7B, 0x6B, 0x66, 0x6E, 0x6F, 0x73, 0x74, 0x76, 0x77, 0x79, 0x77, 0x7B,
    0x6E, 0x65, 0x6D, 0x6F, 0x73, 0x73, 0x76, 0x76, 0x79, 0x78, 0x7B, 0x70, 0x65, 0x6C, 0x6F, 0x72,
    0x73, 0x76, 0x76, 0x79, 0x78, 0x7B, 0x72, 0x65, 0x6B, 0x6E, 0x72, 0x73, 0x76, 0x77, 0x79, 0x78,
    0x7A, 0x74, 0x65, 0x6A, 0x6E, 0x72, 0x74, 0x75, 0x77, 0x78, 0x78, 0x79, 0x77, 0x66, 0x69, 0x6E,
    0x71, 0x73, 0x75, 0x77, 0x77, 0x78, 0x79, 0x78, 0x67, 0x68, 0x6E, 0x71, 0x73, 0x74, 0x77, 0x77,
    0x78, 0x78, 0x79, 0x69, 0x67, 0x6E, 0x70, 0x73, 0x74, 0x77, 0x77, 0x79, 0x78, 0x7A, 0x6B, 0x66,
    0x6D, 0x70, 0x73, 0x74, 0x77, 0x77, 0x79, 0x77, 0x7A, 0x6E, 0x65, 0x6D, 0x6F, 0x73, 0x74, 0x77,
    0x76, 0x79, 0x77, 0x7A, 0x71, 0x65, 0x6C, 0x6F, 0x73, 0x73, 0x76, 0x76, 0x78, 0x78, 0x79, 0x74,
    0x66, 0x6B, 0x6F, 0x72, 0x73, 0x75, 0x76, 0x77, 0x78, 0x79, 0x77, 0x68, 0x69, 0x6F, 0x71, 0x73,
    0x75, 0x76, 0x76, 0x78, 0x78, 0x79, 0x6B, 0x67, 0x6E, 0x71, 0x74, 0x74, 0x76, 0x76, 0x78, 0x77,
    0x7A, 0x6F, 0x66, 0x6D, 0x70, 0x73, 0x73, 0x76, 0x76, 0x78, 0x77, 0x79, 0x74, 0x66, 0x6B, 0x6F,
    0x72, 0x73, 0x75, 0x76, 0x77, 0x78, 0x79, 0x77, 0x68, 0x69, 0x6F, 0x71, 0x73, 0x74, 0x76, 0x76,
    0x78, 0x78, 0x7A, 0x6C, 0x67, 0x6E, 0x70, 0x73, 0x74, 0x76, 0x76, 0x78, 0x77, 0x7A, 0x72, 0x66,
    0x6C, 0x6F, 0x73, 0x73, 0x75, 0x76, 0x77, 0x77, 0x79, 0x77, 0x67, 0x69, 0x6F, 0x71, 0x74, 0x74,
    0x77, 0x76, 0x78, 0x78, 0x7A, 0x6C, 0x65, 0x6E, 0x6F, 0x73, 0x73, 0x77, 0x76, 0x78, 0x78, 0x7B,
    0x73, 0x64, 0x6B, 0x6E, 0x72, 0x73, 0x76, 0x77, 0x78, 0x79, 0x79, 0x79, 0x66, 0x67, 0x6D, 0x70,
    0x73, 0x74, 0x78, 0x77, 0x7A, 0x78, 0x7C, 0x6D, 0x62, 0x6C, 0x6E, 0x73, 0x73, 0x77, 0x77, 0x7A,
    0x79, 0x7C, 0x74, 0x61, 0x69, 0x6D, 0x71, 0x73, 0x76, 0x77, 0x79, 0x7A, 0x7B, 0x7B, 0x64, 0x64,
    0x6C, 0x6F, 0x74, 0x75, 0x78, 0x78, 0x7B, 0x7A, 0x7F, 0x6C, 0x60, 0x6A, 0x6C, 0x73, 0x74, 0x78,
    0x77, 0x7B, 0x7A, 0x7F, 0x74, 0x5D, 0x67, 0x6B, 0x71, 0x74, 0x77, 0x78, 0x7A, 0x7B, 0x7E, 0x7C,
    0x5F, 0x62, 0x6B, 0x6E, 0x73, 0x76, 0x79, 0x79, 0x7D, 0x7C, 0x81, 0x66, 0x5C, 0x69, 0x6C, 0x73,
    0x74, 0x79, 0x79, 0x7E, 0x7D, 0x83, 0x70, 0x58, 0x66, 0x6B, 0x71, 0x73, 0x79, 0x79, 0x7D, 0x7E,
    0x83, 0x79, 0x57, 0x60, 0x6A, 0x6F, 0x73, 0x78, 0x7A, 0x7C, 0x80, 0x81, 0x82, 0x5B, 0x59, 0x68,
    0x6D, 0x73, 0x75, 0x7B, 0x7B, 0x81, 0x80, 0x87, 0x64, 0x53, 0x65, 0x6A, 0x72, 0x73, 0x7B, 0x7B,
    0x82, 0x80, 0x89, 0x70, 0x4F, 0x61, 0x67, 0x71, 0x72, 0x7A, 0x7B, 0x80, 0x82, 0x88, 0x7E, 0x50,
    0x5A, 0x65, 0x6E, 0x72, 0x78, 0x7D, 0x7D, 0x83, 0x84, 0x88, 0x5A, 0x53, 0x64, 0x6A, 0x72, 0x75,
    0x7D, 0x7C, 0x83, 0x82, 0x8C, 0x68, 0x4D, 0x61, 0x66, 0x71, 0x73, 0x7C, 0x7D, 0x82, 0x81, 0x8B,
    0x78, 0x4D, 0x5C, 0x65, 0x6E, 0x72, 0x79, 0x7D, 0x81, 0x82, 0x87, 0x86, 0x54, 0x54, 0x64, 0x6A,
    0x71, 0x76, 0x7D, 0x7E, 0x84, 0x84, 0x8E, 0x63, 0x4B, 0x62, 0x66, 0x70, 0x73, 0x7C, 0x7D, 0x84,
    0x83, 0x8E, 0x76, 0x49, 0x5C, 0x64, 0x6D, 0x72, 0x7A, 0x7D, 0x81, 0x84, 0x8A, 0x85, 0x51, 0x54,
    0x63, 0x69, 0x71, 0x77, 0x7E, 0x80, 0x85, 0x85, 0x8C, 0x5F, 0x4E, 0x62, 0x66, 0x70, 0x74, 0x7D,
    0x7D, 0x84, 0x82, 0x8D, 0x70, 0x4C, 0x5F, 0x65, 0x6E, 0x72, 0x7A, 0x7D, 0x82, 0x82, 0x8A, 0x7E,
    0x4F, 0x59, 0x64, 0x6C, 0x72, 0x77, 0x7E, 0x80, 0x83, 0x86, 0x88, 0x57, 0x52, 0x63, 0x69, 0x72,
    0x75, 0x7E, 0x7F, 0x83, 0x83, 0x8D, 0x61, 0x4E, 0x63, 0x67, 0x71, 0x73, 0x7D, 0x7E, 0x83, 0x82,
    0x8E, 0x6B, 0x4B, 0x60, 0x65, 0x71, 0x73, 0x7C, 0x7D, 0x83, 0x82, 0x8D, 0x74, 0x49, 0x5D, 0x64,
    0x70, 0x72, 0x7B, 0x7D, 0x82, 0x82, 0x8C, 0x7C, 0x4B, 0x5B, 0x64, 0x6E, 0x73, 0x79, 0x7C, 0x81,
    0x82, 0x8A, 0x82, 0x50, 0x58, 0x64, 0x6D, 0x72, 0x78, 0x7C, 0x80, 0x82, 0x87, 0x86, 0x54, 0x57,
    0x65, 0x6C, 0x73, 0x76, 0x7C, 0x7E, 0x82, 0x85, 0x88, 0x59, 0x56, 0x65, 0x6A, 0x73, 0x75, 0x7C,
    0x7D, 0x81, 0x81, 0x88, 0x5E, 0x55, 0x65, 0x6A, 0x74, 0x75, 0x7B, 0x7C, 0x81, 0x80, 0x89, 0x63,
    0x53, 0x66, 0x6A, 0x74, 0x74, 0x7A, 0x7B, 0x81, 0x7F, 0x88, 0x69, 0x53, 0x66, 0x69, 0x73, 0x74,
    0x7A, 0x7A, 0x80, 0x7E, 0x87, 0x70, 0x53, 0x65, 0x68, 0x72, 0x73, 0x78, 0x79, 0x7F, 0x7E, 0x85,
    0x75, 0x56, 0x64, 0x69, 0x71, 0x72, 0x77, 0x77, 0x7E, 0x7F, 0x83, 0x7D, 0x58, 0x60, 0x69, 0x6F,
    0x73, 0x77, 0x79, 0x7C, 0x7F, 0x7F, 0x82, 0x5E, 0x5C, 0x69, 0x6D, 0x74, 0x75, 0x7A, 0x79, 0x7E,
    0x7C, 0x84, 0x68, 0x58, 0x68, 0x6B, 0x74, 0x74, 0x7A, 0x79, 0x7D, 0x7C, 0x83, 0x73, 0x57, 0x64,
    0x6A, 0x72, 0x74, 0x79, 0x7A, 0x7C, 0x7D, 0x80, 0x7D, 0x5C, 0x5F, 0x69, 0x6F, 0x73, 0x77, 0x7A,
    0x7A, 0x7D, 0x7C, 0x82, 0x67, 0x5B, 0x69, 0x6C, 0x73, 0x75, 0x79, 0x79, 0x7C, 0x7B, 0x80, 0x75,
    0x5B, 0x65, 0x6B, 0x70, 0x74, 0x77, 0x79, 0x7A, 0x7C, 0x7C, 0x7E, 0x64, 0x5F, 0x6B, 0x6D, 0x73,
    0x74, 0x79, 0x78, 0x7C, 0x7B, 0x80, 0x72, 0x5D, 0x67, 0x6C, 0x71, 0x73, 0x77, 0x78, 0x7A, 0x7B,
    0x7D, 0x7D, 0x64, 0x61, 0x6C, 0x6E, 0x73, 0x75, 0x77, 0x79, 0x7B, 0x7A, 0x80, 0x71, 0x5E, 0x68,
    0x6D, 0x71, 0x73, 0x76, 0x78, 0x7A, 0x7B, 0x7C, 0x7D, 0x65, 0x62, 0x6C, 0x6D, 0x72, 0x74, 0x78,
    0x78, 0x7B, 0x79, 0x7F, 0x73, 0x5F, 0x67, 0x6C, 0x70, 0x73, 0x76, 0x78, 0x79, 0x7A, 0x7A, 0x7D,
    0x68, 0x61, 0x6C, 0x6E, 0x73, 0x74, 0x78, 0x78, 0x7A, 0x79, 0x7D, 0x76, 0x60, 0x66, 0x6C, 0x70,
    0x74, 0x76, 0x78, 0x78, 0x7A, 0x79, 0x7D, 0x6C, 0x61, 0x6B, 0x6E, 0x73, 0x74, 0x76, 0x77, 0x79,
    0x7A, 0x7B, 0x79, 0x64, 0x65, 0x6D, 0x6F, 0x74, 0x74, 0x77, 0x77, 0x7A, 0x79, 0x7D, 0x71, 0x61,
    0x6A, 0x6E, 0x72, 0x74, 0x75, 0x77, 0x78, 0x79, 0x79, 0x7B, 0x68, 0x64, 0x6D, 0x6F, 0x74, 0x74,
    0x76, 0x77, 0x7A, 0x78, 0x7C, 0x75, 0x62, 0x69, 0x6D, 0x71, 0x74, 0x75, 0x77, 0x78, 0x7A, 0x79,
    0x7C, 0x6B, 0x62, 0x6C, 0x6E, 0x73, 0x74, 0x77, 0x77, 0x79, 0x79, 0x7B, 0x78, 0x64, 0x66, 0x6D,
    0x70, 0x74, 0x75, 0x77, 0x77, 0x7A, 0x78, 0x7C, 0x70, 0x62, 0x6A, 0x6F, 0x72, 0x74, 0x76, 0x77,
    0x78, 0x79, 0x7A, 0x7B, 0x68, 0x63, 0x6D, 0x6F, 0x73, 0x74, 0x78, 0x78, 0x79, 0x78, 0x7B, 0x75,
    0x62, 0x68, 0x6D, 0x70, 0x74, 0x75, 0x78, 0x78, 0x7A, 0x79, 0x7C, 0x6C, 0x62, 0x6C, 0x6E, 0x72,
    0x74, 0x77, 0x77, 0x79, 0x79, 0x7B, 0x79, 0x65, 0x65, 0x6D, 0x6F, 0x73, 0x75, 0x77, 0x77, 0x7A,
    0x79, 0x7D, 0x72, 0x61, 0x69, 0x6D, 0x71, 0x74, 0x76, 0x77, 0x79, 0x7A, 0x7A, 0x7C, 0x69, 0x63,
    0x6C, 0x6E, 0x73, 0x74, 0x77, 0x77, 0x79, 0x79, 0x7C, 0x77, 0x63, 0x68, 0x6D, 0x70, 0x73, 0x75,
    0x77, 0x77, 0x7A, 0x79, 0x7D, 0x6D, 0x62, 0x6B, 0x6E, 0x72, 0x73, 0x77, 0x77, 0x79, 0x79, 0x7B,
    0x79, 0x64, 0x66, 0x6D, 0x70, 0x74, 0x75, 0x78, 0x78, 0x7A, 0x79, 0x7D, 0x6F, 0x61, 0x6B, 0x6D,
    0x72, 0x74, 0x77, 0x78, 0x79, 0x79, 0x7A, 0x7A, 0x66, 0x65, 0x6D, 0x6F, 0x73, 0x75, 0x78, 0x78,
    0x7A, 0x79, 0x7D, 0x71, 0x61, 0x69, 0x6D, 0x71, 0x74, 0x76, 0x78, 0x79, 0x7A, 0x7A, 0x7B, 0x66,
    0x64, 0x6C, 0x6E, 0x74, 0x74, 0x78, 0x77, 0x7A, 0x79, 0x7D, 0x72, 0x61, 0x6A, 0x6D, 0x71, 0x73,
    0x76, 0x77, 0x79, 0x7A, 0x7A, 0x7B, 0x66, 0x65, 0x6C, 0x6F, 0x73, 0x74, 0x78, 0x77, 0x7A, 0x79,
    0x7D, 0x71, 0x61, 0x6A, 0x6D, 0x72, 0x73, 0x76, 0x77, 0x79, 0x79, 0x7A, 0x79, 0x64, 0x66, 0x6D,
    0x70, 0x74, 0x74, 0x77, 0x77, 0x7A, 0x78, 0x7D, 0x6D, 0x62, 0x6C, 0x6E, 0x73, 0x74, 0x77, 0x77,
    0x79, 0x79, 0x7B, 0x77, 0x63, 0x68, 0x6D, 0x70, 0x74, 0x75, 0x77, 0x78, 0x7A, 0x79, 0x7C, 0x6A,
    0x63, 0x6D, 0x6E, 0x74, 0x74, 0x77, 0x77, 0x79, 0x78, 0x7C, 0x73, 0x62, 0x6A, 0x6D, 0x72, 0x74,
    0x76, 0x77, 0x78, 0x79, 0x79, 0x7A, 0x67, 0x66, 0x6D, 0x6F, 0x74, 0x74, 0x77, 0x77, 0x7A, 0x78,
    0x7C, 0x6F, 0x62, 0x6B, 0x6E, 0x72, 0x74, 0x77, 0x77, 0x79, 0x79, 0x7B, 0x78, 0x64, 0x67, 0x6D,
    0x70, 0x74, 0x75, 0x78, 0x77, 0x7A, 0x79, 0x7C, 0x6B, 0x62, 0x6C, 0x6E, 0x73, 0x74, 0x77, 0x77,
    0x79, 0x79, 0x7C, 0x74, 0x61, 0x69, 0x6D, 0x71, 0x74, 0x76, 0x78, 0x78, 0x7A, 0x7B, 0x7B, 0x65,
    0x64, 0x6D, 0x6F, 0x74, 0x75, 0x78, 0x77, 0x7A, 0x79, 0x7E, 0x6D, 0x61, 0x6B, 0x6D, 0x73, 0x74,
    0x77, 0x77, 0x7A, 0x79, 0x7C, 0x76, 0x61, 0x68, 0x6D, 0x70, 0x74, 0x76, 0x77, 0x79, 0x7B, 0x7A,
    0x7C, 0x66, 0x63, 0x6C, 0x6E, 0x73, 0x74, 0x78, 0x78, 0x7B, 0x79, 0x7E, 0x6E, 0x5F, 0x6A, 0x6D,
    0x72, 0x74, 0x77, 0x78, 0x7B, 0x79, 0x7D, 0x77, 0x60, 0x67, 0x6C, 0x70, 0x74, 0x76, 0x78, 0x79,
    0x7B, 0x7B, 0x7D, 0x65, 0x62, 0x6C, 0x6E, 0x73, 0x75, 0x78, 0x78, 0x7C, 0x7A, 0x7F, 0x6E, 0x5E,
    0x6A, 0x6D, 0x72, 0x74, 0x78, 0x78, 0x7B, 0x7B, 0x7E, 0x78, 0x5F, 0x65, 0x6C, 0x70, 0x73, 0x76,
    0x78, 0x79, 0x7C, 0x7B, 0x7E, 0x66, 0x5F, 0x6B, 0x6D, 0x73, 0x74, 0x78, 0x78, 0x7C, 0x7A, 0x7F,
    0x72, 0x5D, 0x68, 0x6C, 0x71, 0x74, 0x77, 0x78, 0x7A, 0x7B, 0x7C, 0x7C, 0x62, 0x62, 0x6C, 0x6F,
    0x74, 0x75, 0x79, 0x79, 0x7B, 0x7A, 0x7E, 0x6C, 0x5E, 0x6A, 0x6D, 0x73, 0x74, 0x77, 0x79, 0x7B,
    0x7B, 0x7D, 0x78, 0x5F, 0x64, 0x6D, 0x70, 0x74, 0x75, 0x79, 0x79, 0x7C, 0x7B, 0x7E, 0x6A, 0x5D,
    0x69, 0x6C, 0x73, 0x74, 0x79, 0x79, 0x7C, 0x7B, 0x7F, 0x79, 0x5D, 0x62, 0x6A, 0x6F, 0x74, 0x76,
    0x7A, 0x7A, 0x7E, 0x7C, 0x82, 0x6A, 0x5A, 0x67, 0x6A, 0x72, 0x74, 0x79, 0x79, 0x7E, 0x7E, 0x81,
    0x7D, 0x5B, 0x5F, 0x67, 0x6C, 0x73, 0x76, 0x7B, 0x7B, 0x7F, 0x7D, 0x86, 0x6D, 0x55, 0x64, 0x68,
    0x71, 0x74, 0x78, 0x79, 0x7F, 0x80, 0x81, 0x82, 0x5C, 0x58, 0x66, 0x6A, 0x74, 0x75, 0x7A, 0x7B,
    0x81, 0x7E, 0x87, 0x74, 0x52, 0x60, 0x66, 0x6F, 0x74, 0x77, 0x7B, 0x7E, 0x81, 0x81, 0x86, 0x61,
    0x54, 0x65, 0x68, 0x72, 0x74, 0x7A, 0x7B, 0x81, 0x7F, 0x87, 0x7C, 0x53, 0x5C, 0x66, 0x6C, 0x73,
    0x76, 0x7B, 0x7D, 0x82, 0x81, 0x8A, 0x67, 0x50, 0x63, 0x66, 0x71, 0x74, 0x7B, 0x7B, 0x80, 0x80,
    0x87, 0x80, 0x53, 0x59, 0x65, 0x6B, 0x73, 0x76, 0x7C, 0x7D, 0x83, 0x82, 0x8C, 0x69, 0x4E, 0x63,
    0x65, 0x6F, 0x72, 0x7A, 0x7C, 0x81, 0x83, 0x8A, 0x83, 0x51, 0x57, 0x63, 0x69, 0x71, 0x76, 0x7D,
    0x7D, 0x85, 0x84, 0x8F, 0x68, 0x4B, 0x60, 0x64, 0x70, 0x72, 0x7B, 0x7B, 0x83, 0x84, 0x8C, 0x84,
    0x4D, 0x55, 0x62, 0x69, 0x71, 0x76, 0x7E, 0x7E, 0x86, 0x86, 0x92, 0x65, 0x47, 0x5F, 0x63, 0x70,
    0x72, 0x7C, 0x7D, 0x84, 0x86, 0x90, 0x85, 0x49, 0x51, 0x60, 0x68, 0x73, 0x76, 0x7E, 0x7F, 0x89,
    0x88, 0x95, 0x65, 0x42, 0x5C, 0x61, 0x6F, 0x73, 0x7D, 0x7D, 0x86, 0x87, 0x8E, 0x87, 0x49, 0x4E,
    0x60, 0x68, 0x72, 0x76, 0x7F, 0x7F, 0x8A, 0x86, 0x93, 0x68, 0x43, 0x5C, 0x63, 0x6F, 0x73, 0x7C,
    0x7D, 0x84, 0x86, 0x8B, 0x89, 0x4E, 0x4F, 0x61, 0x67, 0x73, 0x76, 0x7F, 0x7E, 0x87, 0x84, 0x92,
    0x6E, 0x46, 0x5C, 0x62, 0x6F, 0x73, 0x7B, 0x7D, 0x83, 0x84, 0x88, 0x8A, 0x52, 0x50, 0x62, 0x69,
    0x73, 0x76, 0x7D, 0x7D, 0x86, 0x83, 0x8F, 0x6D, 0x49, 0x5E, 0x64, 0x70, 0x73, 0x7A, 0x7C, 0x82,
    0x83, 0x89, 0x85, 0x51, 0x54, 0x63, 0x69, 0x72, 0x76, 0x7E, 0x7E, 0x85, 0x82, 0x8F, 0x68, 0x4A,
    0x60, 0x64, 0x70, 0x73, 0x7B, 0x7C, 0x84, 0x82, 0x8A, 0x82, 0x4D, 0x57, 0x63, 0x6A, 0x73, 0x76,
    0x7D, 0x80, 0x84, 0x83, 0x8E, 0x63, 0x4D, 0x62, 0x64, 0x72, 0x74, 0x7B, 0x7D, 0x83, 0x82, 0x8C,
    0x7D, 0x4D, 0x59, 0x62, 0x6B, 0x73, 0x77, 0x7E, 0x80, 0x84, 0x85, 0x8C, 0x5D, 0x4E, 0x62, 0x64,
    0x72, 0x73, 0x7D, 0x7E, 0x85, 0x83, 0x8D, 0x78, 0x4A, 0x5A, 0x62, 0x6C, 0x72, 0x78, 0x7F, 0x82,
    0x86, 0x88, 0x8D, 0x58, 0x4D, 0x61, 0x64, 0x71, 0x74, 0x7E, 0x7F, 0x87, 0x85, 0x92, 0x73, 0x44,
    0x5A, 0x61, 0x6B, 0x72, 0x7B, 0x7E, 0x84, 0x87, 0x8E, 0x8D, 0x4F, 0x4B, 0x5F, 0x65, 0x71, 0x75,
    0x7E, 0x80, 0x89, 0x87, 0x98, 0x6A, 0x40, 0x5B, 0x5F, 0x6E, 0x72, 0x7B, 0x7F, 0x87, 0x88, 0x92,
    0x87, 0x46, 0x50, 0x5F, 0x67, 0x72, 0x77, 0x7F, 0x82, 0x8A, 0x89, 0x96, 0x5C, 0x44, 0x5D, 0x62,
    0x6F, 0x73, 0x7F, 0x7F, 0x88, 0x87, 0x95, 0x77, 0x42, 0x57, 0x5F, 0x6C, 0x72, 0x79, 0x80, 0x85,
    0x87, 0x8E, 0x8C, 0x4D, 0x4C, 0x60, 0x66, 0x72, 0x76, 0x7F, 0x80, 0x88, 0x87, 0x93, 0x61, 0x46,
    0x5D, 0x62, 0x71, 0x73, 0x7D, 0x7E, 0x87, 0x85, 0x91, 0x79, 0x45, 0x59, 0x61, 0x6B, 0x71, 0x7A,
    0x7F, 0x83, 0x87, 0x8B, 0x8A, 0x4F, 0x4E, 0x60, 0x66, 0x72, 0x76, 0x80, 0x80, 0x88, 0x87, 0x92,
    0x62, 0x45, 0x5D, 0x62, 0x71, 0x72, 0x7E, 0x80, 0x88, 0x86, 0x93, 0x78, 0x42, 0x56, 0x5F, 0x6B,
    0x71, 0x7A, 0x80, 0x87, 0x89, 0x8F, 0x8C, 0x49, 0x4B, 0x5D, 0x65, 0x71, 0x76, 0x80, 0x84, 0x8C,
    0x8B, 0x99, 0x5B, 0x3E, 0x5A, 0x5F, 0x6F, 0x72, 0x80, 0x81, 0x8E, 0x8C, 0x9E, 0x73, 0x37, 0x53,
    0x5A, 0x6A, 0x6F, 0x7D, 0x80, 0x8D, 0x8E, 0x9A, 0x8D, 0x3C, 0x49, 0x58, 0x62, 0x6F, 0x78, 0x82,
    0x8A, 0x90, 0x93, 0x9B, 0x4A, 0x3F, 0x59, 0x5F, 0x6F, 0x73, 0x80, 0x85, 0x94, 0x8F, 0xA1, 0x61,
    0x37, 0x57, 0x5B, 0x6D, 0x70, 0x7F, 0x82, 0x91, 0x8D, 0x9E, 0x78, 0x35, 0x51, 0x59, 0x6A, 0x70,
    0x7C, 0x81, 0x8C, 0x8E, 0x9A, 0x8B, 0x3A, 0x4A, 0x58, 0x65, 0x71, 0x78, 0x81, 0x89, 0x90, 0x97,
    0x9A, 0x44, 0x41, 0x57, 0x5E, 0x6F, 0x76, 0x81, 0x86, 0x93, 0x93, 0xA5, 0x54, 0x36, 0x55, 0x59,
    0x6E, 0x73, 0x80, 0x84, 0x94, 0x92, 0xAA, 0x69, 0x2D, 0x4F, 0x54, 0x6A, 0x71, 0x7F, 0x83, 0x94,
    0x94, 0xAB, 0x81, 0x2B, 0x47, 0x50, 0x63, 0x6E, 0x7D, 0x85, 0x93, 0x97, 0xA8, 0x96, 0x2E, 0x3D,
    0x4E, 0x5D, 0x6C, 0x78, 0x84, 0x90, 0x9B, 0xA6, 0xA9, 0x39, 0x32, 0x4C, 0x55, 0x69, 0x73, 0x84,
    0x8C, 0x9E, 0xA3, 0xB9, 0x4E, 0x26, 0x49, 0x4E, 0x66, 0x6F, 0x84, 0x86, 0x9E, 0xA1, 0xC1, 0x69,
    0x1C, 0x43, 0x47, 0x63, 0x6C, 0x82, 0x85, 0x9D, 0xA0, 0xC1, 0x88, 0x1B, 0x3B, 0x44, 0x5D, 0x67,
    0x7D, 0x85, 0x9B, 0xA2, 0xBC, 0xA7, 0x23, 0x2F, 0x41, 0x54, 0x65, 0x77, 0x85, 0x94, 0xA5, 0xB3,
    0xC0, 0x39, 0x21, 0x40, 0x4B, 0x63, 0x6F, 0x83, 0x8D, 0xA7, 0xAC, 0xCD, 0x5B, 0x17, 0x3F, 0x43,
    0x5E, 0x67, 0x81, 0x87, 0xA5, 0xA7, 0xCF, 0x84, 0x12, 0x37, 0x40, 0x5B, 0x64, 0x7C, 0x84, 0x9F,
    0xA6, 0xC6, 0xA8, 0x1B, 0x2D, 0x3E, 0x54, 0x63, 0x75, 0x83, 0x97, 0xAA, 0xBA, 0xC3, 0x31, 0x20,
    0x3E, 0x4B, 0x62, 0x6C, 0x84, 0x8F, 0xAC, 0xB1, 0xD3, 0x51, 0x11, 0x3D, 0x43, 0x60, 0x66, 0x82,
    0x89, 0xAB, 0xAB, 0xD5, 0x7A, 0x0C, 0x38, 0x3D, 0x5C, 0x62, 0x7E, 0x87, 0xA4, 0xA9, 0xCC, 0xA0,
    0x13, 0x2F, 0x3A, 0x54, 0x61, 0x77, 0x88, 0x9D, 0xAA, 0xC1, 0xBE, 0x27, 0x21, 0x3A, 0x4B, 0x60,
    0x6E, 0x85, 0x94, 0xAC, 0xB5, 0xD2, 0x47, 0x15, 0x3C, 0x44, 0x5F, 0x68, 0x83, 0x8C, 0xAA, 0xAC,
    0xD5, 0x6F, 0x0E, 0x39, 0x3F, 0x5C, 0x63, 0x7F, 0x87, 0xA4, 0xA7, 0xCE, 0x98, 0x13, 0x32, 0x3D,
    0x55, 0x62, 0x79, 0x87, 0x9D, 0xA7, 0xBE, 0xB5, 0x26, 0x28, 0x40, 0x4F, 0x63, 0x73, 0x86, 0x92,
    0xA5, 0xAF, 0xC6, 0x44, 0x1D, 0x43, 0x49, 0x62, 0x6C, 0x85, 0x8B, 0xA4, 0xA5, 0xC9, 0x66, 0x17,
    0x40, 0x46, 0x60, 0x67, 0x81, 0x88, 0xA1, 0xA2, 0xC5, 0x88, 0x18, 0x3A, 0x44, 0x5A, 0x63, 0x7D,
    0x87, 0x9D, 0xA3, 0xBF, 0xA4, 0x22, 0x30, 0x41, 0x54, 0x62, 0x78, 0x89, 0x97, 0xA3, 0xB4, 0xB6,
    0x32, 0x29, 0x43, 0x4F, 0x61, 0x70, 0x86, 0x91, 0xA6, 0xAD, 0xC2, 0x43, 0x22, 0x44, 0x4B, 0x62,
    0x6B, 0x85, 0x8D, 0xA5, 0xA9, 0xC7, 0x56, 0x1D, 0x44, 0x4A, 0x61, 0x69, 0x82, 0x89, 0xA5, 0xA6,
    0xC9, 0x66, 0x19, 0x41, 0x48, 0x60, 0x67, 0x83, 0x88, 0xA2, 0xA2, 0xC6, 0x76, 0x19, 0x40, 0x47,
    0x60, 0x66, 0x80, 0x87, 0xA0, 0xA1, 0xC3, 0x85, 0x19, 0x3C, 0x44, 0x5D, 0x65, 0x7D, 0x87, 0x9D,
    0xA2, 0xC1, 0x97, 0x1D, 0x37, 0x43, 0x5A, 0x66, 0x7B, 0x86, 0x9A, 0xA1, 0xBA, 0xA3, 0x25, 0x33,
    0x44, 0x58, 0x65, 0x78, 0x85, 0x95, 0xA0, 0xB1, 0xAF, 0x31, 0x2F, 0x46, 0x55, 0x67, 0x73, 0x84,
    0x8F, 0x9F, 0xA8, 0xB8, 0x45, 0x28, 0x48, 0x52, 0x67, 0x6F, 0x84, 0x8B, 0x9D, 0x9F, 0xB9, 0x5C,
    0x26, 0x4A, 0x50, 0x66, 0x6C, 0x81, 0x86, 0x9A, 0x9A, 0xB6, 0x76, 0x26, 0x46, 0x4E, 0x65, 0x6B,
    0x7F, 0x83, 0x93, 0x96, 0xAD, 0x93, 0x2F, 0x3F, 0x4D, 0x61, 0x6B, 0x79, 0x83, 0x8C, 0x98, 0xA1,
    0xA6, 0x40, 0x36, 0x4F, 0x59, 0x6C, 0x73, 0x84, 0x87, 0x98, 0x95, 0xAE, 0x60, 0x2D, 0x50, 0x53,
    0x6A, 0x70, 0x81, 0x82, 0x93, 0x91, 0xAA, 0x83, 0x2E, 0x49, 0x52, 0x63, 0x6F, 0x7B, 0x81, 0x8C,
    0x93, 0x9E, 0x9E, 0x41, 0x3B, 0x53, 0x5C, 0x6F, 0x76, 0x82, 0x84, 0x93, 0x93, 0xA9, 0x62, 0x31,
    0x51, 0x56, 0x6D, 0x72, 0x7F, 0x81, 0x8F, 0x8E, 0xA1, 0x86, 0x37, 0x4A, 0x57, 0x66, 0x71, 0x7A,
    0x80, 0x86, 0x8E, 0x94, 0x9B, 0x51, 0x3D, 0x58, 0x5E, 0x70, 0x73, 0x7F, 0x82, 0x8D, 0x8D, 0x9F,
    0x74, 0x39, 0x52, 0x5A, 0x6A, 0x71, 0x7C, 0x82, 0x89, 0x8D, 0x95, 0x90, 0x45, 0x45, 0x5A, 0x63,
    0x71, 0x76, 0x81, 0x84, 0x8D, 0x8B, 0x9B, 0x64, 0x3C, 0x58, 0x5E, 0x6E, 0x72, 0x7E, 0x81, 0x8A,
    0x8A, 0x96, 0x82, 0x40, 0x4E, 0x5D, 0x6A, 0x72, 0x78, 0x81, 0x84, 0x8A, 0x8D, 0x93, 0x53, 0x44,
    0x5C, 0x64, 0x72, 0x74, 0x80, 0x80, 0x89, 0x88, 0x96, 0x6D, 0x40, 0x58, 0x60, 0x6F, 0x73, 0x7C,
    0x7F, 0x86, 0x87, 0x8F, 0x83, 0x48, 0x50, 0x5E, 0x6B, 0x74, 0x78, 0x7F, 0x81, 0x87, 0x86, 0x8F,
    0x5C, 0x49, 0x5E, 0x65, 0x74, 0x75, 0x7E, 0x7D, 0x85, 0x83, 0x8F, 0x74, 0x47, 0x5A, 0x63, 0x71,
    0x74, 0x7B, 0x7E, 0x81, 0x83, 0x88, 0x85, 0x51, 0x52, 0x62, 0x6C, 0x74, 0x77, 0x7F, 0x7E, 0x83,
    0x82, 0x8C, 0x64, 0x4C, 0x60, 0x67, 0x73, 0x74, 0x7D, 0x7D, 0x82, 0x80, 0x89, 0x77, 0x4E, 0x5B,
    0x65, 0x70, 0x74, 0x79, 0x7D, 0x7F, 0x81, 0x83, 0x84, 0x5B, 0x56, 0x64, 0x6B, 0x75, 0x75, 0x7D,
    0x7C, 0x81, 0x7E, 0x86, 0x6C, 0x53, 0x62, 0x68, 0x73, 0x74, 0x7A, 0x7C, 0x7F, 0x7E, 0x82, 0x7A,
    0x58, 0x5E, 0x68, 0x70, 0x75, 0x77, 0x7B, 0x7C, 0x7E, 0x7E, 0x82, 0x63, 0x5A, 0x67, 0x6D, 0x75,
    0x75, 0x79, 0x7A, 0x7E, 0x7B, 0x82, 0x70, 0x59, 0x65, 0x6A, 0x73, 0x75, 0x77, 0x79, 0x7C, 0x7A,
    0x7E, 0x7A, 0x5E, 0x62, 0x6B, 0x70, 0x75, 0x76, 0x79, 0x7A, 0x7B, 0x7A, 0x7F, 0x67, 0x5E, 0x6A,
    0x6D, 0x75, 0x75, 0x78, 0x79, 0x7B, 0x78, 0x7E, 0x71, 0x5E, 0x68, 0x6C, 0x73, 0x75, 0x77, 0x79,
    0x7A, 0x79, 0x7C, 0x79, 0x61, 0x65, 0x6B, 0x70, 0x75, 0x76, 0x78, 0x79, 0x7A, 0x79, 0x7D, 0x69,
    0x61, 0x6B, 0x6E, 0x74, 0x75, 0x78, 0x78, 0x7A, 0x78, 0x7D, 0x72, 0x60, 0x68, 0x6D, 0x72, 0x74,
    0x77, 0x78, 0x79, 0x79, 0x7A, 0x79, 0x64, 0x65, 0x6D, 0x70, 0x75, 0x75, 0x77, 0x78, 0x79, 0x77,
    0x7C, 0x6C, 0x63, 0x6C, 0x6F, 0x74, 0x74, 0x77, 0x77, 0x79, 0x77, 0x7B, 0x74, 0x64, 0x6A, 0x6E,
    0x72, 0x74, 0x76, 0x77, 0x78, 0x78, 0x79, 0x79, 0x67, 0x67, 0x6E, 0x70, 0x74, 0x74, 0x77, 0x77,
    0x78, 0x77, 0x7A, 0x6D, 0x65, 0x6D, 0x6F, 0x74, 0x74, 0x76, 0x76, 0x78, 0x77, 0x79, 0x74, 0x65,
    0x6B, 0x6E, 0x73, 0x75, 0x75, 0x76, 0x77, 0x77, 0x77, 0x78, 0x68, 0x68, 0x6E, 0x70, 0x75, 0x74,
    0x76, 0x77, 0x78, 0x76, 0x7A, 0x6E, 0x66, 0x6D, 0x6F, 0x74, 0x74, 0x76, 0x76, 0x78, 0x77, 0x79,
    0x74, 0x66, 0x6B, 0x6E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x77, 0x78, 0x78, 0x69, 0x69, 0x6E, 0x71,
    0x74, 0x74, 0x76, 0x77, 0x78, 0x77, 0x7A, 0x6E, 0x66, 0x6D, 0x6F, 0x74, 0x74, 0x76, 0x76, 0x78,
    0x77, 0x79, 0x73, 0x66, 0x6B, 0x6E, 0x73, 0x74, 0x75, 0x77, 0x77, 0x77, 0x78, 0x78, 0x69, 0x69,
    0x6E, 0x71, 0x74, 0x74, 0x77, 0x77, 0x78, 0x76, 0x79, 0x6E, 0x67, 0x6D, 0x6F, 0x74, 0x74, 0x76,
    0x76, 0x78, 0x77, 0x79, 0x73, 0x67, 0x6C, 0x6E, 0x73, 0x74, 0x75, 0x77, 0x77, 0x78, 0x77, 0x78,
    0x6A, 0x69, 0x6E, 0x71, 0x74, 0x74, 0x77, 0x76, 0x78, 0x76, 0x79, 0x6F, 0x67, 0x6D, 0x6F, 0x73,
    0x74, 0x76, 0x76, 0x77, 0x77, 0x78, 0x75, 0x67, 0x6A, 0x6F, 0x72, 0x74, 0x75, 0x76, 0x76, 0x77,
    0x77, 0x79, 0x6D, 0x67, 0x6E, 0x70, 0x74, 0x74, 0x76, 0x76, 0x77, 0x77, 0x79, 0x73, 0x67, 0x6B,
    0x6F, 0x73, 0x74, 0x75, 0x76, 0x76, 0x77, 0x77, 0x78, 0x6C, 0x68, 0x6E, 0x70, 0x74, 0x74, 0x77,
    0x76, 0x77, 0x77, 0x78, 0x73, 0x67, 0x6C, 0x6F, 0x72, 0x73, 0x75, 0x77, 0x76, 0x78, 0x77, 0x78,
    0x6C, 0x68, 0x6E, 0x71, 0x73, 0x74, 0x76, 0x76, 0x77, 0x77, 0x79, 0x73};

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

static uint16_t count = 0;
static uint8_t level = 8;
static uint16_t len = 10;
static uint16_t i = sizeof(wavtable);

static bool update_params(void)
{
    i = rnd(sizeof(wavtable));
    level = 4 + rnd(4);
    count++;
    if (count == len)
    {
        count = 0;
        len = 4 + rnd(4);
        return true;
    }
    else
    {
        return false;
    }
}

bool snd_get_sample(uint8_t *sample)
{
    static uint16_t p = 0;
    static bool rev = false;

    if (rev)
    {
        if (p == i)
        {
            rev = false;
            return update_params();
        }
        else
        {
            if (p == UINT16_MAX)
            {
                p = sizeof(wavtable) - 1;
            }
            int16_t s = pgm_read_byte(&wavtable[p--]);
            *sample = ((s - 128) / level) + 128;
            return false;
        }
    }
    else
    {
        if (p == i)
        {
            rev = true;
            return update_params();
        }
        else
        {
            if (p == sizeof(wavtable))
            {
                p = 0;
            }
            int16_t s = pgm_read_byte(&wavtable[p++]);
            *sample = ((s - 128) / level) + 128;
            return false;
        }
    }
}

void snd_wait(void)
{
    annoy_deep_sleep(2 + rnd(10));
}
