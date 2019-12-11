#include "capture.h"

#define FRAMES 1024
#define CHANNELS 2

int main(int argc, char const *argv[])
{
	snd_pcm_t *cHandle;
	short buf[CHANNELS * FRAMES];

	loadCapSettings("hw:2,1", 44100, 2);
	cHandle = captureSetUp();

	/*************************/
	const char *fname = "recorded.dat";

	printf("Recording to file: %s\n", fname);

	int fd = open(fname, O_CREAT | O_WRONLY, 0666);
	assert(fd != -1);

	for (int i = 0; i < 400; ++i) {
		capture(cHandle, buf, FRAMES);
		write(fd, buf, CHANNELS * FRAMES * sizeof(short));
	}

	close(fd);
	/*************************/
	
	snd_pcm_close(cHandle);
	return 0;
}