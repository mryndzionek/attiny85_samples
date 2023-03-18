#ifndef _SND_H_
#define _SND_H_

#include <stdint.h>
#include <stdbool.h>

extern const uint16_t SND_DIV;

bool snd_get_sample(uint8_t *sample);
void snd_wait(void);

#endif // _SND_H_