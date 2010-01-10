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

Level::Level(EventDispatcher &eventDisp) : _map(0), _screen(0), _monsters(), _monsterAI(0) {
	_map = new Map();

	eventDisp.addHandler(this);
	_monsterAI = new AI::Monster(*this);
	eventDisp.addHandler(_monsterAI);
	_monsterAI->setEventDispatcher(&eventDisp);

	for (int i = 0; i < 10; ++i) {
		int monsterX = 0, monsterY = 0;
		do {
			monsterX = Base::rollDice(_map->width()) - 1;
			monsterY = Base::rollDice(_map->height()) - 1;
		} while (!isWalkable(monsterX, monsterY));

		MonsterID newId = createNewMonsterID();
		Monster *newMonster = new Monster(kMonsterGnome, 2, 4, 4, 6, 3, monsterX, monsterY);
		_monsters[newId] = newMonster;
		_monsterAI->addMonster(newId, newMonster);
	}
}

Level::~Level() {
	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if (i->first != kPlayerMonsterID)
			delete i->second;
	}
	delete _map;
}

void Level::assignScreen(Screen &screen, Monster &player) {
	unassignScreen();

	screen.setMap(_map);
	for (MonsterMap::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i)
		screen.addObject(i->second);
	screen.addObject(&player, true);
	_screen = &screen;

	_monsters[kPlayerMonsterID] = &player;
}

void Level::unassignScreen() {
	removeMonster(kPlayerMonsterID);
	if (_screen)
		_screen->setMap(0);
	_screen = 0;
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
		if (i->second->getX() == x && i->second->getY() == y)
			return i->first;
	}

	return kInvalidMonsterID;
}

Monster *Level::getMonster(const MonsterID monster) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return 0;
	else
		return i->second;
}

const Monster *Level::getMonster(const MonsterID monster) const {
	MonsterMap::const_iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return 0;
	else
		return i->second;
}

void Level::removeMonster(const MonsterID monster) {
	MonsterMap::iterator i = _monsters.find(monster);
	if (i == _monsters.end())
		return;

	if (monster != kPlayerMonsterID) {
		if (_screen)
			_screen->remObject(i->second);

		delete i->second;
		_monsterAI->removeMonster(i->first);
	}

	_monsters.erase(i);
}

void Level::processEvent(const Event &event) {
	if (event.type == Event::kTypeMove) {
		Monster *monster = getMonster(event.data.move.monster);
		assert(monster);

		// TODO: add some error checking
		monster->setX(monster->getX() + event.data.move.offX);
		monster->setY(monster->getY() + event.data.move.offY);

		_screen->flagForUpdate();
	} else if (event.type == Event::kTypeAttack) {
		Monster *target = getMonster(event.data.attack.target);
		assert(target);

		int newHitPoints = target->getHitPoints() - 1;
		target->setHitPoints(newHitPoints);

		if (newHitPoints <= 0)
			removeMonster(event.data.attack.target);
	}
}

} // end of namespace Game

