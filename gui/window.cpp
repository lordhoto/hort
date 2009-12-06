#include "window.h"

#include <cassert>
#include <cstring>

namespace GUI {

Window::Window(int x, int y, int w, int h, bool border)
    : _w(border ? w - 2 : w),
      _h(border ? h - 2 : h),
      _cursesWin(0),
      _hasBorder(border) {
	assert(x >= 0);
	assert(y >= 0);
	assert(x + w <= 80);
	assert(y + h <= 24);

	_cursesWin = newwin(h, w, y, x);
	clear();
}

Window::~Window() {
	delwin(_cursesWin);
	_cursesWin = 0;
}

void Window::printChar(char ch, int x, int y) {
	if (_hasBorder)
		mvwaddch(_cursesWin, y + 1, x + 1, ch);
	else
		mvwaddch(_cursesWin, y, x, ch);
}

void Window::printLine(const char *str) {
	printLine(str, (_w - std::strlen(str)) / 2, _h / 2);
}

void Window::printLine(const char *str, int x, int y) {
	if (y >= _h || x >= _w)
		return;

	while (x < 0) {
		++str;
		++x;
	}

	for (; x < _w && *str; ++str, ++x)
		printChar(*str, x, y);
}

void Window::clear() {
	wclear(_cursesWin);
	if (_hasBorder)
		wborder(_cursesWin, '|', '|', '-', '-', '+', '+', '+', '+');
}

void Window::redraw() {
	redrawwin(_cursesWin);
}

void Window::refresh() {
	wnoutrefresh(_cursesWin);
}

}

