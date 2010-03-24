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

#ifndef BASE_RND_H
#define BASE_RND_H

#include "defs.h"

namespace Base {

/**
 * Initializes our simple minded random number god.
 */
void initRNG();

/**
 * Rolls a dice with the given number of pips.
 * The result will in in [1, pips] and a
 * natural number.
 *
 * @param pips The pips count.
 * @return The result of the roll.
 */
unsigned int rollDice(unsigned int pips);

/**
 * Rolls a given dice a given number of times.
 *
 * @param num How often to roll the dice.
 * @param pips The pips count.
 * @return The accumulated result of all rolls.
 */
unsigned int rollDice(unsigned int num, unsigned int pips);

/**
 * Gets a random number in the given range.
 *
 * @param min Minimal value.
 * @param max Maximal value.
 * @return The random value.
 */
unsigned int rndValueRange(unsigned int min, unsigned int max);

/**
 * Gets a random number in the given range.
 *
 * @param range The range
 * @return The random value.
 */
unsigned int rndValueRange(const IntRange &range);

/**
 * Gets a random number in the given range.
 *
 * @param range The range
 * @return The random value.
 */
unsigned char rndValueRange(const ByteRange &range);

} // end of namespace Base

#endif

