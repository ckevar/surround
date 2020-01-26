#include <signal.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#include "capture.h"
#include "playback.h"
#include "fft.h"
#include "fcomplex.h"
#include "filter.h"
#include "rw.h"

// #include "input_f.dat"
// #include "audio.dat"

#define FRAMES 512
#define CHANNELS 2

snd_pcm_t *cHandle;
snd_pcm_t *pbHandle;


complex XLeft[N];		// Input array
complex XRight[N];		// Input array
complex HLeft[N];		// filter at frequency domain
complex HRight[N];		// filter at frequency domain
complex W[N - 1];		// Twiddle e^(-j2*pi/N) table;
short overlapLeft[L - 1];	// Overlap left		
short overlapRight[L - 1];	// Overlap right

short bufRead[CHANNELS * M];	// buffer for reading
short bufWrite[CHANNELS * M];	// buffer for reading

void INT_handler(int);
void kill_all_pcm();

typedef struct {
	complex *X;
	complex *H;
	short *overlap;
	unsigned short offset;
} dataType;

// variables for synchonization
struct BufferSem inBufferSem, outBufferSem;
int rdCntInBuf, wrtCntOutBuf;
pthread_mutex_t mutexInBuf, mutexOutBuf;
int readData, writeData;

unsigned short exitLoop = 0;
// unsigned short tmp = 0;

static void *filterThread(void *arg) {
	dataType *data = arg;
	unsigned short i, j;
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

		filterAudio(data->X, data->H, W, EXP, data->overlap, tmpData);

		i = data->offset;

		// cast complex to output
		sem_wait(&outBufferSem.priv_w);
		for (j = N - 1; j > (N - 1) - tmpData; --j) {
			bufWrite[i] = data->X[j].re;
			if (data->X[j].re > 32767) fprintf(stderr, "bigg\n");
			if (data->X[j].re < -32768) fprintf(stderr, "bigg\n");
			i += 2;
		}
		writeData = tmpData;
		outBuffer_post_write(&outBufferSem);

		// if (tmp == 2) exitLoop = 1;
		// tmp ++;
		if (exitLoop) break;
	}
	inBuffer_post_read(&inBufferSem);

}

// short *input = audio;

static void *captureThread(void *arg) {
	
	// int totalData = sizeof(audio) / (CHANNELS * sizeof(short));
	// printf("TotalData %d\n", totalData);
	// input += CHANNELS * 120000;

	while (1) {
		sem_wait(&inBufferSem.priv_w);
		readData = capture(cHandle, bufRead, M);
		// memcpy(bufRead, input, CHANNELS * M * sizeof(short));
		inBuffer_post_write(&inBufferSem);

		// input += CHANNELS * M;
		// if (input >= (audio + totalData)) break;
		if (exitLoop) break;
	}
 	
	exitLoop = 1;
}

static void *playbackThread(void *arg) {
	FILE *fd;
	fd = fopen("data/audioFilter.dat", "w");
	
	struct timeval start, end;
	
	outBuffer_post_read(&outBufferSem);
	gettimeofday(&start, NULL);

	while (1) {
		
		sem_wait(&outBufferSem.priv_r);
		playback(pbHandle, bufWrite, writeData);
		gettimeofday(&end, NULL);
		// for (int i = 0; i < M * CHANNELS; i += 2) 
		// 	fprintf(fd, "%d, %d\n", bufWrite[i], bufWrite[i + 1]);
		outBuffer_post_read(&outBufferSem);
		long seconds = (end.tv_sec - start.tv_sec);
		long micros = (seconds*1000000 + end.tv_usec) - start.tv_usec;
		printf("17437.642 %ld\n", micros);
		start = end;
		if (exitLoop) break;
	}
	fclose(fd);
	printf("leaving playback\n");
}


// void linearProcess(dataType *left, dataType *right) {
// 	int totalData = sizeof(audio) / sizeof(short);
// 	unsigned short i, j;
// 	FILE *fd;

// 	fd = fopen("data/audioFilter.dat", "w");
// 	input += CHANNELS * 120000;

// 	unsigned short tmpLeft = 0;


