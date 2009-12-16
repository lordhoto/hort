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

namespace Game {

GameState::GameState() : _screen(GUI::Screen::instance()), _input(GUI::Input::instance()) {
	_initialized = false;
	_messageLine = _levelWindow = _playerStats = 0;
	_curLevel = 0;
}

GameState::~GameState() {
	delete _curLevel;

	_screen.remove(_messageLine);
	delete _messageLine;
	_screen.remove(_levelWindow);
	delete _levelWindow;
	_screen.remove(_playerStats);
	delete _playerStats;
}

bool GameState::initialize() {
	if (!_initialized) {
		_initialized = true;
		_curLevel = new Level();

		_messageLine = new GUI::Window(0,  0, 80,  1, false);
		_levelWindow = new GUI::Window(0,  1, 80, 20, false);
		_playerStats = new GUI::Window(0, 21, 80,  3, false);

		_screen.add(_messageLine);
		_screen.add(_levelWindow);
		_screen.add(_playerStats);
	}

	_screen.clear();

	return true;
}

bool GameState::run() {
	bool redraw = true;
	int input = -1;
	int offsetX = 0, offsetY = 0;

	while (input != GUI::kKeyEscape) {
		if (redraw) {
			offsetX = _player.getX() - (_levelWindow->width() / 2);
			offsetY = _player.getY() - (_levelWindow->height() / 2);

			// Calculate the center
			if (offsetX < 0)
				offsetX = 0;
			else if ((unsigned int)offsetX > _curLevel->width() - _levelWindow->width())
				offsetX = _curLevel->width() - _levelWindow->width();

			if (offsetY < 0)
				offsetY = 0;
			else if ((unsigned int)offsetY > _curLevel->height() - _levelWindow->height())
				offsetY = _curLevel->height() - _levelWindow->height();

			_curLevel->draw(*_levelWindow, offsetX, offsetY);
			_levelWindow->printChar(_player.getSymbol(), _player.getX() - offsetX, _player.getY() - offsetY, GUI::kWhiteOnBlack, GUI::kAttribBold);
			_screen.setCursor(*_levelWindow, _player.getX() - offsetX, _player.getY() - offsetY);
			_screen.update();
			redraw = false;
		}

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
		if (playerX + offX < _curLevel->width() && playerY + offY < _curLevel->height()) {
			Level::Tile newTile = _curLevel->tileAt(playerX + offX, playerY + offY);
			switch (newTile) {
			case Level::kTileWater:
				_curLevel->draw(*_levelWindow, offsetX, offsetY);
				_screen.setCursor(*_levelWindow, playerX + offX - offsetX, playerY + offY - offsetY);
				_messageLine->printLine("You drown... You died -- Press any key to quit --", 0, 0);
				_screen.update();
				_input.poll();
				input = GUI::kKeyEscape;
				break;

			case Level::kTileMeadow:
				playerX += offX;
				playerY += offY;
				break;

			default:
				break;
			}
		}

		if (playerX != _player.getX() || playerY != _player.getY()) {
			_player.setX(playerX);
			_player.setY(playerY);
			redraw = true;
		}
	}

	return true;
}

} // end of namespace Game

