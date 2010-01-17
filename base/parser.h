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

#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include "token.h"

#include <string>
#include <list>
#include <map>

namespace Base {

class Rule {
public:
	Rule(const std::string &rule, const std::string &name);

	typedef std::list<std::string> StringList;
	const StringList &getParts() const { return _parts; }

	const std::string getName() const { return _name; }
private:
	StringList _parts;
	const std::string _name;
};

class Matcher {
public:
	Matcher(Tokenizer &input, const Rule &rule);
	Matcher(const std::string &line, const Rule &rule);

	bool wasSuccessful() const { return _ok; }
	
	const std::string &getError() const { return _error; }

	typedef std::map<std::string, std::string> ValueMap;
	const ValueMap &getValues() const { return _values; }
private:
	const Rule &_rule;

	void doMatch(Tokenizer &input);

	void parseVariable(const std::string &def, Tokenizer &tokenizer);
	void parseString(const std::string &name, Tokenizer &tokenizer);
	void parseInteger(const std::string &name, Tokenizer &tokenizer);

	bool _ok;
	std::string _error;
	ValueMap _values;
};

} // end of namespace Base

#endif

