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

#include "level.h"

#include "base/rnd.h"
#include "ai/monster.h"

#include <cassert>

namespace Game {

Level::Level(GameState &gs) : _map(0), _screen(0), _gameState(gs), _eventDisp(), _monsters(), _monsterAI(0) {
	_map = new Map();

	_eventDisp.addHandler(this);
	_monsterAI = new AI::Monster(*this, _eventDisp);
	_eventDisp.addHandler(_monsterAI);

	for (int i = 0; i < 20; ++i) {
		int monsterX = 0, monsterY = 0;
		do {
			monsterX = Base::rollDice(_map->width()) - 1;
			monsterY = Base::rollDice(_map->height()) - 1;
		} while (!isWalkable(monsterX, monsterY));

		MonsterID newId = createNewMonsterID();
		Monster *newMonster = 0;
		if (Base::rollDice(6) != 1)
			newMonster = new Monster(kMonsterGnome, 2, 4, 4, 6, 3, monsterX, monsterY);
		else
			newMonster = new Monster(kMonsterSquolly, 10, 1, 1, 1, 1, monsterX, monsterY);
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

void Level::makeActive(Screen &screen, Monster &player) {
	makeInactive();

	screen.setMap(_map);
	for (MonsterMap::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i)
		screen.addObject(i->second.monster);
	screen.addObject(&player, true);
	_screen = &screen;

	_gameState.setEventDispatcher(&_eventDisp);
	_eventDisp.addHandler(&_gameState);

	_monsters[kPlayerMonsterID] = MonsterEntry(&player, _gameState.getCurrentTick());
}

void Level::makeInactive() {
	removeMonster(kPlayerMonsterID);
	if (_screen)
		_screen->setMap(0);
	_screen = 0;
	_eventDisp.removeHandler(&_gameState);
	_gameState.setEventDispatcher(0);
}

bool Level::isWalkable(unsigned int x, unsigned int y) const {
	if (x >= _map->width() || y >= _map->height())
		return false;

	if (!_map->isWalkable(x, y))
		return false;

	if (monsterAt(x, y) != kInvalidMonsterID)
		return false;

	return true;
}

MonsterID Level::monsterAt(unsigned int x, unsigned int y) const {
	for (MonsterMap::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if (i->second.monster->getX() == x && i->second.monster->getY() == y)
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

	if (monster != kPlayerMonsterID) {
		if (_screen)
			_screen->remObject(i->second.monster);

		delete i->second.monster;
		_monsterAI->removeMonster(i->first);
	}

	_monsters.erase(i);
}

void Level::update() {
	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end();) {
		if (i->second.monster->getHitPoints() <= 0) {
			MonsterID toRemove = i->first;
			++i;
			removeMonster(toRemove);
		} else {
			++i;
		}
	}

	_monsterAI->update();
}

void Level::processEvent(const Event &event) {
	if (event.type == Event::kTypeMove) {
		assert(isAllowedToAct(event.data.move.monster));
		updateNextActionTick(event.data.move.monster);

		Monster *monster = getMonster(event.data.move.monster);
		assert(monster);

		// TODO: add some error checking
		monster->setX(event.data.move.newX);
		monster->setY(event.data.move.newY);

		_screen->flagForUpdate();
	} else if (event.type == Event::kTypeAttack) {
		assert(isAllowedToAct(event.data.attack.monster));
		updateNextActionTick(event.data.attack.monster);

		const Monster *monster = getMonster(event.data.attack.monster);
		assert(monster);
		Monster *target = getMonster(event.data.attack.target);
		assert(target);

		if (Base::rollDice(20) == 20) {
			_eventDisp.dispatch(createAttackFailEvent(event.data.attack.monster));
		} else {
			int damage = 0;

			if (monster->getType() != kMonsterSquolly)
				damage = 1;

			int newHitPoints = target->getHitPoints() - damage;
			target->setHitPoints(newHitPoints);

			_eventDisp.dispatch(createAttackDamageEvent(event.data.attack.monster, event.data.attack.target, damage != 0));

			if (newHitPoints <= 0)
				_eventDisp.dispatch(createDeathEvent(event.data.attack.target, event.data.attack.monster));
		}

		// Do not remove the monster yet, since some other
		// objects might still use it in the event queue.
	}
}

void Level::updateNextActionTick(MonsterID monster) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i != _monsters.end())
		i->second.nextAction = _gameState.getCurrentTick() + i->second.monster->getSpeed();
}

} // end of namespace Game

