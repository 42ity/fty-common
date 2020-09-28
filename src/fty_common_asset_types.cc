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

// CAUTION: THESE MAPS MUST REFLECT (PARTS OF) THE DATABASE
// select * from t_bios_asset_element_type order by id_asset_element_type asc;
// select * from t_bios_asset_device_type order by id_asset_device_type asc;
// Note: except for "N_A", all string tokens below *must* be lowercase

const std::string STR_UNKNOWN("unknown");

const static std::map<uint16_t, std::string> type_names {
    { asset_type::TUNKNOWN,         STR_UNKNOWN }, // **must* be set
    { asset_type::GROUP,            "group" },
    { asset_type::DATACENTER,       "datacenter" },
    { asset_type::ROOM,             "room" },
    { asset_type::ROW,              "row" },
    { asset_type::RACK,             "rack" },
    { asset_type::DEVICE,           "device" },
    { asset_type::INFRA_SERVICE,    "infra-service" },
    { asset_type::CLUSTER,          "cluster" },
    { asset_type::HYPERVISOR,       "hypervisor" },
    { asset_type::VIRTUAL_MACHINE,  "virtual-machine" },
    { asset_type::STORAGE_SERVICE,  "storage-service" },
    { asset_type::VAPP,             "vapp" },
    { asset_type::CONNECTOR,        "connector" },
    { asset_type::TSERVER,          "server" },
    { asset_type::PLANNER,          "planner" },
    { asset_type::PLAN,             "plan" },
    { asset_type::COPS,             "cops" } // Composite Power System
};

const static std::map<uint16_t, std::string> subtype_names {
    { asset_subtype::SUNKNOWN, STR_UNKNOWN }, // **must* be set

    { asset_subtype::UPS,     "ups" },
    { asset_subtype::GENSET,  "genset" },
    { asset_subtype::EPDU,    "epdu" },
    { asset_subtype::PDU,     "pdu" },
    { asset_subtype::SERVER,  "server" },
    { asset_subtype::FEED,    "feed" },
    { asset_subtype::STS,     "sts" },
    { asset_subtype::SWITCH,  "switch" },
    { asset_subtype::STORAGE, "storage" },
    { asset_subtype::VIRTUAL, "vm" },
    { asset_subtype::VM,      "vm" },
    { asset_subtype::N_A,     "N_A" },

    { asset_subtype::ROUTER,         "router" },
    { asset_subtype::RACKCONTROLLER, "rackcontroller" },
    { asset_subtype::SENSOR,         "sensor" },
    { asset_subtype::APPLIANCE,      "appliance" },
    { asset_subtype::CHASSIS,        "chassis" },
    { asset_subtype::PATCHPANEL,     "patchpanel" },
    { asset_subtype::OTHER,          "other" },
    { asset_subtype::SENSORGPIO,     "sensorgpio" },

    { asset_subtype::GPO,               "gpo" },
    { asset_subtype::NETAPP_ONTAP_NODE, "netapp.ontap.node" },
    { asset_subtype::IPMINFRA_SERVER,   "ipminfra.server" },
    { asset_subtype::IPMINFRA_SERVICE,  "ipminfra.service" },
    { asset_subtype::VMWARE_VCENTER,    "vmware.vcenter" },
    { asset_subtype::CITRIX_POOL,       "citrix.pool" },
    { asset_subtype::VMWARE_CLUSTER,    "vmware.cluster" },
    { asset_subtype::VMWARE_ESXI,       "vmware.esxi" },
    { asset_subtype::MICROSOFT_HYPERV,  "microsoft.hyperv" },
    { asset_subtype::VMWARE_VM,         "vmware.vm" },

    { asset_subtype::MICROSOFT_VM,                      "microsoft.vm" },
    { asset_subtype::CITRIX_VM,                         "citrix.vm" },
    { asset_subtype::NETAPP_NODE,                       "netapp.node" },
    { asset_subtype::VMWARE_STANDALONE_ESXI,            "vmware.standalone.esxi" },
    { asset_subtype::VMWARE_TASK,                       "vmware.task" },
    { asset_subtype::VMWARE_VAPP,                       "vmware.vapp" },
    { asset_subtype::CITRIX_XENSERVER,                  "citrix.xenserver" },
    { asset_subtype::CITRIX_VAPP,                       "citrix.vapp" },
    { asset_subtype::CITRIX_TASK,                       "citrix.task" },
    { asset_subtype::MICROSOFT_VIRTUALIZATION_MACHINE,  "microsoft.virtualization.machine" },

    { asset_subtype::MICROSOFT_TASK,                "microsoft.task" },
    { asset_subtype::MICROSOFT_SERVER_CONNECTOR,    "microsoft.server.connector" },
    { asset_subtype::MICROSOFT_SERVER,              "microsoft.server" },
    { asset_subtype::MICROSOFT_CLUSTER,             "microsoft.cluster" },
    { asset_subtype::HP_ONEVIEW_CONNECTOR,          "hp.oneview.connector" },
    { asset_subtype::HP_ONEVIEW,                    "hp.oneview" },
    { asset_subtype::HP_IT_SERVER,                  "hp.it.server" },
    { asset_subtype::HP_IT_RACK,                    "hp.it.rack" },
    { asset_subtype::NETAPP_SERVER,                 "netapp.server" },
    { asset_subtype::NETAPP_ONTAP_CONNECTOR,        "netapp.ontap.connector" },

    { asset_subtype::NETAPP_ONTAP_CLUSTER,      "netapp.ontap.cluster" },
    { asset_subtype::NUTANIX_VM,                "nutanix.vm" },
    { asset_subtype::NUTANIX_PRISM_GATEWAY,     "nutanix.prism.gateway" },
    { asset_subtype::NUTANIX_NODE,              "nutanix.node" },
    { asset_subtype::NUTANIX_CLUSTER,           "nutanix.cluster" },
    { asset_subtype::NUTANIX_PRISM_CONNECTOR,   "nutanix.prism.connector" },

    { asset_subtype::VMWARE_VCENTER_CONNECTOR,          "vmware.vcenter.connector" },
    { asset_subtype::VMWARE_STANDALONE_ESXI_CONNECTOR,  "vmware.standalone.esxi.connector" },
    { asset_subtype::NETAPP_ONTAP,                      "netapp.ontap" },

    { asset_subtype::VMWARE_SRM,        "vmware.srm" },
    { asset_subtype::VMWARE_SRM_PLAN,   "vmware.srm.plan" },

    { asset_subtype::PCU,   "pcu" } // Parallel Control Unit
};

