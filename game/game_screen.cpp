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
    : _output(window), _needRedraw(false), _level(0), _monsters(), _centerMonster(0) {
	_levelDrawDescs.push_back(DrawDesc('.', GUI::kGreenOnBlack, GUI::kAttribDim));
	_levelDrawDescs.push_back(DrawDesc('+', GUI::kGreenOnBlack, GUI::kAttribUnderline | GUI::kAttribBold));
	_levelDrawDescs.push_back(DrawDesc(kDiamond, GUI::kBlueOnBlack, GUI::kAttribBold));
}

void GameScreen::update() {
	if (!_needRedraw || !_level)
		return;
	_needRedraw = false;

	const unsigned int outputWidth = _output.width(), outputHeight = _output.height();
	const unsigned int levelWidth = _level->width(), levelHeight = _level->height();

	int levelOffsetX = 0, levelOffsetY = 0;

	if (_centerMonster) {
		levelOffsetX = _centerMonster->getX() - outputWidth / 2;
		levelOffsetY = _centerMonster->getY() - outputHeight / 2;

		levelOffsetX = std::max(levelOffsetX, 0);
		levelOffsetX = std::min<unsigned int>(levelOffsetX, levelWidth - outputWidth);

		levelOffsetY = std::max(levelOffsetY, 0);
		levelOffsetY = std::min<unsigned int>(levelOffsetY, levelHeight - outputHeight);
	}

	const unsigned int maxWidth = std::min(outputWidth, levelWidth), maxHeight = std::min(outputHeight, levelHeight);
	for (unsigned int y = 0; y < maxHeight; ++y) {
		for (unsigned int x = 0; x < maxWidth; ++x) {
			const Level::Tile tile = _level->tileAt(x + levelOffsetX, y + levelOffsetY);
			const DrawDesc &desc = _levelDrawDescs[tile];
			_output.printChar(desc.symbol, x, y, desc.color, desc.attribs);
		}
	}

	for (MonsterList::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		const unsigned int monsterX = (*i)->getX(), monsterY = (*i)->getY();

		if (monsterX < (unsigned int)levelOffsetX
		    || monsterY < (unsigned int)levelOffsetY
		    || monsterX >= (unsigned int)levelOffsetX + levelWidth
		    || monsterY >= (unsigned int)levelOffsetY + levelHeight)
			continue;

		const DrawDesc &desc = _monsterDrawDescriptions[(*i)->getType()];
		_output.printChar(desc.symbol, monsterX - levelOffsetX, monsterY - levelOffsetY, desc.color, desc.attribs);
	}

	if (_centerMonster)
		GUI::Screen::instance().setCursor(_output, _centerMonster->getX() - levelOffsetX, _centerMonster->getY() - levelOffsetY);
}

void GameScreen::setLevel(const Level *level) {
	_level = level;
	flagForUpdate();
	clearObjects();
}

void GameScreen::addObject(const Monster *monster, bool center) {
	_monsters.push_back(monster);
	if (center)
		_centerMonster = monster;
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

