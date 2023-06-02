/*  =========================================================================
    fty_common_nut_types - class description

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

#include <map>
#include <string>
#include <vector>

/**
 * \brief Basic NUT data types for 42ity.
 *
 * The reason these types are here and not inside fty-common-nut is to break a
 * circular dependency between fty-common-nut, fty-common-db and
 * fty-security-wallet.
 */

namespace fty::nut {

using KeyValues            = std::map<std::string, std::string>;
using DeviceConfiguration  = std::map<std::string, std::string>;
using DeviceConfigurations = std::vector<DeviceConfiguration>;

} // namespace fty::nut

#endif //__cplusplus
