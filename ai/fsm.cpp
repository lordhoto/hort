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

#include "fsm.h"

#include <cassert>

namespace AI {
namespace FSM {

void State::addTransition(InputID input, StateID newState) {
	_transitions[input] = newState;
}

bool State::isValidInput(InputID input) const {
	return (_transitions.find(input) != _transitions.end());
}

StateID State::queryTransition(InputID input) const {
	assert(isValidInput(input));
	return _transitions.find(input)->second;
}

FSM::~FSM() {
	for (StateMap::iterator i = _states.begin(); i != _states.end(); ++i)
		delete i->second;
	_states.clear();
}

void FSM::addState(StateID stateID, State *state) {
	StateMap::iterator s = _states.find(stateID);
	if (s != _states.end()) {
		delete s->second;
		s->second = state;
	} else {
		_states[stateID] = state;
	}
}

bool FSM::hasState(StateID stateID) const {
	return (_states.find(stateID) != _states.end());
}

void FSM::process(InputID input) {
	if (_curStateID == kInvalidStateID || !_curState)
		return;

	if (_curState->isValidInput(input)) {
		_curStateID = _curState->queryTransition(input);
		_curState = _states.find(_curStateID)->second;
	}
}

bool FSM::setState(StateID newState) {
	if (!hasState(newState)) {
		_curState = 0;
		_curStateID = kInvalidStateID;
		return false;
	} else {
		_curStateID = newState;
		_curState = _states.find(_curStateID)->second;
		return true;
	}
}

} // end of namespace FSM
} // end of namespace AI

