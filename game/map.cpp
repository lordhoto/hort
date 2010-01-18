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

#include <cassert>
#include <algorithm>

#include "base/rnd.h"

namespace Game {

TileDatabase *TileDatabase::_instance = 0;

TileDatabase::TileDatabase()
    : _nextTileID(0), _tileDefinitions() {
}

void TileDatabase::load(const std::string &filename) {
	_nextTileID = 0;
	_tileDefinitions.clear();

	Base::FileParser::RuleMap rules;
	rules["def"] = Base::Rule("def-tile;%S,name;:=;%S,glyph;%D,isWalkable;%D,blocksSight;%D,isLiquid");
	Base::FileParser parser(filename, rules);
	parser.parse(this);
	if (!parser.wasSuccessful())
		throw std::string(parser.getError());
}

const TileDatabase::Definition *TileDatabase::queryTileDefinition(const Tile tile) const {
	TileDefMap::const_iterator i = _tileDefinitions.find(tile);
	if (i == _tileDefinitions.end())
		return 0;
	else
		return &i->second;
}

Tile TileDatabase::queryTile(const std::string &name) const {
	for (TileDefMap::const_iterator i = _tileDefinitions.begin(); i != _tileDefinitions.end(); ++i) {
		if (i->second._name == name)
			return i->first;
	}

	return getTileCount();
}

Tile TileDatabase::queryTile(const char glyph) const {
	for (TileDefMap::const_iterator i = _tileDefinitions.begin(); i != _tileDefinitions.end(); ++i) {
		if (i->second._glyph == glyph)
			return i->first;
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

void TileDatabase::notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) {
	assert(name == "def");

	Definition def;
	def._name = values.find("name")->second;
	def._glyph = values.find("glyph")->second[0];
	def._isWalkable = (values.find("isWalkable")->second[0] == '1');
	def._blocksSight = (values.find("blocksSight")->second[0] == '1');
	def._isLiquid = (values.find("isLiquid")->second[0] == '1');

	_tileDefinitions[_nextTileID++] = def;
}

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
	return _tileDefs[y * _w + x]->_isWalkable;
}

} // end of namespace Game

