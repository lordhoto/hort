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

#ifndef GAME_TILE_H
#define GAME_TILE_H

#include <string>

namespace Game {

/**
 * A tile type.
 */
typedef unsigned int Tile;

/**
 * Definition of a tile.
 */
class TileDefinition {
public:
	TileDefinition() : _name(), _glyph(0), _isWalkable(false), _blocksSight(false), _isLiquid(false) {}
	TileDefinition(const std::string &name, const char glyph, const bool isWalkable, const bool blocksSight, const bool isLiquid)
	    : _name(name), _glyph(glyph), _isWalkable(isWalkable), _blocksSight(blocksSight), _isLiquid(isLiquid) {}

	/**
	 * @return name of the tile.
	 */
	const std::string &getName() const { return _name; }

	/**
	 * @return glyph of the tile, for use in map files.
	 */
	char getGlyph() const { return _glyph; }

	/**
	 * @return whether the tile is walkable.
	 */
	bool getIsWalkable() const { return _isWalkable; }

	/**
	 * @return whether the tile blocks the sight.
	 */
	bool getBlocksSlight() const { return _blocksSight; }

	/**
	 * @return whether the tile is a liquid.
	 */
	bool getIsLiquid() const { return _isLiquid; }
private:
	std::string _name; //< Name of the tile
	char _glyph; //< Glyph for map files

	bool _isWalkable; //< Is the tile walkable?
	bool _blocksSight; //< Does this tile block the sight?
	bool _isLiquid; //< Is this a liquid?
};

} // end of namespace Game

#endif

