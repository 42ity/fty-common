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
#include "fty_common_base64.h"
#include <iostream>

TEST_CASE("Base64 codec")
{
    SECTION("1")
    {
        struct {
            const std::string plaintext;
            const std::string encoded;
        } testVector[] = {
            { "", "" },
            { "f", "Zg==" },
            { "fo", "Zm8=" },
            { "foo", "Zm9v" },
            { "foob", "Zm9vYg==" },
            { "fooba", "Zm9vYmE=" },
            { "foobar", "Zm9vYmFy" },
        };

        for (auto& it : testVector) {
            std::cout << "Base64: '" << it.plaintext << "'/'" << it.encoded << "'" << std::endl;
            CHECK(Base64::encode(it.plaintext) == it.encoded);
            CHECK(it.plaintext == Base64::decode(it.encoded));
        }
    }

    SECTION("2")
    {
        const std::string lorem =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et "
        "dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip "
        "ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu "
        "fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
        "mollit anim id est laborum";

        CHECK(Base64::decode(Base64::encode(lorem)) == lorem);
    }
}
