#include "window.h"

#include <cassert>
#include <cstring>
#include <algorithm>

namespace GUI {

Window::Window(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool border)
    : _w(border ? w - 2 : w),
      _h(border ? h - 2 : h),
      _cursesWin(0),
      _hasBorder(border) {
	assert(x + w <= 80);
	assert(y + h <= 24);

	_cursesWin = newwin(h, w, y, x);
	clear();
}

Window::~Window() {
	delwin(_cursesWin);
	_cursesWin = 0;
}

void Window::printChar(int ch, unsigned int x, unsigned int y, ColorPair color) {
	if (_hasBorder)
		mvwaddch(_cursesWin, y + 1, x + 1, ch | COLOR_PAIR(color));
	else
		mvwaddch(_cursesWin, y, x, ch | COLOR_PAIR(color));
}

void Window::printLine(const char *str, ColorPair color) {
	const size_t strLength = std::max<size_t>(_w, std::strlen(str));
	printLine(str, (_w - strLength) / 2, _h / 2, color);
}

void Window::printLine(const char *str, unsigned int x, unsigned int y, ColorPair color) {
	if (y >= _h || x >= _w)
		return;

	for (; x < _w && *str; ++str, ++x)
		printChar(*str, x, y, color);
}

void Window::clear() {
	wclear(_cursesWin);
	if (_hasBorder)
		wborder(_cursesWin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
}

void Window::redraw() {
	redrawwin(_cursesWin);
}

void Window::refresh() {
	wnoutrefresh(_cursesWin);
}

}

