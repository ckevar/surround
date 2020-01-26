/*
 * File Name: fcomplex.c
 * Created Jan 20, 10:35AM
 * Author: ckevar
 */
#include "fcomplex.h"

// To cast real into complex
void double2complex(complex *cmplx, double *real, short sizeReal) {
	unsigned short i;
	for (i = 0; i < sizeReal; i++){
		cmplx[i].re = real[i] * 2.5;
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
