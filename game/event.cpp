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

void EventDispatcher::dispatch(const Event &event) {
	BOOST_FOREACH(HandlerList::value_type i, _handlers)
		i->processEvent(event);
}

Event createMoveEvent(const MonsterID monster, const Monster *mP, const Base::Point &newPos) {
	Event event;
	event.type = Event::kTypeMove;
	event.move.monster = monster;
	event.move.oldPos = mP->getPos();
	event.move.newPos = newPos;
	return event;
}

Event createAttackEvent(const MonsterID monster, const MonsterID target) {
	Event event;
	event.type = Event::kTypeAttack;
	event.attack.monster = monster;
	event.attack.target = target;
	return event;
}

Event createAttackDamageEvent(const MonsterID monster, const MonsterID target, bool didDmg) {
	Event event;
	event.type = Event::kTypeAttackDamage;
	event.attackDamage.monster = monster;
	event.attackDamage.target = target;
	event.attackDamage.didDmg = didDmg;
	return event;
}

Event createAttackFailEvent(const MonsterID monster) {
	Event event;
	event.type = Event::kTypeAttackFail;
	event.attackFail.monster = monster;
	return event;
}

Event createDeathEvent(const MonsterID monster, Event::Death::Cause cause, const MonsterID killer) {
	Event event;
	event.type = Event::kTypeDeath;
	event.death.monster = monster;
	event.death.killer = killer;
	event.death.cause = cause;
	return event;
}

Event createIdleEvent(const MonsterID monster, Event::Idle::Reason reason) {
	Event event;
	event.type = Event::kTypeIdle;
	event.idle.monster = monster;
	event.idle.reason = reason;
	return event;
}

} // end of namespace Game

