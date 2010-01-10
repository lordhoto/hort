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

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "state.h"
#include "monster.h"
#include "screen.h"
#include "event.h"

#include "gui/window.h"
#include "gui/screen.h"
#include "gui/input.h"

#include <list>
#include <string>

namespace Game {

class Level;

class GameState : public State, public EventHandler {
public:
	GameState();
	~GameState();

	bool initialize();

	bool run();

	void processEvent(const Event &event);

	const Level &getLevel() const { return *_curLevel; }
	const Monster &getPlayer() const { return _player; }
private:
	bool _initialized;

	GUI::Screen &_screen;
	GUI::Window *_messageLine;
	GUI::Window *_mapWindow;
	GUI::Window *_playerStats;

	typedef std::list<std::string> StringList;
	StringList _messages;
	void printMessages();

	void drawStatsWindow();

	GUI::Input &_input;

	Screen *_gameScreen;
	Level *_curLevel;
	Monster _player;

	void handleInput(int input);
	Monster *obtainMonster(const MonsterID monster);
};

} // end of namespace Game

#endif

