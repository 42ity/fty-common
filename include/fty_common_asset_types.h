/*  =========================================================================
    fty_common_asset_types - Definitions of asset types and subtypes

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

#ifndef FTY_COMMON_ASSET_TYPES_H_INCLUDED
#define FTY_COMMON_ASSET_TYPES_H_INCLUDED

#include <inttypes.h>
#include <string>
#include <algorithm>

namespace persist {

    enum asset_type {
        TUNKNOWN     = 0,
        GROUP       = 1,
        DATACENTER  = 2,
        ROOM        = 3,
        ROW         = 4,
        RACK        = 5,
        DEVICE      = 6
    };

    enum asset_subtype {
        SUNKNOWN = 0,
        UPS = 1,
        GENSET,
        EPDU,
        PDU,
        SERVER,
        FEED,
        STS,
        SWITCH,
        STORAGE,
        VIRTUAL,
        N_A = 11,
        /* ATTENTION: don't change N_A id. It is used as default value in initdb.sql for types, that don't have N_A */
        ROUTER,
        RACKCONTROLLER,
        SENSOR,
        APPLIANCE,
        CHASSIS,
        PATCHPANEL,
        OTHER,
        SENSORGPIO,
        GPO
    };

    enum asset_operation
    {
        INSERT = 1,
        DELETE,
        UPDATE,
        GET,
        RETIRE,
        INVENTORY
    };

    std::string
    operation2str (asset_operation operation);

    asset_operation
    str2operation (const std::string &operation);

    uint16_t
    type_to_typeid (const std::string &type);

    std::string
    typeid_to_type (uint16_t type_id);

    uint16_t
    subtype_to_subtypeid (const std::string &subtype);

    std::string
    subtypeid_to_subtype (uint16_t subtype_id);

    bool
    is_epdu(int x);

    bool
    is_pdu(int x);

    bool
    is_rack(int x);

    bool
    is_dc(int x);

    bool
    is_ups(int x);

    bool
    is_container (std::string asset_type);
} //namespace persist

#endif
