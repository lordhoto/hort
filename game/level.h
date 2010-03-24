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
#include <stdexcept>

namespace AI {
class Monster;
} // end of namespace AI

namespace Game {

/**
 * A level object.
 *
 * A level is composed of a map and the monsters on the level.
 * @see Map
 * @see Monster
 */
class Level : public EventHandler {
friend class LevelLoader;
public:
	/**
	 * Constructor for a new level.
	 *
	 * @param map The map to use as base.
	 * @param gs The game state, this level is associated with.
	 */
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
	void makeActive(GUI::Screen &screen, Monster &player) throw (std::out_of_range);

	/**
	 * Unsets the level as the active game level.
	 *
	 * Note that the player will be automatically removed
	 * from the monster list.
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
	bool isWalkable(const Base::Point &p) const throw (std::out_of_range);

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
	MonsterID addMonster(const MonsterType monster, const Base::Point &pos) throw (std::out_of_range);

	/**
	 * Removes the given monster from the level.
	 *
	 * @param monster Monster to remove.
	 */
	void removeMonster(const MonsterID monster);

	void processMoveEvent(const MoveEvent &event) throw ();
	void processIdleEvent(const IdleEvent &event) throw ();
	void processDeathEvent(const DeathEvent &event) throw ();
	void processAttackEvent(const AttackEvent &event) throw ();
	void processAttackDamageEvent(const AttackDamageEvent &event) throw ();
	void processAttackFailEvent(const AttackFailEvent &event) throw ();

	/**
	 * Updates the level's state.
	 *
	 * An update should be called every tick. In this
	 * function the regeneration takes place, the
	 * dead monsters are removed from the level and
	 * the monster's AI is processed.
	 */
	void update();
private:
	/**
	 * The map.
	 */
	Map *_map;

	/**
	 * This vector keeps track where monsters are placed.
	 * It is oganized line-wise, this means you have to 
	 * access it like _monsterField[y * _map->getWidth() + x]
	 */
	std::vector<bool> _monsterField;

	/**
	 * The entrance of the level.
	 */
	Base::Point _start;

	/**
	 * The screen object, which is associated with the level.
	 * This can be 0 in case the level is not currently displayed.
	 */
	GUI::Screen *_screen;

	/**
	 * The game state associated with the level.
	 */
	GameState &_gameState;

	/**
	 * The internal event dispatcher.
	 * This is used to handle AI and game events.
	 */
	EventDispatcher _eventDisp;

	/**
	 * A monster in the level.
	 */
	struct MonsterEntry {
		/**
		 * Pointer to the monster.
		 */
		Monster *_monster;

		/**
		 * When the next action for the monster is available.
		 */
		TickCount _nextAction;

		/**
		 * When the monster regenerates next.
		 */
		TickCount _nextRegeneration;

		MonsterEntry() : _monster(0), _nextAction(0), _nextRegeneration(0) {}
		// TODO: Handle "nextRegeneration" properly
		MonsterEntry(Monster *monster, TickCount curTick) : _monster(monster), _nextAction(curTick), _nextRegeneration(curTick) {}
	};

	/**
	 * Updates the monster's next action tick number.
	 *
	 * @param monster ID of the monster to update.
	 * @param oneTickOnly Whether it should only delayed by one tick instead of the speed of the monster.
	 * @return Pointer to the monster modified.
	 */
	Monster *updateNextActionTick(MonsterID monster, bool oneTickOnly = false);

	/**
	 * Map type, which mapes a monster ID to a concrete monster in the level.
	 */
	typedef std::map<MonsterID, MonsterEntry> MonsterMap;

	/**
	 * The map containing all living monsters in the level.
	 */
	MonsterMap _monsters;

	/**
	 * The AI handler for all the level's monsters.
	 */
	AI::Monster *_monsterAI;
};

} // end of namespace Game

#endif

