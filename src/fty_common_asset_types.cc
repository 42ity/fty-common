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

const static std::map<uint16_t, std::string> type_names {
    { asset_type::TUNKNOWN,         ::fty::TYPE_UNKNOWN }, // **must* be set
    { asset_type::GROUP,            ::fty::TYPE_GROUP },
    { asset_type::DATACENTER,       ::fty::TYPE_DATACENTER },
    { asset_type::ROOM,             ::fty::TYPE_ROOM },
    { asset_type::ROW,              ::fty::TYPE_ROW },
    { asset_type::RACK,             ::fty::TYPE_RACK },
    { asset_type::DEVICE,           ::fty::TYPE_DEVICE },
    { asset_type::INFRA_SERVICE,    ::fty::TYPE_INFRA_SERVICE },
    { asset_type::CLUSTER,          ::fty::TYPE_CLUSTER },
    { asset_type::HYPERVISOR,       ::fty::TYPE_HYPERVISOR },
    { asset_type::VIRTUAL_MACHINE,  ::fty::TYPE_VIRTUAL_MACHINE },
    { asset_type::STORAGE_SERVICE,  ::fty::TYPE_STORAGE_SERVICE },
    { asset_type::VAPP,             ::fty::TYPE_VAPP },
    { asset_type::CONNECTOR,        ::fty::TYPE_CONNECTOR },
    { asset_type::TSERVER,          ::fty::TYPE_SERVER },
    { asset_type::PLANNER,          ::fty::TYPE_PLANNER },
    { asset_type::PLAN,             ::fty::TYPE_PLAN },
    { asset_type::COPS,             ::fty::TYPE_COPS }, // Composite Power System
    { asset_type::OPERATING_SYSTEM, ::fty::TYPE_OPERATING_SYSTEM },
    { asset_type::HOST_GROUP,       ::fty::TYPE_HOST_GROUP }
};

