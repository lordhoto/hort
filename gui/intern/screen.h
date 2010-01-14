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

	/**
	 * Returns the global screen instance.
	 */
	static Screen &instance();

	/**
	 * Destroies the global screen instance.
	 */
	static void destroy();

	/**
	 * Clears all of the screen. This will not erase
	 * content of the currently added windows! Use
	 * update to refresh the user's screen.
	 *
	 * @see update
	 */
	void clear();

	/**
	 * Adds a window to the screen. This is used to
	 * allow the screen to redraw the window when
	 * needed.
	 *
	 * @param window
	 */
	void add(Window *window);

	/**
	 * Removes a window from the screen. Note that this
	 * will not free up the window's memory and this will
	 * not remove the window from the display. Use clear
	 * to clear the screen afterwards.
	 *
	 * @see clear
	 * @see update
	 */
	void remove(Window *window);

	/**
	 * Sets the terminal cursor to the specified position.
	 *
	 * @param x x coordinate (max 79)
	 * @param y y coordinate (max 24)
	 */
	void setCursor(unsigned int x, unsigned int y);

	/**
	 * Sets the cursor to a specific postion relative to the window given.
	 *
	 * @param win Window to use for positioning.
	 * @param x x coordinate (max is win.width() - 1)
	 * @param y y coordinate (max is win.height() - 1)
	 */
	void setCursor(const Window &win, unsigned int x, unsigned int y);

	/**
	 * Queries the cursor coordinates.
	 */
	void getCursor(unsigned int &x, unsigned int &y);

	/**
	 * Updates all window's changed content. This will
	 * also redraw all windows, when clear had been called.
	 *
	 * @see clear
	 */
	void update();

	/**
	 * Queries the width of the screen.
	 *
	 * @return width.
	 */
	unsigned int width() const { return COLS; }

	/**
	 * Queries the height of the screen.
	 *
	 * @return height.
	 */
	unsigned int height() const { return LINES; }
private:
	Screen();
	static Screen *_instance;

	bool _needRedraw;
	unsigned int _curX, _curY;

	typedef std::list<Window *> WindowList;
	WindowList _windows;
};

} // end of namespace GUI

#endif

