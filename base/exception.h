/* Hort - A roguelike inspired by the Nibelungenlied
 *
 * (c) 2009-2010 by Johannes Schickel <lordhoto at scummvm dot org>
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

#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <string>

namespace Base {

/**
 * A base class for all exceptions.
 */
class Exception {
public:
	virtual ~Exception() {}

	/**
	 * @return string description of the exception.
	 */
	virtual std::string toString() const = 0;
};

/**
 * A class for all non recoverable exceptions.
 */
class NonRecoverableException : public Exception {
public:
	NonRecoverableException(const std::string &error) : _error(error) {}

	std::string toString() const {
		return "Non recoverable error occured: \"" + _error + "\"";
	}
private:
	const std::string _error;
};

/**
 * A generic class describing an I/O error.
 */
class IOException : public Exception {};

/**
 * A generic exception in file I/O.
 */
class FileIOException : public IOException {
public:
	FileIOException(const std::string &filename) : _filename(filename) {}

	/**
	 * @return the filename.
	 */
	const std::string &getFilename() const { return _filename; }

private:
	std::string _filename; //< The specified filename
};

/**
 * Generic exception, which describes that a given file does not exist.
 */
class FileNotFoundException : public FileIOException {
public:
	FileNotFoundException(const std::string &filename) : FileIOException(filename) {}

	std::string toString() const {
		return "File not found: \"" + getFilename() + "\"";
	}
};

} // end of namespace Base

#endif

