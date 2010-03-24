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
#include "levelloader.h"
#include "game.h"
#include "tiledatabase.h"
#include "monsterdatabase.h"

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
	_gameScreen = 0;
}

GameState::~GameState() {
	delete _player;
	delete _curLevel;
	delete _gameScreen;
}

bool GameState::initialize() throw (Base::NonRecoverableException) {
	if (!_initialized) {
		_initialized = true;

		g_monsterDatabase.load("./data/monster.def");
		TileDatabase::instance().load("./data/tiles.def");

		_player = g_monsterDatabase.createNewMonster(kMonsterPlayer);
		assert(_player);
		LevelLoader *load = new LevelLoader("./data/levels/test");
		_curLevel = load->load(*this);
		assert(_curLevel);
		delete load;

		_gameScreen = new GUI::Screen(*_player);
		_gameScreen->initialize();

		_player->setPos(_curLevel->getStartPoint());
		_curLevel->makeActive(*_gameScreen, *_player);
		_gameScreen->setCenter(_player->getPos());
	}

	return true;
}

bool GameState::run() {
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

void GameState::processMoveEvent(const MoveEvent &event) {
	if (event.getMonster() == kPlayerMonsterID)
		_gameScreen->setCenter(event.getNewPos());
}

void GameState::processIdleEvent(const IdleEvent &event) {
	if (event.getMonster() == kPlayerMonsterID) {
		// TODO: This should definitly not be stored over here
		static const char * const messages[] = {
			"You seem bored.",
			"You yawn.",
			"You nearly fall asleep."
		};

		if (Base::rollDice(10) == 10)
			_gameScreen->addToMsgWindow(messages[Base::rollDice(3) - 1]);
	} else {
		const Monster *monster = _curLevel->getMonster(event.getMonster());
		assert(monster);

		if (Base::rollDice(20) == 20 && _player->getPos().distanceTo(monster->getPos()) <= 10.0f) {
			std::stringstream ss;
			bool processMessage = true;

			ss << "The " << g_monsterDatabase.getMonsterName(monster->getType()) << " ";
			switch (event.getReason()) {
			case IdleEvent::kNoReason:
				ss << "seems unsure what to do next.";
				break;

			case IdleEvent::kWary: {
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
}

void GameState::processDeathEvent(const DeathEvent &event) {
	const Monster *monster = _curLevel->getMonster(event.getMonster());
	assert(monster);

	if (_player->getPos().distanceTo(monster->getPos()) >= 10.0f)
		return;

	if (event.getMonster() == kPlayerMonsterID) {
		switch (event.getCause()) {
		case DeathEvent::kDrowned:
			_gameScreen->addToMsgWindow("You drown.");
			break;

		case DeathEvent::kKilled: {
			const Monster *killer = _curLevel->getMonster(event.getKiller());
			assert(killer);

			std::stringstream ss;
			ss << "The " << g_monsterDatabase.getMonsterName(killer->getType()) << " kills you!";
			_gameScreen->addToMsgWindow(ss.str());
			} break;
		}
		
	} else {
		std::stringstream ss;
		if (event.getKiller() == kPlayerMonsterID) {
			ss << "You kill the " << g_monsterDatabase.getMonsterName(monster->getType()) << "!";
		} else {
			ss << "The " << g_monsterDatabase.getMonsterName(monster->getType());

			switch (event.getCause()) {
			case DeathEvent::kKilled: {
				const Monster *killer = _curLevel->getMonster(event.getKiller());
				assert(killer);

				ss << " is killed by the " << g_monsterDatabase.getMonsterName(killer->getType()) << "!";
				} break;

			case DeathEvent::kDrowned:
				ss << " drowned.";
				break;
			}
		}

		_gameScreen->addToMsgWindow(ss.str());
	}
}

void GameState::processAttackEvent(const AttackEvent &/*event*/) {
	// Nothing to do here.
}

void GameState::processAttackDamageEvent(const AttackDamageEvent &event) {
	const Monster *monster = _curLevel->getMonster(event.getMonster());
	assert(monster);
	const Monster *target = _curLevel->getMonster(event.getTarget());
	assert(target);

	std::stringstream ss;

	if (event.getTarget() == kPlayerMonsterID)
		ss << "The " << g_monsterDatabase.g_monsterDatabase.getMonsterName(monster->getType()) << " hits you!";
	else
		ss << "You hit the " << g_monsterDatabase.getMonsterName(target->getType()) << "!";

	if (!event.getDidDmg())
		ss << " Somehow the attack does not cause any damage.";

	_gameScreen->addToMsgWindow(ss.str());
}

void GameState::processAttackFailEvent(const AttackFailEvent &event) {
	const Monster *monster = _curLevel->getMonster(event.getMonster());
	assert(monster);

	std::stringstream ss;

	if (event.getMonster() == kPlayerMonsterID)
		ss << "You miss!";
	else
		ss << "The " << g_monsterDatabase.getMonsterName(monster->getType()) << " misses!";

	_gameScreen->addToMsgWindow(ss.str());
}

bool GameState::handleInput(GUI::Input input) {
	if (!_eventDisp)
		return false;

	Base::Point offset;
	switch (input) {
	case GUI::kInputQuit:
		return true;

	case GUI::kInputDir5:
		_eventDisp->dispatch(new IdleEvent(kPlayerMonsterID, IdleEvent::kNoReason));
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
		_eventDisp->dispatch(new AttackEvent(kPlayerMonsterID, monster));
	else if (_curLevel->isWalkable(newPos))
		_eventDisp->dispatch(new MoveEvent(kPlayerMonsterID, _player->getPos(), newPos));
	else
		return false;

	return true;
}

void GameState::examine() {
	Base::Point pos = _player->getPos();

	_gameScreen->addToMsgWindow("You are examining the environment now.");
	_gameScreen->update(true);

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
				ss << "This is just a simple " << _curLevel->getMap().tileDefinition(pos).getName() << ".";

			_gameScreen->addToMsgWindow(ss.str());
			} break;

		default:
			break;
		}

		if (input == GUI::kInputQuit)
			break;

		const Base::Point newPos = pos + offset;
		if (newPos._x >= 0 && static_cast<unsigned int>(newPos._x) < _curLevel->getMap().getWidth()
		    && newPos._y >= 0 && static_cast<unsigned int>(newPos._y) < _curLevel->getMap().getHeight())
			pos = newPos;

		_gameScreen->setCenter(pos);
		_gameScreen->update();
	}

	_gameScreen->setCenter(_player->getPos());
	_gameScreen->update(true);
}

} // end of namespace Game

