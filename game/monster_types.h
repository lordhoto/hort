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

#ifndef GAME_MONSTER_TYPES_H
#define GAME_MONSTER_TYPES_H

namespace Game {

enum MonsterType {
	kMonsterPlayer = 0,
	kMonsterGnome = 1,
	kMonsterSquolly = 2
};

/**
 * Returns the name of the given monster.
 *
 * @param type Type of the monster
 */
const char *getMonsterName(MonsterType type);

} // end of namespace Game

#endif

