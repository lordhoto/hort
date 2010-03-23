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
struct TileDefinition {
	std::string _name; //< Name of the tile
	char _glyph; //< Glyph for map files

	bool _isWalkable; //< Is the tile walkable?
	bool _blocksSight; //< Does this tile block the sight?
	bool _isLiquid; //< Is this a liquid?
};

} // end of namespace Game

#endif

