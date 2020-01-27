/*
 * File Name: main.c
 * Author: ckevar
 * Created: Dec 10, 2019 at 11:11AM
 */
#include <signal.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <ncurses.h>

#include "definition.h"
#include "types.h"
#include "capture.h"
#include "playback.h"
#include "task.h"
#include "filter.h"
#include "gui.h"

complex XLeft[N];			// Input array
complex XRight[N];			// Input array
complex HSurroL[N];			// filter at frequency domain
complex HSurroR[N];			// filter at frequency domain
complex HFrontL[N];			// filter at frequency domain
complex HFrontR[N];			// filter at frequency domain
short overlapLeft[L - 1];	// Overlap left		
short overlapRight[L - 1];	// Overlap right
unsigned char toggle = 0;	// to enable or disable the feature

void INT_handler(int);
void kill_all_pcm();

void dataInit(dataType *dleft, dataType *dright){
	memset(overlapLeft, 0, (L - 1) * sizeof(short));		// cleaning the overlap for left channel
	memset(overlapRight, 0, (L - 1) * sizeof(short));		// cleaning the overlap for right channel

	dleft->X = XLeft;
	dleft->Hs = HSurroL;
	dleft->Hf = HFrontL;
	dleft->offset = 0;
	dleft->overlap = overlapLeft;

	dright->X = XRight;
	dright->Hs = HSurroR;
	dright->Hf = HFrontR;
	dright->offset = 1;
	dright->overlap = overlapRight;
}


int main(int argc, char const *argv[]) {
	pthread_t leftFilterId, rightFilterId;
	pthread_t captureId, playbackId;
	dataType dataLeft, dataRight;
	char c;

	// Init Alsa devices
	initAlsa(CAPTURE_DEVICE, PLAYBACK_DEVICE, FREQUENCY, CHANNELS, FRAMES);

	// creates a precomputed twiddle components
	initFFT(EXP);

	// cload filter at the domain time 
	loadFiltersFront(HFrontL, HFrontR, EXP, L);
	loadFiltersSurr(HSurroR	, HSurroL, EXP, L);

	// initialize data
	dataInit(&dataLeft, &dataRight);

	// Init semaphores
	bufferSemInitAll();

	// Init interface
	initGui();

	
	// Init threads
	pthread_create(&captureId, NULL, &captureThread, NULL);
	usleep(1000);
	pthread_create(&leftFilterId, NULL, &filterThread, &dataLeft); // thread for left channel
	pthread_create(&rightFilterId, NULL, &filterThread, &dataRight); // thread for right channel
	pthread_create(&playbackId, NULL, &playbackThread, NULL);

	while(1) {
		c = getch();
		if (c == '\t') {
			toggle++;
			if (toggle == 3) toggle = 0;

			toogleSelectorGui(toggle);
		} else if (c == 'q' || exitLoop) break;
	}
	exitLoop = 1;

	// wait threads
	pthread_join(captureId, NULL);
	pthread_join(leftFilterId, NULL);
	pthread_join(rightFilterId, NULL);
	pthread_join(playbackId, NULL);
	
	// -------------------------

	kill_all_pcm();
	killGui();

	return 0;
}

void kill_all_pcm(){
	snd_pcm_close(cHandle);
	assert(snd_pcm_drain(pbHandle) == 0);
	snd_pcm_close(pbHandle);
}

void INT_handler(int sig) {
	exitLoop = 1;
	kill_all_pcm();
	exit(0);
}
