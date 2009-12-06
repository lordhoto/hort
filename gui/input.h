#ifndef GUI_INPUT_H
#define GUI_INPUT_H

#include <ncurses.h>

namespace GUI {

enum {
	kKeyKeypad1 = '1',
	kKeyKeypad2 = '2',
	kKeyKeypad3 = '3',
	kKeyKeypad4 = '4',
	kKeyKeypad5 = '5',
	kKeyKeypad6 = '6',
	kKeyKeypad7 = '7',
	kKeyKeypad8 = '8',
	kKeyKeypad9 = '9',
	kKeyBackspace = KEY_BACKSPACE,
	kKeyReturn = KEY_ENTER,
	kKeyEscape = 27
};

class Input {
public:
	int poll();

	static Input &instance();
	static void destroy();
private:
	static Input *_instance;
};

}

#endif

