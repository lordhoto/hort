#include "window.h"

#include <cassert>

namespace GUI {

Window::Window(int x, int y, int w, int h, bool border) : _w(w), _h(h), _cursesWin(0), _hasBorder(border) {
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

