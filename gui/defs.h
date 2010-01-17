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

#ifndef GUI_DEFS_H
#define GUI_DEFS_H

#include <ncurses.h>

namespace GUI {

enum ColorPair {
	kWhiteOnBlack = 1,
	kRedOnBlack,
	kGreenOnBlack,
	kYellowOnBlack,
	kBlueOnBlack,
	kMagentaOnBlack,
	kCyanOnBlack
};

#define kUpperLeftEdge ACS_ULCORNER
#define kUpperRightEdge ACS_URCORNER
#define kLowerLeftEdge ACS_LLCORNER
#define kLowerRightEdge ACS_LRCORNER
#define kCross ACS_PLUS
#define kTeePointRight ACS_LTEE
#define kTeePointLeft ACS_RTEE
#define kTeePointUp ACS_BTEE
#define kTeePointDown ACS_TTEE
#define kVerticalLine ACS_VLINE
#define kHorizontalLine ACS_HLINE
#define kDiamond ACS_DIAMOND

enum Attributes {
	kAttribNormal = A_NORMAL,
	kAttribUnderline = A_UNDERLINE,
	kAttribBold = A_BOLD,
	kAttribReverse = A_REVERSE,
	kAttribDim = A_DIM
};

enum Keys {
	kKeyKeypad1 = '1',
	kKeyKeypad2 = '2',
	kKeyKeypad3 = '3',
	kKeyKeypad4 = '4',
	kKeyKeypad5 = '5',
	kKeyKeypad6 = '6',
	kKeyKeypad7 = '7',
	kKeyKeypad8 = '8',
	kKeyKeypad9 = '9',
	kKeyBackspace = KEY_BACKSPACE,
	kKeyReturn = 10,
	kKeyEscape = 27
};

} // end of namespace GUI

#endif
