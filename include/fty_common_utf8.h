/*  =========================================================================
    fty_common_utf8 - Functions for handling UTF-8 encoded strings

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

#ifndef FTY_COMMON_UTF8_H_INCLUDED
#define FTY_COMMON_UTF8_H_INCLUDED

#ifdef __cplusplus

#include <cstdint>
#include <string>

#else

#include <stdint.h>

#endif

#include <stdarg.h>

//  Self test of this class
void fty_common_utf8_test (bool verbose);

#ifdef __cplusplus
extern "C" {
#endif

// C wrapper for UTF8::escape
char* utf8_escape(const char *string);

// C wrapper for UTF8::escape
char* utf8_bash_escape(const char *string);

// C wrapper for UTF8::jsonify_translation_string
char *
utf8_jsonify_translation_string (const char *key, ...);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace UTF8 {

int8_t utf8_octets (const char *c);

int utf8_to_codepoint(const char *uchar, char **codepoint);

int utf8eq(const char *s1, const char *s2);

// Convenient wrapper for escape"("const char *string")"
std::string escape(const char *string);

//  Escape string for json output
// Returns escaped json on success, "(null_ptr)" string on null argument
std::string escape(const std::string &before);

// Convert translation string + variable number of args into JSON
std::string
jsonify_translation_string (const char *key, ...);

// Convert translation string + va_list into JSON
std::string
vajsonify_translation_string (const char *key, va_list args);

// escape string for use as a bash command parameter
std::string
bash_escape (std::string& param);

}

#endif

#endif
