/*
 *	File Name: gui.h
 *	Author: ckevar
 *	Created: Jan 26, 2020 at 10:03 PM
 */
#ifndef TYPES_H
#define TYPES_H 

#include "fcomplex.h"

typedef struct {
	complex *X;
	complex *Hs;
	complex *Hf;
	short *overlap;
	unsigned short offset;
} dataType;

#endif