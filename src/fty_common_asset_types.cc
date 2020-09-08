/*  =========================================================================
    fty_common_asset_types - Definitions of asset types and subtypes

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

/*
@header
    fty_common_asset_types - Definitions of asset types and subtypes
@discuss
@end
*/

#include "fty_common_asset_types.h"
#include <array>
#include <functional>
#include <map>
#include <string.h>

// t_bios_asset_ext_attributes.keytag
#define MAX_KEYTAG_LENGTH       40
// t_bios_asset_ext_attributes.value
#define MAX_VALUE_LENGTH        255

namespace persist {

/**
 * FIXME: THIS IS REDUNDANT WITH THE DATABASE, REPLACE WITH DATABASE QUERIES!
 * see corresp. asset_type enum in include/fty_common_asset_types.h
 * see mysql box_utf8/ 'select * from t_bios_asset_element_type order by id_asset_element_type;'
 */
const static std::array<std::string, NB_ASSET_TYPES> type_names {
    "unknown",      // = 0

    "group",        // = 1
    "datacenter",
    "room",
    "row",
    "rack",
    "device",
    "infra-service",
    "cluster",
    "hypervisor",

    "virtual-machine",
    "storage-service",
    "vapp",
    "connector",
    "",             // = 14 (not used)
    "server",       // = 15
    "planner",
    "plan",
    "",             // operating-system
    "",             // host-group

    "cops"          // = 20 Composite Power System
};

/**
 * FIXME: THIS IS REDUNDANT WITH THE DATABASE, REPLACE WITH DATABASE QUERIES!
 * see corresp. asset_subtype enum in include/fty_common_asset_types.h
 * see mysql box_utf8/ 'select * from t_bios_asset_device_type order by id_asset_device_type;'
 */
const static std::array<std::string, NB_ASSET_SUBTYPES> subtype_names {
    "unknown",

    "ups",
    "genset",
    "epdu",
    "pdu",
    "server",
    "feed",
    "sts",
    "switch",
    "storage",

    "vm",
    "N_A",
    "router",
    "rackcontroller",
    "sensor",
    "appliance",
    "chassis",
    "patchpanel",
    "other",
    "sensorgpio",

    "gpo",
    "netapp.ontap.node",
    "ipminfra.server",
    "ipminfra.service",
    "vmware.vcenter",
    "citrix.pool",
    "vmware.cluster",
    "vmware.esxi",
    "microsoft.hyperv",
    "vmware.vm",

    "microsoft.vm",
    "citrix.vm",
    "netapp.node",
    "vmware.standalone.esxi",
    "vmware.task",
    "vmware.vapp",
    "citrix.xenserver",
    "citrix.vapp",
    "citrix.task",
    "microsoft.virtualization.machine",

    "microsoft.task",
    "microsoft.server.connector",
    "microsoft.server",
    "microsoft.cluster",
    "hp.oneview.connector",
    "hp.oneview",
    "hp.it.server",
    "hp.it.rack",
    "netapp.server",
    "netapp.ontap.connector",

    "netapp.ontap.cluster",
    "nutanix.vm",
    "nutanix.prism.gateway",
    "nutanix.node",
    "nutanix.cluster",
    "nutanix.prism.connector",
    "",
    "",
    "",
    "",

    "vmware.vcenter.connector",
    "vmware.standalone.esxi.connector",
    "netapp.ontap",
    "",
    "",
    "vmware.srm",
    "vmware.srm.plan",
    "",
    "",
    "",
    "",
    "",

    "pcu" // Parallel Control Unit (marker for UPS Parallel System)
};

const static std::map<std::string, std::string> subtype_equivs {
    // Except "" for N_A, the names on the left are the ones from database
    { "rack controller", "rackcontroller" }, // Mind the space!
    { "patch panel", "patchpanel" }, // Mind the space!
    { "", "N_A" } // Do not change, this string with id==11 (not NULL!) is database default for assets with undefined type
} ;

bool caseInsensitiveCompare(const std::string& a, const std::string& b)
{
    std::string la, lb;
    std::transform(a.begin(), a.end(), std::back_inserter(la), ::tolower);
    std::transform(b.begin(), b.end(), std::back_inserter(lb), ::tolower);
    return la == lb;
}

uint16_t
type_to_typeid (const std::string& type)
{
    auto r = std::find_if(type_names.begin(), type_names.end(), std::bind(caseInsensitiveCompare, type, std::placeholders::_1));
    if (r == type_names.end() || *r == "") {
        return TUNKNOWN;
    }

    return static_cast<asset_type>(std::distance(type_names.begin(), r));
}

std::string
typeid_to_type (uint16_t type_id)
{
    return type_names[type_id < type_names.size() ? type_id : TUNKNOWN];
}

uint16_t
subtype_to_subtypeid (const std::string &subtype)
{
    std::string st(subtype);
    std::transform(st.begin(), st.end(), st.begin(), ::tolower);

    auto itEquiv = subtype_equivs.find(st);
    if (itEquiv != subtype_equivs.end()) {
        st = itEquiv->second;
    }

    auto r = std::find_if(subtype_names.begin(), subtype_names.end(), std::bind(caseInsensitiveCompare, st, std::placeholders::_1));
    if (r == subtype_names.end()) {
        return SUNKNOWN;
    }

    return static_cast<asset_subtype>(std::distance(subtype_names.begin(), r));
}

std::string
subtypeid_to_subtype (uint16_t subtype_id)
{
    std::string st = subtype_names[subtype_id < subtype_names.size() ? subtype_id : SUNKNOWN];

    // For database manipulation, we need database string values, e.g. "rack controller" with the space
    std::string itEquiv = ""; // also "N_A" maps to "" and we DO NOT want this one converted to empty string
    for (auto &i : subtype_equivs) {
        if (i.second == st) {
            itEquiv = i.first;
            break;
        }
    }
    if ( itEquiv != "") {
        st = itEquiv;
    }

    return st;
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
    return typeid_to_type(element_type_id) != "unknown";
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
