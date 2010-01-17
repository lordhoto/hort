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

#ifndef GUI_INTERN_DRAWDESC_H
#define GUI_INTERN_DRAWDESC_H

#include "gui/defs.h"

#include "base/parser.h"

#include "game/map.h"

#include <map>
#include <string>

namespace GUI {
namespace Intern {

struct DrawDesc {
	DrawDesc() : symbol(0), color(kWhiteOnBlack), attribs(0) {}
	DrawDesc(int symbol, ColorPair color, int attribs) : symbol(symbol), color(color), attribs(attribs) {}

	int symbol;
	ColorPair color;
	int attribs;
};

template<typename Key>
class ASCIIRepresentation {
public:
	typedef std::map<Key, DrawDesc> DrawDescMap;

	ASCIIRepresentation(const DrawDescMap &descs) : _descs(descs) {}

	bool hasEntry(const Key &key) const {
		return (_descs.find(key) != _descs.end());
	}

	DrawDesc lookUp(const Key &key) const {
		typename DrawDescMap::const_iterator i = _descs.find(key);
		if (i != _descs.end())
			return i->second;
		else
			return DrawDesc();
	}
private:
	DrawDescMap _descs;
};

class DrawDescParser : public Base::ParserListener {
public:
	DrawDescParser(const std::string &filename, const std::string &suffix);
	~DrawDescParser();

	/**
	 * Does all the parsing.
	 *
	 * Note that this might throw a std::string, in case an
	 * error happened.
	 */
	void parse();

	void notifyRule(const std::string &name, const Base::Matcher::ValueMap &variables);

	typedef std::map<std::string, DrawDesc> DrawDescMap;
	const DrawDescMap &getDescs() const { return _descs; }
private:
	Base::FileParser *_parser;

	DrawDescMap _descs;

	int parseSymbol(const std::string &value);
	ColorPair parseColor(const std::string &value);
	int parseAttribs(const std::string &value);
};

typedef ASCIIRepresentation<Game::Map::Tile> TileDDMap;
TileDDMap *parseTileDefinitons(const std::string &filename);

} // end of namespace Intern
} // end of namespace GUI

#endif

