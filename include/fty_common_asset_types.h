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

#pragma once

#include <algorithm>
#include <inttypes.h>
#include <string>

namespace persist {

// CAUTION: THESE ENUMS REFLECT (PARTS OF) THE DATABASE

enum asset_type
{
    TUNKNOWN = 0,

    GROUP = 1,
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
    TSERVER = 15,
    PLANNER,
    PLAN,
    COPS, // Composite Power System
    OPERATING_SYSTEM,
    HOST_GROUP, // Composite Power System

    NB_ASSET_TYPES
};

} // namespace persist

namespace fty {
// WARNING keep consistent with DB table t_bios_asset_element_type
static constexpr const char* TYPE_UNKNOWN          = "unknown";          // 0
static constexpr const char* TYPE_GROUP            = "group";            // 1
static constexpr const char* TYPE_DATACENTER       = "datacenter";       // 2
static constexpr const char* TYPE_ROOM             = "room";             // 3
static constexpr const char* TYPE_ROW              = "row";              // 4
static constexpr const char* TYPE_RACK             = "rack";             // 5
static constexpr const char* TYPE_DEVICE           = "device";           // 6
static constexpr const char* TYPE_INFRA_SERVICE    = "infra-service";    // 7
static constexpr const char* TYPE_CLUSTER          = "cluster";          // 8
static constexpr const char* TYPE_HYPERVISOR       = "hypervisor";       // 9
static constexpr const char* TYPE_VIRTUAL_MACHINE  = "virtual-machine";  // 10
static constexpr const char* TYPE_STORAGE_SERVICE  = "storage-service";  // 11
static constexpr const char* TYPE_VAPP             = "vapp";             // 12
static constexpr const char* TYPE_CONNECTOR        = "connector";        // 13
static constexpr const char* TYPE_SERVER           = "server";           // 15
static constexpr const char* TYPE_PLANNER          = "planner";          // 16
static constexpr const char* TYPE_PLAN             = "plan";             // 17
static constexpr const char* TYPE_COPS             = "cops";             // 18
static constexpr const char* TYPE_OPERATING_SYSTEM = "operating-system"; // 19
static constexpr const char* TYPE_HOST_GROUP       = "host-group";       // 20

} // namespace fty

namespace persist {
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
        VM,
        VIRTUAL = VM,

        N_A             = 11, /* ATTENTION: don't change N_A id. It is used as default value in initdb.sql for types, that don't have N_A */
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
        MICROSOFT_HYPERV_SERVER,
        VMWARE_VM,

        CITRIX_VM = 31,
        NETAPP_NODE,
        VMWARE_STANDALONE_ESXI,
        VMWARE_TASK,
        VMWARE_VAPP,
        CITRIX_XENSERVER,
        CITRIX_VAPP,
        CITRIX_TASK,
        MICROSOFT_VM,
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

        VMWARE_VCENTER_CONNECTOR            = 60,
        VMWARE_STANDALONE_ESXI_CONNECTOR,
        NETAPP_ONTAP,
        VMWARE_SRM                          = 65,
        VMWARE_SRM_PLAN,
        PCU,      // Parallel Control Unit

        DELL_VXRAIL_CONNECTOR,
        DELL_VXRAIL_MANAGER,
        DELL_VXRAIL_CLUSTER,

        MICROSOFT_HYPERV_SERVICE            = 72,
        VMWARE_CLUSTER_FAULT_DOMAIN,
        MICROSOFT_SCVMM_CONNECTOR,
        MICROSOFT_SCVMM,
        DELL_VXRAIL_EXSI,
        MICROSOFT_STANDALONE_HYPERV_SERVER,

        NB_ASSET_SUBTYPES
    };
}

