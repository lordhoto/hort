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

#include "levelloader.h"
#include "maploader.h"
#include "monsterdatabase.h"

#include <fstream>
#include <cassert>

#include <boost/lexical_cast.hpp>

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

	unsigned int x, y;

	try {
		x = boost::lexical_cast<int>(values.find("x")->second);
		y = boost::lexical_cast<int>(values.find("y")->second);
	} catch (boost::bad_lexical_cast &e) {
		// This should never happen, since the values are
		// prechecked by the parser.
		assert(false);
	}

	try {
		const Base::Point pos(x, y);

		if (!_level->isWalkable(pos))
			throw Base::ParserListener::Exception("Position is blocked");

		MonsterDatabase &mdb = g_monsterDatabase;
		const MonsterType monType = mdb.queryMonsterType(type);
		if (monType >= mdb.getMonsterTypeCount())
			throw Base::ParserListener::Exception("Undefined monster type \"" + type + '"');

		_level->addMonster(monType, pos);
	} catch (std::out_of_range &e) {
		throw Base::ParserListener::Exception("Incorrect monster spawn point");
	}
}

void LevelLoader::processStartPoint(const Base::Matcher::ValueMap &values) {
	unsigned int x, y;
	try {
		x = boost::lexical_cast<int>(values.find("x")->second);
		y = boost::lexical_cast<int>(values.find("y")->second);
	} catch (boost::bad_lexical_cast &e) {
		// This should never happen, since the values are
		// prechecked by the parser.
		assert(false);
	}

	const Base::Point pos(x, y);

	try {
		if (!_level->isWalkable(pos))
			throw Base::ParserListener::Exception("Position is blocked");

		_start = pos;
	} catch (std::out_of_range &e) {
		throw Base::ParserListener::Exception("Start position parameter is out of range");
	}
}

} // end of namespace Game

