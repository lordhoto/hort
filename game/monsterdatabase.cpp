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

#include "monsterdatabase.h"
#include "monsterdefinitionloader.h"

#include "base/rnd.h"

#include <cassert>

#include <boost/foreach.hpp>

namespace Game {

void MonsterDatabase::load(const std::string &filename) throw (Base::NonRecoverableException) {
	_monsterDefs.clear();
	_monsterNames.clear();
	_nextMonsterType = 0;

	MonsterDefinitionLoader loader;
	MonsterDefinitionLoader::MonsterDefinitionList monsters = loader.load(filename);

	BOOST_FOREACH(const MonsterDefinition &def, monsters) {
		_monsterDefs[_nextMonsterType] = def;
		_monsterNames[def.getName()] = _nextMonsterType;
		++_nextMonsterType;
	}
}

Monster *MonsterDatabase::createNewMonster(const MonsterType type) const {
	MonsterDefMap::const_iterator i = _monsterDefs.find(type);
	if (i == _monsterDefs.end())
		return 0;

	const MonsterDefinition &def = i->second;
	const unsigned char wis = Base::rndValueRange(def.getDefaultAttribs(kAttribWisdom));
	const unsigned char dex = Base::rndValueRange(def.getDefaultAttribs(kAttribDexterity));
	const unsigned char agi = Base::rndValueRange(def.getDefaultAttribs(kAttribAgility));
	const unsigned char str = Base::rndValueRange(def.getDefaultAttribs(kAttribStrength));
	const int hp = Base::rndValueRange(def.getDefaultHitPoints());

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

MonsterDatabase *MonsterDatabase::_instance = 0;
} // end of namespace Base

