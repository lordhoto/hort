#include <ncurses.h>

namespace GUI {

class Window {
friend class Screen;
public:
	Window(int x, int y, int w, int h, bool border = false);
	~Window();

	void clear();
private:
	const int _w, _h;
	WINDOW *_cursesWin;
	bool _hasBorder;

	void refresh();
	void redraw();
};

}

