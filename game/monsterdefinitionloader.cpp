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

#include "monsterdefinitionloader.h"

namespace Game {

MonsterDefinitionLoader::MonsterDefinitionLoader()
    : Base::DefinitionLoader<MonsterDefinition>("def-monster;%S,name;:=;%D,wisMin;%D,wisMax;%D,dexMin;%D,dexMax;%D,agiMin;%D,agiMax;%D,strMin;%D,strMax;%D,hpMin;%D,hpMax;%D,speed") {
}

MonsterDefinition MonsterDefinitionLoader::definitionRule(const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception) {
	const std::string &n = values.find("name")->second;
	const unsigned char wisMin = getVariableValue<unsigned char>("wisMin", values);
	const unsigned char wisMax = getVariableValue<unsigned char>("wisMax", values);
	const unsigned char dexMin = getVariableValue<unsigned char>("dexMin", values);
	const unsigned char dexMax = getVariableValue<unsigned char>("dexMax", values);
	const unsigned char agiMin = getVariableValue<unsigned char>("agiMin", values);
	const unsigned char agiMax = getVariableValue<unsigned char>("agiMax", values);
	const unsigned char strMin = getVariableValue<unsigned char>("strMin", values);
	const unsigned char strMax = getVariableValue<unsigned char>("strMax", values);
	const int hpMin = getVariableValue<int>("strMax", values);
	const int hpMax = getVariableValue<int>("strMax", values);
	const unsigned char speed = getVariableValue<unsigned char>("speed", values);

	return MonsterDefinition(n, Base::ByteRange(wisMin, wisMax),
	                         Base::ByteRange(dexMin, dexMax),
	                         Base::ByteRange(agiMin, agiMax),
	                         Base::ByteRange(strMin, strMax),
	                         Base::IntRange(hpMin, hpMax),
	                         speed);
}

} // end of namespace Game

