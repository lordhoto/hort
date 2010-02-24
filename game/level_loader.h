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

#ifndef GAME_LEVEL_LOADER_H
#define GAME_LEVEL_LOADER_H

#include "game.h"
#include "level.h"

#include "base/parser.h"
#include "base/geo.h"

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
	Map *load();
private:
	const std::string _filename;

	void throwError(const std::string &error, int line);

	typedef std::list<std::string> StringList;
	StringList _lines;
};

/**
 * Object which loads a level from a directory.
 *
 * It loads the map from "path/map.def"
 * and the objects on the map from "path/objects.def"
 */
class LevelLoader : private Base::ParserListener {
public:
	/**
	 * Creates a level loader, which loads from the
	 * given path.
	 */
	LevelLoader(const std::string &path);
	~LevelLoader() { delete _level; }

	/**
	 * Load the level.
	 *
	 * @param gs Game state associated with the new level.
	 * @return A pointer to the new level object.
	 */
	Level *load(GameState &gs);
private:
	const std::string _path;

	void notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception);
	void processMonster(const Base::Matcher::ValueMap &values);
	void processStartPoint(const Base::Matcher::ValueMap &values);

	Base::Point _start;
	Level *_level;
};

} // end of namespace Game

#endif

