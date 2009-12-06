#include "input.h"

namespace GUI {

Input *Input::_instance = 0;

int Input::poll() {
	return wgetch(stdscr);
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

