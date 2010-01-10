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

#include "monster.h"

#include <cassert>

namespace Game {

const MonsterID kPlayerMonsterID = 0;
const MonsterID kInvalidMonsterID = 0xFFFFFFFF;

namespace {
MonsterID g_nextMonsterID = kPlayerMonsterID + 1;
} // end of anonymous namespace

MonsterID createNewMonsterID() {
	return g_nextMonsterID++;
}

namespace {

struct MonsterName {
	MonsterType type;
	const char *name;
} g_monsterNames[] = {
	{ kMonsterPlayer, "Player" },
	{ kMonsterGnome, "Gnome" },
	{ kMonsterSquolly, "Squolly" }
};

} // end of anonymous namespace

const char *getMonsterName(MonsterType type) {
	for (size_t i = 0; i < sizeof(g_monsterNames)/sizeof(g_monsterNames[0]); ++i) {
		if (g_monsterNames[i].type == type)
			return g_monsterNames[i].name;
	}

	assert(0);
}

} // end of namespace Game

