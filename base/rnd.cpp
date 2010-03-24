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

#include "rnd.h"

#include <stdint.h>
#include <ctime>

namespace {
uint32_t g_rndSeed = 0;
}

namespace Base {

void initRNG() {
	// TODO: Implement a nicer random number generator
	g_rndSeed = (uint32_t)std::time(0);
}

unsigned int rollDice(unsigned int pips) {
	if (!pips)
		return 0;

	g_rndSeed = 0xDEADBF03 * (g_rndSeed + 1);
	g_rndSeed = (g_rndSeed >> 13) | (g_rndSeed << 19);
	return (g_rndSeed % pips) + 1;
}

unsigned int rollDice(unsigned int num, unsigned int pips) {
	unsigned int count = 0;
	while (num--)
		count += rollDice(pips);
	return count;
}

unsigned int rndValueRange(unsigned int min, unsigned int max) {
	return min + rollDice(max - min + 1) - 1;
}

unsigned int rndValueRange(const IntRange &range) {
	return rndValueRange(range.getMin(), range.getMax());
}

unsigned char rndValueRange(const ByteRange &range) {
	return static_cast<unsigned char>(rndValueRange(range.getMin(), range.getMax()));
}

} // end of namespace Base

