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

#ifndef AI_MONSTER_H
#define AI_MONSTER_H

#include "fsm.h"
#include "game/level.h"
#include "game/monster.h"
#include "game/event.h"

#include <map>
#include <list>

namespace AI {

class Monster : public Game::EventHandler {
public:
	Monster(const Game::Level &parent, Game::EventHandler &handler);
	~Monster();

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

	/**
	 * Processes a given event.
	 *
	 * @param event Event to process.
	 */
	void processEvent(const Game::Event &event);
private:
	const Game::Level &_level;
	Game::EventHandler &_events;
	FSM::FSM *_fsm;

	struct MonsterState {
		FSM::StateID fsmState;
		const Game::Monster *monster;

		MonsterState() : fsmState(FSM::kInvalidStateID), monster(0) {}
		MonsterState(FSM::StateID f, const Game::Monster *m) : fsmState(f), monster(m) {}
	};

	typedef std::map<const Game::MonsterID, MonsterState> MonsterMap;
	MonsterMap _monsters;
};

} // end of namespace AI

#endif

