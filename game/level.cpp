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

#include "base/rnd.h"
#include "ai/monster.h"

#include <cassert>

namespace Game {

Level::Level(GameState &gs) : _map(0), _monsterField(), _screen(0), _gameState(gs), _eventDisp(), _monsters(), _monsterAI(0) {
	_map = new Map();

	_monsterField.resize(_map->width() * _map->height());
	for (unsigned int i = 0; i < _map->width() * _map->height(); ++i)
		_monsterField[i] = false;

	_eventDisp.addHandler(this);
	_monsterAI = new AI::Monster(*this, _eventDisp);
	_eventDisp.addHandler(_monsterAI);

	bool hasDragon = false;
	bool hasNibelung = false;

	for (int i = 0; i < 8192; ++i) {
		Base::Point pos(0, 0);
		do {
			pos._x = Base::rollDice(_map->width()) - 1;
			pos._y = Base::rollDice(_map->height()) - 1;
		} while (!isWalkable(pos));

		MonsterID newId = createNewMonsterID();
		MonsterType newMonsterType = Base::rollDice(g_monsterDatabase.getMonsterTypeCount() - 1);

		if (g_monsterDatabase.getMonsterName(newMonsterType) == std::string("Dragon")) {
			if (hasDragon) {
				--i;
				continue;
			} else {
				hasDragon = true;
			}
		} else if (g_monsterDatabase.getMonsterName(newMonsterType) == std::string("Nibelung")) {
			if (hasNibelung) {
				--i;
				continue;
			} else {
				hasNibelung = true;
			}
		}

		Monster *newMonster = g_monsterDatabase.createNewMonster(newMonsterType);
		newMonster->setPos(pos);
		_monsterField[pos._y * _map->width() + pos._x] = true;
		_monsters[newId] = MonsterEntry(newMonster, _gameState.getCurrentTick());
		_monsterAI->addMonster(newId, newMonster);
	}
}

Level::~Level() {
	makeInactive();

	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if (i->first != kPlayerMonsterID)
			delete i->second.monster;
	}
	delete _map;
}

void Level::makeActive(GUI::Screen &screen, Monster &player) {
	makeInactive();

	screen.setMap(_map);
	for (MonsterMap::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i)
		screen.addObject(i->second.monster);
	screen.addObject(&player);
	_screen = &screen;

	_gameState.setEventDispatcher(&_eventDisp);
	_eventDisp.addHandler(&_gameState);

	_monsterAI->setPlayer(&player);
	_monsters[kPlayerMonsterID] = MonsterEntry(&player, _gameState.getCurrentTick());

	_monsterField[player.getY() * _map->width() + player.getX()] = true;
}

void Level::makeInactive() {
	removeMonster(kPlayerMonsterID);
	_monsterAI->setPlayer(0);
	if (_screen)
		_screen->setMap(0);
	_screen = 0;
	_eventDisp.removeHandler(&_gameState);
	_gameState.setEventDispatcher(0);
}

bool Level::isWalkable(const Base::Point &p) const {
	if ((unsigned int)p._x >= _map->width() || (unsigned int)p._y >= _map->height())
		return false;

	if (!_map->isWalkable(p))
		return false;

	if (_monsterField[p._y * _map->width() + p._x])
		return false;

	return true;
}

MonsterID Level::monsterAt(const Base::Point &p) const {
	for (MonsterMap::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if (i->second.monster->getPos() == p)
			return i->first;
	}

	return kInvalidMonsterID;
}

Monster *Level::getMonster(const MonsterID monster) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return 0;
	else
		return i->second.monster;
}

const Monster *Level::getMonster(const MonsterID monster) const {
	MonsterMap::const_iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return 0;
	else
		return i->second.monster;
}

bool Level::isAllowedToAct(const MonsterID monster) const {
	MonsterMap::const_iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return false;
	else
		return (i->second.nextAction <= _gameState.getCurrentTick());
}

void Level::removeMonster(const MonsterID monster) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return;

	_monsterField[i->second.monster->getY() * _map->width() + i->second.monster->getX()] = false;
	if (monster != kPlayerMonsterID) {
		if (_screen)
			_screen->remObject(i->second.monster);

		delete i->second.monster;
		_monsterAI->removeMonster(i->first);
	}

	_monsters.erase(i);
}

void Level::update() {
	const TickCount curTick = _gameState.getCurrentTick();

	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end();) {
		if (i->second.monster->getHitPoints() <= 0) {
			MonsterID toRemove = i->first;
			++i;
			removeMonster(toRemove);
		} else {
			if (i->second.nextRegeneration <= curTick) {
				int curHitPoints = i->second.monster->getHitPoints();

				if (curHitPoints < i->second.monster->getMaxHitPoints())
					// TODO: Consider increasing the hit points based on some stats (Str?)
					i->second.monster->setHitPoints(curHitPoints + 1);

				// TODO: Handle "nextRegeneration" properly
				i->second.nextRegeneration = curTick + 5 * kTicksPerTurn;
			}
			++i;
		}
	}

	_monsterAI->update();
}

void Level::processEvent(const Event &event) {
	if (event.type == Event::kTypeMove) {
		assert(isAllowedToAct(event.move.monster));
		Monster *monster = updateNextActionTick(event.move.monster);
		assert(monster);

		// TODO: add some error checking
		_monsterField[event.move.oldPos._y * _map->width() + event.move.oldPos._x] = false;
		_monsterField[event.move.newPos._y * _map->width() + event.move.newPos._x] = true;
		monster->setPos(event.move.newPos);

		const TileDatabase::Definition &def = _map->tileDefinition(event.move.newPos);
		if (def._isLiquid) {
			monster->setHitPoints(0);
			_eventDisp.dispatch(createDeathEvent(event.move.monster, Event::Death::kDrowned));
		}

		_screen->flagForUpdate();
	} else if (event.type == Event::kTypeAttack) {
		assert(isAllowedToAct(event.attack.monster));
		const Monster *monster = updateNextActionTick(event.attack.monster);
		assert(monster);
		Monster *target = getMonster(event.attack.target);
		assert(target);

		if (Base::rollDice(20) == 20) {
			_eventDisp.dispatch(createAttackFailEvent(event.attack.monster));
		} else {
			int damage = 1;
			int newHitPoints = target->getHitPoints() - damage;
			target->setHitPoints(newHitPoints);

			_eventDisp.dispatch(createAttackDamageEvent(event.attack.monster, event.attack.target, damage != 0));

			if (newHitPoints <= 0)
				_eventDisp.dispatch(createDeathEvent(event.attack.target, Event::Death::kKilled, event.attack.monster));
		}

		// Do not remove the monster yet, since some other
		// objects might still use it in the event queue.
	} else if (event.type == Event::kTypeIdle) {
		assert(isAllowedToAct(event.idle.monster));
		updateNextActionTick(event.idle.monster, (event.idle.reason == Event::Idle::kWary));
	}
}

Monster *Level::updateNextActionTick(MonsterID monster, bool oneTickOnly) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i != _monsters.end()) {
		i->second.nextAction = _gameState.getCurrentTick() + (oneTickOnly ? 1 : i->second.monster->getSpeed());
		return i->second.monster;
	} else {
		return 0;
	}
}

} // end of namespace Game

