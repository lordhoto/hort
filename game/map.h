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

#include "tile.h"

#include <vector>

namespace Game {

class Map {
public:
	Map(unsigned int w, unsigned int h, const std::vector<Tile> &tiles);

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
	const TileDefinition &tileDefinition(const Base::Point &p) const { return *_tileDefs[p._y * _w + p._x]; }

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
	std::vector<const TileDefinition *> _tileDefs;
};

} // end of namespace Game

#endif

