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

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "state.h"
#include "monster.h"
#include "event.h"
#include "defs.h"

#include "gui/screen.h"
#include "gui/defs.h"

#include <list>
#include <string>

namespace Game {

class Level;

class GameState : public State, public EventHandler {
public:
	GameState();
	~GameState();

	bool initialize() throw (Base::NonRecoverableException);

	bool run();

	void processMoveEvent(const MoveEvent &event) throw ();
	void processIdleEvent(const IdleEvent &event) throw ();
	void processDeathEvent(const DeathEvent &event) throw ();
	void processAttackEvent(const AttackEvent &event) throw ();
	void processAttackDamageEvent(const AttackDamageEvent &event) throw ();
	void processAttackFailEvent(const AttackFailEvent &event) throw ();

	const Monster &getPlayer() const { return *_player; }

	void setEventDispatcher(EventDispatcher *disp) { _eventDisp = disp; }

	TickCount getCurrentTick() const { return _tickCounter; }
private:
	bool _initialized;

	EventDispatcher *_eventDisp;

	TickCount _tickCounter;
	TickCount _nextWarning;

	GUI::Screen *_gameScreen;
	Level *_curLevel;
	Monster *_player;

	bool handleInput(GUI::Input input);
	void examine();
};

} // end of namespace Game

#endif

