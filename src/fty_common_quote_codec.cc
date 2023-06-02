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

#include "fty_common_quote_codec.h"

namespace quotecodec {

std::string quoteEncode(const std::string& string)
{
    std::string ret;

    for (const auto& c : string) {
        ret += (c == '"') ? "\\u0022" : std::string{c};
    }

    return ret;
}

std::string quoteDecode(const std::string& string)
{
    std::string ret{string};

    while(1) {
        size_t pos = ret.find("\\u0022");
        if (pos == std::string::npos)
            break;
        ret.replace(pos, 6, "\""); // 6 = len "\\u0022"
    }

    return ret;
}

} // namespace quotecodec
