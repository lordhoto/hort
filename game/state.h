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

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>

namespace Game {

class State;

class StateHandler {
public:
	StateHandler();
	~StateHandler();

	/**
	 * Adds a state to the running queue.
	 */
	void addStateToQueue(State *state);

	/**
	 * Removes a state from the queue.
	 */
	void remStateFromQueue(State *state);

	/**
	 * Processes the states.
	 */
	void process();
private:
	typedef std::list<State *> StateQueue;

	StateQueue::iterator _curState;
	StateQueue _queue;
};

class State {
public:
	virtual ~State() {}

	/**
	 * Initializes the state. This is always called
	 * before run is called!
	 *
	 * @return true on success, false otherwise.
	 */
	virtual bool initialize() = 0;

	/**
	 * Runs the given state.
	 *
	 * @return true, when the state is finished. false, when it's only paused.
	 */
	virtual bool run() = 0;
};

} // end of namespace Game

#endif

