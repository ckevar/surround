/*
 * 	File name: fft.h
 * 	From the book: "Real-Time Digital Signal Processing Implementation and Application, 2nd Ed" by Sen et al.
 * 	Created: Jan 16, 9:26 PM
 * 	Author: ckevarBased on "Real time 
 */
#ifndef FFT_H
#define FFT_H

#include "fcomplex.h"

#define L 	256			// Filter length
#define M 	769			// Data sample length
#define N 	1024 		// number of frequencies of the FFT, N >= L + M - 1;
#define EXP 10			// Exp = log2(N)

void createTwiddleTable(complex *twiddle, short exp); 		// creates twiddle table
void bit_rev(complex *X, short exp); 			// performs bit-reverse operation
void dft(complex *X, unsigned short exp, complex *W);
void idft(complex *X, unsigned short exp, complex *W);
void freqfilter(complex *x, complex *h); 		//	multiplication
void dftEvenOdd(complex *Xin, complex *Xout); 	// simple even-odd re-order
void olap_add(complex *x, short *o, unsigned short l, unsigned short m, unsigned short n);

#endif


/****************

*/