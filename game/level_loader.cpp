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

#include "level_loader.h"

#include <fstream>
#include <sstream>
#include <cassert>
#include <stdlib.h> // for atoi

namespace Game {

MapLoader::MapLoader(const std::string &filename)
    : _filename(filename), _lines() {
	std::ifstream in(filename.c_str());

	while (!in.eof()) {
		std::string line;
		std::getline(in, line);
		_lines.push_back(line);
	}
}

Map *MapLoader::load() {
	if (_lines.empty() || _lines.size() < 3)
		throwError("Contains too few lines", 0);

	int lineCount = 2;

	TileDatabase &tdb = TileDatabase::instance();
	const unsigned int w = ::atoi(_lines.front().c_str()); _lines.pop_front();
	const unsigned int h = ::atoi(_lines.front().c_str()); _lines.pop_front();

	if (_lines.size() < h)
		throwError("Height exceeds remaining lines", lineCount);

	std::vector<Tile> tiles;
	tiles.resize(w * h);

	const Tile lastValidTile = tdb.getTileCount();

	for (unsigned int y = 0; y < h; ++y) {
		if (_lines.empty())
			throwError("Unexpected end of file", lineCount);

		std::string line = _lines.front(); _lines.pop_front();
		++lineCount;

		if (line.empty()) {
			--y;
			continue;
		}

		if (line.size() < w)
			throwError("Unexpected end of line", lineCount);

		for (unsigned int x = 0; x < w; ++x) {
			const Tile tile = tdb.queryTile(line[x]);
			if (tile >= lastValidTile)
				throwError("Undefined tile glyph \"" + line[x] + '"', lineCount);

			tiles[y * w + x] = tile;
		}
	}

	return new Map(w, h, tiles);
}

void MapLoader::throwError(const std::string &error, int line) {
	std::stringstream ss;
	ss << "File: " << _filename << " Line: " << line << " ERROR: " << error;
	throw ss.str();
}

LevelLoader::LevelLoader(const std::string &path)
    : _path(path), _level(0) {
}

Level *LevelLoader::load(GameState &gs) {
	MapLoader *mapLoader = new MapLoader(_path + "/map.def");
	assert(mapLoader);

	Map *map = mapLoader->load();
	delete mapLoader;

	_level = new Level(map, gs);
	assert(_level);

	Base::FileParser::RuleMap rules;
	rules["monster"] = Base::Rule("def-monster;%S,type;%D,x;%D,y");
	rules["start-point"] = Base::Rule("def-start-point;%D,x;%D,y");
	Base::FileParser parser(_path + "/objects.def", rules);
	parser.parse(this);
	if (!parser.wasSuccessful())
		throw parser.getError();

	Level *level = _level;
	level->_start = _start;
	_level = 0;
	return level;
}

void LevelLoader::notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) {
	if (name == "monster")
		processMonster(values);
	else if (name == "start-point")
		processStartPoint(values);
}

void LevelLoader::processMonster(const Base::Matcher::ValueMap &values) {
	const std::string &type = values.find("type")->second;
	const unsigned int x = ::atoi(values.find("x")->second.c_str());
	const unsigned int y = ::atoi(values.find("y")->second.c_str());
	const Base::Point pos(x, y);

	if (!_level->isWalkable(pos))
		throw std::string("Position is blocked");

	MonsterDatabase &mdb = MonsterDatabase::instance();
	const MonsterType monType = mdb.queryMonsterType(type);
	if (monType >= mdb.getMonsterTypeCount())
		throw std::string("Undefined monster type \"" + type + '"');

	_level->addMonster(monType, pos);
}

void LevelLoader::processStartPoint(const Base::Matcher::ValueMap &values) {
	const unsigned int x = ::atoi(values.find("x")->second.c_str());
	const unsigned int y = ::atoi(values.find("y")->second.c_str());
	const Base::Point pos(x, y);

	if (!_level->isWalkable(pos))
		throw std::string("Position is blocked");

	_start = pos;
}

} // end of namespace Game

