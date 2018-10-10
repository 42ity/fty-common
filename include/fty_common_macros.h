/*  =========================================================================
    fty_common_macros - System-wide preprocessor macros

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

#ifndef FTY_COMMON_MACROS_H_INCLUDED
#define FTY_COMMON_MACROS_H_INCLUDED

#include <fty_common_utf8.h>

// macro for strings we want to process by xgettext
#define TRANSLATE_ME(...) utf8_jsonify_translation_string (__VA_ARGS__)

// macro for strings we want to turn into JSON, but not translate in BE
#define JSONIFY(...) TRANSLATE_ME(__VA_ARGS__)

// macro for lua alert rules for dynamically processed templates, not really intended to be used C
#define TRANSLATE_LUA(...) __VA_ARGS__

#endif
