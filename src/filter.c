/*
 * File Name: filter.c
 * Created Jan 20, 10:50AM
 * Author: ckevar
 */
#include "filter.h"
#include "fft.h"

#include <stdio.h>

void loadFilters(complex *Hl, complex *Hr, complex *twiddle, short exp, unsigned short filterLenght){
	unsigned short i;
	double hl[L] = {
		#include "leftSurround.dat"		// load time domain coefficients
	};

	double hr[L] = {
		#include "rightSurround.dat"		// load time domain coefficients
	};

	double2complex(Hl, hl, filterLenght);
	double2complex(Hr, hr, filterLenght);

	// Zero padding
	for(i = filterLenght; i < N; i++) {	
		Hl[i].re = 0.0;
		Hl[i].im = 0.0;

		Hr[i].re = 0.0;
		Hr[i].im = 0.0;
	}

	// left channel
	bit_rev(Hl, exp);		// arrenge H in a bit-reverse order
	dft(Hl, exp, twiddle);	// perform fft

	// right channel
	bit_rev(Hr, exp);		// arrenge H in a bit-reverse order
	dft(Hr, exp, twiddle);	// perform fft
}

void filterAudio(complex *inOut, complex *fil, complex *twiddle, short exp, short *overlap, unsigned short m) {
	unsigned short n = 1 << exp;

	// Start fast  convolution
	// FFT
	bit_rev(inOut, exp);
	dft(inOut, exp, twiddle);
	// Filter
	freqfilter(inOut, fil);
	
	// IFFT
	bit_rev(inOut, exp);
	idft(inOut, exp, twiddle);

	// overla for circular convolution
	olap_add(inOut, overlap, L, m, N);
}