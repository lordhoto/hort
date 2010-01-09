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

namespace Game {

Screen::Screen(GUI::Window &window)
    : _output(window), _needRedraw(false), _map(0), _outputMapCache(0), _monsters(), _centerMonster(0) {
	_mapDrawDescs.push_back(DrawDesc('.', GUI::kGreenOnBlack, GUI::kAttribDim));
	_mapDrawDescs.push_back(DrawDesc('+', GUI::kGreenOnBlack, GUI::kAttribUnderline | GUI::kAttribBold));
	_mapDrawDescs.push_back(DrawDesc(kDiamond, GUI::kBlueOnBlack, GUI::kAttribBold));
}

void Screen::update() {
	if (!_needRedraw || !_map)
		return;
	_needRedraw = false;

	const unsigned int outputWidth = _output.width(), outputHeight = _output.height();
	const unsigned int mapWidth = _map->width(), mapHeight = _map->height();

	int mapOffsetX = 0, mapOffsetY = 0;

	if (_centerMonster) {
		mapOffsetX = _centerMonster->getX() - outputWidth / 2;
		mapOffsetY = _centerMonster->getY() - outputHeight / 2;

		mapOffsetX = std::max(mapOffsetX, 0);
		mapOffsetX = std::min<unsigned int>(mapOffsetX, mapWidth - outputWidth);

		mapOffsetY = std::max(mapOffsetY, 0);
		mapOffsetY = std::min<unsigned int>(mapOffsetY, mapHeight - outputHeight);
	}

	const unsigned int maxWidth = std::min(outputWidth, mapWidth), maxHeight = std::min(outputHeight, mapHeight);
	_output.putData(0, 0, maxWidth, maxHeight, _outputMapCache + mapOffsetY * mapWidth + mapOffsetX, mapWidth);

	for (MonsterList::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		const unsigned int monsterX = (*i)->getX(), monsterY = (*i)->getY();

		if (monsterX < (unsigned int)mapOffsetX
		    || monsterY < (unsigned int)mapOffsetY
		    || monsterX >= (unsigned int)mapOffsetX + mapWidth
		    || monsterY >= (unsigned int)mapOffsetY + mapHeight)
			continue;

		const DrawDesc &desc = _monsterDrawDescriptions[(*i)->getType()];
		_output.printChar(desc.symbol, monsterX - mapOffsetX, monsterY - mapOffsetY, desc.color, desc.attribs);
	}

	if (_centerMonster)
		GUI::Screen::instance().setCursor(_output, _centerMonster->getX() - mapOffsetX, _centerMonster->getY() - mapOffsetY);
}

void Screen::setMap(const Map *map) {
	delete[] _outputMapCache;
	_map = map;

	if (_map) {
		_outputMapCache = new int[_map->width() * _map->height()];
		assert(_outputMapCache);

		const unsigned int width = _map->width(), height = _map->height();
		for (unsigned int y = 0; y < height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				const Map::Tile tile = _map->tileAt(x, y);
				const DrawDesc &desc = _mapDrawDescs[tile];
				_outputMapCache[y * width + x] = GUI::Window::getCharData(desc.symbol, desc.color, desc.attribs);
			}
		}
	}

	flagForUpdate();
	clearObjects();
}

void Screen::addObject(const Monster *monster, bool center) {
	flagForUpdate();

	_monsters.push_back(monster);
	if (center)
		_centerMonster = monster;
}

void Screen::remObject(const Monster *monster) {
	flagForUpdate();
	_monsters.remove(monster);
	if (_centerMonster == monster)
		_centerMonster = 0;
}

void Screen::clearObjects() {
	flagForUpdate();
	_monsters.clear();
	_centerMonster = 0;
}

// Static data
const Screen::DrawDesc Screen::_monsterDrawDescriptions[] = {
	DrawDesc('@', GUI::kWhiteOnBlack, GUI::kAttribBold),
	DrawDesc('G', GUI::kYellowOnBlack, 0)
};

const size_t Screen::_monsterDrawDescriptionsEntries = sizeof(Screen::_monsterDrawDescriptions) / sizeof(Screen::_monsterDrawDescriptions[0]);

} // end of namespace Game
