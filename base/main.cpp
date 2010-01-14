/* Hort - A roguelike inspired by the Nibelungenlied
 *
 * (c) 2009 by Johannes Schickel <lordhoto at scummvm dot org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "gui/screen.h"
#include "gui/input.h"

#include "game/state.h"
#include "game/game.h"

#include "rnd.h"

#include <cstdio>

int main(int argc, char *argv[]) {
	GUI::Screen::instance();

	if (GUI::Screen::instance().width() < 80 || GUI::Screen::instance().height() < 24) {
		GUI::Screen::destroy();
		std::fprintf(stderr, "ERROR: Terminal size must be at least 80x24\n");
		return -1;
	}

	GUI::Input::instance();
	Base::initRNG();

	Game::StateHandler states;
	states.addStateToQueue(new Game::GameState());
	states.process();

	GUI::Screen::destroy();
	GUI::Input::destroy();
}
