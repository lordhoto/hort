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

#ifndef GAME_MAPLOADER_H
#define GAME_MAPLOADER_H

#include "map.h"

#include "base/exception.h"

#include <string>
#include <list>

namespace Game {

/**
 * Object which loads a map from a file.
 */
class MapLoader {
public:
	/**
	 * Creates a map loader, which loads from the given
	 * filename.
	 *
	 * @param filename Filename of the map
	 */
	MapLoader(const std::string &filename);

	/**
	 * Load the map.
	 *
	 * @return A pointer to a new map object.
	 */
	Map *load() throw (Base::NonRecoverableException);
private:
	const std::string _filename;

	void throwError(const std::string &error, int line) throw (Base::NonRecoverableException);

	typedef std::list<std::string> StringList;
	StringList _lines;
};

} // end of namespace Game

#endif

