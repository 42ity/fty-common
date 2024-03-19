/*  =========================================================================
    fty_common_base64 - Functions for base64 codec

    Copyright (C) 2014 - 2024 Eaton

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

#include "fty_common_base64.h"
#include <cxxtools/base64codec.h>

namespace Base64 {

std::string encode(const std::string& message)
{
    return encode(message.c_str(), message.size());
}

std::string encode(const char* message, size_t len)
{
    std::string ret; // empty

    if (message && (len != 0)) {
        ret = cxxtools::Base64Codec::encode(message, unsigned(len));

        // libcxxtools10 Base64Codec::encode() add automatic lineend wrapping (\r\n @ col 80)
        // *remove* wrappers after encoding
        std::string lineend{cxxtools::Base64Codec().lineend()};
        if (!lineend.empty()) {
            size_t pos{0};
            while ((pos = ret.find(lineend, pos)) != std::string::npos) {
                ret.replace(pos, lineend.size(), "");
            }
        }
    }
    return ret;
}

std::string decode(const std::string& message)
{
    std::string ret; // empty

    if (!message.empty()) {
        ret = cxxtools::Base64Codec::decode(message);
    }
    return ret;
}

}
