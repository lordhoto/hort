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

#ifndef GAME_MONSTER_H
#define GAME_MONSTER_H

#include "monsterdefinition.h"

#include "base/geo.h"

#include <algorithm>

namespace Game {

typedef unsigned int MonsterID;
extern const MonsterID kPlayerMonsterID;
extern const MonsterID kInvalidMonsterID;

MonsterID createNewMonsterID();

class Monster {
public:
	Monster(MonsterType type, unsigned char wis, unsigned char dex, unsigned char agi, unsigned char str, int health, unsigned char speed, unsigned int x, unsigned int y)
	    : _type(type), _pos(x, y), _curHealth(health), _maxHealth(health), _speed(speed) {
		_attrib[kAttribWisdom] = wis;
		_attrib[kAttribDexterity] = dex;
		_attrib[kAttribAgility] = agi;
		_attrib[kAttribStrength] = str;
	}

	/**
	 * Returns the type of the monster.
	 *
	 * @return monster type
	 * @see MonsterType
	 */
	MonsterType getType() const { return _type; }

	/**
	 * Returns the current hit points.
	 *
	 * @return hit points.
	 */
	int getHitPoints() const { return _curHealth; }

	/**
	 * Returns the maximum hit points of the monster.
	 *
	 * @return max hit points.
	 */
	int getMaxHitPoints() const { return _maxHealth; }

	/**
	 * Sets the current hit points.
	 *
	 * @param hitpoints Set the hitpoints of the object (this can not exceed max. health)
	 */
	void setHitPoints(int hitpoints) {
		_curHealth = std::min(hitpoints, _maxHealth);
	}

	/**
	 * Returns the position of the monster.
	 *
	 * @return Position.
	 */
	const Base::Point &getPos() const { return _pos; }

	/**
	 * Sets the position of the monster.
	 *
	 * @param pos New position.
	 */
	void setPos(const Base::Point &pos) { _pos = pos; }

	/**
	 * Returns the x coordinate of the monster.
	 *
	 * @return x coordinate.
	 */
	unsigned int getX() const { return _pos._x; }

	/**
	 * Returns the y coordinate of the monster.
	 *
	 * @return y coordinate.
	 */
	unsigned int getY() const { return _pos._y; }

	/**
	 * Sets the x coordinate of the monster.
	 *
	 * @param x new x coordinate.
	 */
	void setX(unsigned int x) { _pos._x = x; }

	/**
	 * Sets the y coordinate of the monster.
	 *
	 * @param y new y coordinate.
	 */
	void setY(unsigned int y) { _pos._y = y; }

	/**
	 * Queries the given attribute value.
	 *
	 * @param attrib The attribute to query.
	 * @return Monster's attribute value.
	 */
	unsigned char getAttribute(Attribute attrib) const { return _attrib[attrib]; }

	/**
	 * Sets the given attribute.
	 *
	 * @param attrib Attribute to change
	 * @param value New attribute value
	 */
	void setAttribute(Attribute attrib, unsigned char value) { _attrib[attrib] = value; }

	/**
	 * Queries the speed of the monster.
	 *
	 * The "speed" of a monster is how many
	 * ticks are processed before it receives
	 * another action.
	 *
	 * @return speed in ticks.
	 */
	unsigned char getSpeed() const { return _speed; }
private:
	MonsterType _type;

	Base::Point _pos;
	int _curHealth, _maxHealth;
	unsigned char _attrib[kAttribMaxTypes];
	unsigned char _speed;
};

} // end of namespace Game

#endif

