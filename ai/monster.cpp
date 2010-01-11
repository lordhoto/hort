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

#include "monster.h"
#include "fsm.h"

#include "base/rnd.h"
#include "game/monster_types.h"

#include <map>
#include <cmath>

namespace AI {

namespace {

enum kMonsterFSMStateID {
	kMonsterIdle = 0,
	kMonsterWary,
	kMonsterAttack
};

enum kMonsterFSMInputID {
	kPlayerTriggerDist0 = 0,
	kPlayerTriggerDist1,
	kPlayerTriggerDist2,
	kPlayerAttack
};

struct FSMTransition {
	kMonsterFSMStateID state;
	kMonsterFSMInputID input;
	kMonsterFSMStateID newState;
};

const FSMTransition s_monsterFSMTransitions[] = {
	{ kMonsterIdle  , kPlayerTriggerDist1, kMonsterWary   },
	{ kMonsterIdle  , kPlayerAttack      , kMonsterAttack },
	{ kMonsterIdle  , kPlayerTriggerDist2, kMonsterAttack },

	{ kMonsterWary  , kPlayerTriggerDist2, kMonsterAttack },
	{ kMonsterWary  , kPlayerAttack      , kMonsterAttack },
	{ kMonsterWary  , kPlayerTriggerDist0, kMonsterIdle   },

	{ kMonsterAttack, kPlayerTriggerDist1, kMonsterWary   },
	{ kMonsterAttack, kPlayerTriggerDist0, kMonsterIdle   }
};

FSM::FSM *createMonsterFSM() {
	typedef std::map<kMonsterFSMStateID, FSM::State *> StateMap;
	StateMap states;

	for (size_t i = 0; i < sizeof(s_monsterFSMTransitions)/sizeof(s_monsterFSMTransitions[0]); ++i) {
		const FSMTransition &trans = s_monsterFSMTransitions[i];

		StateMap::iterator i = states.find(trans.state);
		if (i == states.end()) {
			FSM::State *newState = new FSM::State();
			newState->addTransition(trans.input, trans.newState);
			states[trans.state] = newState;
		} else {
			i->second->addTransition(trans.input, trans.newState);
		}
	}

	FSM::FSM *fsm = new FSM::FSM();
	for (StateMap::iterator i = states.begin(); i != states.end(); ++i)
		fsm->addState(i->first, i->second);

	return fsm;
}

} // end of anonymous namespace

Monster::Monster(const Game::Level &parent, Game::EventDispatcher &disp) : _level(parent), _eventDisp(disp) {
	_fsm = createMonsterFSM();
}

Monster::~Monster() {
	delete _fsm;
	_fsm = 0;
}

void Monster::addMonster(const Game::MonsterID monster, const Game::Monster *monsterPtr) {
	removeMonster(monster);
	_monsters[monster] = MonsterState(kMonsterIdle, monsterPtr);
}

void Monster::removeMonster(const Game::MonsterID monster) {
	_monsters.erase(monster);
}

void Monster::update() {
	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if (!_level.isAllowedToAct(i->first))
			continue;

		// TODO: Proper implementation of this :-D
		switch (i->second.fsmState) {
		case kMonsterIdle: {
			int offX = 0, offY = 0;
			switch (Base::rollDice(9)) {
			case 1:
				--offX; ++offY;
				break;
			
			case 2:
				++offY;
				break;

			case 3:
				++offX; ++offY;
				break;

			case 4:
				--offX;
				break;

			case 6:
				++offX;
				break;

			case 7:
				--offX; --offY;
				break;

			case 8:
				--offY;
				break;

			case 9:
				++offX; --offY;
				break;

			default:
				break;
			}

			const Game::Monster *monster = _level.getMonster(i->first);
			if (offX && offY && _level.isWalkable(monster->getX() + offX, monster->getY() + offY))
				_eventDisp.dispatch(Game::createMoveEvent(i->first, i->second.monster, offX, offY));
			} break;

		case kMonsterAttack:
			_eventDisp.dispatch(Game::createAttackEvent(i->first, Game::kPlayerMonsterID));
			break;

		default:
			break;
		}
	}
}

void Monster::processEvent(const Game::Event &event) {
	if (event.type == Game::Event::kTypeMove) {
		if (event.data.move.monster == Game::kPlayerMonsterID) {
			for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
				_fsm->setState(i->second.fsmState);

				// Calculate distance
				int xDist = std::abs((int)(event.data.move.newX - i->second.monster->getX()));
				int yDist = std::abs((int)(event.data.move.newY - i->second.monster->getY()));

				if (xDist <= 1 && yDist <= 1)
					_fsm->process(kPlayerTriggerDist2);
				else if (std::sqrt(xDist*xDist + yDist*yDist) <= 4.0f)
					_fsm->process(kPlayerTriggerDist1);
				else
					_fsm->process(kPlayerTriggerDist0);

				i->second.fsmState = _fsm->getState();
			}
		} else if (_level.getMonster(Game::kPlayerMonsterID)) {
			MonsterMap::iterator i = _monsters.find(event.data.move.monster);
			if (i != _monsters.end()) {
				// Calculate distance
				int xDist = std::abs((int)(event.data.move.newX - _level.getMonster(Game::kPlayerMonsterID)->getX()));
				int yDist = std::abs((int)(event.data.move.newY - _level.getMonster(Game::kPlayerMonsterID)->getY()));

				_fsm->setState(i->second.fsmState);

				if (xDist <= 1 && yDist <= 1)
					_fsm->process(kPlayerTriggerDist2);
				else if (std::sqrt(xDist*xDist + yDist*yDist) <= 4.0f)
					_fsm->process(kPlayerTriggerDist1);
				else
					_fsm->process(kPlayerTriggerDist0);

				i->second.fsmState = _fsm->getState();
			}
		}
	} else if (event.type == Game::Event::kTypeAttack) {
		MonsterMap::iterator i = _monsters.find(event.data.attack.target);
		if (i != _monsters.end()) {
			_fsm->setState(i->second.fsmState);
			_fsm->process(kPlayerAttack);
			i->second.fsmState = _fsm->getState();
		}
	}
}

} // end of namespace AI

