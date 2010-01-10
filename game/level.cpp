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

namespace Game {

Level::Level() : _map(0), _screen(0), _monsters() {
	_map = new Map();

	for (int i = 0; i < 10; ++i) {
		int monsterX = 0, monsterY = 0;
		do {
			monsterX = Base::rollDice(_map->width()) - 1;
			monsterY = Base::rollDice(_map->height()) - 1;
		} while (!isWalkable(monsterX, monsterY));

		_monsters[createNewMonsterID()] = new Monster(kMonsterGnome, 2, 4, 4, 6, 3, monsterX, monsterY);
	}
}

Level::~Level() {
	for (MonsterMap::iterator i = _monsters.begin(); i != _monsters.end(); ++i)
		delete i->second;
	delete _map;
}

void Level::assignScreen(Screen &screen, const Monster &player) {
	screen.setMap(_map);
	for (MonsterMap::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i)
		screen.addObject(i->second);
	screen.addObject(&player, true);
	_screen = &screen;
}

void Level::unassignScreen() {
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

	if (_screen)
		_screen->remObject(i->second);
	delete i->second;
	_monsters.erase(i);
}

} // end of namespace Game

