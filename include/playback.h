/*
 *	File Name: playback.h
 *	Author: ckevar
 *	Created: Dec 11, 2019 at 11:04 AM
 */
#include <alsa/asoundlib.h>
#include <libintl.h>


#ifndef PLAYBACK_H
#define PLAYBACK_H

typedef struct {
	char name[10];
	unsigned samplerate;
	unsigned channels;
	long unsigned frames;
} PlaybackHW;

PlaybackHW hwpb;
int err;

void loadPBSettings(const char name[], const unsigned sr, const unsigned c, const unsigned f);
snd_pcm_t * playbackSetup();
int playback(snd_pcm_t *h, short *b, const size_t f);

#define _(msgid) gettext (msgid)

#define	timersub(a, b, result) \
do { \
	(result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
	(result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
	if ((result)->tv_usec < 0) { \
		--(result)->tv_sec; \
		(result)->tv_usec += 1000000; \
	} \
} while (0)

#endif