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

		_monsters.push_back(new Monster(kMonsterGnome, 2, 4, 4, 6, 3, monsterX, monsterY));
	}
}

Level::~Level() {
	for (MonsterList::iterator i = _monsters.begin(); i != _monsters.end(); ++i)
		delete *i;
	delete _map;
}

void Level::assignScreen(GameScreen &screen, const Monster &player) {
	screen.setMap(_map);
	for (MonsterList::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i)
		screen.addObject(*i);
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

	for (MonsterList::const_iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if ((*i)->getX() == x && (*i)->getY() == y)
			return false;
	}

	return true;
}

Monster *Level::monsterAt(unsigned int x, unsigned int y) {
	for (MonsterList::iterator i = _monsters.begin(); i != _monsters.end(); ++i) {
		if ((*i)->getX() == x && (*i)->getY() == y)
			return *i;
	}

	return 0;
}

void Level::removeMonster(Monster *monster) {
	_monsters.remove(monster);
	if (_screen)
		_screen->remObject(monster);
	delete monster;
}

} // end of namespace Game

