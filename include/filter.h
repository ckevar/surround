/*
 * File Name: filter.h
 * Created Jan 20, 10:35AM
 * Author: ckevar
 */
#ifndef FILTER_H
#define FILTER_H 

#include "fcomplex.h"

void loadFilters(complex *Hl, complex *Hr, complex *twiddle, short exp, unsigned short filterLenght);
void filterAudio(complex *inOut, complex *fil, complex *twiddle, short exp, short *overlap, unsigned short m);

#endif