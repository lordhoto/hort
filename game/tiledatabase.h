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

#ifndef GAME_TILEDATABASE_H
#define GAME_TILEDATABASE_H

#include "tile.h"

#include "base/exception.h"

#include <map>

namespace Game {

class TileDatabase {
public:
	/**
	 * Loads the tile database from a file.
	 *
	 * @param filename File to load from.
	 */
	void load(const std::string &filename) throw (Base::NonRecoverableException);

	/**
	 * Queries how many different tiles are defined
	 * currently.
	 */
	Tile getTileCount() const { return _nextTileID; }

	/**
	 * Queries the definition of the given tile
	 *
	 * @param tile Tile type to query.
	 * @return Pointer to a Definition structure (or 0 in case it's not defined).
	 */
	const TileDefinition *queryTileDefinition(const Tile tile) const;

	/**
	 * Queries the tile type of the given name.
	 *
	 * @param name Name of the tile.
	 * @return Tile type (getTileCount() in case of an error).
	 */
	Tile queryTile(const std::string &name) const;

	/**
	 * Queries the tile type with the given glyph.
	 *
	 * @param glyph Glyph of the tile.
	 * @return Tile type (getTileCount() in case of an error).
	 */
	Tile queryTile(const char glyph) const;

	/**
	 * Queries the global tile database instance.
	 */
	static TileDatabase &instance();

	/**
	 * Destroies the global tile database instance.
	 */
	static void destroy();
private:
	TileDatabase();

	static TileDatabase *_instance;

	Tile _nextTileID;
	typedef std::map<Tile, TileDefinition> TileDefMap;
	TileDefMap _tileDefinitions;
};

} // end of namespace Game

#endif
