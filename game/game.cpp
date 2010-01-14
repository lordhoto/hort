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

#include "level.h"
#include "game.h"

#include "base/rnd.h"
#include "ai/monster.h"

#include <cassert>
#include <sstream>
#include <cmath>

namespace Game {

GameState::GameState() : _screen(GUI::Screen::instance()), _input(GUI::Input::instance()), _player(kMonsterPlayer, 8, 8, 8, 8, 10, kTicksPerTurn, 0, 0) {
	_initialized = false;
	_curLevel = 0;
	_eventDisp = 0;
	_tickCounter = 0;
	_nextWarning = 0;
}

GameState::~GameState() {
	delete _curLevel;
	delete _gameScreen;
}

bool GameState::initialize() {
	if (!_initialized) {
		_initialized = true;
		_curLevel = new Level(*this);

		_gameScreen = new Screen(_player);
		_curLevel->makeActive(*_gameScreen, _player);
	}

	_screen.clear();

	return true;
}

bool GameState::run() {
	int input = -1;

	int playerX = 0, playerY = 0;

	do {
		playerX = Base::rollDice(_curLevel->getMap().width()) - 1;
		playerY = Base::rollDice(_curLevel->getMap().height()) - 1;
	} while (!_curLevel->isWalkable(playerX, playerY));

	_player.setX(playerX);
	_player.setY(playerY);

	_gameScreen->setCenter(playerX, playerY);
	_gameScreen->update();
	_screen.update();

	while (input != GUI::kKeyEscape) {
		_gameScreen->setTurn(_tickCounter / kTicksPerTurn);

		if (_curLevel->isAllowedToAct(kPlayerMonsterID)) {
			_gameScreen->update(true);
			_screen.update();

			input = _input.poll();
			if (input == GUI::kNotifyResize) {
				_gameScreen->sizeChanged();
				continue;
			} else if (!handleInput(input)) {
				continue;
			}
		}

		_curLevel->update();
		_gameScreen->update();

		++_tickCounter;

		if (_player.getHitPoints() <= 0) {
			_gameScreen->addToMsgWindow("You die...");
			_gameScreen->update();
			_screen.update();
			_input.poll();
			break;
		}
	}

	return true;
}

void GameState::processEvent(const Event &event) {
	if (event.type == Event::kTypeAttackDamage) {
		const Monster *monster = _curLevel->getMonster(event.data.attackDamage.monster);
		assert(monster);
		const Monster *target = _curLevel->getMonster(event.data.attackDamage.target);
		assert(target);

		std::stringstream ss;

		if (event.data.attackDamage.target == kPlayerMonsterID)
			ss << "The " << getMonsterName(monster->getType()) << " hits you!";
		else
			ss << "You hit the " << getMonsterName(target->getType()) << "!";

		if (!event.data.attackDamage.didDmg)
			ss << " Somehow the attack does not cause any damage.";

		_gameScreen->addToMsgWindow(ss.str());
	} else if (event.type == Event::kTypeAttackFail) {
		const Monster *monster = _curLevel->getMonster(event.data.attackFail.monster);
		assert(monster);

		std::stringstream ss;

		if (event.data.attackFail.monster == kPlayerMonsterID)
			ss << "You miss!";
		else
			ss << "The " << getMonsterName(monster->getType()) << " misses!";

		_gameScreen->addToMsgWindow(ss.str());
	} else if (event.type == Event::kTypeDeath) {
		const Monster *monster = _curLevel->getMonster(event.data.death.monster);
		assert(monster);

		// Calculate distance
		// TODO: Use a real in view test later on here.
		int xDist = std::abs((int)(monster->getX() - _player.getX()));
		int yDist = std::abs((int)(monster->getY() - _player.getY()));

		if (std::sqrt(xDist*xDist + yDist*yDist) >= 10.0f)
			return;

		if (event.data.death.monster == kPlayerMonsterID) {
			switch (event.data.death.cause) {
			case Event::Death::kDrowned:
				_gameScreen->addToMsgWindow("You drown.");
				break;

			case Event::Death::kKilled: {
				const Monster *killer = _curLevel->getMonster(event.data.death.killer);
				assert(killer);

				std::stringstream ss;
				ss << "The " << getMonsterName(killer->getType()) << " kills you!";
				_gameScreen->addToMsgWindow(ss.str());
				} break;
			}
			
		} else {
			std::stringstream ss;
			if (event.data.death.killer == kPlayerMonsterID) {
				ss << "You kill the " << getMonsterName(monster->getType()) << "!";
			} else {
				ss << "The " << getMonsterName(monster->getType());

				switch (event.data.death.cause) {
				case Event::Death::kKilled: {
					const Monster *killer = _curLevel->getMonster(event.data.death.killer);
					assert(killer);

					ss << " is killed by the " << getMonsterName(killer->getType()) << "!";
					} break;

				case Event::Death::kDrowned:
					ss << " drowned.";
					break;
				}
			}

			_gameScreen->addToMsgWindow(ss.str());
		}
	} else if (event.type == Event::kTypeIdle) {
		if (event.data.idle.monster == kPlayerMonsterID) {
			// TODO: This should definitly not be stored over here
			static const char * const messages[] = {
				"You seem bored.",
				"You yawn.",
				"You nearly fall asleep."
			};

			if (Base::rollDice(10) == 10)
				_gameScreen->addToMsgWindow(messages[Base::rollDice(3) - 1]);
		} else {
			const Monster *monster = _curLevel->getMonster(event.data.idle.monster);
			assert(monster);

			// Calculate distance
			// TODO: Use a real in view test later on here.
			int xDist = std::abs((int)(monster->getX() - _player.getX()));
			int yDist = std::abs((int)(monster->getY() - _player.getY()));
			if (Base::rollDice(20) == 20 && std::sqrt(xDist*xDist + yDist*yDist) <= 10.0f) {
				std::stringstream ss;
				bool processMessage = true;

				ss << "The " << getMonsterName(monster->getType()) << " ";
				switch (event.data.idle.reason) {
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
		if (event.data.move.monster == kPlayerMonsterID)
			_gameScreen->setCenter(_player.getX(), _player.getY());
	}
}

bool GameState::handleInput(int input) {
	if (!_eventDisp)
		return false;

	int offX = 0, offY = 0;
	switch (input) {
	case 'h':
	case GUI::kKeyKeypad4:
		--offX;
		break;

	case 'l':
	case GUI::kKeyKeypad6:
		++offX;
		break;

	case 'k':
	case GUI::kKeyKeypad8:
		--offY;
		break;

	case 'j':
	case GUI::kKeyKeypad2:
		++offY;
		break;

	case 'y': case 'z':
	case GUI::kKeyKeypad7:
		--offX; --offY;
		break;

	case 'u':
	case GUI::kKeyKeypad9:
		++offX; --offY;
		break;

	case 'b':
	case GUI::kKeyKeypad1:
		--offX; ++offY;
		break;

	case 'n':
	case GUI::kKeyKeypad3:
		++offX; ++offY;
		break;

	case '.':
	case GUI::kKeyKeypad5:
		_eventDisp->dispatch(createIdleEvent(kPlayerMonsterID, Event::Idle::kNoReason));
		return true;

	case '/':
		examine();
		return false;

	default:
		return false;
	}

	unsigned int playerX = _player.getX(), playerY = _player.getY();
	MonsterID monster = _curLevel->monsterAt(playerX + offX, playerY + offY);
	if (monster != kInvalidMonsterID && monster != kPlayerMonsterID)
		_eventDisp->dispatch(createAttackEvent(kPlayerMonsterID, monster));
	else if (_curLevel->isWalkable(playerX + offX, playerY + offY))
		_eventDisp->dispatch(createMoveEvent(kPlayerMonsterID, &_player, offX, offY));
	else
		return false;

	return true;
}

void GameState::examine() {
	unsigned int x = _player.getX(), y = _player.getY();

	int input = 0;
	while (input != GUI::kKeyEscape) {
		input = _input.poll();
		int offX = 0, offY = 0;

		switch (input) {
		case 'h':
		case GUI::kKeyKeypad4:
			--offX;
			break;

		case 'l':
		case GUI::kKeyKeypad6:
			++offX;
			break;

		case 'k':
		case GUI::kKeyKeypad8:
			--offY;
			break;

		case 'j':
		case GUI::kKeyKeypad2:
			++offY;
			break;

		case 'y': case 'z':
		case GUI::kKeyKeypad7:
			--offX; --offY;
			break;

		case 'u':
		case GUI::kKeyKeypad9:
			++offX; --offY;
			break;

		case 'b':
		case GUI::kKeyKeypad1:
			--offX; ++offY;
			break;

		case 'n':
		case GUI::kKeyKeypad3:
			++offX; ++offY;
			break;

		case '.':
		case GUI::kKeyKeypad5: {
			input = GUI::kKeyEscape;

			std::stringstream ss;
			MonsterID monster = _curLevel->monsterAt(x, y);
			if (monster != kInvalidMonsterID)
				ss << "You see here a " << getMonsterName(_curLevel->getMonster(monster)->getType()) << ".";
			else
				ss << "This is just a simple " << Map::queryTileName(_curLevel->getMap().tileAt(x, y)) << ".";

			_gameScreen->addToMsgWindow(ss.str());
			} break;

		default:
			break;
		}

		if (input == GUI::kKeyEscape)
			break;

		if (x + offX >= 0 && x + offX < _curLevel->getMap().width()
		    && y + offY >= 0 && y + offY < _curLevel->getMap().height()) {
			x += offX;
			y += offY;
		}

		_gameScreen->setCenter(x, y);
		_gameScreen->update();
		_screen.update();
	}

	_gameScreen->setCenter(_player.getX(), _player.getY());
	_gameScreen->update();
	_screen.update();
}

} // end of namespace Game

