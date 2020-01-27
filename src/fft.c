/*
 *	File Name: fft.c
 *	Author: ckevar
 *	Created: Jan 16, 2020 at 9:26 PM
 *	Reference: "Real-Time Digital Signal Processing Implementation and Application, 2nd Ed" by Sen et al.
 */

#include "fft.h"
#include "fcomplex.h"

#include <math.h>
#include <stdio.h>

void createTwiddleTable(complex *twiddle, short exp) {
	complex W;
	unsigned short j, L1, LE1;
	unsigned short i = 0;
	double LE;
	
	LE1 = 1;
	for (L1 = 0; L1 < exp; L1++) {
		LE = M_PI / LE1;

		for (j = 0; j < LE1; j++) {
			W.re = (cos(j * LE));
			W.im =-(sin(j * LE));
			twiddle[i] = W;
			i++;
		}
		LE1 <<= 1;
	}

}

void bit_rev(complex *X, short exp) {
	unsigned short i, j, k;
	unsigned short n = 1 << exp;			// Number of points for FFT
	unsigned short N2 = n >> 1;
	complex temp;
	for (j = 0, i = 1; i < n - 1; i++) {
		k = N2;
		while( k <= j) {
			j -= k;
			k >>= 1;
		}
		j += k;
		if(i <= j) {
			temp = X[j];
			X[j] = X[i];
			X[i] = temp;
		}
	}
}

void idft(complex *X, unsigned short exp, complex *W) {
	complex temp;			// temporary storage 
	complex U;				// Twiddle factor W^k
	complex *pTmp;		
	complex *pTmp2;
	unsigned short i, j, k;	//
	unsigned short n = 1 << exp; 	// Number of points of the FFT
	unsigned short L1;		// FFT stage
	unsigned short LE;		// Number of points in sub DFT at stage L
							// and offset to next DFT in stage
	unsigned short LE1;		// Number of butterflies in one DFT at
							// stage L. Also is offset to lower point 
							// in butterfly at stage L

	LE1 = 1;
	k = 0;
	i = 0;

	for (L1 = 0; L1 < exp; L1++) { 	// FFT butterfly
		LE = LE1 << 1;
		for (j = 0; j < LE1; j++) {
			
			U.re = W[k].re;
			U.im = W[k].im;

			for (i = j; i < n; i += LE) { // Do the butterflies
				pTmp = X + i;	// X[i] = X[idx[i]]
				pTmp2 = pTmp + LE1; // X[i + LE1]

				temp.re = (pTmp2->re * U.re - pTmp2->im * U.im);
				temp.im = (pTmp2->im * U.re + pTmp2->re * U.im);

				pTmp2->re = (pTmp->re - temp.re);
				pTmp2->im = (pTmp->im - temp.im);
						
				pTmp->re = (pTmp->re + temp.re);
				pTmp->im = (pTmp->im + temp.im);

			}
			k++;
		}
		LE1 <<= 1;
	}
}

void dft(complex *X, unsigned short exp, complex *W) {
	complex temp;			// temporary storage 
	complex U;				// Twiddle factor W^k
	complex *pTmp;		
	complex *pTmp2;
	unsigned short i, j, k;	//
	unsigned short n = 1 << exp; 	// Number of points of the FFT
	unsigned short L1;		// FFT stage
	unsigned short LE;		// Number of points in sub DFT at stage L
							// and offset to next DFT in stage
	unsigned short LE1;		// Number of butterflies in one DFT at
							// stage L. Also is offset to lower point 
							// in butterfly at stage L

	LE1 = 1;
	k = 0;
	i = 0;

	for (L1 = 0; L1 < exp; L1++) { 	// FFT butterfly
		LE = LE1 << 1;
		for (j = 0; j < LE1; j++) {
			
			U.re = W[k].re;
			U.im = W[k].im;

			for (i = j; i < n; i += LE) { // Do the butterflies
				pTmp = X + i;	// X[i] = X[idx[i]]
				pTmp2 = pTmp + LE1; // X[i + LE1]

				temp.re = (pTmp2->re * U.re - pTmp2->im * U.im);
				temp.im = (pTmp2->im * U.re + pTmp2->re * U.im);

				pTmp2->re = (pTmp->re - temp.re) * 0.5;
				pTmp2->im = (pTmp->im - temp.im) * 0.5;
						
				pTmp->re = (pTmp->re + temp.re) * 0.5;
				pTmp->im = (pTmp->im + temp.im) * 0.5;

			}
			k++;
		}
		LE1 <<= 1;
	}
}


void freqfilter(complex *x, complex *h, unsigned short n) {
	unsigned short i;
	double AC0, AC1;
	for (i = 0; i < n; i++) {
		AC1 = x[i].re * h[i].re - x[i].im * h[i].im;
		AC0 = x[i].re * h[i].im + x[i].im * h[i].re;
		x[i].re = AC1;
		x[i].im = AC0;
	}
}

void olap_add(complex *x, short *o, unsigned short l, unsigned short m, unsigned short n) {
	/*
	 * l: length of filter
	 * m: length of actual data on buffer
	 * n: lenght of fft
	 */
	short k, i;
	
	for (k = n - 1, i = 0; i < l - 1; i++) 
		x[k--].re += o[i];

	for(i = 0, k = n - 1 - m; k >= 0; k--) 
		o[i++] = x[k].re;
}
