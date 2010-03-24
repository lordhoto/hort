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

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace Base {

/**
 * A generic definition loader layout.
 */
template<typename D>
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
	 * The definition base type.
	 */
	typedef D Definition;

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
	 * @param values The value map
	 * @return The definition parsed.
	 */
	virtual Definition definitionRule(const Matcher::ValueMap &values) throw (ParserListener::Exception) = 0;

	/**
	 * Tries to read the given variable as the template type T.
	 *
	 * This will only work for integer based variables! For example
	 * a type "char" will read a integer in the range of char and
	 * not a character!
	 * This will throw an exception in case the variable is not
	 * of the specified type.
	 *
	 * @param name Variable to read.
	 * @param values Value map to use.
	 * @return Variable as T.
	 */
	template<typename T>
	T getVariableValue(const std::string &name, const Matcher::ValueMap &values) throw (ParserListener::Exception);

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

/**
 * A variable getter, used to implement DefintionLoader::getVariableValue.
 */
template<typename T>
struct VariableGetter {
	/**
	 * This function does exactly the same as DefintionLoader::getVariableValue.
	 *
	 * @see DefintionLoader::getVariableValue
	 */
	T operator()(const std::string &name, const Matcher::ValueMap &values) throw (ParserListener::Exception) {
		try {
			return boost::lexical_cast<T>(values.find(name)->second);
		} catch (boost::bad_lexical_cast &e) {
			throw ParserListener::Exception("Illegal value for variable \"" + name + "\": " + e.what());
		}
	}
};

/**
 * A variable getter, used to implement DefintionLoader::getVariableValue for unsigned char.
 *
 * This is required because of how boost::lexical_cast works.
 */
template<>
struct VariableGetter<unsigned char> {
	/**
	 * This function does exactly the same as DefintionLoader::getVariableValue.
	 *
	 * @see DefintionLoader::getVariableValue
	 */
	unsigned char operator()(const std::string &name, const Matcher::ValueMap &values) throw (ParserListener::Exception) {
		try {
			return boost::numeric_cast<unsigned char>(boost::lexical_cast<int>(values.find(name)->second));
		} catch (boost::bad_lexical_cast &e) {
			throw ParserListener::Exception("Illegal value for variable \"" + name + "\": " + e.what());
		} catch (boost::numeric::bad_numeric_cast &e) {
			throw ParserListener::Exception("Illegal value for variable \"" + name + "\": " + e.what());
		}
	}
};

/**
 * A variable getter, used to implement DefintionLoader::getVariableValue for signed char.
 *
 * This is required because of how boost::lexical_cast works.
 */
template<>
struct VariableGetter<signed char> {
	/**
	 * This function does exactly the same as DefintionLoader::getVariableValue.
	 *
	 * @see DefintionLoader::getVariableValue
	 */
	unsigned char operator()(const std::string &name, const Matcher::ValueMap &values) throw (ParserListener::Exception) {
		try {
			return boost::numeric_cast<signed char>(boost::lexical_cast<int>(values.find(name)->second));
		} catch (boost::bad_lexical_cast &e) {
			throw ParserListener::Exception("Illegal value for variable \"" + name + "\": " + e.what());
		} catch (boost::numeric::bad_numeric_cast &e) {
			throw ParserListener::Exception("Illegal value for variable \"" + name + "\": " + e.what());
		}
	}
};

/**
 * A variable getter, used to implement DefintionLoader::getVariableValue for char.
 *
 * This is required because of how boost::lexical_cast works.
 */
template<>
struct VariableGetter<char> {
	unsigned char operator()(const std::string &name, const Matcher::ValueMap &values) throw (ParserListener::Exception) {
		try {
			return boost::numeric_cast<char>(boost::lexical_cast<int>(values.find(name)->second));
		} catch (boost::bad_lexical_cast &e) {
			throw ParserListener::Exception("Illegal value for variable \"" + name + "\": " + e.what());
		} catch (boost::numeric::bad_numeric_cast &e) {
			throw ParserListener::Exception("Illegal value for variable \"" + name + "\": " + e.what());
		}
	}
};

template<typename Definition>
template<typename T>
T DefinitionLoader<Definition>::getVariableValue(const std::string &name, const Matcher::ValueMap &values) throw (ParserListener::Exception) {
	return VariableGetter<T>()(name, values);
}

} // end of namespace Base

#endif

