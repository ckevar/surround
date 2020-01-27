/*
 *	File Name: task.c
 *	Author: ckevar
 *	Created: Jan 16, 2020 at 9:26 PM
 *	Reference: "Real-Time Digital Signal Processing Implementation and Application, 2nd Ed" by Sen et al.
 */

#include "task.h"
#include "filter.h"
#include "capture.h"
#include "playback.h"

#include <semaphore.h>
#include <pthread.h>

unsigned short exitLoop = 0;

void initAlsa(const char *capDevName, const char* pbDevName, const unsigned fq, const unsigned c, const unsigned f) {
	loadCapSettings(capDevName, fq, c);
	loadPBSettings(pbDevName, fq, c, f);

	cHandle = captureSetUp();
	pbHandle = playbackSetup();
}

void bufferSemInitAll() {
	bufferSem_init(&inBufferSem);
	bufferSem_init(&outBufferSem);
}

void *filterThread(void *arg) {
	dataType *data = arg;
	unsigned short i, j, j0, jn;
	int tmpData;
	if (data->offset) sem_post(&inBufferSem.priv_w);

	while (1) {
		i = data->offset;

		// casting short to complex input
		sem_wait(&inBufferSem.priv_r);
		tmpData = readData;
		for (j = 0; j < tmpData; j++) {
			data->X[j].re = bufRead[i];
			data->X[j].im = 0.0;
			i += 2;
		}
		inBuffer_post_read(&inBufferSem);
		
		for(j = j; j < N; j++) {
			data->X[j].re = 0.0;
			data->X[j].im = 0.0;
		}

		j0 = N - 1;
		jn = j0 - tmpData;

		if (toggle == 1)
			filterAudio(data->X, data->Hf, EXP, data->overlap, tmpData, 1);
		
		else if (toggle == 2)
			filterAudio(data->X, data->Hs, EXP, data->overlap, tmpData, 1);
		
		else if (toggle == 0) 
			filterAudio(data->X, NULL, EXP, data->overlap, tmpData, 0);


		i = data->offset;

		// cast complex to output
		sem_wait(&outBufferSem.priv_w);
		for (j = j0; j > jn; --j) {
			bufWrite[i] = data->X[j].re;
			// if (data->X[j].re > 32767) printf("BIG\n");
			// if (data->X[j].re < -32768) printf("SMALL\n");
			i += 2;
		}
		writeData = tmpData;
		outBuffer_post_write(&outBufferSem);

		if (exitLoop) break;
	}
	inBuffer_post_read(&inBufferSem);
	pthread_exit(NULL);
}


void *captureThread(void *arg) {
	
	while (1) {
		sem_wait(&inBufferSem.priv_w);
		readData = capture(cHandle, bufRead, M);
		inBuffer_post_write(&inBufferSem);

		if (exitLoop) break;
	}
 	
	exitLoop = 1;
	pthread_exit(NULL);
}

void *playbackThread(void *arg) {
	// struct timeval start, end;	
	outBuffer_post_read(&outBufferSem);
	// gettimeofday(&start, NULL);

	while (1) {
		
		sem_wait(&outBufferSem.priv_r);
		playback(pbHandle, bufWrite, writeData);
		// gettimeofday(&end, NULL);
		outBuffer_post_read(&outBufferSem);

		// long seconds = (end.tv_sec - start.tv_sec);
		// long micros = (seconds*1000000 + end.tv_usec) - start.tv_usec;
		// printf("17437.642 %ld\n", micros);
		// start = end;
		if (exitLoop) break;
	}
	pthread_exit(NULL);
}

