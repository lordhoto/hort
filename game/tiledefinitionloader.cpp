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

#include "tiledefinitionloader.h"

namespace Game {

TileDefinitionLoader::TileDefinitionList TileDefinitionLoader::load(const std::string &filename) throw (Base::NonRecoverableException) {
	_tiles.clear();

	Base::FileParser::RuleMap rules;

	try {
		rules["def"] = Base::Rule("def-tile;%S,name;:=;%S,glyph;%D,isWalkable;%D,blocksSight;%D,isLiquid");

		Base::FileParser parser(filename, rules);
		parser.parse(this);
	} catch (Base::Rule::InvalidRuleDefinitionException &e) {
		throw Base::NonRecoverableException(e.toString());
	} catch (Base::Exception &e) {
		// TODO: More information is preferable
		throw Base::NonRecoverableException(e.toString());
	}

	return _tiles;
}

void TileDefinitionLoader::notifyRule(const std::string &name, const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception) {
	if (name != "def")
		throw Base::ParserListener::Exception("Unknown rule \"" + name + "\"");

	_tiles.push_back(TileDefinition(values.find("name")->second,
	                                values.find("glyph")->second[0],
	                                (values.find("isWalkable")->second[0] == '1'),
	                                (values.find("blocksSight")->second[0] == '1'),
	                                (values.find("isLiquid")->second[0] == '1')));
}

} // end of namespace Game

