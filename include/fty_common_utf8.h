/*  =========================================================================
    fty_common_utf8 - Functions for handling UTF-8 encoded strings

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

#ifndef FTY_COMMON_UTF8_H_INCLUDED
#define FTY_COMMON_UTF8_H_INCLUDED

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

//  Self test of this class
void fty_common_utf8_test (bool verbose);

namespace UTF8 {

int8_t utf8_octets (const char *c);

int utf8_to_codepoint(const char *uchar, char **codepoint);

int utf8eq(const char *s1, const char *s2);
}

#ifdef __cplusplus
}
#endif

#endif
