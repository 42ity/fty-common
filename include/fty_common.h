/*  =========================================================================
    fty-common - Provides common tools for agents

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

#pragma once

// Afwul hack, but I need exceptions to be valid for whole content
#ifdef __cplusplus
#include <cxxtools/allocator.h>
#include <stdexcept>
#include <fty_log.h>
/// general IPM exception
class IPMException : public std::exception { };
#endif // __cplusplus

//  Opaque class structures to allow forward references
//  These classes are stable or legacy and built in all releases
typedef struct _fty_common_base_t fty_common_base_t;
#define FTY_COMMON_BASE_T_DEFINED
typedef struct _fty_common_asset_types_t fty_common_asset_types_t;
#define FTY_COMMON_ASSET_TYPES_T_DEFINED
typedef struct _fty_common_str_defs_t fty_common_str_defs_t;
#define FTY_COMMON_STR_DEFS_T_DEFINED
typedef struct _fty_common_filesystem_t fty_common_filesystem_t;
#define FTY_COMMON_FILESYSTEM_T_DEFINED
typedef struct _fty_common_json_t fty_common_json_t;
#define FTY_COMMON_JSON_T_DEFINED
typedef struct _fty_common_utf8_t fty_common_utf8_t;
#define FTY_COMMON_UTF8_T_DEFINED
typedef struct _fty_common_unit_tests_t fty_common_unit_tests_t;
#define FTY_COMMON_UNIT_TESTS_T_DEFINED

#include "fty_common_agents.h"
#include "fty_common_macros.h"
#include "fty_common_nut_types.h"
#include "fty_common_client.h"
#include "fty_common_sync_server.h"
#include "fty_common_base.h"
#include "fty_common_asset_types.h"
#include "fty_common_str_defs.h"
#include "fty_common_filesystem.h"
#include "fty_common_json.h"
#include "fty_common_utf8.h"
