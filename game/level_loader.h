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

#include <string>
#include <list>

namespace Game {

class MapLoader {
public:
	MapLoader(const std::string &filename);

	Map *load();
private:
	const std::string _filename;

	void throwError(const std::string &error, int line);

	typedef std::list<std::string> StringList;
	StringList _lines;
};

class LevelLoader : private Base::ParserListener {
public:
	LevelLoader(const std::string &path);
	~LevelLoader() { delete _level; }

	Level *load(GameState &gs);
private:
	const std::string _path;

	void notifyRule(const std::string &name, const Base::Matcher::ValueMap &values);
	void processMonster(const Base::Matcher::ValueMap &values);

	Level *_level;
};

} // end of namespace Game

#endif

