/*
 * File Name: filter.c
 * Author: ckevar
 * Created: Jan 20, 2020 at 10:50AM
 */
#include "filter.h"
#include "fft.h"
#include "definition.h"

#include <stdio.h>

complex W[N - 1];				// Twiddle e^(-j2*pi/N) table;

void initFFT(short exp) {
	createTwiddleTable(W, exp);
}

void filter2Complex(complex *Hl, complex *Hr, double *hl, double *hr, unsigned short filterLenght) {
	unsigned short i;
	double2complex(Hr, hr, filterLenght);
	double2complex(Hl, hl, filterLenght);
	// Zero padding
	for(i = filterLenght; i < N; i++) {	
		Hr[i].re = 0.0;
		Hr[i].im = 0.0;

		Hl[i].re = 0.0;
		Hl[i].im = 0.0;
	}

}

void filterLRTime2Freq(complex *Hl, complex *Hr, short exp) {
	// left channel
	bit_rev(Hl, exp);	// arrenge H in a bit-reverse order
	dft(Hl, exp, W);	// perform fft

	// right channel
	bit_rev(Hr, exp);	// arrenge H in a bit-reverse order
	dft(Hr, exp, W);	// perform fft
}

void loadFiltersFront(complex *Hl, complex *Hr, short exp, unsigned short filterLenght){
	double hl[L] = {
		#include "leftFront.dat"		// load time domain coefficients
	};

	double hr[L] = {
		#include "rightSurround.dat"		// load time domain coefficients
	};

	filter2Complex(Hl, Hr, hl, hr, filterLenght);
	filterLRTime2Freq(Hl, Hr, exp);

}

void loadFiltersSurr(complex *Hl, complex *Hr, short exp, unsigned short filterLenght){
	double hl[L] = {
		#include "leftSurround.dat"		// load time domain coefficients
	};

	double hr[L] = {
		#include "rightSurround.dat"		// load time domain coefficients
	};

	filter2Complex(Hl, Hr, hl, hr, filterLenght);
	filterLRTime2Freq(Hl, Hr, exp);
}

void filterAudio(complex *inOut, complex *fil, short exp, short *overlap, unsigned short m, unsigned char enable) {
	unsigned short n = 1 << exp;

	// Start fast  convolution
	// FFT
	bit_rev(inOut, exp);
	dft(inOut, exp, W);
	// Filter
	if (enable)
		freqfilter(inOut, fil, n);
	
	// IFFT
	bit_rev(inOut, exp);
	idft(inOut, exp, W);

	// overla for circular convolution
	olap_add(inOut, overlap, L, m, n);
}