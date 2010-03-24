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

#include "level.h"
#include "monsterdatabase.h"

#include "base/rnd.h"
#include "ai/monster.h"

#include <cassert>

#include <boost/foreach.hpp>

namespace Game {

Level::Level(Map *map, GameState &gs)
    : _map(map), _monsterField(), _screen(0), _gameState(gs), _eventDisp(), _monsters(), _monsterAI(0) {
	assert(_map);

	_monsterField.resize(_map->getWidth() * _map->getHeight());
	for (unsigned int i = 0; i < _map->getWidth() * _map->getHeight(); ++i)
		_monsterField[i] = false;

	_eventDisp.addHandler(this);
	_monsterAI = new AI::Monster(*this, _eventDisp);
	_eventDisp.addHandler(_monsterAI);
}

Level::~Level() {
	makeInactive();

	BOOST_FOREACH(MonsterMap::value_type &i, _monsters) {
		if (i.first != kPlayerMonsterID)
			delete i.second._monster;
	}
	delete _map;
}

void Level::makeActive(GUI::Screen &screen, Monster &player) throw (std::out_of_range) {
	// Verify that the monster position is setup correctly.
	if (player.getX() >= _map->getWidth() ||
	    player.getY() >= _map->getHeight())
		throw std::out_of_range("Monster entrance position is out of range");

	// First of all make this level inactive, just to prevent
	// it from being made active twice (or more times).
	makeInactive();

	// Setup the game screen with everything that's on the level.
	screen.setMap(_map);
	BOOST_FOREACH(const MonsterMap::value_type &i, _monsters)
		screen.addObject(i.second._monster);
	screen.addObject(&player);
	_screen = &screen;

	// Setup the game state to patch its events through the
	// level's event dispatcher, so that all the level internals
	// get notified about player events.
	_gameState.setEventDispatcher(&_eventDisp);

	// Add the game state as handler for the level's events
	// to allow the game state to keep track of the level content
	// changes.
	_eventDisp.addHandler(&_gameState);

	// Setup the player in the monster's AI handlers, so that
	// they have some nice target to aim at :-).
	_monsterAI->setPlayer(&player);

	// Add an entry of the player in the monster list.
	// TODO: It should be considerd that the player doesn't get an immediate action here.
	// That could be abused by the player when switching levels often.
	_monsters[kPlayerMonsterID] = MonsterEntry(&player, _gameState.getCurrentTick());
	_monsterField[player.getY() * _map->getWidth() + player.getX()] = true;
}

void Level::makeInactive() {
	// Remove the player from the monster list
	removeMonster(kPlayerMonsterID);

	// Remove the player from the AI handler
	_monsterAI->setPlayer(0);

	// Uninitialize the screen 
	if (_screen)
		_screen->setMap(0);
	_screen = 0;

	// Remove the game state from the event
	// dispatcher.
	_eventDisp.removeHandler(&_gameState);

	// Reset the event dispatcher of the
	// game state.
	_gameState.setEventDispatcher(0);
}

bool Level::isWalkable(const Base::Point &p) const throw (std::out_of_range) {
	if (!_map->isWalkable(p))
		return false;

	if (_monsterField.at(p._y * _map->getWidth() + p._x))
		return false;

	return true;
}

MonsterID Level::monsterAt(const Base::Point &p) const {
	BOOST_FOREACH(const MonsterMap::value_type &i, _monsters) {
		if (i.second._monster->getPos() == p)
			return i.first;
	}

	return kInvalidMonsterID;
}

Monster *Level::getMonster(const MonsterID monster) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return 0;
	else
		return i->second._monster;
}

const Monster *Level::getMonster(const MonsterID monster) const {
	MonsterMap::const_iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return 0;
	else
		return i->second._monster;
}

bool Level::isAllowedToAct(const MonsterID monster) const {
	MonsterMap::const_iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return false;
	else
		return (i->second._nextAction <= _gameState.getCurrentTick());
}

MonsterID Level::addMonster(const MonsterType monster, const Base::Point &pos) throw (std::out_of_range) {
	// Create a new monster object and setup the position.
	Monster *newMonster = g_monsterDatabase.createNewMonster(monster);
	assert(newMonster);
	newMonster->setPos(pos);

	if (static_cast<unsigned int>(pos._x) >= _map->getWidth() ||
	    static_cast<unsigned int>(pos._y) >= _map->getHeight())
		throw std::out_of_range("Monster spawn point is outside the map");

	_monsterField[pos._y * _map->getWidth() + pos._x] = true;

	// Create a new monster ID and add the monster to the map
	const MonsterID newId = createNewMonsterID();
	_monsters[newId] = MonsterEntry(newMonster, _gameState.getCurrentTick());

	// Add the monster to the AI handler
	_monsterAI->addMonster(newId, newMonster);

	// Return the new ID
	return newId;
}

