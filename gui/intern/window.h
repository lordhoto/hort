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

#ifndef GUI_INTERN_WINDOW_H
#define GUI_INTERN_WINDOW_H

#include "gui/defs.h"

#include <ncurses.h>

namespace GUI {
namespace Intern {

class Window {
friend class Screen;
public:
	/**
	 * Creates a window with the given postion and size.
	 * The right most coordinate is (Screen::width() - 1, y).
	 * The down most coordinate is (x, Screen::height() - 1).
	 * This results in the max width being Screen::width() and the max height being Screen::height().
	 *
	 * @param x The x coordinate of the new window.
	 * @param y The y cooridnate of the new window.
	 * @param w The width of the new window.
	 * @param h The height of the new window.
	 * @param border Whether the window should have a border (this effictivly reduces the width and height by 2).
	 */
	Window(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool border = false);
	~Window();

	/**
	 * Returns the x offset of right most column, which is accessable to the user.
	 * @return x offset in real terminca coordinates.
	 */
	unsigned int getOffsetX() const { return _x; }

	/**
	 * Returns the y offset of the top most line, which is accessable to the user.
	 * @return y offset in real terminal coordinates.
	 */
	unsigned int getOffsetY() const { return _y; }

	/**
	 * Returns the width of the window, excluding the border size.
	 * @return width
	 */
	unsigned int getWidth() const { return _w; }

	/**
	 * Returns the height of the window, excluding border.
	 * @return height
	 */
	unsigned int getHeight() const { return _h; }

	/**
	 * Prints a centered string in the window.
	 *
	 * @param str The string to print.
	 * @param color Color of the string.
	 * @param attrib Output attributes.
	 * @see GUI::Attributes
	 */
	void printLine(const char *str, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);

	/**
	 * Prints a string at the specified position in the window.
	 *
	 * @param str The string to print.
	 * @param x x coordinate of the string.
	 * @param y y coordinate of the string.
	 * @param color Color of the string.
	 * @param attrib Output attributes.
	 * @see GUI::Attributes
	 */
	void printLine(const char *str, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);

	/**
	 * Prints a specific character at user specified position in the window.
	 *
	 * @param ch Character to print.
	 * @param x x coordinate of the character.
	 * @param y y coordinate of the character.
	 * @param color Color of the character.
	 * @param attrib Output attributes.
	 * @see GUI::Attributes
	 */
	void printChar(chtype ch, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);

	/**
	 * Returns the character data for the given character and it's attributes.
	 *
	 * @param ch Character.
	 * @param color Color of the character.
	 * @param attrib Output attributes.
	 * @see GUI::Attributes
	 */
	static chtype getCharData(chtype ch, ColorPair color, int attrib = kAttribNormal) { return ch | COLOR_PAIR(color) | (chtype)attrib; }

	/**
	 * Replaces a given part of the screen with the given data.
	 *
	 * @param x x coordinate of the rect
	 * @param y y coordinate of the rect
	 * @param width width of the rect (max is width())
	 * @param height height of the rect (max is height())
	 * @param data data to put
	 * @param pitch pitch of the data
	 */
	void putData(unsigned int x, unsigned int y, unsigned int width, unsigned int height, const chtype *data, unsigned int pitch);

	/**
	 * Clears the window. This will not remove the window's border!
	 */
	void clear();
private:
	const unsigned int _x, _y, _w, _h;
	const unsigned int _rX, _rY, _rW, _rH;
	chtype *_content;

	bool _hasBorder;

	bool _needsRefresh;
	void redraw(bool force);
};

} // end of namespace Intern
} // end of namespace Game

#endif

