/*
 * 	File name: fcomplex.h
 * 	Created: Jan 16, 2020 at 9:54 PM
 * 	Author: ckevar 
 * 	Reference: "Real-Time Digital Signal Processing Implementation and Application, 2nd Ed" by Sen et al.
 */

#ifndef F_COMPLEX_H
#define F_COMPLEX_H 

struct cmplx {
	double re;
	double im;
};

typedef struct cmplx complex;

// utilities
void double2complex(complex *cmplx, double *real, short sizeReal);
void uShort2complex(complex *cmplx, unsigned short *real, short sizeReal);
void short2complex(complex *cmplx, short *real, short sizeReal);

#endif
