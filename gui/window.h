#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "defs.h"

#include <ncurses.h>

namespace GUI {

class Window {
friend class Screen;
public:
	Window(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool border = false);
	~Window();

	const int width() const { return _w; }
	const int height() const { return _h; }

	void printLine(const char *str, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);
	void printLine(const char *str, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);
	void printChar(int ch, unsigned int x, unsigned int y, ColorPair color = kWhiteOnBlack, int attrib = kAttribNormal);

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

