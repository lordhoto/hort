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

#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "map.h"
#include "monster.h"
#include "screen.h"

#include <list>

namespace Game {

class Level {
public:
	Level();
	~Level();

	/**
	 * Assigns the level for output at the specific game screen.
	 *
	 * @param screen Screen to setup.
	 * @param player The player monster.
	 */
	void assignScreen(Screen &screen, const Monster &player);

	/**
	 * Unassigns the level from the screen associated with it.
	 */
	void unassignScreen();

	/**
	 * Returns the map assigned with this level.
	 *
	 * @return map
	 */
	const Map &getMap() const { return *_map; }

	/**
	 * Checks whether the given position is walkable
	 *
	 * @param x x coordiante.
	 * @param y y coordinate.
	 * @return true if walkable, false otherwise
	 */
	bool isWalkable(unsigned int x, unsigned int y) const;

	/**
	 * Returns a pointer to the monster at the given position or
	 * 0, when there is no monster.
	 *
	 * @param x x coordinate.
	 * @param y y coordinate.
	 * @return Pointer to monster.
	 */
	Monster *monsterAt(unsigned int x, unsigned int y);

	/**
	 * Removes the given monster from the level.
	 *
	 * @param monster Monster to remove.
	 */
	void removeMonster(Monster *monster);

	typedef std::list<Monster *> MonsterList;

	/**
	 * Returns a reference to the monster list.
	 *
	 * @return monster list.
	 */
	MonsterList &getMonsters() { return _monsters; }
private:
	Map *_map;
	Screen *_screen;

	MonsterList _monsters;
};

} // end of namespace Game

#endif

