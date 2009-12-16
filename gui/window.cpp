/* Hort - A roguelike inspired by the Nibelungenlied
 *
 * (c) 2009 by Johannes Schickel <lordhoto at scummvm dot org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "window.h"

#include <cassert>
#include <cstring>
#include <algorithm>

namespace GUI {

Window::Window(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool border)
    : _x(border ? x + 1 : x),
      _y(border ? y + 1 : y),
      _w(border ? w - 2 : w),
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

void Window::printChar(int ch, unsigned int x, unsigned int y, ColorPair color, int attrib) {
	if (_hasBorder)
		mvwaddch(_cursesWin, y + 1, x + 1, ch | COLOR_PAIR(color) | attrib);
	else
		mvwaddch(_cursesWin, y, x, ch | COLOR_PAIR(color) | attrib);
}

void Window::printLine(const char *str, ColorPair color, int attrib) {
	const size_t strLength = std::min<size_t>(_w, std::strlen(str));
	printLine(str, (_w - strLength) / 2, _h / 2, color, attrib);
}

void Window::printLine(const char *str, unsigned int x, unsigned int y, ColorPair color, int attrib) {
	if (y >= _h || x >= _w)
		return;

	for (; x < _w && *str; ++str, ++x)
		printChar(*str, x, y, color, attrib);
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

} // end of namespace GUI

