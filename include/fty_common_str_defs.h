/*  =========================================================================
    fty_common_str_defs -

    Copyright (C) 2015 - 2022 Eaton

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

/*!
 * \file str_defs.h
 * \author Karol Hrdina <KarolHrdina@Eaton.com>
 * \brief Not yet documented file
 */

#pragma once

#ifdef __cplusplus

#define AVG_STEPS_SIZE 7
extern const char* AVG_STEPS[AVG_STEPS_SIZE];
#define AVG_TYPES_SIZE 4
extern const char* AVG_TYPES[AVG_TYPES_SIZE];

extern const char* DATETIME_FORMAT;
#define DATETIME_FORMAT_LENGTH 15

extern const char* STRFTIME_DATETIME_FORMAT;

// protocol related
extern const char* BIOS_WEB_AVERAGE_REPLY_JSON_TMPL;
extern const char* BIOS_WEB_AVERAGE_REPLY_JSON_DATA_ITEM_TMPL;

// environment variables
extern const char* EV_BIOS_LOG_LEVEL;
extern const char* EV_LICENSE_DIR;        // directory holding license file
extern const char* EV_DATA_DIR;           // directory holding data (?)
extern const char* EV_DB_INITIALIZED_DIR; // directory holding database ready file

#endif // __cplusplus
