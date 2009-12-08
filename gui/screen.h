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

#ifndef GUI_SCREEN_H
#define GUI_SCREEN_H

#include "defs.h"
#include "window.h"

#include <list>

namespace GUI {

class Window;

class Screen {
public:
	~Screen();

	static Screen &instance();
	static void destroy();

	void clear();

	void add(Window *window);
	void remove(Window *window);

	void setCursor(unsigned int x, unsigned int y);
	void setCursor(const Window &win, unsigned int x, unsigned int y);

	void update();
private:
	Screen();
	static Screen *_instance;

	bool _needRedraw;
	unsigned int _curX, _curY;

	typedef std::list<Window *> WindowList;
	WindowList _windows;
};

}

#endif

