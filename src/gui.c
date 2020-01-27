/*
 * File Name: gui.c
 * Author: ckevar
 * Created: Jan 27, 2020 at 09:22AM
 */
#include "gui.h"
#include "definition.h"

#include <ncurses.h>

void computeGeometricalParams(){
	// Geometry for Container
	LEN_CONTAINER = COLS / 3;
	POSX_CONTAINER = COLS / 2 - LEN_CONTAINER / 2; 
	// Geometry for selector
	LEN_SELECTOR = LEN_CONTAINER / 3;
	POSX0_SELECTOR = POSX_CONTAINER + 1; 
	POSX1_SELECTOR = POSX_CONTAINER + LEN_CONTAINER / 2 - LEN_SELECTOR / 2; 
	POSX2_SELECTOR = POSX_CONTAINER + LEN_CONTAINER - LEN_SELECTOR - 1; 
	
	// Geometry for words
	POSX_STEREO = POSX_CONTAINER - 3; 
	POSX_FRONT = COLS / 2 - 2; 
	POSX_SURROUND = COLS / 2 + LEN_CONTAINER / 2 - 4; 

	if (LEN_CONTAINER < 25) {
		POSX_STEREO -= 1;
		POSX_SURROUND += 2; 
	}
}

void placeText(){	
	mvaddstr(0, COLS / 2 - 7, "Surround v0.0.1");
	mvaddstr(LINES / 4, COLS / 2 - 9, "Use TAB to toggle");
	mvaddstr(LINES / 2 + 3, POSX_STEREO, "Stereo");
	mvaddstr(LINES / 2 + 3, POSX_FRONT, "Front");
	mvaddstr(LINES / 2 + 3, POSX_SURROUND, "Surround");
	mvaddstr(LINES - 2, COLS - 7, "q:quit");
}

void initGui(){
	initscr();
	cbreak();
	noecho();
	// keypad(stdscr, TRUE);

	if(has_colors() == FALSE) {
		endwin();
		printf("your terminal does not support color\n");
	}

	// Init color
	start_color();
	init_pair(GUI_BACKGROUND, COLOR_YELLOW, COLOR_BLACK);
	init_pair(GUI_SELECTOR, COLOR_RED, COLOR_BLUE);

	computeGeometricalParams();

	// Windows creation
	selectorContainer = subwin(stdscr, 3, LEN_CONTAINER, LINES/2 - 1, POSX_CONTAINER);
	selector = newwin(1, LEN_SELECTOR, LINES/2, POSX0_SELECTOR);
	
	// stdscr features
	bkgd(COLOR_PAIR(GUI_BACKGROUND));
	box(stdscr, 0, 0);
	placeText();
	refresh();

	// selector container features
	wbkgd(selectorContainer, COLOR_PAIR(GUI_BACKGROUND));
	box(selectorContainer, 0, 0);
	wrefresh(selectorContainer);

	// selector features
	wbkgd(selector, COLOR_PAIR(GUI_SELECTOR));
	wrefresh(selector);	
}

void updateGui(){
	wbkgd(selectorContainer, COLOR_PAIR(GUI_BACKGROUND));
	wrefresh(selectorContainer);
	wbkgd(selector, COLOR_PAIR(GUI_SELECTOR));
	wrefresh(selector);	
}

void toogleSelectorGui(unsigned char t) {
	int POSX_SELECTOR;


	switch(t) {
		case 0: 
			POSX_SELECTOR = POSX0_SELECTOR;
			break;
		case 1:
			POSX_SELECTOR = POSX1_SELECTOR;
			break;
		case 2:
			POSX_SELECTOR = POSX2_SELECTOR;
	}

	mvwin(selector, LINES/2, POSX_SELECTOR);
	updateGui();
}

void killGui() {
	endwin();	
}