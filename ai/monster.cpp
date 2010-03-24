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

#include "monster.h"
#include "fsm.h"

#include "base/rnd.h"

#include "game/monsterdefinition.h"
#include "game/defs.h"

#include <map>
#include <cmath>

#include <boost/foreach.hpp>

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
	kMonsterFSMStateID _state;
	kMonsterFSMInputID _input;
	kMonsterFSMStateID _newState;
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

	BOOST_FOREACH(const FSMTransition &trans, s_monsterFSMTransitions) {
		StateMap::iterator i = states.find(trans._state);
		if (i == states.end()) {
			FSM::State *newState = new FSM::State();
			newState->addTransition(trans._input, trans._newState);
			states[trans._state] = newState;
		} else {
			i->second->addTransition(trans._input, trans._newState);
		}
	}

	FSM::FSM *fsm = new FSM::FSM();
	BOOST_FOREACH(StateMap::value_type &i, states)
		fsm->addState(i.first, i.second);

	return fsm;
}

} // end of anonymous namespace

Monster::Monster(const Game::Level &parent, Game::EventDispatcher &disp) : _level(parent), _eventDisp(disp), _fsm(0), _monsters(), _player(0) {
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
	BOOST_FOREACH(MonsterMap::value_type &i, _monsters) {
		if (!_level.isAllowedToAct(i.first))
			continue;

		// TODO: Proper implementation of this :-D
		switch (i.second._fsmState) {
		case kMonsterIdle: {
			Base::Point newPos = i.second._monster->getPos() + Game::getDirection(static_cast<unsigned char>(Base::rollDice(9)));

			bool didAction = false;
			if (newPos != i.second._monster->getPos()) {
				if (static_cast<unsigned int>(newPos._x) <= _level.getMap().getWidth()
				    && static_cast<unsigned int>(newPos._y) <= _level.getMap().getHeight()) {
					if (_level.isWalkable(newPos)) {
						const Game::TileDefinition &def = _level.getMap().tileDefinition(newPos);
						if (!def.getIsLiquid()) {
							_eventDisp.dispatch(new Game::MoveEvent(i.first, i.second._monster->getPos(), newPos));
							didAction = true;
						}
					}
				}
			}

			if (!didAction)
				_eventDisp.dispatch(new Game::IdleEvent(i.first, Game::IdleEvent::kNoReason));
			} break;

		case kMonsterWary:
			if (_player) {
				int xDist = _player->getX() - i.second._monster->getX();
				int yDist = _player->getY() - i.second._monster->getY();

				int xAdd = (xDist < 0) ? -1 : ((xDist > 0) ? +1 : 0);
				int yAdd = (yDist < 0) ? -1 : ((yDist > 0) ? +1 : 0);

				Base::Point newPos = i.second._monster->getPos();
				newPos._x += xAdd;
				newPos._y += yAdd;

				if (_level.isWalkable(newPos)) {
					const Game::TileDefinition &def = _level.getMap().tileDefinition(newPos);
					if (!def.getIsLiquid())
						_eventDisp.dispatch(new Game::MoveEvent(i.first, i.second._monster->getPos(), newPos));
				} else {
					_eventDisp.dispatch(new Game::IdleEvent(i.first, Game::IdleEvent::kWary));
				}
			} else {
				_eventDisp.dispatch(new Game::IdleEvent(i.first, Game::IdleEvent::kWary));
			}
			break;

		case kMonsterAttack:
			if (_player)
				_eventDisp.dispatch(new Game::AttackEvent(i.first, Game::kPlayerMonsterID));
			break;

		default:
			break;
		}
	}
}

void Monster::processMoveEvent(const Game::MoveEvent &event) {
	if (event.getMonster() == Game::kPlayerMonsterID) {
		BOOST_FOREACH(MonsterMap::value_type &i, _monsters) {
			_fsm->setState(i.second._fsmState);

			// Calculate distance
			const double dist = event.getNewPos().distanceTo(i.second._monster->getPos());

			if (dist <= std::sqrt(2))
				_fsm->process(kPlayerTriggerDist2);
			else if (dist <= 4.0f)
				_fsm->process(kPlayerTriggerDist1);
			else
				_fsm->process(kPlayerTriggerDist0);

			i.second._fsmState = _fsm->getState();
		}
	} else if (_player) {
		MonsterMap::iterator i = _monsters.find(event.getMonster());
		if (i != _monsters.end()) {
			// Calculate distance
			const double dist = event.getNewPos().distanceTo(_player->getPos());

			_fsm->setState(i->second._fsmState);

			if (dist <= std::sqrt(2))
				_fsm->process(kPlayerTriggerDist2);
			else if (dist <= 4.0f)
				_fsm->process(kPlayerTriggerDist1);
			else
				_fsm->process(kPlayerTriggerDist0);

			i->second._fsmState = _fsm->getState();
		}
	}
}

void Monster::processIdleEvent(const Game::IdleEvent &/*event*/) {
	// Nothing to do here.
}

void Monster::processDeathEvent(const Game::DeathEvent &/*event*/) {
	// Nothing to do here.
}

void Monster::processAttackEvent(const Game::AttackEvent &event) {
	MonsterMap::iterator i = _monsters.find(event.getTarget());
	if (i != _monsters.end()) {
		_fsm->setState(i->second._fsmState);
		_fsm->process(kPlayerAttack);
		i->second._fsmState = _fsm->getState();
	}
}

void Monster::processAttackDamageEvent(const Game::AttackDamageEvent &/*event*/) {
	// Nothing to do here.
}

void Monster::processAttackFailEvent(const Game::AttackFailEvent &/*event*/) {
	// Nothing to do here.
}

} // end of namespace AI

