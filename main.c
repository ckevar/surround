#include <signal.h>

#include "capture.h"
#include "playback.h"

#define FRAMES 512
#define CHANNELS 2

snd_pcm_t *cHandle;
snd_pcm_t *pbHandle;

void INT_handler(int);
void kill_all_pcm();

int main(int argc, char const *argv[])
{
	short buf[CHANNELS * FRAMES];

	loadCapSettings("hw:2,1", 44100, 2);
	loadPBSettings("hw:0,0", 44100, 2, FRAMES);

	cHandle = captureSetUp();
	pbHandle = playbackSetup();

	signal(SIGINT, INT_handler);

	while(1){
		int readData = capture(cHandle, buf, FRAMES);
		// FILTER
		playback(pbHandle, buf, readData);
	}

	kill_all_pcm();
	return 0;
}

void kill_all_pcm(){
	snd_pcm_close(cHandle);
	assert(snd_pcm_drain(pbHandle) == 0);
	snd_pcm_close(pbHandle);
}

void INT_handler(int sig) {
	kill_all_pcm();
	exit(0);
}
