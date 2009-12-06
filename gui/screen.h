#ifndef GUI_SCREEN_H
#define GUI_SCREEN_H

#include <list>

namespace GUI {

class Window;

enum ColorPair {
	kWhiteOnBlack = 1,
	kRedOnBlack,
	kGreenOnBlack,
	kYellowOnBlack,
	kBlueOnBlack,
	kMagentaOnBlack,
	kCyanOnBlack,
	kBlackOnWhite,
};

class Screen {
public:
	~Screen();

	static Screen &instance();
	static void destroy();

	void clear();

	void add(Window *window);
	void remove(Window *window);

	void update();
private:
	Screen();
	static Screen *_instance;

	bool _needRedraw;

	typedef std::list<Window *> WindowList;
	WindowList _windows;
};

}

#endif

