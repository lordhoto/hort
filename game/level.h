#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "gui/window.h"

#include <vector>

namespace Game {

class Level {
public:
	enum Tile {
		kTileMeadow,
		kTileTree,
		kTileWater
	};

	Level();

	void draw(GUI::Window &out, unsigned int oX, unsigned int oY) const;
	void draw(GUI::Window &out, unsigned int oX, unsigned int oY, unsigned int x, unsigned int y, unsigned int w, unsigned int h) const;

	Tile tileAt(unsigned int x, unsigned int y) const { return _tiles[y * _w + x]; }

	unsigned int width() const { return _w; }
	unsigned int height() const { return _h; }
private:
	unsigned int _w, _h;
	std::vector<Tile> _tiles;

	void printTile(GUI::Window &out, const Tile t, unsigned int x, unsigned int y) const;
};

}

#endif

