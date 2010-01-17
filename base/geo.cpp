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

#include "geo.h"

#include <cmath>

namespace Base {

namespace {

int sign(int value) {
	return (value < 0) ? -1 : ((value > 0) ? 1 : 0);
}

int abs(int value) {
	return (value < 0) ? -value : value;
}

} // end of anonymous namespace

double Point::distanceTo(const Point &p) const {
	int xDiff = p._x - _x;
	int yDiff = p._y - _y;

	return std::sqrt(xDiff*xDiff + yDiff*yDiff);
}

Bresenham::Bresenham(const Point &start, const Point &end)
    : _start(start), _end(end), _cur(start) {
	int dX = _end._x - _start._x;
	int dY = _end._y - _start._y;

	int incX = sign(dX), incY = sign(dY);
	dX = abs(dX);
	dY = abs(dY);

	_diaIncX = incX;
	_diaIncY = incY;

	if (dX > dY) {
		_parIncX = incX;
		_parIncY = 0;

		_errSlow = dX;
		_errFast = dY;
	} else {
		_parIncX = 0;
		_parIncY = incY;

		_errSlow = dY;
		_errFast = dX;
	}

	_err = _errSlow / 2;
}

const Point &Bresenham::getNext() {
	if (_cur == _end)
		return _end;

	_err -= _errFast;
	if (_err < 0) {
		_err += _errSlow;
		_cur._x += _diaIncX;
		_cur._y += _diaIncY;
	} else {
		_cur._x += _parIncX;
		_cur._y += _parIncY;
	}

	return _cur;
}

} // end of namespace Base

