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

namespace Game {

GameState::GameState() : _screen(GUI::Screen::instance()), _input(GUI::Input::instance()), _player(kMonsterPlayer, 8, 8, 8, 8, 10, 0, 0) {
	_initialized = false;
	_messageLine = _mapWindow = _playerStats = 0;
	_curMap = 0;
}

GameState::~GameState() {
	delete _curMap;
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
		_curMap = new Map();

		_messageLine = new GUI::Window(0,  0, 80,  1, false);
		_mapWindow = new GUI::Window(0,  1, 80, 20, false);
		_playerStats = new GUI::Window(0, 21, 80,  3, false);

		_screen.add(_messageLine);
		_screen.add(_mapWindow);
		_screen.add(_playerStats);

		_gameScreen = new GameScreen(*_mapWindow);
		_gameScreen->setMap(_curMap);
		_gameScreen->addObject(&_player, true);
	}

	_screen.clear();

	return true;
}

bool GameState::run() {
	int input = -1;

	do {
		_player.setX(Base::rollDice(_curMap->width()) - 1);
		_player.setY(Base::rollDice(_curMap->height()) - 1);
	} while (_curMap->tileAt(_player.getX(), _player.getY()) != Map::kTileMeadow);

	while (input != GUI::kKeyEscape) {
		_gameScreen->update();
		_screen.update();

		input = _input.poll();
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
		if (playerX + offX < _curMap->width() && playerY + offY < _curMap->height()) {
			if (_curMap->tileAt(playerX + offX, playerY + offY) == Map::kTileMeadow) {
				playerX += offX;
				playerY += offY;
			}
		}

		if (playerX != _player.getX() || playerY != _player.getY()) {
			_player.setX(playerX);
			_player.setY(playerY);
			_gameScreen->flagForUpdate();
		}
	}

	return true;
}

} // end of namespace Game

