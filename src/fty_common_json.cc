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

/*
@header
    fty_common_json - Functions for parsing JSON strings
@discuss
@end
*/

#include "fty_common_json.h"
#include <cxxtools/jsondeserializer.h>
#include <cxxtools/jsonserializer.h>

//
// cxxtools SerializationInfo simple interface
//

namespace JSON {

// write SI to JSON ostringstream
void writeToStream(std::ostringstream& output, const cxxtools::SerializationInfo& si, bool beautify)
{
    cxxtools::JsonSerializer serializer;
    serializer.beautify(beautify);
    serializer.begin(output);
    serializer.serialize(si);
    serializer.finish();
}

// write SI to JSON string
std::string writeToString(const cxxtools::SerializationInfo& si, bool beautify)
{
    std::ostringstream output;
    writeToStream(output, si, beautify);
    return output.str();
}

// write SI to JSON file
void writeToFile(const std::string& path_name, const cxxtools::SerializationInfo& si, bool beautify)
{
    std::ofstream output;
    output.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    output.open(path_name);
    cxxtools::JsonSerializer serializer;
    serializer.beautify(beautify);
    serializer.begin(output);
    serializer.serialize(si);
    serializer.finish();
    output.close();
}

// read/set SI from JSON istringstream
void readFromStream(std::istringstream& input, cxxtools::SerializationInfo& si)
{
    cxxtools::JsonDeserializer deserializer(input);
    deserializer.deserialize(si);
}

// read/set SI from JSON string
void readFromString(const std::string& string, cxxtools::SerializationInfo& si)
{
    std::istringstream input(string);
    readFromStream(input, si);
}

// read/set SI from JSON file
void readFromFile(const std::string& path_name, cxxtools::SerializationInfo& si)
{
    std::ifstream input;
    input.open(path_name);
    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    cxxtools::JsonDeserializer deserializer(input);
    deserializer.deserialize(si);
    input.close();
}

} // namespace JSON
