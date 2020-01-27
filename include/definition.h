/*
 *	File Name: definition.h
 *	Author: ckevar
 *	Created: Jan 26, 2020 at 10:17 PM
 *	Reference: "Real-Time Digital Signal Processing Implementation and Application, 2nd Ed" by Sen et al.
 */
#ifndef DEFINITION_H
#define DEFINITION_H 

// General params
#define FREQUENCY 	44100
#define FRAMES 		1024
#define CHANNELS 	2

// Alsa devices
#define CAPTURE_DEVICE "hw:2,1"
#define PLAYBACK_DEVICE "hw:0,0"

// FFT params
#define L 	256			// Filter length
#define M 	769			// Data sample length
#define N 	1024 		// number of frequencies of the FFT, N >= L + M - 1;
#define EXP 10			// Exp = log2(N)

// Interface color pairs
#define GUI_BACKGROUND	1
#define GUI_SELECTOR 	2

#endif