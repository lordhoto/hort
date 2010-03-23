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

#include <string>
#include <cassert>

namespace Game {

/**
 * A type describing a monster type.
 */
typedef unsigned int MonsterType;

/**
 * The monster type of the player.
 */
extern const MonsterType kMonsterPlayer;

/**
 * The monster attributes.
 */
enum Attribute {
	/**
	 * The wisdom attribute of a monster.
	 */
	kAttribWisdom = 0,

	/**
	 * The dexterity attribute of a monster.
	 */
	kAttribDexterity,

	/**
	 * The agility attribute of a monster.
	 */
	kAttribAgility,

	/**
	 * The strength attribute of a monster.
	 */
	kAttribStrength,

	/**
	 * How many different attributes exist.
	 *
	 * Note that his enumeration values should
	 * never be used for accesing an specific
	 * attribute value.
	 */
	kAttribMaxTypes
};

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

} // end of namespace Game

#endif

