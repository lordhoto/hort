/* Hort - A roguelike inspired by the Nibelungenlied
 *
 * (c) 2009 by Johannes Schickel <lordhoto at scummvm dot org>
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

#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "gui/window.h"

#include <vector>

namespace Game {

class Level {
public:
	enum Tile {
		kTileMeadow,
		kTileTree,
		kTileWater
	};

	Level();

	void draw(GUI::Window &out, unsigned int oX, unsigned int oY) const;
	void draw(GUI::Window &out, unsigned int oX, unsigned int oY, unsigned int x, unsigned int y, unsigned int w, unsigned int h) const;

	Tile tileAt(unsigned int x, unsigned int y) const { return _tiles[y * _w + x]; }

	unsigned int width() const { return _w; }
	unsigned int height() const { return _h; }
private:
	unsigned int _w, _h;
	std::vector<Tile> _tiles;

	void printTile(GUI::Window &out, const Tile t, unsigned int x, unsigned int y) const;
};

}

#endif

