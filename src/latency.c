/*
 * file Name: latency.c
 * Author: ckevar
 * created Feb 5, 2021 at 11:20 AM
 */

#ifdef MEASURE_LATENCY

#include <latency.h>

void initLatencyBuffer(struct Latency *lb) {
	lb->size = LATENCY_BUFFER_SIZE;
	lb->tail = 0;
	lb->file = fopen(LATENCY_LOG_FILE, "w");
	fclose(lb->file);
}

void logLatency(struct Latency *lb, const float latency_val) {
	lb->buffer[lb->tail] = latency_val;
	lb->tail = lb->tail + 1;


	if(LATENCY_BUFFER_SIZE == lb->tail) {
		lb->file = fopen(LATENCY_LOG_FILE, "a");
		for (unsigned short i = 0; i < lb->size; ++i)
			fprintf(lb->file, "%f\n", *(lb->buffer + i));
		fclose(lb->file);
		lb->tail = 0;
	}
}

void computeLatency(struct Latency *lb) {
	long seconds = (lb->end.tv_sec - lb->start.tv_sec);
	lb->micros = (seconds*1000000 + lb->end.tv_usec) - lb->start.tv_usec;
}

#endif