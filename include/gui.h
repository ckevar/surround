/*
 *	File Name: gui.h
 *	Author: ckevar
 *	Created: Jan 27, 2020 at 09:09 AM
 */
#ifndef GUI_H
#define GUI_H 

#include <ncurses.h>

// Windows
WINDOW *selectorContainer;
WINDOW *selector;

// Geometrical features
int LEN_CONTAINER, POSX_CONTAINER;
int LEN_SELECTOR, POSX0_SELECTOR, POSX1_SELECTOR, POSX2_SELECTOR; 
int POSX_STEREO, POSX_FRONT, POSX_SURROUND; 

void initGui();		// init interface
void refreshGui();	// updates screen
void killGui(); 	// deallocate used memory

void toogleSelectorGui(unsigned char t);

#endif