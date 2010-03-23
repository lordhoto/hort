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

#ifndef GAME_MONSTER_TYPES_H
#define GAME_MONSTER_TYPES_H

#include "base/defs.h"
#include "base/parser.h"

#include <string>
#include <map>

namespace Game {

typedef unsigned int MonsterType;

extern const MonsterType kMonsterPlayer;

enum Attribute {
	kAttribWisdom = 0,
	kAttribDexterity,
	kAttribAgility,
	kAttribStrength,
	kAttribMaxTypes
};

class Monster;

/**
 * Object which handles all monster types.
 *
 * It is used to keep track of all the different monsters,
 * which can be defined via external files.
 */
class MonsterDatabase : private Base::ParserListener {
public:
	/**
	 * Loads the monster database from a file.
	 *
	 * This might throw an std::string on error.
	 *
	 * @param filename File to load from.
	 */
	void load(const std::string &filename) throw (Base::NonRecoverableException);

	/**
	 * Creates a monster of the given type.
	 *
	 * @param type Type of the monster.
	 */
	Monster *createNewMonster(const MonsterType type) const;

	/**
	 * Queries the type of a monster with the given name.
	 *
	 * @param name Name of the monster.
	 * @return Type of the monster (getMounsterTypeCount() in case of an error).
	 */
	MonsterType queryMonsterType(const std::string &name) const;

	/**
	 * Queries the name of a given monster type.
	 *
	 * @param type Type of the monster.
	 */
	const char *getMonsterName(const MonsterType type) const;

	/**
	 * Queries the number of different monster types.
	 */
	unsigned int getMonsterTypeCount() const { return _nextMonsterType; }

	/**
	 * Queries the global monster database
	 */
	static MonsterDatabase &instance();

	/**
	 * Destroies the global monster database
	 */
	static void destroy();
private:
	MonsterDatabase();
	static MonsterDatabase *_instance;

	void notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception);

	/**
	 * Definition of a monster.
	 */
	class MonsterDefinition {
	public:
		MonsterDefinition() : _name(), _defaultAttribs(), _defaultHitPoints(), _defaultSpeed() {}
		MonsterDefinition(const std::string &name, const Base::ByteRange &wisdom, const Base::ByteRange &dexterity,
		                  const Base::ByteRange &agility, const Base::ByteRange &strength, const Base::IntRange &hitPoints,
		                  const unsigned char defaultSpeed)
		    : _name(name), _defaultAttribs(), _defaultHitPoints(hitPoints), _defaultSpeed(defaultSpeed) {
			_defaultAttribs[kAttribWisdom] = wisdom;
			_defaultAttribs[kAttribDexterity] = dexterity;
			_defaultAttribs[kAttribAgility] = agility;
			_defaultAttribs[kAttribStrength] = strength;
		}

		/**
		 * @return the name of the monster.
		 */
		const std::string &getName() const { return _name; }

		/**
		 * @return the specified attribute.
		 */
		const Base::ByteRange &getDefaultAttribs(Attribute attrib) const { assert(attrib != kAttribMaxTypes); return _defaultAttribs[attrib]; }

		/**
		 * @return the default hit points.
		 */
		const Base::IntRange &getDefaultHitPoints() const { return _defaultHitPoints; }

		/**
		 * @return the default speed.
		 */
		unsigned char getDefaultSpeed() const { return _defaultSpeed; }
	private:
		std::string _name;

		Base::ByteRange _defaultAttribs[kAttribMaxTypes];
		Base::IntRange _defaultHitPoints;
		unsigned char _defaultSpeed;
	};

	MonsterType _nextMonsterType;
	typedef std::map<MonsterType, MonsterDefinition> MonsterDefMap;
	MonsterDefMap _monsterDefs;

	typedef std::map<std::string, MonsterType> MonsterNameMap;
	MonsterNameMap _monsterNames;
};

#define g_monsterDatabase Game::MonsterDatabase::instance()

} // end of namespace Game

#endif

