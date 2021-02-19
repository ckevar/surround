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

#include "latency.h"

#include <semaphore.h>
#include <pthread.h>
// #include <sys/time.h>

unsigned short exitLoop = 0;

/*
#ifdef MEASURE_LATENCY
struct Latency m_latency;
#endif
*/
//struct timeval start, end;	

void initAlsa(const char *dev_name[], const unsigned fq, const unsigned c, const unsigned f) {
	loadCapSettings(dev_name[1], fq, c);
	loadPBSettings(dev_name[2], fq, c, f);

	cHandle = captureSetUp();
	pbHandle = playbackSetup();
}

void terminateAlsa() {
	snd_pcm_close(cHandle);

	assert(snd_pcm_drain(pbHandle) == 0);
	snd_pcm_close(pbHandle);
}

void bufferSemInitAll() {
	bufferSem_init(&inBufferSem);
	bufferSem_init(&outBufferSem);
}

void *filterThread(void *arg) {
	dataType *data = arg;
	unsigned short i, j, j0, jn;
	unsigned short incrementor;
	int tmpData;
	if (data->offset) sem_post(&inBufferSem.priv_w);

	while (1) {
		i = data->offset;

		// casting short to complex input
		sem_wait(&inBufferSem.priv_r[i]);
		tmpData = readData;
		for (j = 0; j < tmpData; j++) {
			data->X[j].re = bufRead[i];
			data->X[j].im = 0.0;
			i += 2;
		}
		inBuffer_post_read(&inBufferSem);
		
		// Complete with zeros if the data series isnt filling the buffer
		// for filtering
		for(j = j; j < N; j++) {
			data->X[j].re = 0.0;
			data->X[j].im = 0.0;
		}

		j0 = N - 1;
		jn = j0 - tmpData;
		incrementor = -1;

		if (toggle == 1)
			filterAudio(data->X, data->Hf, EXP, data->overlap, tmpData, 1);
		
		else if (toggle == 2)
			filterAudio(data->X, data->Hs, EXP, data->overlap, tmpData, 1);
		
		else if(toggle == 0) {
			j0 = 0; 
			jn = tmpData; 
			incrementor = 1;
		}

		i = data->offset;
		
		// cast complex to output
		sem_wait(&outBufferSem.priv_w);

		while(j0 != jn) {
			bufWrite[i] = data->X[j0].re;
			i += 2;
			j0 += incrementor;
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

		/*
		#ifdef MEASURE_LATENCY
		gettimeofday(&m_latency.start, NULL);
		#endif
		*/

		readData = capture(cHandle, bufRead, M);
		inBuffer_post_write(&inBufferSem);

		if (exitLoop) break;
	}
 	
	exitLoop = 1;
	pthread_exit(NULL);
}

void *playbackThread(void *arg) {

	/*
	#ifdef MEASURE_LATENCY
	initLatencyBuffer(&m_latency);
	#endif
	*/

	outBuffer_post_read(&outBufferSem);

	while (1) {
		
		sem_wait(outBufferSem.priv_r);
		playback(pbHandle, bufWrite, writeData);

		/*
		#ifdef MEASURE_LATENCY
		gettimeofday(&m_latency.end, NULL);
		#endif
		*/

		outBuffer_post_read(&outBufferSem);

		/*
		#ifdef MEASURE_LATENCY
		computeLatency(&m_latency);
		logLatency(&m_latency, (float)m_latency.micros/writeData);
		#endif
		*/

		if (exitLoop) break;
	}
	pthread_exit(NULL);
}

