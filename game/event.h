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

#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "monster.h"

namespace Game {

struct Event {
	enum kType {
		kTypeMove,
		kTypeAttack
	};

	kType type;

	union {
		struct Move {
			const Monster *monster;

			unsigned int oldX, oldY;
			unsigned int newX, newY;
		} move;

		struct Attack {
			const Monster *monster;
			const Monster *target;
		} attack;
	} data;
};

/**
 * Creates an event for the monster's movement.
 *
 * @param monster Monster to move.
 * @param offX X offset to move.
 * @param offY Y offset to move.
 * @return Event structure.
 */
Event createMoveEvent(const Monster *monster, int offX, int offY);

/**
 * Creates an attack event.
 *
 * @param monster Monster which attacks.
 * @param target Monster which is attacked.
 * @return Event structure.
 */
Event createAttackEvent(const Monster *monster, const Monster *target);

} // end of namespace Game

#endif

