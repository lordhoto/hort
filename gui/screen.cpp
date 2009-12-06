#include "screen.h"

#include <ncurses.h>

namespace GUI {

Screen *Screen::_instance = 0;

Screen::Screen() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}

Screen::~Screen() {
	endwin();
}

Screen &Screen::instance() {
	if (!_instance)
		_instance = new Screen;
	return *_instance;
}

void Screen::destroy() {
	delete _instance;
	_instance = 0;
}

}

