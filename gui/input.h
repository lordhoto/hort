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

#ifndef GUI_INPUT_H
#define GUI_INPUT_H

#include "window.h"

#include <ncurses.h>
#include <string>

namespace GUI {

enum Keys {
	kKeyKeypad1 = '1',
	kKeyKeypad2 = '2',
	kKeyKeypad3 = '3',
	kKeyKeypad4 = '4',
	kKeyKeypad5 = '5',
	kKeyKeypad6 = '6',
	kKeyKeypad7 = '7',
	kKeyKeypad8 = '8',
	kKeyKeypad9 = '9',
	kKeyBackspace = KEY_BACKSPACE,
	kKeyReturn = 10,
	kKeyEscape = 27
};

enum Notifications {
	kNotifyResize = KEY_RESIZE
};

class Input {
public:
	/**
	 * Waits for the user to enter any key.
	 *
	 * This is either the ASCII representation of the key, one of 
	 * values defined via Keys or an unknown value in case the key
	 * pressed is not known.
	 *
	 * @see Keys
	 * @return User's input
	 */
	int poll();

	/**
	 * Reads a line from the user. This will allow for a terminal a-like
	 * input for the user. The users string will be started at (x, y)
	 * of the given window.
	 *
	 * Note that this function does overwrite the window's content at this
	 * position!
	 *
	 * @param win Window to use.
	 * @param x x coordinate of the window to place the string
	 * @param y y coordinate of the window to place the string
	 * @return the string the user entered
	 */
	const std::string getLine(Window &win, unsigned int x, unsigned int y);

	/**
	 * Returns the global input instance.
	 */
	static Input &instance();

	/**
	 * Destroies the global input instance.
	 */
	static void destroy();
private:
	static Input *_instance;
};

} // end of namespace GUI

#endif

