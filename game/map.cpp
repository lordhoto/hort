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

#include "map.h"

#include "tiledatabase.h"

#include <cassert>

namespace Game {

Map::Map(unsigned int w, unsigned int h, const std::vector<Tile> &tiles)
    : _w(w), _h(h), _tiles(tiles), _tileDefs() {
	_tileDefs.resize(_w * _h);
	assert(_tiles.size() == _w * _h);

	for (unsigned int i = 0; i < _w * _h; ++i) {
		_tileDefs[i] = TileDatabase::instance().queryTileDefinition(_tiles[i]);
		assert(_tileDefs[i]);
	}
}

bool Map::isWalkable(unsigned int x, unsigned int y) const {
	return _tileDefs[y * _w + x]->getIsWalkable();
}

} // end of namespace Game

