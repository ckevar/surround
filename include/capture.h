#include <alsa/asoundlib.h>

#ifndef CAPTURE_H
#define CAPTURE_H 


typedef struct {
	char name[10];
	unsigned samplerate;
	unsigned channels;
} capHW;

capHW hw;
int err;

void loadCapSettings(const char name[], const unsigned sr, const unsigned c);

snd_pcm_t * captureSetUp();
int capture(snd_pcm_t *h, short *b, const size_t f);

#endif
