namespace GUI {

class Screen {
public:
	~Screen();

	static Screen &instance();
	static void destroy();
private:
	Screen();
	static Screen *_instance;
};

}

