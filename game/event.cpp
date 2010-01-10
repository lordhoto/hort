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

#include "event.h"

#include <algorithm>
#include <functional>

namespace Game {

void EventDispatcher::addHandler(EventHandler *handler) {
	removeHandler(handler);
	_handlers.push_back(handler);
}

void EventDispatcher::removeHandler(EventHandler *handler) {
	_handlers.remove(handler);
}

void EventDispatcher::dispatch(const Event &event) {
	std::for_each(_handlers.begin(), _handlers.end(), std::bind2nd(std::mem_fun(&EventHandler::processEvent), event));
}

Event createMoveEvent(const MonsterID monster, const Monster *mP, int offX, int offY) {
	Event event;
	event.type = Event::kTypeMove;
	event.data.move.monster = monster;
	event.data.move.oldX = mP->getX();
	event.data.move.oldY = mP->getY();
	event.data.move.newX = mP->getX() + offX;
	event.data.move.newY = mP->getY() + offY;
	return event;
}

Event createAttackEvent(const MonsterID monster, const MonsterID target) {
	Event event;
	event.type = Event::kTypeAttack;
	event.data.attack.monster = monster;
	event.data.attack.target = target;
	return event;
}

} // end of namespace Game

