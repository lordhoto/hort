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

#ifndef BASE_GEO_H
#define BASE_GEO_H

namespace Base {

/**
 * Structure representing a point.
 */
struct Point {
	Point() : _x(0), _y(0) {}
	Point(unsigned int x, unsigned int y) : _x(x), _y(y) {}

	/**
	 * Compares whether two points are equal.
	 *
	 * @param p Point to compare with.
	 * @return true, if the are equal, false otherwise.
	 */
	bool operator==(const Point &p) const {
		return (_x == p._x) && (_y == p._y);
	}

	/**
	 * Checks whether two points are unequal.
	 *
	 * @param p Point to compare with.
	 * @return true, if they are not equal, false otherwise.
	 */
	bool operator!=(const Point &p) const {
		return !(*this == p);
	}

	/**
	 * Calculates the distance between two points.
	 *
	 * @param p Point to calculate the distance with.
	 * @return distance.
	 */
	double distanceTo(const Point &p) const;

	unsigned int _x, _y;
};

/**
 * An object implementing the Bresenham algorithm.
 */
class Bresenham {
public:
	/**
	 * Creates a new line drawer for the given
	 * line.
	 *
	 * @param start Start point of the line.
	 * @param end End point of the line.
	 */
	Bresenham(const Point &start, const Point &end);

	/**
	 * Checks whether all of the line has been progressed
	 * already.
	 */
	bool finished() const { return _cur == _end; }

	/**
	 * Returns the next point of the line.
	 *
	 * @return Next point.
	 */
	const Point &getNext();
private:
	const Point _start, _end;
	Point _cur;

	int _parIncX, _parIncY;
	int _diaIncX, _diaIncY;
	int _errFast, _errSlow;
	int _err;
};

} // end of namespace Base

#endif

