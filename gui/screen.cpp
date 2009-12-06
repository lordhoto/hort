#include "screen.h"
#include "window.h"

#include <ncurses.h>

#include <algorithm>
#include <functional>
#include <string>

namespace GUI {

Screen *Screen::_instance = 0;

Screen::Screen() : _needRedraw(false), _windows() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

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

void Screen::update() {
	wnoutrefresh(stdscr);

	if (_needRedraw)
		std::for_each(_windows.begin(), _windows.end(), std::mem_fun(&Window::redraw));
	_needRedraw = false;

	std::for_each(_windows.begin(), _windows.end(), std::mem_fun(&Window::refresh));

	doupdate();
}

}

