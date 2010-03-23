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

#ifndef GUI_SCREEN_H
#define GUI_SCREEN_H

#include "intern/screen.h"
#include "intern/window.h"
#include "intern/input.h"
#include "intern/drawdesc.h"

#include "game/map.h"
#include "game/monster.h"

#include "base/geo.h"

#include <list>
#include <vector>
#include <string>
#include <map>

namespace GUI {

class Screen {
public:
	Screen(const Game::Monster &player);
	~Screen();

	/**
	 * Initialize the screen.
	 */
	void initialize() throw (Base::NonRecoverableException);

	/**
	 * Tells the game screen some object state changed.
	 */
	void flagForUpdate() { _needRedraw = true; }

	/**
	 * Updates the game screen (when needed).
	 */
	void update(bool drawMsg = false);

	/**
	 * Sets the map to draw upon.
	 *
	 * This automatically updates the refresh flag!
	 * It also clears all object lists.
	 *
	 * @param map The map to draw.
	 */
	void setMap(const Game::Map *map);

	/**
	 * Sets the position, which should be centered.
	 *
	 * @param p Position (in map coordinates).
	 */
	void setCenter(const Base::Point &p) {
		setCenter(p._x, p._y);
	}

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
	void addObject(const Game::Monster *monster);

	/**
	 * Removes a monster from the display object list.
	 *
	 * This automatically updates the refresh flag!
	 *
	 * @param monster The monster to remove.
	 */
	void remObject(const Game::Monster *monster);

	/**
	 * Clears all object lists.
	 *
	 * This automatically updates the refresh flag!
	 */
	void clearObjects();

	/**
	 * Adds a message to the message window.
	 *
	 * @param str Message to add.
	 */
	void addToMsgWindow(const std::string &str);

	/**
	 * Sets the current turn.
	 *
	 * @param turn Current turn.
	 */
	void setTurn(unsigned int turn);

	/**
	 * Waits for the user to enter any key.
	 *
	 * @see Input
	 * @return User's input.
	 */
	Input getInput();
private:
	GUI::Intern::Screen &_screen;
	GUI::Intern::Input &_input;

	GUI::Intern::Window *_messageLine;
	GUI::Intern::Window *_mapWindow;
	GUI::Intern::Window *_playerStats;

	int getKey();
	typedef std::map<int, Input> KeyMap;
	KeyMap _keyMap;

	void setupKeyMap();

	void createOutputWindows();

	typedef std::list<std::string> StringList;
	StringList _messages;
	void printMessages();

	unsigned int _turn;
	const Game::Monster &_player;
	void drawStatsWindow();

	bool _needRedraw;
	const Game::Map *_map;

	typedef std::list<const Game::Monster *> MonsterList;
	MonsterList _monsters;

	unsigned int _centerX, _centerY;
	unsigned int _mapOffsetX, _mapOffsetY;

	Intern::MonsterDDMap *_monsterDrawDescs;
	Intern::TileDDMap *_mapDrawDescs;
};

} // end of namespace Game

#endif

