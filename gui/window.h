#include <ncurses.h>

namespace GUI {

class Window {
friend class Screen;
public:
	Window(int x, int y, int w, int h, bool border = false);
	~Window();

	const int width() const { return _w; }
	const int height() const { return _h; }

	void printLine(const char *str);
	void printLine(const char *str, int x, int y);
	void printChar(char ch, int x, int y);

	void clear();
private:
	const int _w, _h;
	WINDOW *_cursesWin;
	bool _hasBorder;

	void refresh();
	void redraw();
};

}

