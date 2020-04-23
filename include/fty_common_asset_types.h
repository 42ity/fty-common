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

#ifndef FTY_COMMON_ASSET_TYPES_H_INCLUDED
#define FTY_COMMON_ASSET_TYPES_H_INCLUDED

#include <inttypes.h>
#include <string>
#include <algorithm>

namespace persist {

    enum asset_type {
        TUNKNOWN                        = 0,

        GROUP                           = 1,
        DATACENTER,
        ROOM,
        ROW,
        RACK,
        DEVICE,
        INFRA_SERVICE,
        CLUSTER,
        HYPERVISOR,

        VIRTUAL_MACHINE,
        STORAGE_SERVICE,
        VAPP,
        CONNECTOR,
        TSERVER                          = 15,
        PLANNER,
        PLAN,

        NB_ASSET_TYPES
    };

    enum asset_subtype {
        SUNKNOWN                        = 0,

        UPS                             = 1,
        GENSET,
        EPDU,
        PDU,
        SERVER,
        FEED,
        STS,
        SWITCH,
        STORAGE,
        VIRTUAL,

        VM,
        N_A                             = 11, /* ATTENTION: don't change N_A id. It is used as default value in initdb.sql for types, that don't have N_A */
        ROUTER,
        RACKCONTROLLER,
        SENSOR,
        APPLIANCE,
        CHASSIS,
        PATCHPANEL,
        OTHER,
        SENSORGPIO,

        GPO,
        NETAPP_ONTAP_NODE,
        IPMINFRA_SERVER,
        IPMINFRA_SERVICE,
        VMWARE_VCENTER,
        CITRIX_POOL,
        VMWARE_CLUSTER,
        VMWARE_ESXI,
        MICROSOFT_HYPERV,
        VMWARE_VM,

        MICROSOFT_VM,
        CITRIX_VM,
        NETAPP_NODE,
        VMWARE_STANDALONE_ESXI,
        VMWARE_TASK,
        VMWARE_VAPP,
        CITRIX_XENSERVER,
        CITRIX_VAPP,
        CITRIX_TASK,
        MICROSOFT_VIRTUALIZATION_MACHINE,

        MICROSOFT_TASK,
        MICROSOFT_SERVER_CONNECTOR,
        MICROSOFT_SERVER,
        MICROSOFT_CLUSTER,
        HP_ONEVIEW_CONNECTOR,
        HP_ONEVIEW,
        HP_IT_SERVER,
        HP_IT_RACK,
        NETAPP_SERVER,
        NETAPP_ONTAP_CONNECTOR,

        NETAPP_ONTAP_CLUSTER,
        NUTANIX_VM,
        NUTANIX_PRISM_GATEWAY,
        NUTANIX_NODE,
        NUTANIX_CLUSTER,
        NUTANIX_PRISM_CONNECTOR,

        VMWARE_VCENTER_CONNECTOR = 60,
        VMWARE_STANDALONE_ESXI_CONNECTOR,
        NETAPP_ONTAP,
        VMWARE_SRM = 65,
        VMWARE_SRM_PLAN,

        NB_ASSET_SUBTYPES
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

    bool
    is_ok_element_type (uint16_t element_type_id);

    bool
    is_ok_name (const char* name);

    bool
    is_ok_keytag (const char* keytag);

    bool
    is_ok_value (const char* value);

    bool
    is_ok_link_type (uint8_t link_type_id);
} //namespace persist

#endif
