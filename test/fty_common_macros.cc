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

#include <catch2/catch.hpp>
#include "fty_common_macros.h"

#include <iostream>

TEST_CASE("common macros")
{
    SECTION("TR_ME")
    {
        struct {
            std::string tm;
        } testVector[] = {
            { TRANSLATE_ME("File not found") },
            { TRANSLATE_ME("File not found (%s)", "/tmp/fakefile.txt") },
            { TRANSLATE_ME("File '%s' not found", "/tmp/fakefile.txt") },
        };

        for (const auto& it : testVector) {
            std::cout << "TRANSLATE_ME: " << it.tm << std::endl;
        }
    }
    SECTION("JSONIFY")
    {
        struct {
            std::string tm;
        } testVector[] = {
            { JSONIFY("File not found") },
            { JSONIFY("File not found (%s)", "/tmp/fakefile.txt") },
            { JSONIFY("File '%s' not found", "/tmp/fakefile.txt") },
        };

        for (const auto& it : testVector) {
            std::cout << "JSONIFY: " << it.tm << std::endl;
        }
    }
}
