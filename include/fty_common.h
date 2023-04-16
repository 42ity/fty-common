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

#ifdef __cplusplus

// Afwul hack, but I need exceptions to be valid for whole content
#include <stdexcept>
/// general IPM exception
class IPMException : public std::exception
{
};

#endif // __cplusplus

#include "fty_common_agents.h"
#include "fty_common_asset_types.h"
#include "fty_common_base.h"
#include "fty_common_client.h"
#include "fty_common_filesystem.h"
#include "fty_common_json.h"
#include "fty_common_macros.h"
#include "fty_common_nut_types.h"
#include "fty_common_quote_codec.h"
#include "fty_common_str_defs.h"
#include "fty_common_sync_server.h"
#include "fty_common_utf8.h"
