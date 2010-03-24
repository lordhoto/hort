/* Hort - A roguelike inspired by the Nibelungenlied
 *
 * (c) 2009-2010 by Johannes Schickel <lordhoto at scummvm dot org>
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

#include "screen.h"
#include "window.h"

#include <ncurses.h>

#include <algorithm>
#include <functional>
#include <string>
#include <cassert>
#include <stdlib.h>

namespace GUI {
namespace Intern {

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
	refresh();
	_needRedraw = true;
}

void Screen::add(Window *window) {
	remove(window);
	_windows.push_back(window);
	_needRedraw = true;
}

void Screen::remove(Window *window) {
	_windows.remove(window);
	_needRedraw = true;
}

void Screen::setCursor(unsigned int x, unsigned int y) {
	assert(x < width());
	assert(y < height());
	_curX = x;
	_curY = y;
}

void Screen::setCursor(const Window &win, unsigned int x, unsigned int y) {
	assert(x < win.getWidth());
	assert(y < win.getHeight());
	setCursor(x + win.getOffsetX(), y + win.getOffsetY());
}

void Screen::getCursor(unsigned int &x, unsigned int &y) {
	x = _curX;
	y = _curY;
}

void Screen::update() {
	std::for_each(_windows.begin(), _windows.end(), std::bind2nd(std::mem_fun(&Window::redraw), _needRedraw));
	_needRedraw = false;
	move(_curY, _curX);

	// According to the ncurses manpage we should need to call refresh
	// here to update the screen. Somehow that is not needed though,
	// if any curses implementation requires that, just uncomment this
	// line.
	//refresh();
}

} // end of namespace Intern
} // end of namespace GUI

