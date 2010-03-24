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

#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "monster.h"

#include "base/geo.h"

#include <list>

namespace Game {

/**
 * Base class for game events.
 */
class Event {
public:
	enum Type {
		/**
		 * Indicates that a monster moves.
		 */
		kTypeMove,

		/**
		 * Indicates that a monster *tries*
		 * to attack another monster.
		 */
		kTypeAttack,

		/**
		 * Indicates that a monster successfully
		 * attacked another monster.
		 */
		kTypeAttackDamage,

		/**
		 * Indicates that a monster attack
		 * failed.
		 */
		kTypeAttackFail,

		/**
		 * Indicates that a monster dies.
		 */
		kTypeDeath,

		/**
		 * An idle event.
		 */
		kTypeIdle
	};

	Event(const Type type) : _type(type) {}
	virtual ~Event() = 0;

	/**
	 * @return the type of the event.
	 */
	Type getType() const { return _type; }
private:
	const Type _type;
};

/**
 * An event from a monster.
 */
class MonsterEvent : public Event {
public:
	MonsterEvent(Type type, const MonsterID monster) : Event(type), _monster(monster) {}
	virtual ~MonsterEvent() = 0;

	/**
	 * @return the monster, which is the cause of the event
	 */
	MonsterID getMonster() const { return _monster; }
private:
	const MonsterID _monster;
};

/**
 * An idle monster event.
 */
class IdleEvent : public MonsterEvent {
public:
	/**
	 * The reason for the monster to idle.
	 */
	enum Reason {
		/**
		 * No reason, i.e. just idling :-).
		 */
		kNoReason,

		/**
		 * The monster is in a wary state and thus watching
		 * the enviorment for changes.
		 */
		kWary
	};

	IdleEvent(const MonsterID monster, const Reason reason) : MonsterEvent(kTypeIdle, monster), _reason(reason) {}

	/**
	 * @return the reason, why the monster is idling.
	 */
	Reason getReason() const { return _reason; }
private:
	const Reason _reason;
};

/**
 * A monster death event.
 */
class DeathEvent : public MonsterEvent {
public:
	/**
	 * The cause for the monster's death.
	 */
	enum Cause {
		/**
		 * Indicates, that the monster has been killed.
		 */
		kKilled,

		/**
		 * Indicates, that the monster drowned.
		 */
		kDrowned
	};

	DeathEvent(const MonsterID monster, const Cause cause, const MonsterID killer = kInvalidMonsterID)
	    : MonsterEvent(kTypeDeath, monster), _cause(cause), _killer(killer) {}

	/**
	 * @return the cause for the death.
	 */
	Cause getCause() const { return _cause; }

	/**
	 * @return the (optional) killer.
	 */
	MonsterID getKiller() const { return _killer; }
private:
	const Cause _cause;
	const MonsterID _killer;
};

/**
 * A monster movement event.
 */
class MoveEvent : public MonsterEvent {
public:
	MoveEvent(const MonsterID monster, const Base::Point &oldPos, const Base::Point &newPos)
	    : MonsterEvent(kTypeMove, monster), _newPos(newPos), _oldPos(oldPos) {}

	/**
	 * @return the old position of the monster.
	 */
	const Base::Point &getOldPos() const { return _oldPos; }

	/**
	 * @return the new postion of the monster.
	 */
	const Base::Point &getNewPos() const { return _newPos; }
private:
	const Base::Point _newPos, _oldPos;
};

/**
 * A generic monster attack event.
 */
class GenericAttackEvent : public MonsterEvent {
public:
	GenericAttackEvent(const Type type, const MonsterID monster, const MonsterID target)
	    : MonsterEvent(type, monster), _target(target) {}
	virtual ~GenericAttackEvent() = 0;

	/**
	 * @return the attacker
	 */
	MonsterID getTarget() const { return _target; }
private:
	const MonsterID _target;
};

/**
 * A monster attack event.
 */
class AttackEvent : public GenericAttackEvent {
public:
	AttackEvent(const MonsterID monster, const MonsterID target) : GenericAttackEvent(kTypeAttack, monster, target) {}
	~AttackEvent() {}
};

/**
 * A monster attack damage event.
 */
class AttackDamageEvent : public GenericAttackEvent {
public:
	AttackDamageEvent(const MonsterID monster, const MonsterID target, const bool didDmg)
	    : GenericAttackEvent(kTypeAttackDamage, monster, target), _didDmg(didDmg) {}

	/**
	 * @return whether the attack did any damage.
	 */
	bool getDidDmg() const { return _didDmg; }
private:
	const bool _didDmg;
};

/**
 * A monster attack fail event.
 */
class AttackFailEvent : public GenericAttackEvent {
public:
	AttackFailEvent(const MonsterID monster, const MonsterID target) : GenericAttackEvent(kTypeAttackFail, monster, target) {}
};

/**
 * A class capable of handling events.
 */
class EventHandler {
public:
	virtual ~EventHandler() {}

	/**
	 * Processes a movement event.
	 *
	 * @param event event to process.
	 */
	virtual void processMoveEvent(const MoveEvent &event) = 0;

	/**
	 * Processes an idle event.
	 *
	 * @param event event to process.
	 */
	virtual void processIdleEvent(const IdleEvent &event) = 0;

	/**
	 * Processes an death event.
	 *
	 * @param event event to process.
	 */
	virtual void processDeathEvent(const DeathEvent &event) = 0;

	/**
	 * Processes an attack event.
	 *
	 * @param event event to process.
	 */
	virtual void processAttackEvent(const AttackEvent &event) = 0;

	/**
	 * Processes an attack damage event.
	 *
	 * @param event event to process.
	 */
	virtual void processAttackDamageEvent(const AttackDamageEvent &event) = 0;

	/**
	 * Processes an attack fail event.
	 *
	 * @param event event to process.
	 */
	virtual void processAttackFailEvent(const AttackFailEvent &event) = 0;
};

/**
 * A event dispatcher. This object is used to dispatch
 * events to various event handlers.
 */
class EventDispatcher {
public:
	/**
	 * Adds a new event handler to dispatch the events to.
	 * 
	 * @param handler Pointer to the handler.
	 */
	void addHandler(EventHandler *handler);

	/**
	 * Removes a handler from the dispatcher.
	 *
	 * @param handler Handler to remove.
	 */
	void removeHandler(EventHandler *handler);

	/**
	 * Dispatches the given event.
	 *
	 * Note that this event automatically destroies
	 * the event after it has dispatched it to every
	 * handler.
	 *
	 * @param event Event to dispatch accross
	 *              the setup handlers.
	 */
	void dispatch(const Event *event);
private:
	typedef std::list<EventHandler *> HandlerList;
	HandlerList _handlers;
};

} // end of namespace Game

#endif

