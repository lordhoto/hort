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

#ifndef GAME_GAME_SCREEN_H
#define GAME_GAME_SCREEN_H

#include "gui/window.h"

#include "map.h"
#include "monster.h"

#include <list>
#include <vector>

namespace Game {

class Screen {
public:
	Screen(GUI::Window &window);

	/**
	 * Tells the game screen some object state changed.
	 */
	void flagForUpdate() { _needRedraw = true; }

	/**
	 * Updates the game screen (when needed).
	 */
	void update();

	/**
	 * Sets the map to draw upon.
	 *
	 * This automatically updates the refresh flag!
	 * It also clears all object lists.
	 *
	 * @param map The map to draw.
	 */
	void setMap(const Map *map);

	/**
	 * Sets the position, which should be centered.
	 *
	 * @param x x position (in map coordinates).
	 * @param y y position (in map coordinates).
	 */
	void setCenter(unsigned int x, unsigned int y);

	/**
	 * Adds a monster to the display object list.
	 *
	 * This automatically updates the refresh flag!
	 *
	 * @param monster The monster to draw.
	 */
	void addObject(const Monster *monster);

	/**
	 * Removes a monster from the display object list.
	 *
	 * This automatically updates the refresh flag!
	 *
	 * @param monster The monster to remove.
	 */
	void remObject(const Monster *monster);

	/**
	 * Clears all object lists.
	 *
	 * This automatically updates the refresh flag!
	 */
	void clearObjects();

	/**
	 * Queries the current map X offset.
	 */
	int mapOffsetX() const { return _mapOffsetX; }

	/**
	 * Queries the current map Y offset.
	 */
	int mapOffsetY() const { return _mapOffsetY; }
private:
	GUI::Window &_output;

	bool _needRedraw;
	const Map *_map;

	typedef std::list<const Monster *> MonsterList;
	MonsterList _monsters;

	unsigned int _centerX, _centerY;
	int _mapOffsetX, _mapOffsetY;

	struct DrawDesc {
		DrawDesc() {}
		DrawDesc(int symbol, GUI::ColorPair color, int attribs) : symbol(symbol), color(color), attribs(attribs) {}

		int symbol;
		GUI::ColorPair color;
		int attribs;
	};
	typedef std::vector<DrawDesc> DrawDescVector;

	static const DrawDesc _monsterDrawDescriptions[];
	static const size_t _monsterDrawDescriptionsEntries;

	DrawDescVector _mapDrawDescs;
};

} // end of namespace Game

#endif

