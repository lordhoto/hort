#include "gui/screen.h"
#include "gui/window.h"

int main(int argc, char *argv[]) {
	using namespace GUI;

	Screen &scr = Screen::instance();

	/*Window *win1 = new Window(0, 0, 80, 1, false);
	Window *win2 = new Window(0, 1, 80, 23, true);

	win1->printLine("Squolly's RL game will suck!");
	for (int i = 0; i < 78; ++i)
		for (int j = 0; j < 21; ++j)
			win2->printChar('.', i, j);
	win2->printChar('@', 0, 0);
	win2->printChar('S', 20, 5);

	scr.add(win1);
	scr.add(win2);

	scr.update();

	for(;;)
		;*/


	Screen::destroy();
}
