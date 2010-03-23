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

#ifndef BASE_DEFINITIONLOADER_H
#define BASE_DEFINITIONLOADER_H

#include "base/parser.h"

#include <list>

namespace Base {

/**
 * A generic definition loader layout.
 */
template<typename Definition>
class DefinitionLoader : private ParserListener {
public:
	/**
	 * Constructor for the definition loader.
	 *
	 * @param rule The rule definition string.
	 * @see Base::Rule
	 */
	DefinitionLoader(const std::string &rule) : _rule(rule) {}

	/**
	 * The definitions.
	 */
	typedef std::list<Definition> DefinitionList;

	/**
	 * Load monster definitions from the given file.
	 *
	 * @param filename File to load from
	 * @return the monster definitions
	 */
	DefinitionList load(const std::string &filename) throw (NonRecoverableException);

protected:
	/**
	 * Callback for the specific definition loader.
     *
	 * The value map
	 */
	virtual Definition definitionRule(const Matcher::ValueMap &values) throw (ParserListener::Exception) = 0;

private:
	/**
	 * The rule definition.
	 */
	const std::string _rule;

	/**
	 * The storage for all the definitions.
	 */
	DefinitionList _definitions;

	/**
	 * Rule notifiaction as required by Base::ParserListener.
	 *
	 * @see Base::ParserListener::notifyRule
	 */
	void notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) throw (ParserListener::Exception);
};

template<typename Definition>
typename DefinitionLoader<Definition>::DefinitionList DefinitionLoader<Definition>::load(const std::string &filename) throw (NonRecoverableException) {
	_definitions.clear();

	Base::FileParser::RuleMap rules;

	try {
		rules["def"] = Base::Rule(_rule);

		Base::FileParser parser(filename, rules);
		parser.parse(this);
	} catch (Base::Rule::InvalidRuleDefinitionException &e) {
		throw Base::NonRecoverableException(e.toString());
	} catch (Base::Exception &e) {
		// TODO: More information is preferable
		throw Base::NonRecoverableException(e.toString());
	}

	return _definitions;
}

template<typename Definition>
void DefinitionLoader<Definition>::notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) throw (ParserListener::Exception) {
	if (name != "def")
		throw ParserListener::Exception("Unknown rule \"" + name + "\" in DefinitionLoader");

	_definitions.push_back(definitionRule(values));
}

} // end of namespace Base

#endif

