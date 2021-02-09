/* 
 * file name: lantency.h
 * Author: ckevar
 * created Feb 5, 2021 at 11:14 AM
 */

#ifdef MEASURE_LATENCY

#ifndef LATENCY_H
#define LATENCY_H 

#include <stdio.h>
#include <sys/time.h>


#define LATENCY_BUFFER_SIZE 256
#define LATENCY_LOG_FILE "latency_micros.dat"

struct Latency {
	FILE *file;
	float buffer[LATENCY_BUFFER_SIZE];
	unsigned short size;
	unsigned short tail;
	struct timeval start;
	struct timeval end;
	long micros;
};

void initLatencyBuffer(struct Latency *lb);
void logLatency(struct Latency *lb, const float latency_val);
void computeLatency(struct Latency *lb);

#endif

#endif
