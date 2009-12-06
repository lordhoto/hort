#include "input.h"
#include "screen.h"

#include <cctype>

namespace GUI {

Input *Input::_instance = 0;

int Input::poll() {
	return wgetch(stdscr);
}

const std::string Input::getLine(Window &win, unsigned int x, unsigned int y) {
	if (x >= win.width() || y >= win.height())
		return std::string();

	int oX, oY;
	getyx(stdscr, oY, oX);
	int sX, sY;
	getbegyx(win._cursesWin, sY, sX);

	Screen &scr = Screen::instance();

	std::string line;

	int input = -1;
	while (input != kKeyReturn) {
		scr.setCursor(sX + x, sY + y);
		scr.update();
		input = poll();

		if (x + 1 < win.width() && std::isprint(input)) {
			win.printChar(input, x, y);
			++x;
			line += input;
		} else if (input == kKeyBackspace && !line.empty()) {
			--x;
			win.printChar(' ', x, y);
			line.erase(line.size() - 1);
		}
	}

	scr.setCursor(oX, oY);

	return line;
}

Input &Input::instance() {
	if (!_instance)
		_instance = new Input();
	return *_instance;
}

void Input::destroy() {
	delete _instance;
	_instance = 0;
}

}

