/*  =========================================================================
    fty_common_quote_codec - Functions for handling quote codec

    Copyright (C) 2014 - 2022 Eaton

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
#include <iostream>
#include "fty_common_quote_codec.h"


TEST_CASE("string_quote_codec", "[quote]") {
    struct {
        const std::string in; //input
    } testVector[] = {
        { "" },
        { "\"" },
        { "\"\"\"\"" },
        { "\"hello\"" },
        { "hello" },
        { "привет" }, //Russian
        { "你好" }, //Chinese simplified
        { "صباح الخير" }, //Arab
    };

    int cnt = 0;
    for (auto & it: testVector) {
        std::string input = it.in;
        std::string enc = quotecodec::quoteEncode(input);
        std::string dec = quotecodec::quoteDecode(enc);

        std::cout << cnt << ": " << "input('" << input << "'), enc('" << enc << "'), dec('" << dec << "')" << std::endl;
        cnt++;

        bool hasQuote = (input.find('"') != std::string::npos);

        if (hasQuote) {
            CHECK(input != enc);
        } else {
            CHECK(input == enc);
        }

        CHECK(enc.find('"') == std::string::npos);
        CHECK(input == dec);

        std::string enc2 = quotecodec::quoteEncode(enc);
        CHECK(enc == enc2);
    }
}
