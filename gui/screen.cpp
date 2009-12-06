#include "screen.h"
#include "window.h"

#include <ncurses.h>

#include <algorithm>
#include <functional>

namespace GUI {

Screen *Screen::_instance = 0;

Screen::Screen() : _needRedraw(false), _windows() {
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

