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

#include "event.h"

#include <boost/foreach.hpp>

namespace Game {

void EventDispatcher::addHandler(EventHandler *handler) {
	removeHandler(handler);
	_handlers.push_back(handler);
}

void EventDispatcher::removeHandler(EventHandler *handler) {
	_handlers.remove(handler);
}

void EventDispatcher::dispatch(const Event *event) {
	BOOST_FOREACH(HandlerList::value_type i, _handlers) {
		switch (event->getType()) {
		case Event::kTypeIdle:
			i->processIdleEvent(*(const IdleEvent *)event);
			break;

		case Event::kTypeMove:
			i->processMoveEvent(*(const MoveEvent *)event);
			break;

		case Event::kTypeDeath:
			i->processDeathEvent(*(const DeathEvent *)event);
			break;

		case Event::kTypeAttack:
			i->processAttackEvent(*(const AttackEvent *)event);
			break;

		case Event::kTypeAttackDamage:
			i->processAttackDamageEvent(*(const AttackDamageEvent *)event);
			break;

		case Event::kTypeAttackFail:
			i->processAttackFailEvent(*(const AttackFailEvent *)event);
			break;

		default:
			assert(false && "Unknown event type passed");
		}
	}

	delete event;
}

Event::~Event() {
}

MonsterEvent::~MonsterEvent() {
}

GenericAttackEvent::~GenericAttackEvent() {
}

} // end of namespace Game

