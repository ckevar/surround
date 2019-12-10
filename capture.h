#ifndef CAPTURE_H
#define CAPTURE_H 


typedef struct {
	char name[10];
	unsigned samplerate;
	unsigned channels;
} capHW;

static capHW hw;

void loadCapSettings(const char name[], const unsigned sr, const unsigned c);

// int captureSetUp(snd_pcm_t *capture_handle);

#endif