const static std::map<uint16_t, std::string> subtype_names {
    { asset_subtype::SUNKNOWN, ::fty::SUB_UNKNOWN }, // **must* be set

    { asset_subtype::UPS,     ::fty::SUB_UPS },
    { asset_subtype::GENSET,  ::fty::SUB_GENSET },
    { asset_subtype::EPDU,    ::fty::SUB_EPDU },
    { asset_subtype::PDU,     ::fty::SUB_PDU },
    { asset_subtype::SERVER,  ::fty::SUB_SERVER },
    { asset_subtype::FEED,    ::fty::SUB_FEED },
    { asset_subtype::STS,     ::fty::SUB_STS },
    { asset_subtype::SWITCH,  ::fty::SUB_SWITCH },
    { asset_subtype::STORAGE, ::fty::SUB_STORAGE },
    { asset_subtype::VIRTUAL, ::fty::SUB_VM },
    { asset_subtype::VM,      ::fty::SUB_VM },
    { asset_subtype::N_A,     ::fty::SUB_N_A },

    { asset_subtype::ROUTER,         ::fty::SUB_ROUTER },
    { asset_subtype::RACKCONTROLLER, ::fty::SUB_RACK_CONTROLLER },
    { asset_subtype::SENSOR,         ::fty::SUB_SENSOR },
    { asset_subtype::APPLIANCE,      ::fty::SUB_APPLIANCE },
    { asset_subtype::CHASSIS,        ::fty::SUB_CHASSIS },
    { asset_subtype::PATCHPANEL,     ::fty::SUB_PATCH_PANEL },
    { asset_subtype::OTHER,          ::fty::SUB_OTHER },
    { asset_subtype::SENSORGPIO,     ::fty::SUB_SENSORGPIO },

    { asset_subtype::GPO,                     ::fty::SUB_GPO },
    { asset_subtype::NETAPP_ONTAP_NODE,       ::fty::SUB_NETAPP_ONTAP_NODE },
    { asset_subtype::IPMINFRA_SERVER,         ::fty::SUB_IPMINFRA_SERVER },
    { asset_subtype::IPMINFRA_SERVICE,        ::fty::SUB_IPMINFRA_SERVICE },
    { asset_subtype::VMWARE_VCENTER,          ::fty::SUB_VMWARE_VCENTER },
    { asset_subtype::CITRIX_POOL,             ::fty::SUB_CITRIX_POOL },
    { asset_subtype::VMWARE_CLUSTER,          ::fty::SUB_VMWARE_CLUSTER },
    { asset_subtype::VMWARE_ESXI,             ::fty::SUB_VMWARE_ESXI },
    { asset_subtype::MICROSOFT_HYPERV_SERVER, ::fty::SUB_MICROSOFT_HYPERV_SERVER },
    { asset_subtype::VMWARE_VM,               ::fty::SUB_VMWARE_VM },

    { asset_subtype::CITRIX_VM,                         ::fty::SUB_CITRIX_VM },
    { asset_subtype::NETAPP_NODE,                       ::fty::SUB_NETAPP_NODE },
    { asset_subtype::VMWARE_STANDALONE_ESXI,            ::fty::SUB_VMWARE_STANDALONE_ESXI },
    { asset_subtype::VMWARE_TASK,                       ::fty::SUB_VMWARE_TASK },
    { asset_subtype::VMWARE_VAPP,                       ::fty::SUB_VMWARE_VAPP },
    { asset_subtype::CITRIX_XENSERVER,                  ::fty::SUB_CITRIX_XENSERVER },
    { asset_subtype::CITRIX_VAPP,                       ::fty::SUB_CITRIX_VAPP },
    { asset_subtype::CITRIX_TASK,                       ::fty::SUB_CITRIX_TASK },

    { asset_subtype::MICROSOFT_VM,                 ::fty::SUB_MICROSOFT_VM },
    { asset_subtype::MICROSOFT_TASK,               ::fty::SUB_MICROSOFT_TASK },
    { asset_subtype::MICROSOFT_SERVER_CONNECTOR,   ::fty::SUB_MICROSOFT_SERVER_CONNECTOR },
    { asset_subtype::MICROSOFT_SERVER,             ::fty::SUB_MICROSOFT_SERVER },
    { asset_subtype::MICROSOFT_CLUSTER,            ::fty::SUB_MICROSOFT_CLUSTER },
    { asset_subtype::HP_ONEVIEW_CONNECTOR,         ::fty::SUB_HP_ONEVIEW_CONNECTOR },
    { asset_subtype::HP_ONEVIEW,                   ::fty::SUB_HP_ONEVIEW },
    { asset_subtype::HP_IT_SERVER,                 ::fty::SUB_HP_IT_SERVER },
    { asset_subtype::HP_IT_RACK,                   ::fty::SUB_HP_IT_RACK },
    { asset_subtype::NETAPP_SERVER,                ::fty::SUB_NETAPP_SERVER },
    { asset_subtype::NETAPP_ONTAP_CONNECTOR,       ::fty::SUB_NETAPP_ONTAP_CONNECTOR },

    { asset_subtype::NETAPP_ONTAP_CLUSTER,     ::fty::SUB_NETAPP_ONTAP_CLUSTER },
    { asset_subtype::NUTANIX_VM,               ::fty::SUB_NUTANIX_VM },
    { asset_subtype::NUTANIX_PRISM_GATEWAY,    ::fty::SUB_NUTANIX_PRISM_GATEWAY },
    { asset_subtype::NUTANIX_NODE,             ::fty::SUB_NUTANIX_NODE },
    { asset_subtype::NUTANIX_CLUSTER,          ::fty::SUB_NUTANIX_CLUSTER },
    { asset_subtype::NUTANIX_PRISM_CONNECTOR,  ::fty::SUB_NUTANIX_PRISM_CONNECTOR },

    { asset_subtype::VMWARE_VCENTER_CONNECTOR,         ::fty::SUB_VMWARE_VCENTER_CONNECTOR },
    { asset_subtype::VMWARE_STANDALONE_ESXI_CONNECTOR, ::fty::SUB_VMWARE_STANDALONE_ESXI_CONNECTOR },
    { asset_subtype::NETAPP_ONTAP,                     ::fty::SUB_NETAPP_ONTAP },

    { asset_subtype::VMWARE_SRM,        ::fty::SUB_VMWARE_SRM },
    { asset_subtype::VMWARE_SRM_PLAN,   ::fty::SUB_VMWARE_SRM_PLAN },

    { asset_subtype::PCU,   ::fty::SUB_PCU }, // Parallel Control Unit
    { asset_subtype::DELL_VXRAIL_CONNECTOR,                 ::fty::SUB_DELL_VXRAIL_CONNECTOR },
    { asset_subtype::DELL_VXRAIL_MANAGER,                   ::fty::SUB_DELL_VXRAIL_MANAGER },
    { asset_subtype::DELL_VXRAIL_CLUSTER,                   ::fty::SUB_DELL_VXRAIL_CLUSTER },
    { asset_subtype::MICROSOFT_HYPERV_SERVICE,              ::fty::SUB_MICROSOFT_HYPERV_SERVICE },
    { asset_subtype::VMWARE_CLUSTER_FAULT_DOMAIN,           ::fty::SUB_VMWARE_CLUSTER_FAULT_DOMAIN },
    { asset_subtype::MICROSOFT_SCVMM_CONNECTOR,             ::fty::SUB_MICROSOFT_SCVMM_CONNECTOR },
    { asset_subtype::MICROSOFT_SCVMM,                       ::fty::SUB_MICROSOFT_SCVMM },
    { asset_subtype::DELL_VXRAIL_EXSI,                      ::fty::SUB_DELL_VXRAIL_EXSI },
    { asset_subtype::MICROSOFT_STANDALONE_HYPERV_SERVER,    ::fty::SUB_MICROSOFT_STANDALONE_HYPERV_SERVER }
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
    return ::fty::TYPE_UNKNOWN; //type_names.at(asset_type::TUNKNOWN);
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
        return subtype;
    }
    catch (...) {}
    return ::fty::SUB_UNKNOWN; //subtype_names.at(asset_subtype::SUNKNOWN);
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
is_container (const std::string & asset_type)
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
    return typeid_to_type(element_type_id) != ::fty::TYPE_UNKNOWN;
}

bool
is_ok_name (const char* name)
{
    size_t length = name ? strlen (name) : 0;
    if (length == 0)
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
