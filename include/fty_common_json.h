/*  =========================================================================
    fty_common_json - Functions for parsing JSON strings

    Copyright (C) 2014 - 2018 Eaton

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

#ifndef FTY_COMMON_JSON_H_INCLUDED
#define FTY_COMMON_JSON_H_INCLUDED

#ifdef __cplusplus
#include <climits>
#else
#include <limits.h>
#endif

typedef enum {
    JT_Invalid = INT_MIN,
    JT_None,
    JT_String = 0,
    JT_Object,
    JT_Object_End
} JSON_TYPE;

#ifdef __cplusplus
namespace JSON {
    //  @interface
    // determine start and type of next object in json line
    JSON_TYPE getNextObject (const std::string &line, size_t &start_pos);
    // read string from json
    std::string readString (const std::string &line, size_t &start_pos, size_t &end_pos);
    // read object from json
    std::string readObject (const std::string &line, size_t &start_pos, size_t &end_pos);
    class NotFoundException {};
    class CorruptedLineException {};
}
#endif
//  @end


#endif
