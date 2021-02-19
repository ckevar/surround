/* 
 * File Name: playback.c
 * Author: ckevar
 * Created: Dec 11, 2019 at 09:33AM
 */

#include <string.h>
#include <sys/time.h>
#include "playback.h"

void loadPBSettings(const char name[], const unsigned sr, const unsigned c, const unsigned f) {
	strcpy(hwpb.name, name);
	hwpb.samplerate = sr;
	hwpb.channels = c;
	hwpb.frames = f;
}

snd_pcm_t * playbackSetup() {
	snd_pcm_hw_params_t *hw_params;
	static snd_pcm_t *pb_handle;

	if ((err = snd_pcm_open(&pb_handle, hwpb.name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		fprintf(stderr, "[ERROR:] cannot open audio device %s (%s)\n", hwpb.name, snd_strerror(err));
		exit(1);
	}
// the same as capture
	if((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		fprintf(stderr, "[ERROR:] cannot allocate harware (%s)\n", snd_strerror(err));
		exit(1);
	}
// capture
	if((err = snd_pcm_hw_params_any(pb_handle, hw_params)) < 0) {
		fprintf(stderr, "[ERROR:] cannot initialize harware parameter structure (%s)\n", snd_strerror(err));
		exit(1);
	}
// capture
	if ((err = snd_pcm_hw_params_set_access(pb_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set access type (%s)\n", snd_strerror(err));
		exit(1);
	}
// capture
	if ((err = snd_pcm_hw_params_set_format(pb_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set format tyoe (%s)\n", snd_strerror(err));
		exit(1);
	}
//capture
	if((err = snd_pcm_hw_params_set_rate_near(pb_handle, hw_params, &hwpb.samplerate, 0)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set sample rate (%s)\n", snd_strerror(err));
		exit(1);
	}
//capture
	if ((err = snd_pcm_hw_params_set_channels(pb_handle, hw_params, hwpb.channels)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set channel count (%s) \n", snd_strerror(err));
		exit(1);
	}
//different
	if((err = snd_pcm_hw_params_set_buffer_size_near(pb_handle, hw_params, &hwpb.frames)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set buffer near %lu (%s)\n", hwpb.frames, snd_strerror(err));
		exit(1);
	}
	printf("got actual buffer size: %lu\n", hwpb.frames);

	if((err = snd_pcm_hw_params(pb_handle, hw_params)) < 0) {
		fprintf(stderr, "[ERROR:] cannot set parameters (%s)\n", snd_strerror(err));
		exit(1);
	}

	snd_pcm_hw_params_free(hw_params);

	if((err = snd_pcm_prepare(pb_handle)) < 0) {
		fprintf(stderr, "[ERROR:] cannot prepare audio interface for use (%s)\n", snd_strerror(err));
		exit(1);
	}
	return pb_handle;
}

static void xrun(snd_pcm_t *handle) {
	// copied from aplay
	snd_pcm_status_t *status;
	int resInt;

	snd_pcm_status_alloca(&status);
	if ((resInt = snd_pcm_status(handle, status))<0) {
		// fprintf(stderr, "[ERROR:] status (%s)\n", snd_strerror(resInt));
		exit(EXIT_FAILURE);
	}
	if (snd_pcm_status_get_state(status) == SND_PCM_STATE_XRUN) {
		struct timeval now, diff, tstamp;
		gettimeofday(&now, 0);
		snd_pcm_status_get_trigger_tstamp(status, &tstamp);
		timersub(&now, &tstamp, &diff);

		if ((resInt = snd_pcm_prepare(handle))<0) {
			// fprintf(stderr, "[ERROR:] prepare (%s)\n", snd_strerror(resInt));
			exit(EXIT_FAILURE);
		}
		return;		/* ok, data should be accepted again */
	} 
	exit(EXIT_FAILURE);
}

long int sta = 0;

int playback(snd_pcm_t *h, short *b, const size_t f) {
	static snd_pcm_sframes_t res;
	size_t localf = f;
	short *b_tmp = b;

	while(localf > 0) {
		res = snd_pcm_writei(h, b_tmp, localf);

		if(res == -EAGAIN) {
			// fprintf(stderr, "[ERROR:] Try it again (%s)\n", snd_strerror(res));
			return 1;
		}
		else if (res == -EPIPE) {
			// fprintf(stderr, "[ERROR:] EPIPE\n");
			sta++;
			xrun(h);
		}
		else if (res == -ESTRPIPE) {
			// fprintf(stderr, "[ERROR:] ESTRPIPE (%s)\n", snd_strerror(res));
			return 1;
		} else 	if (res < 0) {
			// fprintf(stderr, "[ERROR:] Unknown error (%s)\n", snd_strerror(res));
			snd_pcm_prepare(h);
		}
		
		if (res > 0) {
			b_tmp += res * hwpb.channels;
			localf -= res;
		}
	}
	return 0;
}
