/*
 *	File Name: task.h
 *	Author: ckevar
 *	Created: Jan 26, 2020 at 09:59 PM
 */
#ifndef TASK_H
#define TASK_H 

#include "rw.h"
#include "definition.h"
#include "types.h"

#include <alsa/asoundlib.h>

// Alsa handlers
snd_pcm_t *cHandle;			// capture handler
snd_pcm_t *pbHandle;		// playback handler

// Variables
short bufRead[CHANNELS * M];	// buffer for capturing
short bufWrite[CHANNELS * M];	// buffer for playback
int readData, writeData;		// number of samples captured

struct BufferSem inBufferSem, outBufferSem;	// to synch tasks

extern unsigned short exitLoop;
extern unsigned char toggle;

// Functions
void initAlsa(const char *dev_name[], const unsigned fq, const unsigned c, const unsigned f);
void bufferSemInitAll();			// init semaphores

void *filterThread(void *arg);
void *captureThread(void *arg);
void *playbackThread(void *arg);


#endif