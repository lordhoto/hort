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
#include "maploader.h"

#include <fstream>
#include <cassert>

namespace Game {

LevelLoader::LevelLoader(const std::string &path)
    : _path(path), _level(0) {
}

Level *LevelLoader::load(GameState &gs) throw (Base::NonRecoverableException) {
	MapLoader *mapLoader = new MapLoader(_path + "/map.def");
	assert(mapLoader);

	Map *map = mapLoader->load();
	delete mapLoader;

	_level = new Level(map, gs);
	assert(_level);

	Base::FileParser::RuleMap rules;
	try {
		rules["monster"] = Base::Rule("def-monster;%S,type;%D,x;%D,y");
		rules["start-point"] = Base::Rule("def-start-point;%D,x;%D,y");

		Base::FileParser parser(_path + "/objects.def", rules);
		parser.parse(this);
	} catch (Base::Rule::InvalidRuleDefinitionException &e) {
		throw Base::NonRecoverableException(e.toString());
	} catch (Base::Exception &e) {
		// TODO: More information is preferable
		throw Base::NonRecoverableException(e.toString());
	}

	Level *level = _level;
	level->_start = _start;
	_level = 0;
	return level;
}

void LevelLoader::notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception) {
	if (name == "monster")
		processMonster(values);
	else if (name == "start-point")
		processStartPoint(values);
	else
		throw Base::ParserListener::Exception("Unknown rule \"" + name + "\"");
}

void LevelLoader::processMonster(const Base::Matcher::ValueMap &values) {
	const std::string &type = values.find("type")->second;
	const unsigned int x = ::atoi(values.find("x")->second.c_str());
	const unsigned int y = ::atoi(values.find("y")->second.c_str());
	const Base::Point pos(x, y);

	if (!_level->isWalkable(pos))
		throw Base::ParserListener::Exception("Position is blocked");

	MonsterDatabase &mdb = MonsterDatabase::instance();
	const MonsterType monType = mdb.queryMonsterType(type);
	if (monType >= mdb.getMonsterTypeCount())
		throw Base::ParserListener::Exception("Undefined monster type \"" + type + '"');

	_level->addMonster(monType, pos);
}

void LevelLoader::processStartPoint(const Base::Matcher::ValueMap &values) {
	const unsigned int x = ::atoi(values.find("x")->second.c_str());
	const unsigned int y = ::atoi(values.find("y")->second.c_str());
	const Base::Point pos(x, y);

	if (!_level->isWalkable(pos))
		throw Base::ParserListener::Exception("Position is blocked");

	_start = pos;
}

} // end of namespace Game

