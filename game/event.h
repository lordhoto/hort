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

#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "monster.h"

#include <list>

namespace Game {

struct Event {
	enum kType {
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

	kType type;

	struct Idle {
		MonsterID monster;

		enum Reason {
			kNoReason,
			kWary
		} reason;
	};

	struct Death {
		MonsterID monster;
		MonsterID killer; // This might be kInvalidMonsterID!

		enum Cause {
			kKilled,
			kDrowned
		} cause;
	};

	union {
		struct Move {
			MonsterID monster;

			unsigned int newX, newY;
			unsigned int oldX, oldY;
		} move;

		struct Attack {
			MonsterID monster;
			MonsterID target;
		} attack;

		struct AttackDamage {
			MonsterID monster;
			MonsterID target;

			bool didDmg;
		} attackDamage;

		struct AttackFail {
			MonsterID monster;
		} attackFail;

		Death death;

		Idle idle;
	} data;
};

/**
 * A class capable of handling events.
 */
class EventHandler {
public:
	virtual ~EventHandler() {}

	/**
	 * Process the event in the manner the EventHandler
	 * sees fitting.
	 *
	 * @param event to process.
	 */
	virtual void processEvent(const Event &event) = 0;
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
	 * @param event Event to dispatch accross
	 *              the setup handlers.
	 */
	void dispatch(const Event &event);
private:
	typedef std::list<EventHandler *> HandlerList;
	HandlerList _handlers;
};

/**
 * Creates an event for the monster's movement.
 *
 * @param monster Monster to move.
 * @param mP Pointer to the monster to move.
 * @param offX X offset to move.
 * @param offY Y offset to move.
 * @return Event structure.
 */
Event createMoveEvent(const MonsterID monster, const Monster *mP, int offX, int offY);

/**
 * Creates an attack event.
 *
 * @param monster Monster which attacks.
 * @param target Monster which is attacked.
 * @return Event structure.
 */
Event createAttackEvent(const MonsterID monster, const MonsterID target);

/**
 * Creates an attack damage event.
 *
 * @param monster Monster which attacks.
 * @param target Monster which is attacked.
 * @param didDmg Whether the attack caused any damage.
 * @return Event structure.
 */
Event createAttackDamageEvent(const MonsterID monster, const MonsterID target, bool didDmg);

/**
 * Creates an attack fail event.
 *
 * @param monster Monster which failed to attack.
 * @return Event sturcture.
 */
Event createAttackFailEvent(const MonsterID monster);

/**
 * Creates a death event.
 *
 * @param monster Monster which dies.
 * @param cause The cause of the death.
 * @param killer (Optional) killer monster.
 * @return Event structure.
 */
Event createDeathEvent(const MonsterID monster, Event::Death::Cause cause, const MonsterID killer = kInvalidMonsterID);

/**
 * Creates an idle event.
 *
 * @param monster Monster which idles.
 * @param reason The reason why it idles.
 * @return Event sturcture.
 */
Event createIdleEvent(const MonsterID monster, Event::Idle::Reason reason);

} // end of namespace Game

#endif

