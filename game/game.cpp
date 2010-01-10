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

namespace Game {

GameState::GameState() : _screen(GUI::Screen::instance()), _input(GUI::Input::instance()), _player(kMonsterPlayer, 8, 8, 8, 8, 10, 0, 0) {
	_initialized = false;
	_messageLine = _mapWindow = _playerStats = 0;
	_curLevel = 0;
	_eventDisp = 0;
}

GameState::~GameState() {
	delete _curLevel;
	delete _gameScreen;

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
		_mapWindow = new GUI::Window(0,  1, 80, 22, false);
		_playerStats = new GUI::Window(0, 23, 80,  1, false);

		_screen.add(_messageLine);
		_screen.add(_mapWindow);
		_screen.add(_playerStats);

		_gameScreen = new Screen(*_mapWindow);
		_curLevel->makeActive(*_gameScreen, *this, _player);
	}

	_screen.clear();

	return true;
}

bool GameState::run() {
	int input = -1;

	int playerX = 0, playerY = 0;
	do {
		playerX = _curLevel->getMap().width() - 1;
		playerY = _curLevel->getMap().height() - 1;
	} while (!_curLevel->isWalkable(playerX, playerY));
	_player.setX(playerX);
	_player.setY(playerY);

	_gameScreen->update();
	drawStatsWindow();
	_screen.update();

	while (input != GUI::kKeyEscape) {
		_messages.clear();
		_screen.update();

		input = _input.poll();
		handleInput(input);

		_curLevel->update();
		_gameScreen->update();
		drawStatsWindow();
		printMessages();

		if (_player.getHitPoints() <= 0) {
			_screen.update();
			_input.poll();
			break;
		}
	}

	return true;
}

void GameState::processEvent(const Event &event) {
	if (event.type == Event::kTypeAttack) {
		if (event.data.attack.target == kPlayerMonsterID) {
			if (_player.getHitPoints() <= 0)
				_messages.push_back("You die...");
			else
				_messages.push_back("The Gnome hits you!");
		} else {
			if (_player.getHitPoints() <= 0)
				_messages.push_back("You kill the Gnome!");
			else
				_messages.push_back("You hit the Gnome!");
		}
	}
}

void GameState::handleInput(int input) {
	if (!_eventDisp)
		return;

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

	default:
		return;
	}

	unsigned int playerX = _player.getX(), playerY = _player.getY();
	MonsterID monster = _curLevel->monsterAt(playerX + offX, playerY + offY);
	if (monster != kInvalidMonsterID && monster != kPlayerMonsterID) {
		if (Base::rollDice(20) == 20) {
			_messages.push_back("You fumble.");
		} else {
			_eventDisp->dispatch(createAttackEvent(kPlayerMonsterID, monster));
		}
	} else if (_curLevel->isWalkable(playerX + offX, playerY + offY)) {
		_eventDisp->dispatch(createMoveEvent(kPlayerMonsterID, &_player, offX, offY));
	}
}

Monster *GameState::obtainMonster(const MonsterID monster) {
	return _curLevel->getMonster(monster);
}

void GameState::printMessages() {
	_messageLine->clear();

	std::string line;
	while (!_messages.empty()) {
		line.clear();
		while (!_messages.empty()) {
			std::string front = _messages.front();
			if (!line.empty() && front.size() < 80) {
				if (line.size() + front.size() > 80 || (_messages.size() > 1 && line.size() + front.size() > 70))
					break;
			}

			_messages.pop_front();
			if (!line.empty())
				line += ' ';
			line += front;
		}

		if (!_messages.empty())
			line += " -- more --";

		_messageLine->printLine(line.c_str(), 0, 0);
		if (!_messages.empty()) {
			_screen.update();
			_input.poll();
		}
	}
}

void GameState::drawStatsWindow() {
	std::stringstream line;

	// Stats line
	line << "Str: " << (int)_player.getAttribute(Monster::kAttribStrength)
	     << " Dex: " << (int)_player.getAttribute(Monster::kAttribDexterity)
	     << " Agi: " << (int)_player.getAttribute(Monster::kAttribAgility)
	     << " Wis: " << (int)_player.getAttribute(Monster::kAttribWisdom)
	     << " | " << "HP: " << _player.getHitPoints() << "/" << _player.getMaxHitPoints();

	_playerStats->clear();
	_playerStats->printLine(line.str().c_str(), 0, 0);
}

} // end of namespace Game

