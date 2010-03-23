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

#include "monster.h"

#include "base/rnd.h"

#include <cassert>

#include <boost/lexical_cast.hpp>

namespace Game {

void MonsterDatabase::load(const std::string &filename) throw (Base::NonRecoverableException) {
	_monsterDefs.clear();
	_monsterNames.clear();
	_nextMonsterType = 0;

	Base::FileParser::RuleMap rules;

	try {
		rules["def"] = Base::Rule("def-monster;%S,name;:=;%D,wisMin;%D,wisMax;%D,dexMin;%D,dexMax;%D,agiMin;%D,agiMax;%D,strMin;%D,strMax;%D,hpMin;%D,hpMax;%D,speed");

		Base::FileParser parser(filename, rules);
		parser.parse(this);
	} catch (Base::Rule::InvalidRuleDefinitionException &e) {
		throw Base::NonRecoverableException(e.toString());
	} catch (Base::Exception &e) {
		// TODO: More information is preferable
		throw Base::NonRecoverableException(e.toString());
	}
}

Monster *MonsterDatabase::createNewMonster(const MonsterType type) const {
	MonsterDefMap::const_iterator i = _monsterDefs.find(type);
	if (i == _monsterDefs.end())
		return 0;

	const MonsterDefinition &def = i->second;
	const unsigned char wis = Base::rndValueRange(def.getDefaultAttribs(kAttribWisdom).min(), def.getDefaultAttribs(kAttribWisdom).max());
	const unsigned char dex = Base::rndValueRange(def.getDefaultAttribs(kAttribDexterity).min(), def.getDefaultAttribs(kAttribDexterity).max());
	const unsigned char agi = Base::rndValueRange(def.getDefaultAttribs(kAttribAgility).min(), def.getDefaultAttribs(kAttribAgility).max());
	const unsigned char str = Base::rndValueRange(def.getDefaultAttribs(kAttribStrength).min(), def.getDefaultAttribs(kAttribStrength).max());
	const int hp = Base::rndValueRange(def.getDefaultHitPoints().min(), def.getDefaultHitPoints().max());

	return new Monster(type, wis, dex, agi, str, hp, def.getDefaultSpeed(), 0, 0);
}

const char *MonsterDatabase::getMonsterName(const MonsterType type) const {
	MonsterDefMap::const_iterator i = _monsterDefs.find(type);
	if (i == _monsterDefs.end())
		return 0;
	else
		return i->second.getName().c_str();
}

MonsterType MonsterDatabase::queryMonsterType(const std::string &name) const {
	MonsterNameMap::const_iterator i = _monsterNames.find(name);
	if (i == _monsterNames.end())
		return getMonsterTypeCount();
	else
		return i->second;
}

MonsterDatabase &MonsterDatabase::instance() {
	if (!_instance)
		_instance = new MonsterDatabase();
	return *_instance;
}

void MonsterDatabase::destroy() {
	delete _instance;
	_instance = 0;
}

MonsterDatabase::MonsterDatabase()
    : _nextMonsterType(0), _monsterDefs(), _monsterNames() {
}

void MonsterDatabase::notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception) {
	if (name != "def")
		throw Base::ParserListener::Exception("Unknown rule \"" + name + "\"");

	try {
		const std::string &n = values.find("name")->second;
		const int wisMin = boost::lexical_cast<int>(values.find("wisMin")->second);
		const int wisMax = boost::lexical_cast<int>(values.find("wisMax")->second);
		const int dexMin = boost::lexical_cast<int>(values.find("dexMin")->second);
		const int dexMax = boost::lexical_cast<int>(values.find("dexMax")->second);
		const int agiMin = boost::lexical_cast<int>(values.find("agiMin")->second);
		const int agiMax = boost::lexical_cast<int>(values.find("agiMax")->second);
		const int strMin = boost::lexical_cast<int>(values.find("strMin")->second);
		const int strMax = boost::lexical_cast<int>(values.find("strMax")->second);
		const int hpMin = boost::lexical_cast<int>(values.find("hpMin")->second);
		const int hpMax = boost::lexical_cast<int>(values.find("hpMax")->second);
		const unsigned char speed = boost::lexical_cast<int>(values.find("speed")->second);

		_monsterDefs[_nextMonsterType] = MonsterDefinition(n,
		                                     Base::ByteRange(wisMin, wisMax),
		                                     Base::ByteRange(dexMin, dexMax),
		                                     Base::ByteRange(agiMin, agiMax),
		                                     Base::ByteRange(strMin, strMax),
		                                     Base::IntRange(hpMin, hpMax),
		                                     speed);

		_monsterNames[n] = _nextMonsterType;
		++_nextMonsterType;
	} catch (boost::bad_lexical_cast &e) {
		// This should never happen, since the values are
		// prechecked by the parser.
		assert(false);
	}
}

MonsterDatabase *MonsterDatabase::_instance = 0;

const MonsterType kMonsterPlayer = 0;

const MonsterID kPlayerMonsterID = 0;
const MonsterID kInvalidMonsterID = 0xFFFFFFFF;

namespace {
MonsterID g_nextMonsterID = kPlayerMonsterID + 1;
} // end of anonymous namespace

MonsterID createNewMonsterID() {
	return g_nextMonsterID++;
}

} // end of namespace Game

