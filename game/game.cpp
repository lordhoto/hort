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
	unsigned int offX = 0, offY = 0;

	bool redraw = true;
	int input = -1;
	while (input != GUI::kKeyEscape) {
		if (redraw) {
			_curLevel->draw(*_levelWindow, offX, offY);
			_screen.update();
			redraw = false;
		}

		input = _input.poll();
		switch (input) {
		case GUI::kKeyKeypad4:
			if (offX > 0) {
				--offX;
				redraw = true;
			}
			break;

		case GUI::kKeyKeypad6:
			if (_curLevel->width() - offX > _levelWindow->width()) {
				++offX;
				redraw = true;
			}
			break;

		case GUI::kKeyKeypad8:
			if (offY > 0) {
				--offY;
				redraw = true;
			}
			break;

		case GUI::kKeyKeypad2:
			if (_curLevel->height() - offY > _levelWindow->height()) {
				++offY;
				redraw = true;
			}
			break;

		case GUI::kKeyKeypad7:
			if (offX > 0 && offY > 0) {
				--offX; --offY;
				redraw = true;
			}
			break;

		case GUI::kKeyKeypad9:
			if (_curLevel->width() - offX > _levelWindow->width() && offY > 0) {
				++offX; --offY;
				redraw = true;
			}
			break;

		case GUI::kKeyKeypad1:
			if (offX > 0 && _curLevel->height() - offY > _levelWindow->height()) {
				--offX; ++offY;
				redraw = true;
			}
			break;

		case GUI::kKeyKeypad3:
			if (_curLevel->width() - offX > _levelWindow->width() && _curLevel->height() - offY > _levelWindow->height()) {
				++offX; ++offY;
				redraw = true;
			}
			break;

		default:
			break;
		}
	}

	return true;
}

} // end of namespace Game

