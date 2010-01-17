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

#include "state.h"

#include <algorithm>
#include <string>

namespace Game {

StateHandler::StateHandler() : _curState(), _queue() {
	_curState = _queue.end();
}

StateHandler::~StateHandler() {
	for (StateQueue::iterator i = _queue.begin(); i != _queue.end(); ++i)
		delete *i;
}

void StateHandler::addStateToQueue(State *state) {
	if (std::find(_queue.begin(), _queue.end(), state) != _queue.end())
		return;

	_queue.push_back(state);
}

void StateHandler::remStateFromQueue(State *state) {
	if (*_curState == state)
		throw std::string("remStateFromQueue can not remove the current state");
	_queue.remove(state);
}

void StateHandler::process() {
	while (!_queue.empty()) {
		if (_curState == _queue.end())
			_curState = _queue.begin();

		if (!(*_curState)->initialize())
			throw std::string("Failed to initialize state");

		if ((*_curState)->run()) {
			delete *_curState;
			_curState = _queue.erase(_curState);
		} else {
			++_curState;
		}
	}
}

} // end of namespace Game

