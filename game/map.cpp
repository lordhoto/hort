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

#include "map.h"

#include <cassert>
#include <algorithm>

#include "base/rnd.h"

namespace Game {

Map::Map() : _w(160), _h(48) {
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

bool Map::isWalkable(unsigned int x, unsigned int y) const {
	return (tileAt(x, y) != kTileTree);
}

const char *Map::queryTileName(Tile t) {
	static const char *tileNames[] = {
		"meadow",
		"tree",
		"water"
	};

	return tileNames[t];
}

} // end of namespace Game

