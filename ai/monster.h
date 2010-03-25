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

#ifndef AI_MONSTER_H
#define AI_MONSTER_H

#include "fsm.h"
#include "game/level.h"
#include "game/monster.h"
#include "game/event.h"

#include <map>
#include <list>

namespace AI {

/**
 * The AI handler for all NPC monsters in a level.
 */
class Monster : public Game::EventHandler {
public:
	/**
	 * Constructor for the object.
	 *
	 * @param parent Level in which all monsters are placed.
	 * @param disp Dispatcher to use for dispatching game events.
	 */
	Monster(const Game::Level &parent, Game::EventDispatcher &disp);
	~Monster();

	/**
	 * Sets the player monster.
	 */
	void setPlayer(const Game::Monster *player) { _player = player; }

	/**
	 * Adds a new monster to the AI.
	 *
	 * @param monster New monster.
	 * @param monsterPtr Pointer to the monster.
	 */
	void addMonster(const Game::MonsterID monster, const Game::Monster *monsterPtr);

	/**
	 * Removes a monster from the AI.
	 *
	 * @param monster Monster to remove.
	 */
	void removeMonster(const Game::MonsterID monster);

	/**
	 * Updates all monsters.
	 */
	void update();

	void processMoveEvent(const Game::MoveEvent &event) throw ();
	void processIdleEvent(const Game::IdleEvent &event) throw ();
	void processDeathEvent(const Game::DeathEvent &event) throw ();
	void processAttackEvent(const Game::AttackEvent &event) throw ();
	void processAttackDamageEvent(const Game::AttackDamageEvent &event) throw ();
	void processAttackFailEvent(const Game::AttackFailEvent &event) throw ();
private:
	/**
	 * The level the monsters are on.
	 */
	const Game::Level &_level;

	/**
	 * The event dispatcher through which all AI events should be
	 * dispatched.
	 */
	Game::EventDispatcher &_eventDisp;

	/**
	 * The FSM to use for internal use.
	 */
	FSM::FSM *_fsm;

	/**
	 * The AI state of a monster.
	 */
	struct MonsterState {
		/**
		 * The FSM state. Based on this state
		 * the monster makes it "decisions".
		 */
		FSM::StateID _fsmState;

		/**
		 * Pointer to the monster object.
		 */
		const Game::Monster *_monster;

		MonsterState() : _fsmState(FSM::kInvalidStateID), _monster(0) {}
		MonsterState(FSM::StateID f, const Game::Monster *m) : _fsmState(f), _monster(m) {}
	};

	/**
	 * The monster map type. It maps a given monster ID to its AI state.
	 */
	typedef std::map<const Game::MonsterID, MonsterState> MonsterMap;

	/**
	 * The state of all monsters.
	 */
	MonsterMap _monsters;

	/**
	 * A pointer to the player monster. This might
	 * be NULl to indicate that the player monster
	 * is not in the same level as the monsters.
	 */
	const Game::Monster *_player;
};

} // end of namespace AI

#endif

