#include "screen.h"
#include "window.h"

#include <ncurses.h>

#include <algorithm>
#include <functional>
#include <string>
#include <cassert>
#include <stdlib.h>

namespace GUI {

Screen *Screen::_instance = 0;

Screen::Screen() : _needRedraw(false), _curX(0), _curY(0), _windows() {
	setenv("ESCDELAY", "10", 1);
	initscr();

	// TODO
	if (!has_colors())
		throw std::string("Your terminal misses color support");

	start_color();

	init_pair(kWhiteOnBlack, COLOR_WHITE, COLOR_BLACK);
	init_pair(kRedOnBlack, COLOR_RED, COLOR_BLACK);
	init_pair(kGreenOnBlack, COLOR_GREEN, COLOR_BLACK);
	init_pair(kYellowOnBlack, COLOR_YELLOW, COLOR_BLACK);
	init_pair(kBlueOnBlack, COLOR_BLUE, COLOR_BLACK);
	init_pair(kMagentaOnBlack, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(kCyanOnBlack, COLOR_CYAN, COLOR_BLACK);
	init_pair(kBlackOnWhite, COLOR_BLACK, COLOR_WHITE);

	attron(COLOR_PAIR(kWhiteOnBlack));

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	notimeout(stdscr, TRUE);
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

void Screen::clear() {
	::erase();
	_needRedraw = true;
}

void Screen::add(Window *window) {
	remove(window);
	_windows.push_back(window);
}

void Screen::remove(Window *window) {
	_windows.remove(window);
}

void Screen::setCursor(unsigned int x, unsigned int y) {
	assert(x <= 79);
	assert(y <= 23);
	_curX = x;
	_curY = y;
}

void Screen::update() {
	wnoutrefresh(stdscr);

	if (_needRedraw)
		std::for_each(_windows.begin(), _windows.end(), std::mem_fun(&Window::redraw));
	_needRedraw = false;

	std::for_each(_windows.begin(), _windows.end(), std::mem_fun(&Window::refresh));

	move(_curY, _curX);
	wnoutrefresh(stdscr);

	doupdate();
}

}

