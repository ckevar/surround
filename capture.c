#include "capture.h"

int captureSetUp(snd_pcm_t *capture_handle, capHW const &hw); {
	int err;
	snd_pcm_hw_params_t *hw_params;
	printf("HW: %s\n", hw->name);
/*
	if((err = snd_pcm_open(&capture_handle, hw, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf(stderr, "[ERROR:] cannot open audio device (%s)\n", snd_strerror(err));
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

	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &samplerate, 0)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set sample rate (%s)\n", snd_strerror(err));
		exit(1);
	}

	if((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, CHANNELS)) < 0) {
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

	*/
}