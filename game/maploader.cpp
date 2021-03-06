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

#include "maploader.h"
#include "tiledatabase.h"

#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>

namespace Game {

MapLoader::MapLoader(const std::string &filename)
    : _filename(filename), _lines() {
	std::ifstream in(filename.c_str());

	while (!in.eof()) {
		std::string line;
		std::getline(in, line);
		_lines.push_back(line);
	}
}

Map *MapLoader::load() throw (Base::NonRecoverableException) {
	if (_lines.empty() || _lines.size() < 3)
		throwError("Contains too few lines", 0);

	int lineCount = 2;

	TileDatabase &tdb = TileDatabase::instance();
	unsigned int w = -1, h = -1;
   
	try {
		w = boost::lexical_cast<int>(_lines.front());
		_lines.pop_front();

		if (w <= 0)
			throwError("Width is zero or less", 0);
	} catch (boost::bad_lexical_cast &) {
		throwError("Width definition is no integer", 0);
	}

	try {
		h = boost::lexical_cast<int>(_lines.front());
		_lines.pop_front();

		if (h <= 0)
			throwError("Height is zero or less", 1);
	} catch (boost::bad_lexical_cast &) {
		throwError("Height definition is no integer", 1);
	}

	if (_lines.size() < h)
		throwError("Height exceeds remaining lines", lineCount);

	std::vector<Tile> tiles;
	tiles.resize(w * h);

	const Tile lastValidTile = tdb.getTileCount();

	for (unsigned int y = 0; y < h; ++y) {
		if (_lines.empty())
			throwError("Unexpected end of file", lineCount);

		std::string line = _lines.front(); _lines.pop_front();
		++lineCount;

		if (line.empty()) {
			--y;
			continue;
		}

		if (line.size() < w)
			throwError("Unexpected end of line", lineCount);

		for (unsigned int x = 0; x < w; ++x) {
			const Tile tile = tdb.queryTile(line[x]);
			if (tile >= lastValidTile)
				throwError(std::string("Undefined tile glyph \"") + line[x] + "\"", lineCount);

			tiles[y * w + x] = tile;
		}
	}

	return new Map(w, h, tiles);
}

void MapLoader::throwError(const std::string &error, int line) throw (Base::NonRecoverableException) {
	std::stringstream ss;
	ss << "Map loading failed. File: " << _filename << " Line: " << line << " ERROR: " << error;
	throw Base::NonRecoverableException(ss.str());
}

} // end of namespace Game