// Except "" for N_A, the names on the left are the ones from database
const static std::map<std::string, std::string> subtype_equivs {
    { "rack controller", "rackcontroller" }, // Mind the space!
    { "patch panel",     "patchpanel" },     // Mind the space!
    { "",                "N_A" }             // Do not change, string for assets with undefined type
};

// see fty_common_base.cc for selftests

std::string
typeid_to_type (uint16_t type_id)
{
    try { return type_names.at(type_id); }
    catch (...) {}
    return STR_UNKNOWN; //type_names.at(asset_type::TUNKNOWN);
}

uint16_t
type_to_typeid (const std::string& type_)
{
    std::string type(type_);
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    for (auto& it : type_names)
        if (it.second == type)
            return it.first;
    return asset_type::TUNKNOWN;
}

std::string
subtypeid_to_subtype (uint16_t subtype_id)
{
    try {
        std::string subtype = subtype_names.at(subtype_id);
        for (auto& it : subtype_equivs)
            if (subtype == it.second)
                { if (!it.first.empty()) return it.first; break; }
        return subtype;
    }
    catch (...) {}
    return STR_UNKNOWN; //subtype_names.at(asset_subtype::SUNKNOWN);
}

uint16_t
subtype_to_subtypeid (const std::string &subtype_)
{
    std::string subtype(subtype_);
    if (subtype != "N_A")
        std::transform(subtype.begin(), subtype.end(), subtype.begin(), ::tolower);

    auto ite = subtype_equivs.find(subtype);
    if (ite != subtype_equivs.end())
        subtype = ite->second;

    for (auto& it : subtype_names)
        if (it.second == subtype)
            return it.first;
    return asset_subtype::SUNKNOWN;
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
    return typeid_to_type(element_type_id) != STR_UNKNOWN;
}

bool
is_ok_name (const char* name)
{
    size_t length = name ? strlen (name) : 0;
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
    size_t length = keytag ? strlen(keytag) : 0;
    if ( ( length > 0 ) && ( length <= MAX_KEYTAG_LENGTH ) )
        return true;
    else
        return false;
}

bool
is_ok_value (const char *value)
{
    size_t length = value ? strlen(value) : 0;
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
