#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "screen.h"

#include <ncurses.h>

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

namespace GUI {

class Window {
friend class Screen;
public:
	Window(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool border = false);
	~Window();

	const int width() const { return _w; }
	const int height() const { return _h; }

	void printLine(const char *str, ColorPair color = kWhiteOnBlack);
	void printLine(const char *str, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack);
	void printChar(int ch, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack);

	void clear();
private:
	const unsigned int _w, _h;
	WINDOW *_cursesWin;
	bool _hasBorder;

	void refresh();
	void redraw();
};

}

#endif

