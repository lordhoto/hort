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

#include "screen.h"
#include "gui/screen.h"

#include <cassert>
#include <sstream>

namespace Game {

Screen::Screen(const Monster &player)
    : _screen(GUI::Screen::instance()), _input(GUI::Input::instance()), _messageLine(0), _mapWindow(0),
      _playerStats(0), _messages(), _turn(0), _player(player), _needRedraw(false), _map(0), _monsters(),
      _centerX(0), _centerY(0), _mapOffsetX(0), _mapOffsetY(0) {
	_mapDrawDescs.push_back(DrawDesc('.', GUI::kGreenOnBlack, GUI::kAttribDim));
	_mapDrawDescs.push_back(DrawDesc('+', GUI::kGreenOnBlack, GUI::kAttribUnderline | GUI::kAttribBold));
	_mapDrawDescs.push_back(DrawDesc(kDiamond, GUI::kBlueOnBlack, GUI::kAttribBold));

	createOutputWindows();
}

Screen::~Screen() {
	_screen.remove(_messageLine);
	delete _messageLine;
	_screen.remove(_mapWindow);
	delete _mapWindow;
	_screen.remove(_playerStats);
	delete _playerStats;
}

void Screen::update(bool drawMsg) {
	if (!_needRedraw || !_map)
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
			const Map::Tile tile = _map->tileAt(x + _mapOffsetX, y + _mapOffsetY);
			const DrawDesc &desc = _mapDrawDescs[tile];
			_mapWindow->printChar(desc.symbol, x, y, desc.color, desc.attribs);
		}
	}

	for (MonsterList::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		const unsigned int monsterX = (*i)->getX(), monsterY = (*i)->getY();

		if (monsterX < _mapOffsetX || monsterY < _mapOffsetY
		    || monsterX >= _mapOffsetX + outputWidth
		    || monsterY >= _mapOffsetY + outputHeight)
			continue;

		const DrawDesc &desc = _monsterDrawDescriptions[(*i)->getType()];
		_mapWindow->printChar(desc.symbol, monsterX - _mapOffsetX, monsterY - _mapOffsetY, desc.color, desc.attribs);
	}

	GUI::Screen::instance().setCursor(*_mapWindow, _centerX - _mapOffsetX, _centerY - _mapOffsetY);
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

void Screen::setMap(const Map *map) {
	_map = map;
	flagForUpdate();
	clearObjects();
}

void Screen::addObject(const Monster *monster) {
	flagForUpdate();
	remObject(monster);

	_monsters.push_back(monster);
}

void Screen::remObject(const Monster *monster) {
	flagForUpdate();
	_monsters.remove(monster);
}

void Screen::clearObjects() {
	flagForUpdate();
	_monsters.clear();
}

void Screen::createOutputWindows() {
	_screen.remove(_messageLine);
	delete _messageLine;
	_screen.remove(_mapWindow);
	delete _mapWindow;
	_screen.remove(_playerStats);
	delete _playerStats;

	_messageLine = new GUI::Window(0, 0, _screen.width(), 1, false);
	assert(_messageLine);

	_mapWindow = new GUI::Window(0, 1, _screen.width(), _screen.height() - 2, false);
	assert(_mapWindow);

	_playerStats = new GUI::Window(0, _screen.height() - 1, _screen.width(), 1, false);
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
	line << "Str: " << (int)_player.getAttribute(Monster::kAttribStrength)
	     << " Dex: " << (int)_player.getAttribute(Monster::kAttribDexterity)
	     << " Agi: " << (int)_player.getAttribute(Monster::kAttribAgility)
	     << " Wis: " << (int)_player.getAttribute(Monster::kAttribWisdom)
	     << " | " << "HP: " << _player.getHitPoints() << "/" << _player.getMaxHitPoints()
	     << " | T: " << _turn;

	_playerStats->clear();
	_playerStats->printLine(line.str().c_str(), 0, 0);
}

// Static data
const Screen::DrawDesc Screen::_monsterDrawDescriptions[] = {
	DrawDesc('@', GUI::kWhiteOnBlack, GUI::kAttribBold),
	DrawDesc('G', GUI::kYellowOnBlack, 0),
	DrawDesc('@', GUI::kBlueOnBlack, GUI::kAttribBold)
};

const size_t Screen::_monsterDrawDescriptionsEntries = sizeof(Screen::_monsterDrawDescriptions) / sizeof(Screen::_monsterDrawDescriptions[0]);

} // end of namespace Game

