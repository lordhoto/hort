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

#include "parser.h"

#include <sstream>

namespace Base {

Rule::Rule(const std::string &rule, const std::string &name)
    : _parts(), _name(name) {
	Tokenizer tokenizer(rule, ";");

	while (!tokenizer.finished())
		_parts.push_back(tokenizer.nextToken());
}

Matcher::Matcher(const std::string &line, const Rule &rule)
    : _rule(rule), _ok(false), _error(), _values() {
	Tokenizer input(line);
	doMatch(input);
}

void Matcher::doMatch(Tokenizer &input) {
	const Rule::StringList &parts = _rule.getParts();
	Rule::StringList::const_iterator curPart = parts.begin();

	_ok = true;

	while (!input.finished() && curPart != parts.end()) {
		if ((*curPart)[0] == '%') {
			parseVariable(*curPart, input);
		} else {
			std::string curToken = input.nextToken();
			if (*curPart != curToken) {
				_error = "Found " + curToken + ", but expected " + *curPart;
				_ok = false;
			}
		}

		if (!_ok)
			return;

		++curPart;
	}

	_ok = (curPart == parts.end() && input.finished());
}

void Matcher::parseVariable(const std::string &def, Tokenizer &tokenizer) {
	Tokenizer var(def, "%,");
	std::string type = var.nextToken();
	std::string name = var.nextToken();

	if (type.empty()) {
		_error = "Variable type empty";
		_ok = false;
	} else if (name.empty()) {
		_error = "Variable name empty";
		_ok = false;
	} else {
		if (type == "s" || type == "S") {
			parseString(name, tokenizer);
		} else if (type == "d" || type == "D") {
			parseInteger(name, tokenizer);
		} else {
			_error = "Unknown variable type " + type;
			_ok = false;
		}
	}
}

void Matcher::parseString(const std::string &name, Tokenizer &tokenizer) {
	std::string value = tokenizer.nextToken();

	if (value[0] == '"') {
		value.erase(0, 1);
		bool foundEnd = false;
		while (!tokenizer.finished() && !foundEnd) {
			value += " ";
			value += tokenizer.nextToken();

			size_t end = value.find('"');
			if (end != std::string::npos) {
				foundEnd = true;
				value.erase(end);
			}
		}

		if (!foundEnd) {
			_error = "Expected ending \" for string variable \"" + name + '"';
			_ok = false;
			return;
		}
	}

	_values[name] = value;
}

void Matcher::parseInteger(const std::string &name, Tokenizer &tokenizer) {
	std::string value = tokenizer.nextToken();

	std::stringstream ss(value);
	int val = 0;
	ss >> val;

	std::stringstream testSS;
	testSS << val;

	if (testSS.str() != value) {
		_error = '"' + value + "\" is not a valid integer";
		_ok = false;
	} else {
		_values[name] = value;
	}
}

} // end of namespace Base

