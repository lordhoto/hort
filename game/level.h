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
#include "event.h"

#include <list>
#include <map>

namespace AI {
class Monster;
} // end of namespace AI

namespace Game {

class Level : public EventHandler {
public:
	Level(EventDispatcher &eventDisp);
	~Level();

	/**
	 * Assigns the level for output at the specific game screen.
	 *
	 * @param screen Screen to setup.
	 * @param player The player monster.
	 */
	void assignScreen(Screen &screen, Monster &player);

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
	 * @return Monster's ID.
	 */
	MonsterID monsterAt(unsigned int x, unsigned int y) const;

	/**
	 * Tries to access the monster with the given id.
	 *
	 * @param monster id
	 * @return Pointer to the monster
	 */
	Monster *getMonster(const MonsterID monster);

	/**
	 * Tries to access the monster with the given id.
	 *
	 * @param monster id
	 * @return Pointer to the monster
	 */
	const Monster *getMonster(const MonsterID monster) const;

	/**
	 * Removes the given monster from the level.
	 *
	 * @param monster Monster to remove.
	 */
	void removeMonster(const MonsterID monster);

	/**
	 * Processes the given event.
	 *
	 * @param event Event to process.
	 */
	void processEvent(const Event &event);

	AI::Monster *monsterAI() { return _monsterAI; }
private:
	Map *_map;
	Screen *_screen;

	typedef std::map<MonsterID, Monster *> MonsterMap;
	MonsterMap _monsters;

	AI::Monster *_monsterAI;
};

} // end of namespace Game

#endif