// 	while (1) {
// 		memcpy(bufRead, input, CHANNELS * M * sizeof(short));
// 		i = 0;
// 		for (j = 0; j < M; j++) {
// 			left->X[j].re = bufRead[i];
// 			left->X[j].im = 0.0;
// 			i += 2;
// 		}
// 		for (j = j; j < N; j ++) {
// 			left->X[j].re = 0.;
// 			left->X[j].im = 0.;
// 		}

// 		i = 1;
// 		for (j = 0; j < M; j++) {
// 			right->X[j].re = bufRead[i];
// 			right->X[j].im = 0.0;
// 			i += 2;
// 		}
// 		for(j = j; j < N; j ++) {
// 			right->X[j].re = 0.;
// 			right->X[j].im = 0.;
// 		}

// 		filterAudio(left->X, left->H, W, EXP, left->overlap);
// 		filterAudio(right->X, left->H, W, EXP, right->overlap);
// 		i = 0;
// 		for (j = N - 1; j > (N - 1) - M; --j) {
// 			bufWrite[i] = left->X[j].re;
// 			i += 2;
// 		}
// 		i = 1;
// 		for (j = N - 1; j > (N - 1) - M; --j) {
// 			bufWrite[i] = right->X[j].re;
// 			i += 2;
// 		}
// 		for (i = 0; i < M * CHANNELS; i += 2) 
// 			fprintf(fd, "%d, %d\n", bufWrite[i], bufWrite[i + 1]);

// 		// for (i = 0; i < N; ++i)
// 		// 	fprintf(fd, "%0.24f, %0.24f\n", left->X[i].re, right->X[i].re);
// 		// if (tmpLeft == 2) break;
// 		// tmpLeft++;
// 		// break;
// 		input += CHANNELS * M;
// 		if (input >= (audio + totalData)) break;
// 	}

// 	fclose(fd);
// }

int main(int argc, char const *argv[]) {
	// TODO: semaphores for sync, 4 per buffer, two say, im using, two to say it's free now
	// https://www.geeksforgeeks.org/readers-writers-problem-set-1-introduction-and-readers-preference-solution/
	unsigned short i, j;				// for iteration
	pthread_t filterThreadId[2];
	pthread_t captureId, playbackId;
	dataType dataLeft, dataRight;

	loadCapSettings("hw:2,1", 44100, 2);
	loadPBSettings("hw:0,0", 44100, 2, 1024);

	cHandle = captureSetUp();
	pbHandle = playbackSetup();

	// creates a precomputed twiddle components
	createTwiddleTable(W, EXP); 	

	// cload filter at the domain time 
	loadFilters(HLeft, HRight, W, EXP, L);

	memset(overlapLeft, 0, (L - 1) * sizeof(short));
	memset(overlapRight, 0, (L - 1) * sizeof(short));

	dataLeft.X = XLeft;
	dataLeft.H = HLeft;
	dataLeft.offset = 0;
	dataLeft.overlap = overlapLeft;

	dataRight.X = XRight;
	dataRight.H = HLeft;
	dataRight.offset = 1;
	dataRight.overlap = overlapRight;

	// linearProcess(&dataLeft, &dataRight);
	bufferSem_init(&inBufferSem);
	bufferSem_init(&outBufferSem);
	pthread_create(&captureId, NULL, &captureThread, NULL);
	usleep(100);
	while(sem_trywait(&inBufferSem.priv_w) > -1);
	pthread_create(&filterThreadId[0], NULL, &filterThread, &dataLeft); // thread for right channel
	pthread_create(&filterThreadId[1], NULL, &filterThread, &dataRight); // thread for left channel
	pthread_create(&playbackId, NULL, &playbackThread, NULL);

	// while(1) {
	// 	int readData = capture(cHandle, bufRead, FRAMES);
	// 	filter(X, H, W, EXP, OVERLAP);
	// 	playback(pbHandle, buf, readData);
	// }


	pthread_join(captureId, NULL);
	pthread_join(filterThreadId[0], NULL);
	pthread_join(filterThreadId[1], NULL);
	pthread_join(playbackId, NULL);
	/*****************/
	kill_all_pcm();

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
