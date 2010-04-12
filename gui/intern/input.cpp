/* Hort - A roguelike inspired by the Nibelungenlied
 *
 * (c) 2009-2010 by Johannes Schickel <lordhoto at scummvm dot org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
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

#include "input.h"
#include "screen.h"

#include <cctype>

#include <boost/numeric/conversion/cast.hpp>

namespace GUI {
namespace Intern {

Input *Input::_instance = 0;

int Input::poll() {
	refresh();
	return wgetch(stdscr);
}

const std::string Input::getLine(Window &win, unsigned int x, unsigned int y) {
	if (x >= win.getWidth() || y >= win.getHeight())
		return std::string();

	int oX, oY;
	getyx(stdscr, oY, oX);
	Screen &scr = Screen::instance();

	std::string line;

	int input = -1;
	while (input != kKeyReturn) {
		scr.setCursor(win, x, y);
		scr.update();
		input = poll();

		if (x + 1 < win.getWidth() && std::isprint(input)) {
			try {
				line += boost::numeric_cast<char>(input);
				win.printChar(input, x, y);
				++x;
			} catch (boost::numeric::bad_numeric_cast &) {
				// In case the input character does not fit
				// into a "char", we just ignore it.
			}
		} else if (input == kKeyBackspace && !line.empty()) {
			--x;
			win.printChar(' ', x, y);
			line.erase(line.size() - 1);
		}
	}

	scr.setCursor(oX, oY);

	return line;
}

Input &Input::instance() {
	if (!_instance)
		_instance = new Input();
	return *_instance;
}

void Input::destroy() {
	delete _instance;
	_instance = 0;
}

} // end of namespace Intern
} // end of namespace GUI

