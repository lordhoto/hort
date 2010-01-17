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

#include "defs.h"

namespace Game {

Base::Point getDirection(unsigned char dir) {
	switch (dir) {
	case 1:
		return Base::Point(-1, +1);

	case 2:
		return Base::Point( 0, +1);

	case 3:
		return Base::Point(+1, +1);
	
	case 4:
		return Base::Point(-1,  0);

	case 6:
		return Base::Point(+1,  0);

	case 7:
		return Base::Point(-1, -1);

	case 8:
		return Base::Point( 0, -1);

	case 9:
		return Base::Point(+1, -1);

	default:
		return Base::Point(0, 0);
	}
}

} // end of namespace Game

