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

TileDefinitionLoader::TileDefinitionLoader()
    : Base::DefinitionLoader<TileDefinition>("def-tile;%S,name;:=;%S,glyph;%D,isWalkable;%D,blocksSight;%D,isLiquid") {
}

TileDefinition TileDefinitionLoader::definitionRule(const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception) {
	return TileDefinition(values.find("name")->second,
	                      values.find("glyph")->second[0],
	                      (values.find("isWalkable")->second[0] == '1'),
	                      (values.find("blocksSight")->second[0] == '1'),
	                      (values.find("isLiquid")->second[0] == '1'));
}

} // end of namespace Game

