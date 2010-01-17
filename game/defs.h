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

#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include <stdint.h>

#include "base/geo.h"

#include "gui/defs.h"

namespace Game {

typedef uint32_t TickCount;

enum {
	/**
	 * How many internal ticks are processed, before
	 * the turn counter increases.
	 */
	kTicksPerTurn = 10
};

/**
 * Returns the direction pointer of the given direction.
 *
 * @param dir Direction (must be inside [1, 9])
 * @return Direction pointer.
 */
Base::Point getDirection(unsigned char dir);

/**
 * Returns the direction pointer of the given direction.
 *
 * @param dir Direction (must be one of kInputDir1 to kInputDir9)
 * @return Direction pointer.
 */
Base::Point getDirection(GUI::Input dir);

} // end of namespace Game

#endif

