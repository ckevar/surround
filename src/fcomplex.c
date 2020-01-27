/*
 * File Name: fcomplex.c
 * Author: ckevar
 * Created: Jan 20, 2020 at 10:35AM
 */
#include "fcomplex.h"

// To cast real into complex
void double2complex(complex *cmplx, double *real, short sizeReal) {
	unsigned short i;
	for (i = 0; i < sizeReal; i++){
		cmplx[i].re = real[i] * 2.2;	// multiply by a addditional gain to the filter
		cmplx[i].im = 0.0;
	}
}

// to cast usigned short into complex
void uShort2complex(complex *cmplx, unsigned short *real, short sizeReal) {
	unsigned short i;
	for (i = 0; i < sizeReal; i++){
		cmplx[i].re = real[i] * 2;
		cmplx[i].im = 0.0;
	}
}

// to cast short into complex
void short2complex(complex *cmplx, short *real, short sizeReal) {
	unsigned short i;
	for (i = 0; i < sizeReal; i++){
		cmplx[i].re = real[i];
		cmplx[i].im = 0.0;
	}
}
