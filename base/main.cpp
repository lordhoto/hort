#include "gui/screen.h"
#include "gui/window.h"
#include "gui/input.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
	using namespace GUI;

	Screen &scr = Screen::instance();
	Input &in = Input::instance();

	/*Window *win1 = new Window(0, 0, 80, 1, false);
	Window *win2 = new Window(0, 1, 80, 23, true);

	win1->printLine("You read here: Squolly's RL game will suck!", kBlueOnBlack, kAttribBold | kAttribUnderline);
	for (int i = 0; i < 78; ++i)
		for (int j = 0; j < 21; ++j)
			win2->printChar('.', i, j);
	win2->printChar('@', 0, 0, kBlackOnWhite);
	win2->printChar('L', 2, 0, kMagentaOnBlack);
	win2->printChar('c', 1, 2, kYellowOnBlack);
	win2->printChar('c', 2, 2, kYellowOnBlack, kAttribBold);
	win2->printChar('c', 2, 1, kYellowOnBlack, kAttribBold);
	win2->printChar('!', 3, 0, kRedOnBlack);
	win2->printChar('!', 4, 0, kRedOnBlack, kAttribBold);
	win2->printChar(' ', 5, 0, kWhiteOnBlack, kAttribUnderline);
	win2->printChar('H', 0, 2, kMagentaOnBlack);
	win2->printChar('S', 22, 7, kGreenOnBlack);
	win2->printChar('!', 20, 5, kMagentaOnBlack);
	win2->printChar('(', 20, 7, kYellowOnBlack, kAttribBold);
	win2->printChar(kDiamond, 1, 0, kBlueOnBlack);
	win2->printChar(kDiamond, 1, 1, kBlueOnBlack);
	win2->printChar(kDiamond, 0, 1, kBlueOnBlack);

	win2->printChar(kUpperLeftEdge, 19, 4);
	win2->printChar(kHorizontalLine, 20, 4);
	win2->printChar(kUpperRightEdge, 21, 4);
	win2->printChar(kVerticalLine, 19, 5);
	win2->printChar(kVerticalLine, 21, 5);
	win2->printChar(kTeePointRight, 19, 6);
	win2->printChar(kHorizontalLine, 20, 6);
	win2->printChar(kCross, 21, 6);
	win2->printChar(kVerticalLine, 19, 7);
	win2->printChar(kVerticalLine, 21, 7);
	win2->printChar(kVerticalLine, 23, 7);
	win2->printChar(kLowerLeftEdge, 19, 8);
	win2->printChar(kHorizontalLine, 20, 8);
	win2->printChar(kTeePointUp, 21, 8);
	win2->printChar(kHorizontalLine, 22, 8);
	win2->printChar(kLowerRightEdge, 23, 8);

	scr.add(win1);
	scr.add(win2);

	scr.setCursor(1, 2);

	scr.update();

	for(;;) {
		win1->clear();
		win1->printLine("You wish for: ", 0, 0);
		std::string input = in.getLine(*win1, 14, 0);
		scr.update();

		if (input == "exit")
			break;
	}*/

	Screen::destroy();
	Input::destroy();
}
