#ifndef CAPTURE_H
#define CAPTURE_H 


typedef struct {
	char name[10];
	unsigned int samplerate;
	unsigned int channels;
} capHW;

int captureSetUp(snd_pcm_t *capture_handle, capHW const &hw);

#endif
