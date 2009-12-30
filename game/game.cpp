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

#include "game.h"

#include "base/rnd.h"
#include "ai/monster.h"

#include <cassert>
#include <sstream>

namespace Game {

GameState::GameState() : _screen(GUI::Screen::instance()), _input(GUI::Input::instance()), _player(kMonsterPlayer, 8, 8, 8, 8, 10, 0, 0) {
	_initialized = false;
	_messageLine = _mapWindow = _playerStats = 0;
	_curLevel = 0;
}

GameState::~GameState() {
	delete _curLevel;
	delete _gameScreen;
	delete _monsterAI;

	_screen.remove(_messageLine);
	delete _messageLine;
	_screen.remove(_mapWindow);
	delete _mapWindow;
	_screen.remove(_playerStats);
	delete _playerStats;
}

bool GameState::initialize() {
	if (!_initialized) {
		_initialized = true;
		_curLevel = new Level();

		_messageLine = new GUI::Window(0,  0, 80,  1, false);
		_mapWindow = new GUI::Window(0,  1, 80, 21, false);
		_playerStats = new GUI::Window(0, 22, 80,  2, false);

		_screen.add(_messageLine);
		_screen.add(_mapWindow);
		_screen.add(_playerStats);

		_gameScreen = new GameScreen(*_mapWindow);
		_curLevel->assignScreen(*_gameScreen, _player);

		_monsterAI = new AI::Monster(*this);
		const Level::MonsterList &monsters = _curLevel->getMonsters();
		for (Level::MonsterList::const_iterator i = monsters.begin(); i != monsters.end(); ++i)
			_monsterAI->addMonster(*i);
	}

	_screen.clear();

	return true;
}

bool GameState::run() {
	int input = -1;

	do {
		_player.setX(Base::rollDice(_curLevel->getMap().width()) - 1);
		_player.setY(Base::rollDice(_curLevel->getMap().height()) - 1);
	} while (!_curLevel->isWalkable(_player.getX(), _player.getY()));

	_gameScreen->update();
	drawStatsWindow();
	_screen.update();

	while (input != GUI::kKeyEscape) {
		_messages.clear();
		_screen.update();

		input = _input.poll();
		handleInput(input);

		_monsterAI->update();
		_gameScreen->update();
		drawStatsWindow();
		printMessages();

		if (_player.getHitPoints() <= 0) {
			_input.poll();
			break;
		}
	}

	return true;
}

void GameState::processEvent(const Event &event) {
	if (event.type == Event::kTypeMove) {
		Monster *monster = obtainMonster(event.data.move.monster);
		assert(monster);

		// TODO: add some error checking
		monster->setX(event.data.move.newX);
		monster->setY(event.data.move.newY);
		_gameScreen->flagForUpdate();
	} else if (event.type == Event::kTypeAttack) {
		Monster *target = obtainMonster(event.data.attack.target);
		assert(target);

		int newHitPoints = target->getHitPoints() - 1;
		target->setHitPoints(newHitPoints);

		if (newHitPoints <= 0) {
			if (target != &_player) {
				_messages.push_back("You kill the Gnome!");
				_monsterAI->removeMonster(target);
				_curLevel->removeMonster(target);
			} else {
				_messages.push_back("You die...");
			}
		} else {
			if (target == &_player)
				_messages.push_back("The Gnome hits!");
			else
				_messages.push_back("You hit the Gnome!");
		}
	}

	_monsterAI->processEvent(event);
}

void GameState::handleInput(int input) {
	int offX = 0, offY = 0;
	switch (input) {
	case GUI::kKeyKeypad4:
		--offX;
		break;

	case GUI::kKeyKeypad6:
		++offX;
		break;

	case GUI::kKeyKeypad8:
		--offY;
		break;

	case GUI::kKeyKeypad2:
		++offY;
		break;

	case GUI::kKeyKeypad7:
		--offX; --offY;
		break;

	case GUI::kKeyKeypad9:
		++offX; --offY;
		break;

	case GUI::kKeyKeypad1:
		--offX; ++offY;
		break;

	case GUI::kKeyKeypad3:
		++offX; ++offY;
		break;

	default:
		break;
	}

	unsigned int playerX = _player.getX(), playerY = _player.getY();
	Monster *monster = _curLevel->monsterAt(playerX + offX, playerY + offY);
	if (monster) {
		if (Base::rollDice(20) == 20) {
			_messages.push_back("You fumble");
		} else {
			processEvent(createAttackEvent(&_player, monster));
		}
	} else if (_curLevel->isWalkable(playerX + offX, playerY + offY)) {
		processEvent(createMoveEvent(&_player, offX, offY));
	}
}

Monster *GameState::obtainMonster(const Monster *monster) {
	if (monster == &_player) {
		return &_player;
	} else {
		Level::MonsterList &monsters = _curLevel->getMonsters();
		Level::MonsterList::iterator i = std::find(monsters.begin(), monsters.end(), monster);
		if (i != monsters.end())
			return *i;

		return 0;
	}
}

void GameState::printMessages() {
	_messageLine->clear();

	std::string line;
	while (!_messages.empty()) {
		line.clear();
		while (!_messages.empty()) {
			std::string front = _messages.front();
			if (line.size() + front.size() > 80 || (_messages.size() > 1 && line.size() + front.size() > 70))
				break;

			_messages.pop_front();
			if (!line.empty())
				line += ' ';
			line += front;
		}

		if (!_messages.empty())
			line += " -- more --";

		_messageLine->printLine(line.c_str(), 0, 0);
		_screen.update();
		if (!_messages.empty())
			_input.poll();
	}
}

void GameState::drawStatsWindow() {
	std::stringstream line;

	// Stats line
	line << "Str: " << (int)_player.getAttribute(Monster::kAttribStrength)
	     << " Dex: " << (int)_player.getAttribute(Monster::kAttribDexterity)
	     << " Agi: " << (int)_player.getAttribute(Monster::kAttribAgility)
	     << " Wis: " << (int)_player.getAttribute(Monster::kAttribWisdom);

	_playerStats->clear();
	_playerStats->printLine(line.str().c_str(), 0, 0);

	line.str("");

	line << "HP: " << _player.getHitPoints() << "/" << _player.getMaxHitPoints();

	_playerStats->printLine(line.str().c_str(), 0, 1);
}

} // end of namespace Game

