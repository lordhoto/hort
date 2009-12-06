#include "gui/screen.h"
#include "gui/window.h"

int main(int argc, char *argv[]) {
	using namespace GUI;

	Screen &scr = Screen::instance();

	/*Window *win1 = new Window(0, 0, 80, 1, false);
	Window *win2 = new Window(0, 1, 80, 23, true);

	win1->printLine("You read here: Squolly's RL game will suck!", kBlueOnBlack);
	for (int i = 0; i < 78; ++i)
		for (int j = 0; j < 21; ++j)
			win2->printChar('.', i, j);
	win2->printChar('@', 0, 0, kBlackOnWhite);
	win2->printChar('S', 22, 7, kGreenOnBlack);
	win2->printChar('!', 20, 5, kMagentaOnBlack);
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

	scr.update();

	for(;;)
		;*/


	Screen::destroy();
}
