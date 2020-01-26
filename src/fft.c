/*
 *	Name: fft.c
 *	From the book: "Real-Time Digital Signal Processing Implementation and Application, 2nd Ed" by Sen et al.
 *	Created: Jan 16, 9:26 PM
 *	Author: ckevar
 */

#include "fft.h"
#include "fcomplex.h"

#include <math.h>
#include <stdio.h>

void createTwiddleTable(complex *twiddle, short exp) {
	complex U, W;
	complex temp;
	unsigned short j, L1, LE1;
	unsigned short i = 0;
	double LE;
	
	// LE1 = 1;
	// for (L1 = 0; L1 < exp; L1++) {
	// 	LE = M_PI / LE1;
	// 	W.re =  (cos(LE));
	// 	W.im = -(sin(LE));

	// 	U.re = 1.0;
	// 	U.im = 0.;

	// 	for (j = 0; j < LE1; j++) {
	// 		twiddle[i].re = U.re;
	// 		twiddle[i].im = U.im;
	// 		// Recursively compute W^k as U*W^(k-1)
	// 		printf("%f %f\n", temp.re, temp.im);
	// 		temp.re = U.re * W.re - U.im * W.im;
	// 		U.im    = U.re * W.im + U.im * W.re;
	// 		U.re = temp.re;
	// 		i++;
	// 	}
	// 	LE1 <<= 1;
	// }

	LE1 = 1;
	for (L1 = 0; L1 < exp; L1++) {
		LE = M_PI / LE1;
		// U.re = 1.0;
		// U.im = 0.;

		for (j = 0; j < LE1; j++) {
			// W.re = (short)(0x7fff * cos(j * LE) + 0.5);
			// W.im =-(short)(0x7fff * sin(j * LE) + 0.5);
			W.re = (cos(j * LE));
			W.im =-(sin(j * LE));
			// printf("%f %f\n", W.re, W.im);
			twiddle[i] = W;
			i++;
		}
		LE1 <<= 1;
	}

}

void bit_rev(complex *X, short exp) {
	unsigned short i, j, k;
	// unsigned short N = 1 << EXP;			// Number of points for FFT
	unsigned short N2 = N >> 1;
	complex temp;
	for (j = 0, i = 1; i < N - 1; i++) {
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
	unsigned short id;		// index
	// unsigned short N = 1 << exp; 	// Number of points of the FFT
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
			// pTmp2 = X + i + LE1; // X[i + LE1]

			for (i = j; i < N; i += LE) { // Do the butterflies
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
	unsigned short id;		// index
	// unsigned short N = 1 << exp; 	// Number of points of the FFT
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
			// pTmp2 = X + i + LE1;

			for (i = j; i < N; i += LE) { // Do the butterflies
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


void freqfilter(complex *x, complex *h) {
	unsigned short i;
	double AC0, AC1;
	// (a + ib) * (c + id);
	// a*c + iad + ibc + iibd;
	// a*c - bd + i(ad+bc)
	for (i = 0; i < N; i++) {
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
	// for (i = 0; i < l - 1; i++)
	// 	x[i].re += o[i];
	// for(k = 0, i = m; i < n; i++) 
	// 	o[k++] = x[i].re;	
	
	for (k = n - 1, i = 0; i < l - 1; i++) 
		x[k--].re += o[i];

	for(i = 0, k = n - 1 - m; k >= 0; k--) 
		o[i++] = x[k].re;
}


void dftEvenOdd(complex *Xin, complex *Xout) {
	short i, n, k, j;
	double angle;
	complex X[N];
	double W[2];
	for (i = 0, k = 0; k < N; k ++){
		X[k].re = 0;
		X[k].im = 0;
		// printf("------------------\n");
		for (j = 0, n = 0; n < N; n ++){
			angle = (2.0*M_PI*k*n) / N;
			W[0] = cos(angle);
			W[1] = -sin(angle);
			// printf("%f %f\n", W[0], W[1]);
			X[k].re = X[k].re + (Xin[j].re * W[0] + Xin[j].im * W[1]) / 32767.0;
			X[k].im = X[k].im + (Xin[j].im * W[0] + Xin[j].re * W[1]) / 32767.0;
			j += 2;
		}
		Xout[i].re = (short)(X[k].re * 32767.0 + 0.5);
		Xout[i++].im = (short)(X[k].im * 32767.0 + 0.5);
	}
}