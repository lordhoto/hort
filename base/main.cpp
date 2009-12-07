#include "gui/screen.h"
#include "gui/window.h"
#include "gui/input.h"

#include "rnd.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
	using namespace GUI;

	Screen &scr = Screen::instance();
	Input &in = Input::instance();

	/*Window *win1 = new Window(0, 0, 80, 1, false);
	Window *win2 = new Window(0, 1, 80, 23, true);

	win1->printLine("You read here: Squolly's RL game will suck!", 0, 0, kBlueOnBlack, kAttribBold | kAttribUnderline);

	scr.add(win1);
	scr.add(win2);

	int x = 0, y = 0;

	win2->printChar('@', x, y, kWhiteOnBlack, kAttribBold);
	scr.setCursor(*win2, x, y);
	bool redraw = false;

	scr.update();

	Base::initRNG();

	int input = -1;
	while (input != kKeyEscape) {
		input = in.poll();
		switch (input) {
		case kKeyKeypad7:
			--x; --y;
			redraw = true;
			break;

		case kKeyKeypad8:
			--y;
			redraw = true;
			break;

		case kKeyKeypad9:
			++x; --y;
			redraw = true;
			break;

		case kKeyKeypad4:
			--x;
			redraw = true;
			break;

		case kKeyKeypad6:
			++x;
			redraw = true;
			break;

		case kKeyKeypad1:
			--x; ++y;
			redraw = true;
			break;

		case kKeyKeypad2:
			++y;
			redraw = true;
			break;

		case kKeyKeypad3:
			++x; ++y;
			redraw = true;
			break;

		case '#': {
			win1->clear();
			win1->printLine("#", 0, 0);
			std::string line = in.getLine(*win1, 1, 0);
			if (line == "exit") {
				input = kKeyEscape;
			} else if (line == "rnd") {
				win1->clear();
				int d6 = Base::rollDice(6);
				char buf[12];
				snprintf(buf, 12, "%d", d6);
				win1->printLine(buf, 0, 0);
				scr.update();
				in.poll();
			} else {
				win1->clear();
				win1->printLine("Unknown command: ", 0, 0);
				win1->printLine(line.c_str(), 17, 0);
				win1->printLine("-- More --", 17 + line.size() + 1, 0, kWhiteOnBlack, kAttribBold);
				scr.update();
				in.poll();
				win1->clear();
				win1->printLine("You read here: Squolly's RL game will suck!", 0, 0, kBlueOnBlack, kAttribBold | kAttribUnderline);
				scr.update();
			}
			} break;

		default:
			break;
		}

		if (redraw) {
			x = std::max(x, 0);
			y = std::max(y, 0);
			x = std::min<unsigned int>(x, win2->width() - 1);
			y = std::min<unsigned int>(y, win2->height() - 1);

			win2->clear();
			win2->printChar('@', x, y, kWhiteOnBlack, kAttribBold);
			scr.setCursor(*win2, x, y);
			scr.update();
		}
	}*/

	Screen::destroy();
	Input::destroy();
}
