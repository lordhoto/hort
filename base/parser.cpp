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
#include <fstream>
#include <iterator>
#include <algorithm>

#include <boost/foreach.hpp>

namespace Base {

Rule::Rule(const std::string &rule) throw (InvalidRuleDefinitionException)
    : _parts() {
	typedef boost::char_separator<char> Separator;
	typedef boost::tokenizer<Separator> Tokenizer;

	Tokenizer tokenizer(rule, Separator(";", "%"));

	for (Tokenizer::const_iterator t = tokenizer.begin(); t != tokenizer.end(); ++t) {
		if (*t == "%") {
			// The next token should contain both type and name
			++t;

			// Check for premature end of the token list
			if (t == tokenizer.end())
				throw InvalidRuleDefinitionException(rule, "Missing variable information");

			createVariable(rule, *t);
		} else {
			_parts.push_back(PartPointer(new StringPart(*t)));
		}
	}
}

void Rule::createVariable(const std::string &rule, const std::string &definition) throw (InvalidRuleDefinitionException) {
	typedef boost::char_separator<char> Separator;
	typedef boost::tokenizer<Separator> Tokenizer;

	Tokenizer varDef(definition, Separator(","));
	Tokenizer::const_iterator vT = varDef.begin();

	// Check for an empty token list
	if (vT == varDef.end())
		throw InvalidRuleDefinitionException(rule, "Variable definition is empty");

	// Extract the variable type
	const std::string type = *vT++;

	// Check for premature end of the definition
	if (vT == varDef.end())
		throw InvalidRuleDefinitionException(rule, "Missing variable name");

	// Extract the variable name
	const std::string name = *vT++;

	// Check for an empty name
	if (name.empty())
		throw InvalidRuleDefinitionException(rule, "Empty variable name given");

	// Check whether a valid type is specified.
	VariablePart::VariableType pType;

	if (type == "s" || type == "S")
		pType = VariablePart::kVariableTypeString;
	else if (type == "d" || type == "D")
		pType = VariablePart::kVariableTypeInteger;
	else
		throw InvalidRuleDefinitionException(rule, "Invalid type \"" + type + "\"");

	// Check whether we have any extra tokens
	if (vT != varDef.end())
		throw InvalidRuleDefinitionException(rule, "Variable definition has extra tokens");

	// Create a part description
	_parts.push_back(PartPointer(new VariablePart(name, pType)));
}

Matcher::Matcher(const std::string &line, const Rule &rule)
    : _rule(rule), _ok(false), _error(), _values() {
	Tokenizer input(line, Separator(" \t\r\n"));
	doMatch(input);
}

void Matcher::doMatch(const Tokenizer &input) {
	const Rule::PartList &parts = _rule.getParts();
	Tokenizer::const_iterator token = input.begin();

	_ok = true;

	BOOST_FOREACH(Rule::PartPointer p, parts) {
		// Check whether we reached the end of the token
		// list yet.
		if (token == input.end()) {
			_error = "Premature token list end";
			_ok = false;

			return;
		}

		// Check the part type.
		switch (p->type) {
		case Rule::Part::kTypeString:
			matchString(*(const Rule::StringPart *)p.get(), *token++);
			break;

		case Rule::Part::kTypeVariable:
			matchVariable(*(const Rule::VariablePart *)p.get(), *token++);
			break;
		}

		if (!_ok)
			return;
	}

	_ok = (token == input.end());
}

void Matcher::matchString(const Rule::StringPart &part, const std::string &token) {
	if (part.string != token) {
		_error = "Found \"" + token + "\" but expected: \"" + part.string + "\"";
		_ok = false;
	}
}

void Matcher::matchVariable(const Rule::VariablePart &part, const std::string &token) {
	switch (part.variableType) {
	case Rule::VariablePart::kVariableTypeInteger: {
		std::stringstream input(token), verify;

		int variable;
		input >> variable;
		verify << variable;

		if (verify.str() != token) {
			_error = "\"" + token + "\" is no integer variable";
			_ok = false;
		} else {
			_values[part.name] = token;
		}
		} break;

	case Rule::VariablePart::kVariableTypeString:
		if (token.empty()) {
			_error = "Found empty string variable \"" + part.name + "\"";
			_ok = false;
		} else {
			_values[part.name] = token;
		}
		break;
	}
}

std::string FileParser::NoMatchingRuleException::toString() const {
	std::stringstream sstr;
	sstr << "Line " << _line << " \"" << _contents << "\" could not be matched against any rule!";
	return sstr.str();
}

FileParser::FileParser(const std::string &filename, const RuleMap &rules) throw (FileNotFoundException)
    : _file(0), _rules(rules) {
	_file = new std::ifstream(filename.c_str());

	if (!*_file) {
		delete _file;
		throw FileNotFoundException(filename);
	}
}

void FileParser::parse(ParserListener *listener) throw (NoMatchingRuleException, ListenerErrorException) {
	_file->seekg(0);

	size_t lineCount = 0;

	while (!_file->eof()) {
		std::string line;
		std::getline(*_file, line);

		if (!line.empty()) {
			try {
				if (!parseLine(line, listener))
					throw NoMatchingRuleException(line, lineCount);
			} catch (ParserListener::Exception &e) {
				throw ListenerErrorException(e.getDescription());
			}
		}

		++lineCount;
	}
}

bool FileParser::parseLine(const std::string &line, ParserListener *listener) throw (ParserListener::Exception) {
	for (RuleMap::const_iterator i = _rules.begin(); i != _rules.end(); ++i) {
		Matcher matcher(line, i->second);

		if (matcher.wasSuccessful()) {
			if (listener)
				listener->notifyRule(i->first, matcher.getValues());
			return true;
		}
	}

	return false;
}

} // end of namespace Base

