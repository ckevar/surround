/* 
 * File Name: capture.c
 * Author: ckevar
 * Created: Dec 10, 2019 at 10:57AM
 */

#include <string.h>

#include "capture.h"

void loadCapSettings(const char name[], const unsigned sr, const unsigned c){
	strcpy(hw.name, name);
	hw.samplerate = sr;
	hw.channels = c;
}

snd_pcm_t * captureSetUp() {

	snd_pcm_hw_params_t *hw_params;
	static snd_pcm_t *capture_handle;

	if((err = snd_pcm_open(&capture_handle, hw.name, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf(stderr, "[ERROR:] cannot open audio device %s (%s)\n", hw.name, snd_strerror(err));
		exit(1);
	}

	if((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		fprintf(stderr, "[ERROR:] cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
		exit(1);
	}

	if((err = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0) {
		fprintf(stderr, "[ERROR:] cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
		exit(1);
	}

	if((err = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set access type (%s)\n", snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set sample format (%s)\n", snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &hw.samplerate, 0)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set sample rate (%s)\n", snd_strerror(err));
		exit(1);
	}

	if((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, hw.channels)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set channel (%s)\n", snd_strerror(err));
		exit(1);
	}

	if((err = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set parameter (%s)\n", snd_strerror(err));
		exit(1);
	}

	snd_pcm_hw_params_free(hw_params);

	if ((err = snd_pcm_prepare(capture_handle)) < 0) {
		fprintf(stderr, "[ERROR:] cannot prepare audio interface for use (%s)\n", snd_strerror(err));
		exit(1);
	}

	return capture_handle;
}

int capture(snd_pcm_t *h, short *b, const size_t f) {
	static snd_pcm_sframes_t res; 
	size_t count = f;

	res = snd_pcm_readi(h, b, count);

	if(res == -EAGAIN) {
		// fprintf(stderr, "[ERROR:] Try it again (%s)\n", snd_strerror(res));
		return -EAGAIN;
	}
	else if (res == -EPIPE) {
		// fprintf(stderr, "[ERROR:] (%s)\n", snd_strerror(res));
		return -EPIPE;
	}	
	else if (res == -ESTRPIPE) {
		// fprintf(stderr, "[ERROR:] (%s)\n", snd_strerror(res));
		return -ESTRPIPE;
	} else if (res < 0) {
		// fprintf(stderr, "[ERROR:] Unknown error (%s)\n", snd_strerror(res));
		return -1;
	}

	if(res > 0) 
		count -= res;

	return res;
}