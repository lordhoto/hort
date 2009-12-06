#include "gui/screen.h"
#include "gui/window.h"

int main(int argc, char *argv[]) {
	using namespace GUI;

	Screen &scr = Screen::instance();
	Screen::destroy();
}
