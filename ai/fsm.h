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

#ifndef AI_FSM_H
#define AI_FSM_H

#include <stdint.h>
#include <map>

namespace AI {
namespace FSM {

typedef uint32_t StateID;
enum {
	kInvalidStateID = 0xFFFFFFFF
};

typedef uint32_t InputID;

class State {
public:
	/**
	 * Adds a new transition.
	 *
	 * WARNING: This does overwrite already defined transitions!
	 *
	 * @param input Input data.
	 * @param newState New state.
	 */
	void addTransition(InputID input, StateID newState);
	
	/**
	 * Tests whether the given input data is valid.
	 *
	 * @param input Input data.
	 * @return true when the data is allowed, false otherwise.
	 */
	bool isValidInput(InputID input) const;

	/**
	 * Queries the new state given the input data.
	 *
	 * You need to assure that "input" is valid for this
	 * state. Check out isInputValid for that.
	 * @see isInputValid
	 *
	 * @param input Input data.
	 * @return new state ID.
	 */
	StateID queryTransition(InputID input) const;
private:
	typedef std::map<InputID, StateID> TransitionMap;
	TransitionMap _transitions;
};

class FSM {
public:
	FSM() : _states(), _curState(0), _curStateID(kInvalidStateID) {}
	~FSM();

	/**
	 * Adds a state to the machine.
	 *
	 * NOTE: This does take the ownership of the state object.
	 *
	 * WARNING: This does overwrite already defined states!
	 *
	 * @param stateID ID of the state.
	 * @param state State description.
	 */
	void addState(StateID stateID, State *state);

	/**
	 * Checks whether the given state is present
	 *
	 * @param stateID ID of the state.
	 * @return true if present, false otherwise.
	 */
	bool hasState(StateID stateID) const;

	/**
	 * Processes the given input data.
	 *
	 * @param input Input data.
	 */
	void process(InputID input);

	/**
	 * Returns the current state ID.
	 *
	 * @return state id.
	 */
	StateID getState() const { return _curStateID; }

	/**
	 * Sets the current state ID.
	 *
	 * @param newState New state ID.
	 * @return true on success, false otherwise.
	 */
	bool setState(StateID newState);
private:
	typedef std::map<StateID, State *> StateMap;

	StateMap _states;
	const State *_curState;
	StateID _curStateID;
};

} // end of namespace FSM
} // end of namespace AI

#endif

