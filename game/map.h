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

#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "base/geo.h"
#include "base/parser.h"

#include <vector>
#include <map>
#include <string>

namespace Game {

/**
 * A tile type.
 */
typedef unsigned int Tile;

class TileDatabase : private Base::ParserListener {
public:
	/**
	 * Loads the tile database from a file.
	 *
	 * This might throw an std::string on error.
	 *
	 * @param filename File to load from.
	 */
	void load(const std::string &filename);

	/**
	 * Queries how many different tiles are defined
	 * currently.
	 */
	Tile getTileCount() const { return _nextTileID; }

	/**
	 * Definition of a tile.
	 */
	struct Definition {
		std::string _name; /// Name of the tile
		char _glyph; /// Glyph for map files

		bool _isWalkable; /// Is the tile walkable?
		bool _blocksSight; /// Does this tile block the sight?
		bool _isLiquid; /// Is this a liquid?
	};

	/**
	 * Queries the definition of the given tile
	 *
	 * @param Tile Tile type to query.
	 * @return Pointer to a Definition structure (or 0 in case it's not defined).
	 */
	const Definition *queryTileDefinition(const Tile tile) const;

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

	void notifyRule(const std::string &name, const Base::Matcher::ValueMap &values);

	Tile _nextTileID;
	typedef std::map<Tile, Definition> TileDefMap;
	TileDefMap _tileDefinitions;
};

class Map {
public:
	Map();

	/**
	 * Checks whether the given map tile is walkable.
	 * Walkable does not mean that the player will surive
	 * walking onto the tile though :-).
	 *
	 * @param p Position.
	 * @return true if walkable, false otherwise
	 */
	bool isWalkable(const Base::Point &p) const {
		return isWalkable(p._x, p._y);
	}

	/**
	 * Checks whether the given map tile is walkable.
	 * Walkable does not mean that the player will surive
	 * walking onto the tile though :-).
	 *
	 * @param x x coordinate
	 * @param y y coordinate
	 * @return true if walkable, false otherwise
	 */
	bool isWalkable(unsigned int x, unsigned int y) const;

	/**
	 * Returns the tile at the given position.
	 *
	 * @param p Position.
	 * @return Tile type.
	 */
	Tile tileAt(const Base::Point &p) const { return _tiles[p._y * _w + p._x]; }

	/**
	 * Returns the tile at the given position.
	 *
	 * @param x x coordinate of the tile (must not exceed width - 1)
	 * @param y y coordinate of the tile (must not exceed height - 1)
	 * @return Tile type.
	 */
	Tile tileAt(unsigned int x, unsigned int y) const { return _tiles[y * _w + x]; }

	/**
	 * Queries the tile definition at the given position.
	 *
	 * @param p Position.
	 * @return Tile definition.
	 */
	const TileDatabase::Definition &tileDefinition(const Base::Point &p) const { return *_tileDefs[p._y * _w + p._x]; }

	/**
	 * Returns the width of the map.
	 * @return width
	 */
	unsigned int width() const { return _w; }

	/**
	 * Returns the height of the map.
	 * @return height
	 */
	unsigned int height() const { return _h; }
private:
	unsigned int _w, _h;
	std::vector<Tile> _tiles;
	std::vector<const TileDatabase::Definition *> _tileDefs;
};

} // end of namespace Game

#endif

