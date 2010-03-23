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

#include "tiledatabase.h"
#include "tiledefinitionloader.h"

#include <boost/foreach.hpp>

namespace Game {

TileDatabase *TileDatabase::_instance = 0;

TileDatabase::TileDatabase()
    : _nextTileID(0), _tileDefinitions() {
}

void TileDatabase::load(const std::string &filename) throw (Base::NonRecoverableException) {
	_nextTileID = 0;
	_tileDefinitions.clear();

	TileDefinitionLoader loader;
	TileDefinitionLoader::TileDefinitionList tiles = loader.load(filename);

	BOOST_FOREACH(TileDefinition &def, tiles)
		_tileDefinitions[_nextTileID++] = def;
}

const TileDefinition *TileDatabase::queryTileDefinition(const Tile tile) const {
	TileDefMap::const_iterator i = _tileDefinitions.find(tile);
	if (i == _tileDefinitions.end())
		return 0;
	else
		return &i->second;
}

Tile TileDatabase::queryTile(const std::string &name) const {
	BOOST_FOREACH(const TileDefMap::value_type &i, _tileDefinitions) {
		if (i.second.getName() == name)
			return i.first;
	}

	return getTileCount();
}

Tile TileDatabase::queryTile(const char glyph) const {
	BOOST_FOREACH(const TileDefMap::value_type &i, _tileDefinitions) {
		if (i.second.getGlyph() == glyph)
			return i.first;
	}

	return getTileCount();
}

TileDatabase &TileDatabase::instance() {
	if (!_instance)
		_instance = new TileDatabase();
	return *_instance;
}

void TileDatabase::destroy() {
	delete _instance;
	_instance = 0;
}

} // end of namespace Game

