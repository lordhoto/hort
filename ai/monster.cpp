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
#include "game/monster_types.h"
#include "game/defs.h"

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
	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if (!_level.isAllowedToAct(i->first))
			continue;

		// TODO: Proper implementation of this :-D
		switch (i->second.fsmState) {
		case kMonsterIdle: {
			Base::Point newPos = i->second.monster->getPos() + Game::getDirection(Base::rollDice(9));

			bool didAction = false;
			if (newPos != i->second.monster->getPos()) {
				if ((unsigned int)newPos._x <= _level.getMap().width()
				    && (unsigned int)newPos._y <= _level.getMap().height()) {
					if (_level.isWalkable(newPos)) {
						Game::Map::Tile tile = _level.getMap().tileAt(newPos);

						if (tile != Game::Map::kTileWater) {
							_eventDisp.dispatch(Game::createMoveEvent(i->first, i->second.monster, newPos));
							didAction = true;
						}
					}
				}
			}

			if (!didAction)
				_eventDisp.dispatch(Game::createIdleEvent(i->first, Game::Event::Idle::kNoReason));
			} break;

		case kMonsterWary:
			_eventDisp.dispatch(Game::createIdleEvent(i->first, Game::Event::Idle::kWary));
			break;

		case kMonsterAttack:
			if (_player)
				_eventDisp.dispatch(Game::createAttackEvent(i->first, Game::kPlayerMonsterID));
			break;

		default:
			break;
		}
	}
}

void Monster::processEvent(const Game::Event &event) {
	if (event.type == Game::Event::kTypeMove) {
		if (event.move.monster == Game::kPlayerMonsterID) {
			for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
				_fsm->setState(i->second.fsmState);

				// Calculate distance
				const double dist = event.move.newPos.distanceTo(i->second.monster->getPos());

				if (dist <= std::sqrt(2))
					_fsm->process(kPlayerTriggerDist2);
				else if (dist <= 4.0f)
					_fsm->process(kPlayerTriggerDist1);
				else
					_fsm->process(kPlayerTriggerDist0);

				i->second.fsmState = _fsm->getState();
			}
		} else if (_player) {
			MonsterMap::iterator i = _monsters.find(event.move.monster);
			if (i != _monsters.end()) {
				// Calculate distance
				const double dist = event.move.newPos.distanceTo(_player->getPos());

				_fsm->setState(i->second.fsmState);

				if (dist <= std::sqrt(2))
					_fsm->process(kPlayerTriggerDist2);
				else if (dist <= 4.0f)
					_fsm->process(kPlayerTriggerDist1);
				else
					_fsm->process(kPlayerTriggerDist0);

				i->second.fsmState = _fsm->getState();
			}
		}
	} else if (event.type == Game::Event::kTypeAttack) {
		MonsterMap::iterator i = _monsters.find(event.attack.target);
		if (i != _monsters.end()) {
			_fsm->setState(i->second.fsmState);
			_fsm->process(kPlayerAttack);
			i->second.fsmState = _fsm->getState();
		}
	}
}

} // end of namespace AI

