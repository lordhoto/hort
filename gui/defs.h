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
	kCyanOnBlack,
	kBlackOnWhite,
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

}

#endif
