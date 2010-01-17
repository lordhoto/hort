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
#include <stdlib.h> // for ::atoi

namespace Game {

void MonsterDatabase::load(const std::string &filename) {
	_monsterDefs.clear();
	_monsterNames.clear();
	_nextMonsterType = 0;

	Base::FileParser::RuleMap rules;
	rules["def"] = Base::Rule("def-monster;%S,name;:=;%D,wisMin;%D,wisMax;%D,dexMin;%D,dexMax;%D,agiMin;%D,agiMax;%D,strMin;%D,strMax;%D,hpMin;%D,hpMax;%D,speed");
	Base::FileParser parser(filename, rules);
	parser.parse(this);
	if (!parser.wasSuccessful())
		throw std::string(parser.getError());
}

Monster *MonsterDatabase::createNewMonster(const MonsterType type) const {
	MonsterDefMap::const_iterator i = _monsterDefs.find(type);
	if (i == _monsterDefs.end())
		return 0;

	const MonsterDefinition &def = i->second;
	const unsigned char wis = Base::rndValueRange(def.defaultAttribs[kAttribWisdom].min(), def.defaultAttribs[kAttribWisdom].max());
	const unsigned char dex = Base::rndValueRange(def.defaultAttribs[kAttribDexterity].min(), def.defaultAttribs[kAttribDexterity].max());
	const unsigned char agi = Base::rndValueRange(def.defaultAttribs[kAttribAgility].min(), def.defaultAttribs[kAttribAgility].max());
	const unsigned char str = Base::rndValueRange(def.defaultAttribs[kAttribStrength].min(), def.defaultAttribs[kAttribStrength].max());
	const int hp = Base::rndValueRange(def.defaultHitPoints.min(), def.defaultHitPoints.max());

	return new Monster(type, wis, dex, agi, str, hp, def.defaultSpeed, 0, 0);
}

const char *MonsterDatabase::getMonsterName(const MonsterType type) const {
	MonsterDefMap::const_iterator i = _monsterDefs.find(type);
	if (i == _monsterDefs.end())
		return 0;
	else
		return i->second.name.c_str();
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

void MonsterDatabase::notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) {
	assert(name == "def");

	const std::string &n = values.find("name")->second;
	const int wisMin = ::atoi(values.find("wisMin")->second.c_str());
	const int wisMax = ::atoi(values.find("wisMax")->second.c_str());
	const int dexMin = ::atoi(values.find("dexMin")->second.c_str());
	const int dexMax = ::atoi(values.find("dexMax")->second.c_str());
	const int agiMin = ::atoi(values.find("agiMin")->second.c_str());
	const int agiMax = ::atoi(values.find("agiMax")->second.c_str());
	const int strMin = ::atoi(values.find("strMin")->second.c_str());
	const int strMax = ::atoi(values.find("strMax")->second.c_str());
	const int hpMin = ::atoi(values.find("hpMin")->second.c_str());
	const int hpMax = ::atoi(values.find("hpMax")->second.c_str());
	const unsigned char speed = ::atoi(values.find("speed")->second.c_str());

	MonsterDefinition def;
	def.name = n;
	def.defaultAttribs[kAttribWisdom] = Base::ByteRange(wisMin, wisMax);
	def.defaultAttribs[kAttribDexterity] = Base::ByteRange(dexMin, dexMax);
	def.defaultAttribs[kAttribAgility] = Base::ByteRange(agiMin, agiMax);
	def.defaultAttribs[kAttribStrength] = Base::ByteRange(strMin, strMax);
	def.defaultHitPoints = Base::IntRange(hpMin, hpMax);
	def.defaultSpeed = speed;

	_monsterDefs[_nextMonsterType] = def;
	_monsterNames[n] = _nextMonsterType;
	++_nextMonsterType;
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

