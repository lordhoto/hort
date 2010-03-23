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

#include "drawdesc.h"

#include "game/tiledatabase.h"
#include "game/monsterdatabase.h"

#include <cassert>

#include <boost/foreach.hpp>

namespace GUI {
namespace Intern {

DrawDescParser::DrawDescParser(const std::string &suffix)
    : DefinitionLoader("def" + suffix + ";%S,name;:=;%S,glyph;%S,color;%S,attribs") {
}

DrawDescParser::DefinitionLoader::Definition DrawDescParser::definitionRule(const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception) {
	Base::Matcher::ValueMap::const_iterator n = values.find("name");
	Base::Matcher::ValueMap::const_iterator g = values.find("glyph");
	Base::Matcher::ValueMap::const_iterator c = values.find("color");
	Base::Matcher::ValueMap::const_iterator a = values.find("attribs");

	return DrawDescParser::DefinitionLoader::Definition(n->second, DrawDesc(parseSymbol(g->second), parseColor(c->second), parseAttribs(a->second)));
}

int DrawDescParser::parseSymbol(const std::string &value) throw (Base::ParserListener::Exception) {
	if (value.size() == 1) {
		return value[0];
	} else {
		if (value == "kUpperLeftEdge")
			return kUpperLeftEdge;
		else if (value == "kUpperRightEdge")
			return kUpperRightEdge;
		else if (value == "kLowerLeftEdge")
			return kLowerLeftEdge;
		else if (value == "kLowerRightEdge")
			return kLowerRightEdge;
		else if (value == "kCross")
			return kCross;
		else if (value == "kTeePointRight")
			return kTeePointRight;
		else if (value == "kTeePointLeft")
			return kTeePointLeft;
		else if (value == "kTeePointUp")
			return kTeePointUp;
		else if (value == "kTeePointDown")
			return kTeePointDown;
		else if (value == "kVerticalLine")
			return kVerticalLine;
		else if (value == "kHorizontalLine")
			return kHorizontalLine;
		else if (value == "kDiamond")
			return kDiamond;
		else
			throw Base::ParserListener::Exception("Unknown glyph value \"" + value + '"');
	}
}

ColorPair DrawDescParser::parseColor(const std::string &value) throw (Base::ParserListener::Exception) {
	if (value == "kWhiteOnBlack")
		return kWhiteOnBlack;
	else if (value == "kRedOnBlack")
		return kRedOnBlack;
	else if (value == "kGreenOnBlack")
		return kGreenOnBlack;
	else if (value == "kYellowOnBlack")
		return kYellowOnBlack;
	else if (value == "kBlueOnBlack")
		return kBlueOnBlack;
	else if (value == "kMagentaOnBlack")
		return kMagentaOnBlack;
	else if (value == "kCyanOnBlack")
		return kCyanOnBlack;
	else
		throw Base::ParserListener::Exception("Unknown color value \"" + value + '"');
}

int DrawDescParser::parseAttribs(const std::string &value) throw (Base::ParserListener::Exception) {
	if (value == "kAttribNormal")
		return kAttribNormal;
	else if (value == "kAttribUnderline")
		return kAttribUnderline;
	else if (value == "kAttribBold")
		return kAttribBold;
	else if (value == "kAttribReverse")
		return kAttribReverse;
	else if (value == "kAttribDim")
		return kAttribDim;
	else if (value == "kAttribUnderlineBold")
		return (kAttribUnderline | kAttribBold);
	else if (value == "kAttribUnderlineBoldReverse")
		return (kAttribUnderline | kAttribBold | kAttribReverse);
	else if (value == "kAttribUnderlineReverse")
		return (kAttribUnderline | kAttribReverse);
	else if (value == "kAttribUnderlineDimReverse")
		return (kAttribUnderline | kAttribDim | kAttribReverse);
	else if (value == "kAttribBoldReverse")
		return (kAttribBold | kAttribReverse);
	else if (value == "kAttribDimReverse")
		return (kAttribDim | kAttribReverse);
	else
		throw Base::ParserListener::Exception("Unknown attribs value \"" + value + '"');
}

TileDDMap *parseTileDefinitons(const std::string &filename) throw (std::string, Base::NonRecoverableException) {
	DrawDescParser parser("-tile");
	DrawDescParser::DefinitionList dds = parser.load(filename);
	TileDDMap::DrawDescMap drawDescs;

	Game::TileDatabase &tdb = Game::TileDatabase::instance();
	const Game::Tile lastTileType = tdb.getTileCount();

	BOOST_FOREACH(const DrawDescParser::DefinitionList::value_type &i, dds) {
		const Game::Tile tile = tdb.queryTile(i.first);
		if (tile < lastTileType)
			drawDescs[tile] = i.second;
		else
			throw std::string("Unknown tile \"" + i.first + '"');
	}

	for (Game::Tile i = 0; i < lastTileType; ++i) {
		if (drawDescs.find(i) == drawDescs.end()) {
			const Game::TileDefinition *def = tdb.queryTileDefinition(i);
			assert(i);
			throw std::string("Missing tile definition for \"" + def->getName() + '"');
		}
	}

	return new TileDDMap(drawDescs);
}

MonsterDDMap *parseMonsterDefinitions(const std::string &filename) throw (std::string, Base::NonRecoverableException) {
	DrawDescParser parser("-monster");
	DrawDescParser::DefinitionList dds = parser.load(filename);
	MonsterDDMap::DrawDescMap drawDescs;

	Game::MonsterDatabase &mdb = g_monsterDatabase;
	const Game::MonsterType lastMonsterType = mdb.getMonsterTypeCount();

	BOOST_FOREACH(const DrawDescParser::DefinitionList::value_type &i, dds) {
		const Game::MonsterType type = mdb.queryMonsterType(i.first);
		if (type == lastMonsterType)
			throw std::string("Undefined monster \"" + i.first + '"');

		drawDescs[type] = i.second;
	}

	for (Game::MonsterType i = 0; i < lastMonsterType; ++i) {
		if (drawDescs.find(i) == drawDescs.end())
			throw std::string("Missing monster definition for \"" + std::string(mdb.getMonsterName(i)) + '"');
	}

	return new MonsterDDMap(drawDescs);
}

} // end of namespace Intern
} // end of namespace GUI

