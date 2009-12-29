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

#include <map>

namespace AI {

namespace {

enum kMonsterFSMStateID {
	kMonsterIdle = 0,
	kMonsterWary,
	kMonsterAttack
};

enum kMonsterFSMInputID {
	kPlayerEnterSight = 0,
	kPlayerLeaveSight,
	kPlayerEnterRange,
	kPlayerLeaveRange,
	kPlayerAttack
};

struct FSMTransition {
	kMonsterFSMStateID state;
	kMonsterFSMInputID input;
	kMonsterFSMStateID newState;
};

const FSMTransition s_monsterFSMTransitions[] = {
	{ kMonsterIdle  , kPlayerEnterSight, kMonsterWary   },
	{ kMonsterIdle  , kPlayerAttack    , kMonsterAttack },
	{ kMonsterIdle  , kPlayerEnterRange, kMonsterAttack },

	{ kMonsterWary  , kPlayerEnterRange, kMonsterAttack },
	{ kMonsterWary  , kPlayerAttack    , kMonsterAttack },
	{ kMonsterWary  , kPlayerLeaveSight, kMonsterIdle   },

	{ kMonsterAttack, kPlayerLeaveRange, kMonsterWary   },
	{ kMonsterAttack, kPlayerLeaveSight, kMonsterWary   }
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

Monster::Monster(Game::GameState &game) : _player(game.getPlayer()), _game(game) {
	_fsm = createMonsterFSM();
}

Monster::~Monster() {
	delete _fsm;
	_fsm = 0;
}

void Monster::addMonster(const Game::Monster *monster) {
	remMonster(monster);
	_monsters[monster] = kMonsterIdle;
}

void Monster::remMonster(const Game::Monster *monster) {
	_monsters.erase(monster);
}

void Monster::update() {
	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		// TODO: Proper implementation of this :-D
		switch (i->second) {
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

			if (offX && offY && _game.getLevel().isWalkable(i->first->getX() + offX, i->first->getY() + offY))
				_game.processEvent(Game::createMoveEvent(i->first, offX, offY));
			} break;

		default:
			break;
		}
	}
}

void Monster::processEvent(const Game::Event &event) {
}

} // end of namespace AI