void Level::removeMonster(const MonsterID monster) {
	// Verify that the monster exists.
	MonsterMap::iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return;

	// Unset the monster.
	assert(i->second._monster->getY() <= _map->getHeight() && "Corrupted monster position");
	assert(i->second._monster->getX() <= _map->getWidth() && "Corrupted monster position");
	_monsterField[i->second._monster->getY() * _map->getWidth() + i->second._monster->getX()] = false;

	// We only destroy the monster object, in case it's not the player
	if (monster != kPlayerMonsterID) {
		if (_screen)
			_screen->remObject(i->second._monster);

		delete i->second._monster;
		_monsterAI->removeMonster(i->first);
	}

	// Remove the monster from the map
	_monsters.erase(i);
}

void Level::update() {
	const TickCount curTick = _gameState.getCurrentTick();

	// There is intentionally no "++i" done every loop here, since we might remove
	// the element from the list in this loop and thus invalidate the current
	// iterator.
	for (MonsterMap::iterator i = _monsters.begin(), end = _monsters.end(); i != end;) {
		// We remove dead monsters here, since that should be after all event processing
		// has taken place. Of course we just do it in case the monster is already dead :-).
		if (i->second._monster->getHitPoints() <= 0) {
			MonsterID toRemove = i->first;
			++i;
			removeMonster(toRemove);
		} else {
			// In case the tick for the next regeneration has been reached, we
			// will handle it.
			if (i->second._nextRegeneration <= curTick) {
				const int curHitPoints = i->second._monster->getHitPoints();

				// Only do regeneration in case the monster hasn't reached full
				// hit points.
				if (curHitPoints < i->second._monster->getMaxHitPoints())
					// TODO: Consider increasing the hit points based on some stats (Str?)
					i->second._monster->setHitPoints(curHitPoints + 1);

				// TODO: Handle "nextRegeneration" properly
				i->second._nextRegeneration = curTick + 5 * kTicksPerTurn;
			}

			++i;
		}
	}

	// Process the AI
	_monsterAI->update();
}

void Level::processMoveEvent(const MoveEvent &event) throw () {
	assert(isAllowedToAct(event.getMonster()));
	Monster *monster = updateNextActionTick(event.getMonster());
	assert(monster);

	assert(static_cast<unsigned int>(event.getOldPos()._y) <= _map->getHeight() && "Invalid old monster position");
	assert(static_cast<unsigned int>(event.getOldPos()._x) <= _map->getWidth() && "Invalid old monster position");

	assert(static_cast<unsigned int>(event.getNewPos()._y) <= _map->getHeight() && "Invalid new monster position");
	assert(static_cast<unsigned int>(event.getNewPos()._x) <= _map->getWidth() && "Invalid new monster position");

	_monsterField[event.getOldPos()._y * _map->getWidth() + event.getOldPos()._x] = false;
	_monsterField[event.getNewPos()._y * _map->getWidth() + event.getNewPos()._x] = true;
	monster->setPos(event.getNewPos());

	try {
		const TileDefinition &def = _map->tileDefinition(event.getNewPos());
		if (def.getIsLiquid()) {
			monster->setHitPoints(0);
			_eventDisp.dispatch(new DeathEvent(event.getMonster(), DeathEvent::kDrowned));
		}
	} catch (std::out_of_range &e) {
		// This should never happen.
		assert(false);
	}

	_screen->flagForUpdate();
}

void Level::processIdleEvent(const IdleEvent &event) throw () {
	assert(isAllowedToAct(event.getMonster()));
	updateNextActionTick(event.getMonster(), (event.getReason() == IdleEvent::kWary));
}

void Level::processDeathEvent(const DeathEvent &/*event*/) throw () {
	// Nothing to do here.
}

void Level::processAttackEvent(const AttackEvent &event) throw () {
	assert(isAllowedToAct(event.getMonster()));
	const Monster *monster = updateNextActionTick(event.getMonster());
	assert(monster);
	Monster *target = getMonster(event.getTarget());
	assert(target);

	if (Base::rollDice(20) == 20) {
		_eventDisp.dispatch(new AttackFailEvent(event.getMonster(), event.getTarget()));
	} else {
		int damage = 1;
		int newHitPoints = target->getHitPoints() - damage;
		target->setHitPoints(newHitPoints);

		_eventDisp.dispatch(new AttackDamageEvent(event.getMonster(), event.getTarget(), damage != 0));

		if (newHitPoints <= 0)
			_eventDisp.dispatch(new DeathEvent(event.getTarget(), DeathEvent::kKilled, event.getMonster()));
	}

	// Do not remove the monster yet, since some other
	// objects might still use it in the event queue.
}

void Level::processAttackDamageEvent(const AttackDamageEvent &/*event*/) throw () {
	// Nothing to do here.
}

void Level::processAttackFailEvent(const AttackFailEvent &/*event*/) throw () {
	// Nothing to do here.
}

Monster *Level::updateNextActionTick(MonsterID monster, bool oneTickOnly) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i != _monsters.end()) {
		// TODO: In the future the speed should be modified by items the player wears and his condition
		i->second._nextAction = _gameState.getCurrentTick() + (oneTickOnly ? 1 : i->second._monster->getSpeed());
		return i->second._monster;
	} else {
		return 0;
	}
}

} // end of namespace Game

