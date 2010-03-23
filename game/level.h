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

#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "map.h"
#include "monster.h"
#include "event.h"
#include "game.h"
#include "defs.h"

#include "gui/screen.h"

#include "base/geo.h"

#include <list>
#include <map>
#include <vector>

namespace AI {
class Monster;
} // end of namespace AI

namespace Game {

class Level : public EventHandler {
friend class LevelLoader;
public:
	Level(Map *map, GameState &gs);
	~Level();

	/**
	 * Queries the start point of the level.
	 */
	const Base::Point &getStartPoint() const { return _start; }

	/**
	 * Sets the level as the active game level.
	 *
	 * @param screen Screen to setup.
	 * @param player The player monster.
	 */
	void makeActive(GUI::Screen &screen, Monster &player);

	/**
	 * Unsets the level as the active game level.
	 */
	void makeInactive();

	/**
	 * Returns the map assigned with this level.
	 *
	 * @return map
	 */
	const Map &getMap() const { return *_map; }

	/**
	 * Checks whether the given position is walkable.
	 *
	 * @param p Position.
	 * @return true if walkable, false otherwise.
	 */
	bool isWalkable(const Base::Point &p) const;

	/**
	 * Returns a monster id of the monster at the given position or
	 * kInvalidMonsterID, when there is no monster.
	 *
	 * @param p Position.
	 * @return Monster's ID.
	 */
	MonsterID monsterAt(const Base::Point &p) const;

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
	 * Checks whether the given monster is free to make
	 * an action this tick.
	 *
	 * @param monster id
	 * @return true, when it is free to make an action, false otherwise.
	 */
	bool isAllowedToAct(const MonsterID monster) const;

	/**
	 * Adds a monster to the level.
	 *
	 * @param monster Monster type to add.
	 * @param pos Position to add.
	 * @return The monster id.
	 */
	MonsterID addMonster(const MonsterType monster, const Base::Point &pos);

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

	/**
	 * Updates the level's state.
	 */
	void update();
private:
	Map *_map;
	std::vector<bool> _monsterField; // Keeps track, where monsters are placed
	Base::Point _start;

	GUI::Screen *_screen;
	GameState &_gameState;
	EventDispatcher _eventDisp;

	struct MonsterEntry {
		Monster *_monster;
		TickCount _nextAction;
		TickCount _nextRegeneration;

		MonsterEntry() : _monster(0), _nextAction(0), _nextRegeneration(0) {}
		// TODO: Handle "nextRegeneration" properly
		MonsterEntry(Monster *monster, TickCount curTick) : _monster(monster), _nextAction(curTick), _nextRegeneration(curTick) {}
	};

	Monster *updateNextActionTick(MonsterID monster, bool oneTickOnly = false);

	typedef std::map<MonsterID, MonsterEntry> MonsterMap;
	MonsterMap _monsters;

	AI::Monster *_monsterAI;
};

} // end of namespace Game

#endif

