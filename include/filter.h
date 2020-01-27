/*
 * File Name: filter.h
 * Created Jan 20, 10:35AM
 * Author: ckevar
 */
#ifndef FILTER_H
#define FILTER_H 

#include "fcomplex.h"
#include "definition.h"

// extern complex HSurroL[N];
// extern complex HSurroR[N];
// extern complex HFrontL[N];
// extern complex HFrontR[N];

void initFFT(short exp);
void loadFiltersFront(complex *Hl, complex *Hr, short exp, unsigned short filterLenght);
void loadFiltersSurr(complex *Hl, complex *Hr, short exp, unsigned short filterLenght);
void filterAudio(complex *inOut, complex *fil, short exp, short *overlap, unsigned short m, unsigned char enable);

#endif