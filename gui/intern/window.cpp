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

#include "window.h"
#include "screen.h"

#include <cassert>
#include <cstring>
#include <algorithm>

namespace GUI {

Window::Window(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool border)
    : _x(border ? x + 1 : x),
      _y(border ? y + 1 : y),
      _w(border ? w - 2 : w),
      _h(border ? h - 2 : h),
      _rX(x), _rY(y), _rW(w), _rH(h),
      _content(0),
      _hasBorder(border),
      _needsRefresh(true) {
	Screen &screen = Screen::instance();

	assert(x + w <= screen.width());
	assert(y + h <= screen.height());

	_content = new int[_rW * _rH];
	assert(_content);
	clear();
}

Window::~Window() {
	delete[] _content;
}

int Window::getCharData(int ch, ColorPair color, int attrib) {
	return ch | COLOR_PAIR(color) | attrib; 
}

void Window::putData(unsigned int x, unsigned int y, unsigned int width,
                     unsigned int height, const int *data, unsigned int pitch) {
	if (y >= _h || x >= _w)
		return;
	if (y + height > _h || x + width > _w)
		return;

	if (_hasBorder) {
		++y;
		++x;
	}

	int *dst = _content + y * _rW + x;

	while (height--) {
		std::memcpy(dst, data, width * sizeof(int));
		dst += _rW;
		data += pitch;
	}

	_needsRefresh = true;
}

void Window::printChar(int ch, unsigned int x, unsigned int y, ColorPair color, int attrib) {
	if (y >= _h || x >= _w)
		return;

	if (_hasBorder) {
		++y;
		++x;
	}

	_content[y * _rW + x] = getCharData(ch, color, attrib);
	_needsRefresh = true;
}

void Window::printLine(const char *str, ColorPair color, int attrib) {
	const size_t strLength = std::min<size_t>(_w, std::strlen(str));
	printLine(str, (_w - strLength) / 2, _h / 2, color, attrib);
}

void Window::printLine(const char *str, unsigned int x, unsigned int y, ColorPair color, int attrib) {
	if (y >= _h || x >= _w)
		return;

	for (; x < _w && *str; ++str, ++x)
		printChar(*str, x, y, color, attrib);
}

void Window::clear() {
	int *dst = _content;
	for (unsigned int y = 0; y < _rH; ++y) {
		for (unsigned int x = 0; x < _rW; ++x)
			*dst++ = ' ';
	}

	if (_hasBorder) {
		dst[      0] = ACS_ULCORNER;
		dst[_rW - 1] = ACS_URCORNER;
		dst[(_rH - 1) * _rW + 0] = ACS_LLCORNER;
		dst[(_rH - 1) * _rW + _rW - 1] = ACS_LLCORNER;

		int *dst1 = _content + 1, *dst2 = _content + (_rH - 1) * _rW + 1;
		// Top/Bottom line
		for (unsigned int i = 0; i < _w; ++i)
			*dst1++ = *dst2++ = ACS_HLINE;

		// Left/Right line
		dst1 = _content + 1 * _rW;
		dst2 = _content + 2 * _rW - 1; 

		for (unsigned int i = 0; i < _h; ++i) {
			*dst1 = *dst2 = ACS_VLINE;
			dst1 += _rW;
			dst2 += _rW;
		}
	}
	_needsRefresh = true;
}

void Window::redraw(bool force) {
	if (_needsRefresh || force) {
		const int *src = _content;

		move(_rY, _rX);
		for (unsigned int y = 0; y < _rH; ++y) {
			if (_rY != 0 || _rX != 0 || _rW != 80)
				move(_rY + y, _rX);

			for (unsigned int x = 0; x < _rW; ++x)
				addch(*src++);
		}
	}
}

} // end of namespace GUI

