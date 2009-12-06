#include <list>

namespace GUI {

class Window;

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

