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

#include "level.h"
#include "game.h"

#include "base/rnd.h"

#include "ai/monster.h"

#include "gui/defs.h"

#include <cassert>
#include <sstream>
#include <cmath>

namespace Game {

GameState::GameState() : _player(0) {
	_initialized = false;
	_curLevel = 0;
	_eventDisp = 0;
	_tickCounter = 0;
	_nextWarning = 0;
}

GameState::~GameState() {
	delete _player;
	delete _curLevel;
	delete _gameScreen;
}

bool GameState::initialize() {
	if (!_initialized) {
		_initialized = true;

		g_monsterDatabase.load("./data/monster.def");
		TileDatabase::instance().load("./data/tiles.def");

		_player = g_monsterDatabase.createNewMonster(kMonsterPlayer);
		assert(_player);
		_curLevel = new Level(*this);

		_gameScreen = new GUI::Screen(*_player);
		_gameScreen->initialize();
		_curLevel->makeActive(*_gameScreen, *_player);
	}

	return true;
}

bool GameState::run() {
	Base::Point playerPos;

	do {
		playerPos._x = Base::rollDice(_curLevel->getMap().width()) - 1;
		playerPos._y = Base::rollDice(_curLevel->getMap().height()) - 1;
	} while (!_curLevel->isWalkable(playerPos));

	_player->setPos(playerPos);

	_gameScreen->setCenter(playerPos);
	_gameScreen->update();

	GUI::Input input = GUI::kInputNone;
	while (input != GUI::kInputQuit) {
		_gameScreen->setTurn(_tickCounter / kTicksPerTurn);

		if (_curLevel->isAllowedToAct(kPlayerMonsterID)) {
			_gameScreen->update(true);

			input = _gameScreen->getInput();
			if (!handleInput(input))
				continue;
		}

		_curLevel->update();
		_gameScreen->update();

		++_tickCounter;

		if (_player->getHitPoints() <= 0) {
			_gameScreen->addToMsgWindow("You die...");
			_gameScreen->update(true);
			_gameScreen->getInput();
			break;
		}
	}

	return true;
}

void GameState::processEvent(const Event &event) {
	if (event.type == Event::kTypeAttackDamage) {
		const Monster *monster = _curLevel->getMonster(event.attackDamage.monster);
		assert(monster);
		const Monster *target = _curLevel->getMonster(event.attackDamage.target);
		assert(target);

		std::stringstream ss;

		if (event.attackDamage.target == kPlayerMonsterID)
			ss << "The " << g_monsterDatabase.g_monsterDatabase.getMonsterName(monster->getType()) << " hits you!";
		else
			ss << "You hit the " << g_monsterDatabase.getMonsterName(target->getType()) << "!";

		if (!event.attackDamage.didDmg)
			ss << " Somehow the attack does not cause any damage.";

		_gameScreen->addToMsgWindow(ss.str());
	} else if (event.type == Event::kTypeAttackFail) {
		const Monster *monster = _curLevel->getMonster(event.attackFail.monster);
		assert(monster);

		std::stringstream ss;

		if (event.attackFail.monster == kPlayerMonsterID)
			ss << "You miss!";
		else
			ss << "The " << g_monsterDatabase.getMonsterName(monster->getType()) << " misses!";

		_gameScreen->addToMsgWindow(ss.str());
	} else if (event.type == Event::kTypeDeath) {
		const Monster *monster = _curLevel->getMonster(event.death.monster);
		assert(monster);

		if (_player->getPos().distanceTo(monster->getPos()) >= 10.0f)
			return;

		if (event.death.monster == kPlayerMonsterID) {
			switch (event.death.cause) {
			case Event::Death::kDrowned:
				_gameScreen->addToMsgWindow("You drown.");
				break;

			case Event::Death::kKilled: {
				const Monster *killer = _curLevel->getMonster(event.death.killer);
				assert(killer);

				std::stringstream ss;
				ss << "The " << g_monsterDatabase.getMonsterName(killer->getType()) << " kills you!";
				_gameScreen->addToMsgWindow(ss.str());
				} break;
			}
			
		} else {
			std::stringstream ss;
			if (event.death.killer == kPlayerMonsterID) {
				ss << "You kill the " << g_monsterDatabase.getMonsterName(monster->getType()) << "!";
			} else {
				ss << "The " << g_monsterDatabase.getMonsterName(monster->getType());

				switch (event.death.cause) {
				case Event::Death::kKilled: {
					const Monster *killer = _curLevel->getMonster(event.death.killer);
					assert(killer);

					ss << " is killed by the " << g_monsterDatabase.getMonsterName(killer->getType()) << "!";
					} break;

				case Event::Death::kDrowned:
					ss << " drowned.";
					break;
				}
			}

			_gameScreen->addToMsgWindow(ss.str());
		}
	} else if (event.type == Event::kTypeIdle) {
		if (event.idle.monster == kPlayerMonsterID) {
			// TODO: This should definitly not be stored over here
			static const char * const messages[] = {
				"You seem bored.",
				"You yawn.",
				"You nearly fall asleep."
			};

			if (Base::rollDice(10) == 10)
				_gameScreen->addToMsgWindow(messages[Base::rollDice(3) - 1]);
		} else {
			const Monster *monster = _curLevel->getMonster(event.idle.monster);
			assert(monster);

			if (Base::rollDice(20) == 20 && _player->getPos().distanceTo(monster->getPos()) <= 10.0f) {
				std::stringstream ss;
				bool processMessage = true;

				ss << "The " << g_monsterDatabase.getMonsterName(monster->getType()) << " ";
				switch (event.idle.reason) {
				case Event::Idle::kNoReason:
					ss << "seems unsure what to do next.";
					break;

				case Event::Idle::kWary: {
					// TODO: This should definitly not be stored over here
					static const char * const messages[] = {
						"seems to be watching you.",
						"makes you nervous.",
						"seems to be aware of your presence."
					};

					ss << messages[Base::rollDice(3) - 1];

					if (_nextWarning <= _tickCounter)
						// TODO: How often the player has the chance to catch this
						// messages should be dependant on his wisdom.
						_nextWarning = _tickCounter + 3 * kTicksPerTurn;
					else
						processMessage = false;
					} break;
				}

				if (processMessage)
					_gameScreen->addToMsgWindow(ss.str());
			}
		}
	} else if (event.type == Event::kTypeMove) {
		if (event.move.monster == kPlayerMonsterID)
			_gameScreen->setCenter(event.move.newPos);
	}
}

bool GameState::handleInput(GUI::Input input) {
	if (!_eventDisp)
		return false;

	Base::Point offset;
	switch (input) {
	case GUI::kInputQuit:
		return true;

	case GUI::kInputDir5:
		_eventDisp->dispatch(createIdleEvent(kPlayerMonsterID, Event::Idle::kNoReason));
		return true;

	case GUI::kInputNone:
		return false;

	case GUI::kInputExamine:
		examine();
		return false;

	case GUI::kInputDir1:
	case GUI::kInputDir2:
	case GUI::kInputDir3:
	case GUI::kInputDir4:
	case GUI::kInputDir6:
	case GUI::kInputDir7:
	case GUI::kInputDir8:
	case GUI::kInputDir9:
		offset = getDirection(input);
		break;
	}

	const Base::Point newPos = _player->getPos() + offset;

	MonsterID monster = _curLevel->monsterAt(newPos);
	if (monster != kInvalidMonsterID && monster != kPlayerMonsterID)
		_eventDisp->dispatch(createAttackEvent(kPlayerMonsterID, monster));
	else if (_curLevel->isWalkable(newPos))
		_eventDisp->dispatch(createMoveEvent(kPlayerMonsterID, _player, newPos));
	else
		return false;

	return true;
}

void GameState::examine() {
	Base::Point pos = _player->getPos();

	GUI::Input input = GUI::kInputNone;
	while (input != GUI::kInputQuit) {
		input = _gameScreen->getInput();
		Base::Point offset;

		switch (input) {
		case GUI::kInputDir1:
		case GUI::kInputDir2:
		case GUI::kInputDir3:
		case GUI::kInputDir4:
		case GUI::kInputDir6:
		case GUI::kInputDir7:
		case GUI::kInputDir8:
		case GUI::kInputDir9:
			offset = getDirection(input);
			break;

		case GUI::kInputDir5: {
			input = GUI::kInputQuit;

			std::stringstream ss;
			MonsterID monster = _curLevel->monsterAt(pos);
			if (monster != kInvalidMonsterID)
				ss << "You see here a " << g_monsterDatabase.getMonsterName(_curLevel->getMonster(monster)->getType()) << ".";
			else
				ss << "This is just a simple " << Map::queryTileName(_curLevel->getMap().tileAt(pos)) << ".";

			_gameScreen->addToMsgWindow(ss.str());
			} break;

		default:
			break;
		}

		if (input == GUI::kInputQuit)
			break;

		const Base::Point newPos = pos + offset;
		if (newPos._x >= 0 && (unsigned int)newPos._x < _curLevel->getMap().width()
		    && newPos._y >= 0 && (unsigned int)newPos._y < _curLevel->getMap().height())
			pos = newPos;

		_gameScreen->setCenter(pos);
		_gameScreen->update();
	}

	_gameScreen->setCenter(_player->getPos());
	_gameScreen->update(true);
}

} // end of namespace Game

