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
#include <stdexcept>

namespace Game {

class Map {
public:
	Map(unsigned int width, unsigned int height, const std::vector<Tile> &tiles);

	/**
	 * Checks whether the given map tile is walkable.
	 * Walkable does not mean that the player will surive
	 * walking onto the tile though :-).
	 *
	 * @param p Position.
	 * @return true if walkable, false otherwise
	 */
	bool isWalkable(const Base::Point &p) const throw (std::out_of_range) {
		return tileDefinition(p).getIsWalkable();
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
	bool isWalkable(unsigned int x, unsigned int y) const throw (std::out_of_range) {
		return tileDefinition(x, y).getIsWalkable();
	}

	/**
	 * Returns the tile at the given position.
	 *
	 * @param p Position.
	 * @return Tile type.
	 */
	Tile tileAt(const Base::Point &p) const throw (std::out_of_range) {
		return _tiles.at(p._y * _width + p._x);
	}

	/**
	 * Returns the tile at the given position.
	 *
	 * @param x x coordinate of the tile (must not exceed width - 1)
	 * @param y y coordinate of the tile (must not exceed height - 1)
	 * @return Tile type.
	 */
	Tile tileAt(unsigned int x, unsigned int y) const throw (std::out_of_range) {
		return _tiles.at(y * _width + x);
	}

	/**
	 * Queries the tile definition at the given position.
	 *
	 * @param p Position.
	 * @return Tile definition.
	 */
	const TileDefinition &tileDefinition(const Base::Point &p) const throw (std::out_of_range) {
		return *_tileDefs.at(p._y * _width + p._x);
	}

	/**
	 * Queries the tile definition at the given position.
	 *
	 * @param x x coordinate of the tile (must not exceed width - 1)
	 * @param y y coordinate of the tile (must not exceed height - 1)
	 * @return Tile definition.
	 */
	const TileDefinition &tileDefinition(unsigned int x, unsigned int y) const throw (std::out_of_range) {
		return *_tileDefs.at(y * _width + x);
	}

	/**
	 * Returns the width of the map.
	 * @return width
	 */
	unsigned int getWidth() const { return _width; }

	/**
	 * Returns the height of the map.
	 * @return height
	 */
	unsigned int getHeight() const { return _height; }
private:
	unsigned int _width, _height;
	std::vector<Tile> _tiles;
	std::vector<const TileDefinition *> _tileDefs;
};

} // end of namespace Game

#endif

