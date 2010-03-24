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

#include "exception.h"

#include <string>
#include <list>
#include <map>
#include <exception>
#include <istream>

#include <boost/shared_ptr.hpp>

namespace Base {

/**
 * A simple rule.
 *
 * This allows for defining a parsing rule with optional
 * variables.
 *
 * Consider the following statement:
 *  def MonsterType := Name HP
 * where "MonsterType", "Name" and "HP" are variables.
 *
 * To write a rule for this one has to use the following:
 *  "def;%S,type;:=;%S,name;%D,hp"
 *
 * Note the ";" defining the urge for a delimiter in the input.
 *
 * Via %X,name a variable is defined, where "X" is the type
 * and "name" is the variable's name.
 *
 * Currently the following types are allowed:
 * s/S : string
 * d/D : integer
 */
class Rule {
public:
	Rule() : _parts() {}

	/**
	 * This exception is raised, when the rule does not match the above
	 * mentioned syntax.
	 */
	class InvalidRuleDefinitionException : public Base::Exception {
	public:
		InvalidRuleDefinitionException(const std::string &rule, const std::string &error) : _rule(rule), _error(error) {}

		/**
		 * Returns the offending rule.
		 */
		const std::string getRule() const { return _rule; }

		/**
		 * Returns the error string.
		 */
		const std::string getError() const { return _error; }

		std::string toString() const {
			return "Invalid rule \"" + _rule + "\":" + _error;
		}

	private:
		const std::string _rule;
		const std::string _error;
	};

	/**
	 * Create a new rule.
	 *
	 * @param rule Definition of the rule (see above).
	 * @param name Name of the rule.
	 */
	Rule(const std::string &rule) throw (InvalidRuleDefinitionException);

	/**
	 * A "part" of a rule, this is either a string to match against
	 * or a variable definition.
	 */
	class Part {
	public:
		/**
		 * Type definition of the part.
		 */
		enum Type {
			/**
			 * Whether the part is a fixed string to match.
			 */
			kTypeString,

			/**
			 * Whether the part is a variable to match.
			 */
			kTypeVariable
		};

		Part(Type type) : _type(type) {}
		virtual ~Part() {}

		/**
		 * @return the type of the rule part.
		 */
		Type getType() const { return _type; }

	private:
		const Type _type;
	};

	/**
	 * A string part.
	 */
	class StringPart : public Part {
	public:
		StringPart(const std::string &s) : Part(kTypeString), _string(s) {}

		/**
		 * @return the string which must be present.
		 */
		const std::string &getString() const { return _string; }
	private:
		const std::string _string;
	};

	/**
	 * A variable part.
	 */
	class VariablePart : public Part {
	public:
		/**
		 * The variable type.
		 */
		enum VariableType {
			/**
			 * Whether it's an integer variable.
			 */
			kVariableTypeInteger,

			/**
			 * Whether it's an string variable.
			 */
			kVariableTypeString
		};

		VariablePart(const std::string &n, VariableType vT) : Part(kTypeVariable), _name(n), _variableType(vT) {}

		/**
		 * @return the name of the variable.
		 */
		const std::string &getName() const { return _name; }

		/**
		 * @return the variable type.
		 */
		VariableType getVariableType() const { return _variableType; }
	private:
		const std::string _name;
		const VariableType _variableType;
	};

	typedef boost::shared_ptr<const Part> PartPointer;
	typedef std::list<PartPointer> PartList;

	/**
	 * Queries all the tokens required by the rule.
	 */
	const PartList &getParts() const { return _parts; }
private:
	void createVariable(const std::string &rule, const std::string &definition) throw (InvalidRuleDefinitionException);

	PartList _parts;
};

/**
 * A simple matcher, which matches a given line against a given rule.
 * It will save all the variables defined by the rule in a map.
 *
 * It allso allows some simple error checking.
 */
class Matcher {
public:
	/**
	 * Creates a matcher, which will match the input "line"
	 * with the rule "rule".
	 *
	 * @param line Input line.
	 * @param rule Rule to match against.
	 */
	Matcher(const std::string &line, const Rule &rule);

	/**
	 * Whether the rule was successfully matched.
	 */
	bool wasSuccessful() const { return _ok; }

	/**
	 * Returns an error description.
	 */
	const std::string &getError() const { return _error; }

	typedef std::map<std::string, std::string> ValueMap;

	/**
	 * Queries the map containing all variable values.
	 * The variable name is the "key" in this case.
	 */
	const ValueMap &getValues() const { return _values; }
private:
	const Rule &_rule;

	void matchString(const Rule::StringPart &part, const std::string &token);
	void matchVariable(const Rule::VariablePart &part, const std::string &token);

	bool _ok;
	std::string _error;
	ValueMap _values;
};

/**
 * A listener to parsing. This object gets notified, whenever
 * a rule has been parsed successfully.
 */
class ParserListener {
public:
	virtual ~ParserListener() {}

	/**
	 * An exception the listener can throw, in case something is wrong with the matched rule.
	 */
	class Exception : public Base::Exception {
	public:
		Exception(const std::string &description) : _error(description) {}

		const std::string &getDescription() const { return _error; }

		std::string toString() const {
			return _error;
		}
	private:
		std::string _error;
	};


	/**
	 * Notifies the listener about the given rule being parsed successfully.
	 *
	 * @param name Name of the rule, which was parsed successfully.
	 * @param variables Variables, which were parsed.
	 */
	virtual void notifyRule(const std::string &name, const Matcher::ValueMap &variables) throw (Exception) = 0;
};

/**
 * An object which parses a file given the specified rules.
 */
class FileParser {
public:
	/**
	 * A generic parsing exception.
	 */
	class ParseException : public Exception {};

	/**
	 * Exception, which is thrown in case no matching rule could be found.
	 */
	class NoMatchingRuleException : public ParseException {
	public:
		NoMatchingRuleException(const std::string &contents, int line) : _contents(contents), _line(line) {}

		/**
		 * @return the line, which could not be matched.
		 */
		int getLine() const { return _line; }

		std::string toString() const;
	private:
		const std::string _contents; //< Contents of the line
		int _line; //< Line which could not be matched against any rule
	};

	/**
	 * Indicates that even though a rule was matched fine, the listener
	 * thought that something is wrong.
	 */
	class ListenerErrorException : public ParseException {
	public:
		ListenerErrorException(const std::string &desc) : _desc(desc) {}

		/**
		 * @return return the description of the error.
		 */
		const std::string &getDescription() const { return _desc; }

		std::string toString() const {
			return "Listener threw the following error: \"" + _desc + "\"";
		}
	private:
		std::string _desc; //< The description
	};

	/**
	 * The rule map.
	 *
	 * key = name of the rule
	 * value = rule description
	 */
	typedef std::map<std::string, Rule> RuleMap;

	/**
	 * Creates a file parser.
	 *
	 * @param filename File to parse.
	 * @param rules All the allowed rules.
	 */
	FileParser(const std::string &filename, const RuleMap &rules) throw (FileNotFoundException);
	~FileParser() { delete _file; }

	/**
	 * Parses the file and sends all notifications
	 * to the (optional) listener.
	 *
	 * @param listener Where to send notifications.
	 */
	void parse(ParserListener *listener) throw (NoMatchingRuleException, ListenerErrorException);
private:
	std::istream *_file;
	const RuleMap _rules;

	bool parseLine(const std::string &line, ParserListener *listener) throw (ParserListener::Exception);
};

} // end of namespace Base

#endif

