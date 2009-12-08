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

#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "defs.h"

#include <ncurses.h>

namespace GUI {

class Window {
friend class Screen;
public:
	Window(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool border = false);
	~Window();

	unsigned int offsetX() const { return _x; }
	unsigned int offsetY() const { return _y; }
	unsigned int width() const { return _w; }
	unsigned int height() const { return _h; }

	void printLine(const char *str, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);
	void printLine(const char *str, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);
	void printChar(int ch, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);

	void clear();
private:
	const unsigned int _x, _y, _w, _h;
	WINDOW *_cursesWin;
	bool _hasBorder;

	void refresh();
	void redraw();
};

}

#endif

