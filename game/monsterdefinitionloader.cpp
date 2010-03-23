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

#include <boost/lexical_cast.hpp>

namespace Game {

MonsterDefinitionLoader::MonsterDefinitionLoader()
    : Base::DefinitionLoader<MonsterDefinition>("def-monster;%S,name;:=;%D,wisMin;%D,wisMax;%D,dexMin;%D,dexMax;%D,agiMin;%D,agiMax;%D,strMin;%D,strMax;%D,hpMin;%D,hpMax;%D,speed") {
}

MonsterDefinition MonsterDefinitionLoader::definitionRule(const Base::Matcher::ValueMap &values) throw (Base::ParserListener::Exception) {
	try {
		const std::string &n = values.find("name")->second;
		const int wisMin = boost::lexical_cast<int>(values.find("wisMin")->second);
		const int wisMax = boost::lexical_cast<int>(values.find("wisMax")->second);
		const int dexMin = boost::lexical_cast<int>(values.find("dexMin")->second);
		const int dexMax = boost::lexical_cast<int>(values.find("dexMax")->second);
		const int agiMin = boost::lexical_cast<int>(values.find("agiMin")->second);
		const int agiMax = boost::lexical_cast<int>(values.find("agiMax")->second);
		const int strMin = boost::lexical_cast<int>(values.find("strMin")->second);
		const int strMax = boost::lexical_cast<int>(values.find("strMax")->second);
		const int hpMin = boost::lexical_cast<int>(values.find("hpMin")->second);
		const int hpMax = boost::lexical_cast<int>(values.find("hpMax")->second);
		const unsigned char speed = boost::lexical_cast<int>(values.find("speed")->second);

		return MonsterDefinition(n, Base::ByteRange(wisMin, wisMax),
		                         Base::ByteRange(dexMin, dexMax),
		                         Base::ByteRange(agiMin, agiMax),
		                         Base::ByteRange(strMin, strMax),
		                         Base::IntRange(hpMin, hpMax),
		                         speed);
	} catch (boost::bad_lexical_cast &e) {
		// This should never happen, since the values are
		// prechecked by the parser.
		assert(false);
		return MonsterDefinition();
	}
}

} // end of namespace Game

