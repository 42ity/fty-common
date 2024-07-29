/*  =========================================================================
    fty_common_json - Functions for parsing JSON strings

    Copyright (C) 2014 - 2020 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

#pragma once

//
// cxxtools SerializationInfo simple interface
//

#include <string>
#include <fstream>
#include <iostream>

namespace cxxtools {
class SerializationInfo;
}

namespace JSON {

/**
 * \brief Read/set a SerializationInfo object from a JSON file.
 * \param[in]   path_name - the path to the JSON file
 * \param[out]  si - cxxtools::SerializationInfo object
 * \throw std::ifstream::failbit | std::ifstream::badbit | generic exceptions
 */
void readFromFile(const std::string& path_name, cxxtools::SerializationInfo& si);

/**
 * \brief Read/set a SerializationInfo object from a JSON string.
 * \param[in]   string - the JSON string
 * \param[out]  si - cxxtools::SerializationInfo object
 * \throw generic exceptions
 */
void readFromString(const std::string& string, cxxtools::SerializationInfo& si);

/**
 * \brief Read/set a SerializationInfo object from a JSON istringstream.
 * \param[in]   input - the stream
 * \param[out]  si - cxxtools::SerializationInfo object
 * \throw generic exceptions
 */
void readFromStream(std::istringstream& input, cxxtools::SerializationInfo& si);

/**
 * \brief Write a SerializationInfo object into a JSON file.
 * \param[in]  path_name - the path to JSON file
 * \param[in]  si - cxxtools::SerializationInfo object
 * \param[in]  beautify - beautify'er
 * \throw std::ofstream::failbit | std::ofstream::badbit | generic exceptions
 */
void writeToFile(const std::string& path_name, const cxxtools::SerializationInfo& si, bool beautify = true);

/**
 * \brief Write a SerializationInfo object into a JSON string.
 * \param[in]  si - cxxtools::SerializationInfo object
 * \param[in]  beautify - beautify'er
 * \return the result string
 * \throw generic exceptions
 */
std::string writeToString(const cxxtools::SerializationInfo& si, bool beautify = true);

/**
 * \brief Write a SerializationInfo object into a JSON ostringstream.
 * \param[out] output - the stream
 * \param[in]  si - cxxtools::SerializationInfo object
 * \param[in]  beautify - beautify'er
 * \throw generic exceptions
 */
void writeToStream(std::ostringstream& output, const cxxtools::SerializationInfo& si, bool beautify = true);

} // namespace JSON
