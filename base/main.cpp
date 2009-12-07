#include "gui/screen.h"
#include "gui/window.h"
#include "gui/input.h"
#include "game/level.h"

#include "rnd.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
	using namespace GUI;

	Screen &scr = Screen::instance();
	Input &in = Input::instance();
	Base::initRNG();

	Window *win1 = new Window(0, 0, 80, 1, false);
	Window *win2 = new Window(0, 1, 80, 23, true);

	win1->clear();
	win2->clear();
	scr.add(win1);
	scr.add(win2);

	Game::Level level;
	level.draw(*win2, 0, 0);
	scr.update();

	unsigned int offX = 0, offY = 0;
	bool redraw = false;
	int input = -1;
	while (input != kKeyEscape) {
		input = in.poll();
		switch (input) {
		case kKeyKeypad4:
			if (offX > 0) {
				--offX;
				redraw = true;
			}
			break;

		case kKeyKeypad6:
			if (level.width() - offX > win2->width()) {
				++offX;
				redraw = true;
			}
			break;

		case kKeyKeypad8:
			if (offY > 0) {
				--offY;
				redraw = true;
			}
			break;

		case kKeyKeypad2:
			if (level.height() - offY > win2->height()) {
				++offY;
				redraw = true;
			}
			break;

		case kKeyKeypad7:
			if (offX > 0 && offY > 0) {
				--offX; --offY;
				redraw = true;
			}
			break;

		case kKeyKeypad9:
			if (level.width() - offX > win2->width() && offY > 0) {
				++offX; --offY;
				redraw = true;
			}
			break;

		case kKeyKeypad1:
			if (offX > 0 && level.height() - offY > win2->height()) {
				--offX; ++offY;
				redraw = true;
			}
			break;

		case kKeyKeypad3:
			if (level.width() - offX > win2->width() && level.height() - offY > win2->height()) {
				++offX; ++offY;
				redraw = true;
			}
			break;

		default:
			break;
		}

		if (redraw) {
			level.draw(*win2, offX, offY);
			scr.update();
			redraw = false;
		}
	}

	delete win1;
	delete win2;

	Screen::destroy();
	Input::destroy();
}