namespace fty {
 // WARNING keep consistent with DB table t_bios_asset_device_type
    static constexpr const char* SUB_UNKNOWN                            = "unknown";
    static constexpr const char* SUB_UPS                                = "ups";
    static constexpr const char* SUB_GENSET                             = "genset";
    static constexpr const char* SUB_EPDU                               = "epdu";
    static constexpr const char* SUB_PDU                                = "pdu";
    static constexpr const char* SUB_SERVER                             = "server";
    static constexpr const char* SUB_FEED                               = "feed";
    static constexpr const char* SUB_STS                                = "sts";
    static constexpr const char* SUB_SWITCH                             = "switch";
    static constexpr const char* SUB_STORAGE                            = "storage";
    static constexpr const char* SUB_VM                                 = "vm";
    static constexpr const char* SUB_N_A                                = "N_A";
    static constexpr const char* SUB_ROUTER                             = "router";
    static constexpr const char* SUB_RACK_CONTROLLER                    = "rackcontroller";
    static constexpr const char* SUB_SENSOR                             = "sensor";
    static constexpr const char* SUB_APPLIANCE                          = "appliance";
    static constexpr const char* SUB_CHASSIS                            = "chassis";
    static constexpr const char* SUB_PATCH_PANEL                        = "patchpanel";
    static constexpr const char* SUB_OTHER                              = "other";
    static constexpr const char* SUB_SENSORGPIO                         = "sensorgpio";
    static constexpr const char* SUB_GPO                                = "gpo";
    static constexpr const char* SUB_NETAPP_ONTAP_NODE                  = "netapp.ontap.node";
    static constexpr const char* SUB_IPMINFRA_SERVER                    = "ipminfra.server";
    static constexpr const char* SUB_IPMINFRA_SERVICE                   = "ipminfra.service";
    static constexpr const char* SUB_VMWARE_VCENTER                     = "vmware.vcenter";
    static constexpr const char* SUB_CITRIX_POOL                        = "citrix.pool";
    static constexpr const char* SUB_VMWARE_CLUSTER                     = "vmware.cluster";
    static constexpr const char* SUB_VMWARE_ESXI                        = "vmware.esxi";
    static constexpr const char* SUB_MICROSOFT_HYPERV_SERVER            = "microsoft.hyperv.server";
    static constexpr const char* SUB_VMWARE_VM                          = "vmware.vm";
    static constexpr const char* SUB_CITRIX_VM                          = "citrix.vm";
    static constexpr const char* SUB_NETAPP_NODE                        = "netapp.node";
    static constexpr const char* SUB_VMWARE_STANDALONE_ESXI             = "vmware.standalone.esxi";
    static constexpr const char* SUB_VMWARE_TASK                        = "vmware.task";
    static constexpr const char* SUB_VMWARE_VAPP                        = "vmware.vapp";
    static constexpr const char* SUB_CITRIX_XENSERVER                   = "citrix.xenserver";
    static constexpr const char* SUB_CITRIX_VAPP                        = "citrix.vapp";
    static constexpr const char* SUB_CITRIX_TASK                        = "citrix.task";
    static constexpr const char* SUB_MICROSOFT_VM                       = "microsoft.vm";
    static constexpr const char* SUB_MICROSOFT_TASK                     = "microsoft.task";
    static constexpr const char* SUB_MICROSOFT_SERVER_CONNECTOR         = "microsoft.server.connector";
    static constexpr const char* SUB_MICROSOFT_SERVER                   = "microsoft.server";
    static constexpr const char* SUB_MICROSOFT_CLUSTER                  = "microsoft.cluster";
    static constexpr const char* SUB_HP_ONEVIEW_CONNECTOR               = "hp.oneview.connector";
    static constexpr const char* SUB_HP_ONEVIEW                         = "hp.oneview";
    static constexpr const char* SUB_HP_IT_SERVER                       = "hp.it.server";
    static constexpr const char* SUB_HP_IT_RACK                         = "hp.it.rack";
    static constexpr const char* SUB_NETAPP_SERVER                      = "netapp.server";
    static constexpr const char* SUB_NETAPP_ONTAP_CONNECTOR             = "netapp.ontap.connector";
    static constexpr const char* SUB_NETAPP_ONTAP_CLUSTER               = "netapp.ontap.cluster";
    static constexpr const char* SUB_NUTANIX_VM                         = "nutanix.vm";
    static constexpr const char* SUB_NUTANIX_PRISM_GATEWAY              = "nutanix.prism.gateway";
    static constexpr const char* SUB_NUTANIX_NODE                       = "nutanix.node";
    static constexpr const char* SUB_NUTANIX_CLUSTER                    = "nutanix.cluster";
    static constexpr const char* SUB_NUTANIX_PRISM_CONNECTOR            = "nutanix.prism.connector";
    static constexpr const char* SUB_VMWARE_VCENTER_CONNECTOR           = "vmware.vcenter.connector";
    static constexpr const char* SUB_VMWARE_STANDALONE_ESXI_CONNECTOR   = "vmware.standalone.esxi.connector";
    static constexpr const char* SUB_NETAPP_ONTAP                       = "netapp.ontap";
    static constexpr const char* SUB_VMWARE_SRM                         = "vmware.srm";
    static constexpr const char* SUB_VMWARE_SRM_PLAN                    = "vmware.srm.plan";
    static constexpr const char* SUB_PCU                                = "pcu";
    static constexpr const char* SUB_DELL_VXRAIL_CONNECTOR              = "dell.vxrail.connector";
    static constexpr const char* SUB_DELL_VXRAIL_MANAGER                = "dell.vxrail.manager";
    static constexpr const char* SUB_DELL_VXRAIL_CLUSTER                = "dell.vxrail.cluster";
    static constexpr const char* SUB_MICROSOFT_HYPERV_SERVICE           = "microsoft.hyperv.service";
    static constexpr const char* SUB_VMWARE_CLUSTER_FAULT_DOMAIN        = "vmware.cluster.fault.domain";
    static constexpr const char* SUB_MICROSOFT_SCVMM_CONNECTOR          = "microsoft.scvmm.connector";
    static constexpr const char* SUB_MICROSOFT_SCVMM                    = "microsoft.scvmm";
    static constexpr const char* SUB_DELL_VXRAIL_EXSI                   = "dell.vxrail.exsi";
    static constexpr const char* SUB_MICROSOFT_STANDALONE_HYPERV_SERVER = "microsoft.standalone.hyperv.server";
} //namespace fty

namespace persist {

enum asset_operation
{
    INSERT = 1,
    DELETE,
    UPDATE,
    GET,
    RETIRE,
    INVENTORY
};

std::string operation2str(asset_operation operation);

asset_operation str2operation(const std::string& operation);

uint16_t type_to_typeid(const std::string& type);

std::string typeid_to_type(uint16_t type_id);

uint16_t subtype_to_subtypeid(const std::string& subtype);

std::string subtypeid_to_subtype(uint16_t subtype_id);

bool is_epdu(int x);

bool is_pdu(int x);

bool is_rack(int x);

bool is_dc(int x);

bool is_ups(int x);

bool is_container(const std::string& asset_type);

bool is_ok_element_type(uint16_t element_type_id);

bool is_ok_name(const char* name);

bool is_ok_keytag(const char* keytag);

bool is_ok_value(const char* value);

bool is_ok_link_type(uint8_t link_type_id);
} // namespace persist
