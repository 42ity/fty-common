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

/*
@header
    fty_common_asset_types - Definitions of asset types and subtypes
@discuss
@end
*/

#include "fty_common_asset_types.h"
#include <string.h>

// t_bios_asset_ext_attributes.keytag
#define MAX_KEYTAG_LENGTH       40
// t_bios_asset_ext_attributes.value
#define MAX_VALUE_LENGTH        255

namespace persist {

uint16_t
type_to_typeid (const std::string &type)
{
    std::string t (type);
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    if(t == "datacenter") {
        return asset_type::DATACENTER;
    } else if(t == "room") {
        return asset_type::ROOM;
    } else if(t == "row") {
        return asset_type::ROW;
    } else if(t == "rack") {
        return asset_type::RACK;
    } else if(t == "group") {
        return asset_type::GROUP;
    } else if(t == "device") {
        return asset_type::DEVICE;
    } else
        return asset_type::TUNKNOWN;
}

std::string
typeid_to_type (uint16_t type_id)
{
    switch(type_id) {
        case asset_type::DATACENTER:
            return "datacenter";
        case asset_type::ROOM:
            return "room";
        case asset_type::ROW:
            return "row";
        case asset_type::RACK:
            return "rack";
        case asset_type::GROUP:
            return "group";
        case asset_type::DEVICE:
            return "device";
        default:
            return "N_A";
    }
}

uint16_t
subtype_to_subtypeid (const std::string &subtype)
{
    std::string st(subtype);
    std::transform(st.begin(), st.end(), st.begin(), ::tolower);
    if(st == "ups") {
        return asset_subtype::UPS;
    }
    else if(st == "genset") {
        return asset_subtype::GENSET;
    }
    else if(st == "epdu") {
        return asset_subtype::EPDU;
    }
    else if(st == "server") {
        return asset_subtype::SERVER;
    }
    else if(st == "pdu") {
        return asset_subtype::PDU;
    }
    else if(st == "feed") {
        return asset_subtype::FEED;
    }
    else if(st == "sts") {
        return asset_subtype::STS;
    }
    else if(st == "switch") {
        return asset_subtype::SWITCH;
    }
    else if(st == "storage") {
        return asset_subtype::STORAGE;
    }
    else if (st == "vm") {
        return asset_subtype::VIRTUAL;
    }
    else if (st == "router") {
        return asset_subtype::ROUTER;
    }
    else if (st == "rack controller") {
        return asset_subtype::RACKCONTROLLER;
    }
    else if (st == "rackcontroller") {
        return asset_subtype::RACKCONTROLLER;
    }
    else if (st == "sensor") {
        return asset_subtype::SENSOR;
    }
    else if (st == "sensorgpio") {
        return asset_subtype::SENSORGPIO;
    }
    else if (st == "gpo") {
        return asset_subtype::GPO;
    }
    else if (st == "appliance") {
        return asset_subtype::APPLIANCE;
    }
    else if (st == "chassis") {
        return asset_subtype::CHASSIS;
    }
    else if (st == "patch panel") {
        return asset_subtype::PATCHPANEL;
    }
    else if (st == "patchpanel") {
        return asset_subtype::PATCHPANEL;
    }
    else if (st == "other") {
        return asset_subtype::OTHER;
    }
    else if(st == "n_a") {
        return asset_subtype::N_A;
    }
    else if(st == "N_A") {
        return asset_subtype::N_A;
    }
    else if(st == "") {
        return asset_subtype::N_A;
    }
    else
        return asset_subtype::SUNKNOWN;
}

std::string
subtypeid_to_subtype (uint16_t subtype_id)
{
    switch(subtype_id) {
        case asset_subtype::UPS:
            return "ups";
        case asset_subtype::GENSET:
            return "genset";
        case asset_subtype::STORAGE:
            return "storage";
        case asset_subtype::STS:
            return "sts";
        case asset_subtype::FEED:
            return "feed";
        case asset_subtype::EPDU:
            return "epdu";
        case asset_subtype::PDU:
            return "pdu";
        case asset_subtype::SERVER:
            return "server";
        case asset_subtype::SWITCH:
            return "switch";
        case asset_subtype::ROUTER:
            return "router";
        case asset_subtype::RACKCONTROLLER:
            return "rackcontroller";
        case asset_subtype::SENSOR:
            return "sensor";
        case asset_subtype::SENSORGPIO:
            return "sensorgpio";
        case asset_subtype::GPO:
            return "gpo";
        case asset_subtype::APPLIANCE:
            return "appliance";
        case asset_subtype::CHASSIS:
            return "chassis";
        case asset_subtype::PATCHPANEL:
            return "patchpanel";
        case asset_subtype::OTHER:
            return "other";
        case asset_subtype::VIRTUAL:
            return "vm";
        case asset_subtype::N_A:
            return "N_A";
        default:
            return "N_A";
    }
}

std::string
operation2str (asset_operation operation)
{
    switch(operation) {
        case asset_operation::INSERT:
            return "create";
        case asset_operation::DELETE:
            return "delete";
        case asset_operation::UPDATE:
            return "update";
        case asset_operation::GET:
            return "get";
        case asset_operation::RETIRE:
            return "retire";
        case asset_operation::INVENTORY:
            return "inventory";
        default:
            return "unknown";
    }
}

asset_operation
str2operation (const std::string &operation)
{
    std::string t (operation);
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    if(t == "create") {
        return asset_operation::INSERT;
    } else if(t == "delete") {
        return asset_operation::DELETE;
    } else if(t == "retire") {
        return asset_operation::RETIRE;
    } else if(t == "inventory") {
        return asset_operation::INVENTORY;
    } else if(t == "update") {
        return asset_operation::UPDATE;
    } else if(t == "get") {
        return asset_operation::GET;
    }
    return asset_operation::INVENTORY;
}

bool
is_epdu(int x) {
    return x == asset_subtype::EPDU;
}

bool
is_pdu(int x) {
    return x == asset_subtype::PDU;
}

bool
is_rack(int x) {
    return x == asset_type::RACK;
}

bool
is_dc(int x) {
    return x == asset_type::DATACENTER;
}

bool
is_ups(int x) {
    return x == asset_subtype::UPS;
}


bool
is_container (std::string asset_type)
{
    if (asset_type == "datacenter" ||
        asset_type == "room"       ||
        asset_type == "row"        ||
        asset_type == "rack" ) {
        return true;
    }

    return false;
}

bool
is_ok_element_type (uint16_t element_type_id)
{
    switch(element_type_id) {
        case persist::asset_type::DATACENTER:
        case persist::asset_type::ROOM:
        case persist::asset_type::ROW:
        case persist::asset_type::RACK:
        case persist::asset_type::GROUP:
        case persist::asset_type::DEVICE:
            return true;
        default:
            return false;
    }
}

bool
is_ok_name (const char* name)
{
    size_t length = strlen (name);
    if ( length == 0)
        return false;

    // Bad characters _ % @
    if (strchr (name, '_') != NULL ||
        strchr (name, '%') != NULL ||
        strchr (name, '@') != NULL)
       return false;

    return true;
}

bool
is_ok_keytag (const char *keytag)
{
    auto length = strlen(keytag);
    if ( ( length > 0 ) && ( length <= MAX_KEYTAG_LENGTH ) )
        return true;
    else
        return false;
}

bool
is_ok_value (const char *value)
{
    auto length = strlen(value);
    if ( ( length > 0 ) && ( length <= MAX_VALUE_LENGTH ) )
        return true;
    else
        return false;
}

bool
is_ok_link_type (uint8_t link_type_id)
{
    // TODO: manage link types
    if ( link_type_id > 0 )
        return true;
    else
        return false;
}

} // namespace end
