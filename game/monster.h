/* Hort - A roguelike inspired by the Nibelungenlied
 *
 * (c) 2009 by Johannes Schickel <lordhoto at scummvm dot org>
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

namespace Game {

class Monster {
public:
	Monster() : _symbol('@'), _x(0), _y(0) {}

	/**
	 * Gets the symbol representing the monster.
	 *
	 * @return symbol.
	 */
	char getSymbol() const { return _symbol; }

	/**
	 * Returns the x coordinate of the monster.
	 *
	 * @return x coordinate.
	 */
	unsigned int getX() const { return _x; }

	/**
	 * Returns the y coordinate of the monster.
	 *
	 * @return y coordinate.
	 */
	unsigned int getY() const { return _y; }

	/**
	 * Sets the x coordinate of the monster.
	 *
	 * @param x new x coordinate.
	 */
	void setX(unsigned int x) { _x = x; }

	/**
	 * Sets the y coordinate of the monster.
	 *
	 * @param y new y coordinate.
	 */
	void setY(unsigned int y) { _y = y; }
private:
	char _symbol;
	unsigned int _x, _y;
};

} // end of namespace Game

#endif

