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

#include "base/definitionloader.h"

#include "game/map.h"
#include "game/monsterdefinition.h"

#include <map>
#include <string>

namespace GUI {
namespace Intern {

struct DrawDesc {
	DrawDesc() : _symbol(0), _color(kWhiteOnBlack), _attribs(0) {}
	DrawDesc(chtype symbol, ColorPair color, int attribs) : _symbol(symbol), _color(color), _attribs(attribs) {}

	chtype _symbol;
	ColorPair _color;
	int _attribs;
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

class DrawDescParser : public Base::DefinitionLoader<std::pair<std::string, DrawDesc> > {
public:
	typedef Base::DefinitionLoader<std::pair<std::string, DrawDesc> > DefinitionLoader;

	DrawDescParser(const std::string &suffix);
private:
	DefinitionLoader::Definition definitionRule(const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception);

	chtype parseSymbol(const std::string &value) throw (Base::ParserListener::Exception);
	ColorPair parseColor(const std::string &value) throw (Base::ParserListener::Exception);
	int parseAttribs(const std::string &value) throw (Base::ParserListener::Exception);
};

typedef ASCIIRepresentation<Game::Tile> TileDDMap;
TileDDMap *parseTileDefinitons(const std::string &filename) throw (Base::NonRecoverableException);

typedef ASCIIRepresentation<Game::MonsterType> MonsterDDMap;
MonsterDDMap *parseMonsterDefinitions(const std::string &filename) throw (Base::NonRecoverableException);

} // end of namespace Intern
} // end of namespace GUI

#endif

