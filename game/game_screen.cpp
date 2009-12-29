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

#include "game_screen.h"

#include "gui/screen.h"

namespace Game {

GameScreen::GameScreen(GUI::Window &window)
    : _output(window), _needRedraw(false), _map(0), _monsters(), _centerMonster(0) {
	_mapDrawDescs.push_back(DrawDesc('.', GUI::kGreenOnBlack, GUI::kAttribDim));
	_mapDrawDescs.push_back(DrawDesc('+', GUI::kGreenOnBlack, GUI::kAttribUnderline | GUI::kAttribBold));
	_mapDrawDescs.push_back(DrawDesc(kDiamond, GUI::kBlueOnBlack, GUI::kAttribBold));
}

void GameScreen::update() {
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
	for (unsigned int y = 0; y < maxHeight; ++y) {
		for (unsigned int x = 0; x < maxWidth; ++x) {
			const Map::Tile tile = _map->tileAt(x + mapOffsetX, y + mapOffsetY);
			const DrawDesc &desc = _mapDrawDescs[tile];
			_output.printChar(desc.symbol, x, y, desc.color, desc.attribs);
		}
	}

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

void GameScreen::setMap(const Map *map) {
	_map = map;
	flagForUpdate();
	clearObjects();
}

void GameScreen::addObject(const Monster *monster, bool center) {
	flagForUpdate();

	_monsters.push_back(monster);
	if (center)
		_centerMonster = monster;
}

void GameScreen::remObject(const Monster *monster) {
	flagForUpdate();
	_monsters.remove(monster);
	if (_centerMonster == monster)
		_centerMonster = 0;
}

void GameScreen::clearObjects() {
	flagForUpdate();
	_monsters.clear();
	_centerMonster = 0;
}

// Static data
const GameScreen::DrawDesc GameScreen::_monsterDrawDescriptions[] = {
	DrawDesc('@', GUI::kWhiteOnBlack, GUI::kAttribBold),
	DrawDesc('G', GUI::kYellowOnBlack, 0)
};

const size_t GameScreen::_monsterDrawDescriptionsEntries = sizeof(GameScreen::_monsterDrawDescriptions) / sizeof(GameScreen::_monsterDrawDescriptions[0]);

} // end of namespace Game

