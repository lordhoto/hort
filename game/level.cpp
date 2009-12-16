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

#include <cassert>
#include <algorithm>

#include "base/rnd.h"

namespace Game {

Level::Level() : _w(160), _h(48) {
	_tiles.resize(_w * _h);
	for (unsigned int i = 0; i < _w * _h; ++i) {
		switch (Base::rollDice(100)) {
		case 1:
			_tiles[i] = kTileWater;
			break;

		case 2: case 3: case 4:
		case 5: case 6: case 7:
			_tiles[i] = kTileTree;
			break;

		default:
			_tiles[i] = kTileMeadow;
			break;
		}
	}
}

void Level::draw(GUI::Window &out, unsigned int oX, unsigned int oY) const {
	assert(oX < _w);
	assert(oY < _h);
	draw(out, oX, oY, 0, 0, std::min(out.width(), _w - oX), std::min(out.height(), _h - oY));
}

void Level::draw(GUI::Window &out, unsigned int oX, unsigned int oY, unsigned int x, unsigned int y, unsigned int w, unsigned int h) const {
	assert(oX < _w);
	assert(oY < _h);
	assert(oX + w <= _w);
	assert(oY + h <= _h);
	assert(x + w <= out.width());
	assert(y + h <= out.width());

	while (y < h) {
		for (unsigned int curW = 0; curW < w; ++curW)
			printTile(out, _tiles[(y + oY) * _w + (x + curW + oX)], x + curW, y);
		++y;
	}
}

void Level::printTile(GUI::Window &out, const Tile t, unsigned int x, unsigned int y) const {
	switch (t) {
	case kTileMeadow:
		out.printChar('.', x, y, GUI::kGreenOnBlack, GUI::kAttribDim);
		break;

	case kTileTree:
		out.printChar('+', x, y, GUI::kGreenOnBlack, GUI::kAttribUnderline | GUI::kAttribBold);
		break;

	case kTileWater:
		out.printChar(kDiamond, x, y, GUI::kBlueOnBlack, GUI::kAttribBold);
		break;
	}
}

} // end of namespace Game

