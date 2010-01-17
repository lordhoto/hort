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

#ifndef BASE_TOKEN_H
#define BASE_TOKEN_H

#include <string>

namespace Base {

/**
 * A simple object, which allows to tokenize a string based on a list of delimiters.
 */
class Tokenizer {
public:
	/**
	 * Creates a tokenizer.
	 *
	 * @param input String to tokenize.
	 * @param delims List of delimiters (by default space, horizontal tab, carriage return and line feed).
	 */
	Tokenizer(const std::string &input, const std::string &delims = " \t\r\n");

	/**
	 * Queries whether no more tokens are left.
	 */
	bool finished() const;

	/**
	 * Queries the next token.
	 */
	std::string nextToken();
private:
	const std::string _input;
	const std::string _delimiters;

	size_t _start, _end;
};

} // end of namespace Base

#endif

