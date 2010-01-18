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

#include "screen.h"

#include <cassert>
#include <sstream>

namespace GUI {

Screen::Screen(const Game::Monster &player)
    : _screen(GUI::Intern::Screen::instance()), _input(GUI::Intern::Input::instance()), _messageLine(0),
      _mapWindow(0), _playerStats(0), _keyMap(), _messages(), _turn(0), _player(player), _needRedraw(false),
      _map(0), _monsters(), _centerX(0), _centerY(0), _mapOffsetX(0), _mapOffsetY(0), _monsterDrawDescs(0),
      _mapDrawDescs(0) {
}

Screen::~Screen() {
	delete _monsterDrawDescs;
	delete _mapDrawDescs;

	_screen.remove(_messageLine);
	delete _messageLine;
	_screen.remove(_mapWindow);
	delete _mapWindow;
	_screen.remove(_playerStats);
	delete _playerStats;
}

void Screen::initialize() {
	if (!_mapDrawDescs) {
		_mapDrawDescs = Intern::parseTileDefinitons("./data/gui/tiles.def");
		_monsterDrawDescs = Intern::parseMonsterDefinitions("./data/gui/monster.def");
		createOutputWindows();
		setupKeyMap();
	}
}

void Screen::update(bool drawMsg) {
	if ((!_needRedraw || !_map) && !(drawMsg && !_messages.empty()))
		return;
	_needRedraw = false;

	drawStatsWindow();

	if (drawMsg)
		printMessages();

	const unsigned int outputWidth = _mapWindow->width(), outputHeight = _mapWindow->height();
	const unsigned int mapWidth = _map->width(), mapHeight = _map->height();

	const unsigned int maxWidth = std::min(outputWidth, mapWidth), maxHeight = std::min(outputHeight, mapHeight);
	for (unsigned int y = 0; y < maxHeight; ++y) {
		for (unsigned int x = 0; x < maxWidth; ++x) {
			const Game::Tile tile = _map->tileAt(x + _mapOffsetX, y + _mapOffsetY);
			const Intern::DrawDesc &desc = _mapDrawDescs->lookUp(tile);
			_mapWindow->printChar(desc.symbol, x, y, desc.color, desc.attribs);
		}
	}

	for (MonsterList::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		const unsigned int monsterX = (*i)->getX(), monsterY = (*i)->getY();

		if (monsterX < _mapOffsetX || monsterY < _mapOffsetY
		    || monsterX >= _mapOffsetX + outputWidth
		    || monsterY >= _mapOffsetY + outputHeight)
			continue;

		const Intern::DrawDesc &desc = _monsterDrawDescs->lookUp((*i)->getType());
		_mapWindow->printChar(desc.symbol, monsterX - _mapOffsetX, monsterY - _mapOffsetY, desc.color, desc.attribs);
	}

	_screen.setCursor(*_mapWindow, _centerX - _mapOffsetX, _centerY - _mapOffsetY);
	_screen.update();
}

void Screen::setCenter(unsigned int x, unsigned int y) {
	assert(x < _map->width());
	assert(y < _map->height());

	_centerX = x;
	_centerY = y;

	const unsigned int outputWidth = _mapWindow->width(), outputHeight = _mapWindow->height();
	const unsigned int mapWidth = _map->width(), mapHeight = _map->height();

	int offsetX = _centerX - outputWidth / 2;
	int offsetY = _centerY - outputHeight / 2;

	offsetX = std::max(offsetX, 0);
	offsetX = std::min(offsetX, (int)mapWidth - (int)outputWidth);
	_mapOffsetX = std::max(offsetX, 0);

	offsetY = std::max(offsetY, 0);
	offsetY = std::min(offsetY, (int)mapHeight - (int)outputHeight);
	_mapOffsetY = std::max(offsetY, 0);

	_needRedraw = true;
}

void Screen::setMap(const Game::Map *map) {
	_map = map;
	flagForUpdate();
	clearObjects();
}

void Screen::addObject(const Game::Monster *monster) {
	flagForUpdate();
	remObject(monster);

	_monsters.push_back(monster);
}

void Screen::remObject(const Game::Monster *monster) {
	flagForUpdate();
	_monsters.remove(monster);
}

void Screen::clearObjects() {
	flagForUpdate();
	_monsters.clear();
}

Input Screen::getInput() {
	while (true) {
		int key = getKey();
		KeyMap::const_iterator i = _keyMap.find(key);
		if (i != _keyMap.end())
			return i->second;
	}
}

int Screen::getKey() {
	int input = 0;

	do {
		input = _input.poll();

		if (input == Intern::kNotifyResize) {
			createOutputWindows();
			setCenter(_centerX, _centerY);
			update();

			input = 0;
		} else if (input == Intern::kNotifyError) {
			input = 0;
		}
	} while (!input);

	return input;
}

void Screen::setupKeyMap() {
	_keyMap[kKeyKeypad1] = kInputDir1;
	_keyMap['b'] = kInputDir1;

	_keyMap[kKeyKeypad2] = kInputDir2;
	_keyMap['j'] = kInputDir2;

	_keyMap[kKeyKeypad3] = kInputDir3;
	_keyMap['n'] = kInputDir3;

	_keyMap[kKeyKeypad4] = kInputDir4;
	_keyMap['h'] = kInputDir4;

	_keyMap[kKeyKeypad5] = kInputDir5;
	_keyMap['.'] = kInputDir5;

	_keyMap[kKeyKeypad6] = kInputDir6;
	_keyMap['l'] = kInputDir6;

	_keyMap[kKeyKeypad7] = kInputDir7;
	_keyMap['z'] = kInputDir7;
	_keyMap['y'] = kInputDir7;

	_keyMap[kKeyKeypad8] = kInputDir8;
	_keyMap['k'] = kInputDir8;

	_keyMap[kKeyKeypad9] = kInputDir9;
	_keyMap['u'] = kInputDir9;

	_keyMap['/'] = kInputExamine;
	_keyMap[kKeyEscape] = kInputQuit;

	_keyMap[' '] = kInputNone;
}

void Screen::createOutputWindows() {
	_screen.remove(_messageLine);
	delete _messageLine;
	_screen.remove(_mapWindow);
	delete _mapWindow;
	_screen.remove(_playerStats);
	delete _playerStats;

	assert(_screen.width() >= 80 && _screen.height() >= 24);

	_messageLine = new GUI::Intern::Window(0, 0, _screen.width(), 1, false);
	assert(_messageLine);

	_mapWindow = new GUI::Intern::Window(0, 1, _screen.width(), _screen.height() - 2, false);
	assert(_mapWindow);

	_playerStats = new GUI::Intern::Window(0, _screen.height() - 1, _screen.width(), 1, false);
	assert(_playerStats);

	_screen.add(_messageLine);
	_screen.add(_mapWindow);
	_screen.add(_playerStats);

	_needRedraw = true;
}

void Screen::addToMsgWindow(const std::string &str) {
	_messages.push_back(str);
	_needRedraw = true;
}

void Screen::printMessages() {
	_messageLine->clear();
	std::string line;

	while (!_messages.empty()) {
		_messageLine->clear();
		line.clear();
		while (!_messages.empty()) {
			std::string front = _messages.front();
			if (!line.empty() && front.size() < _messageLine->width()) {
				if (line.size() + front.size() > _messageLine->width() || (_messages.size() > 1 && line.size() + front.size() > _messageLine->width() - 10))
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

void Screen::setTurn(unsigned int turn) {
	if (_turn != turn) {
		_turn = turn;
		_needRedraw = true;
	}
}

void Screen::drawStatsWindow() {
	std::stringstream line;

	// Stats line
	line << "Str: " << (int)_player.getAttribute(Game::kAttribStrength)
	     << " Dex: " << (int)_player.getAttribute(Game::kAttribDexterity)
	     << " Agi: " << (int)_player.getAttribute(Game::kAttribAgility)
	     << " Wis: " << (int)_player.getAttribute(Game::kAttribWisdom)
	     << " | " << "HP: " << _player.getHitPoints() << "/" << _player.getMaxHitPoints()
	     << " | T: " << _turn;

	_playerStats->clear();
	_playerStats->printLine(line.str().c_str(), 0, 0);
}

} // end of namespace Game

