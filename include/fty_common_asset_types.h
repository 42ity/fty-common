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

//
// asset type (literal)
// WARNING keep consistent with DB table t_bios_asset_element_type
// select * from t_bios_asset_element_type order by id_asset_element_type asc;
//
namespace fty {

static constexpr const char* TYPE_UNKNOWN           = "unknown";           // 0
static constexpr const char* TYPE_GROUP             = "group";             // 1
static constexpr const char* TYPE_DATACENTER        = "datacenter";        // 2
static constexpr const char* TYPE_ROOM              = "room";              // 3
static constexpr const char* TYPE_ROW               = "row";               // 4
static constexpr const char* TYPE_RACK              = "rack";              // 5
static constexpr const char* TYPE_DEVICE            = "device";            // 6
static constexpr const char* TYPE_INFRA_SERVICE     = "infra-service";     // 7
static constexpr const char* TYPE_CLUSTER           = "cluster";           // 8
static constexpr const char* TYPE_HYPERVISOR        = "hypervisor";        // 9
static constexpr const char* TYPE_VIRTUAL_MACHINE   = "virtual-machine";   // 10
static constexpr const char* TYPE_STORAGE_SERVICE   = "storage-service";   // 11
static constexpr const char* TYPE_VAPP              = "vapp";              // 12
static constexpr const char* TYPE_CONNECTOR         = "connector";         // 13
static constexpr const char* TYPE_SERVER            = "server";            // 15
static constexpr const char* TYPE_PLANNER           = "planner";           // 16
static constexpr const char* TYPE_PLAN              = "plan";              // 17
static constexpr const char* TYPE_COPS              = "cops";              // 18
static constexpr const char* TYPE_OPERATING_SYSTEM  = "operating-system";  // 19
static constexpr const char* TYPE_HOST_GROUP        = "host-group";        // 20
static constexpr const char* TYPE_CONTAINER_CLUSTER = "container-cluster"; // 21
static constexpr const char* TYPE_CONTAINER_NODE    = "container-node";    // 22

} // namespace fty

namespace persist {

// CAUTION: THESE ENUMS REFLECT (PARTS OF) THE DATABASE
// keep consistent with DB table t_bios_asset_element_type @id_asset_element_type

enum asset_type // IDs
{
    TUNKNOWN = 0,

    GROUP = 1,
    DATACENTER,         // 2
    ROOM,               // 3
    ROW,                // 4
    RACK,               // 5
    DEVICE,             // 6
    INFRA_SERVICE,      // 7
    CLUSTER,            // 8
    HYPERVISOR,         // 9
    VIRTUAL_MACHINE,    // 10
    STORAGE_SERVICE,    // 11
    VAPP,               // 12
    CONNECTOR,          // 13

    TSERVER = 15,
    PLANNER,            // 16
    PLAN,               // 17
    COPS,               // 18 (Composite Power System)
    OPERATING_SYSTEM,   // 19
    HOST_GROUP,         // 20
    CONTAINER_CLUSTER,  // 21
    CONTAINER_NODE,     // 22

    MAX_ASSET_TYPE_ID
};

} // namespace persist

//
// device type (literal)
// WARNING keep consistent with DB table t_bios_asset_device_type
// select * from t_bios_asset_device_type order by id_asset_device_type asc;
//
namespace fty {

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
    static constexpr const char* SUB_IPMINFRA_SERVER                    = "ipminfra.server";
    static constexpr const char* SUB_IPMINFRA_SERVICE                   = "ipminfra.service";
    static constexpr const char* SUB_VMWARE_VCENTER                     = "vmware.vcenter";
    static constexpr const char* SUB_CITRIX_POOL                        = "citrix.pool";
    static constexpr const char* SUB_VMWARE_CLUSTER                     = "vmware.cluster";
    static constexpr const char* SUB_VMWARE_ESXI                        = "vmware.esxi";
    static constexpr const char* SUB_MICROSOFT_HYPERV_SERVER            = "microsoft.hyperv.server";
    static constexpr const char* SUB_VMWARE_VM                          = "vmware.vm";
    static constexpr const char* SUB_CITRIX_VM                          = "citrix.vm";
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
    static constexpr const char* SUB_NUTANIX_VM                         = "nutanix.vm";
    static constexpr const char* SUB_NUTANIX_PRISM_GATEWAY              = "nutanix.prism.gateway";
    static constexpr const char* SUB_NUTANIX_NODE                       = "nutanix.node";
    static constexpr const char* SUB_NUTANIX_CLUSTER                    = "nutanix.cluster";
    static constexpr const char* SUB_NUTANIX_PRISM_CONNECTOR            = "nutanix.prism.connector";
    static constexpr const char* SUB_VMWARE_VCENTER_CONNECTOR           = "vmware.vcenter.connector";
    static constexpr const char* SUB_VMWARE_STANDALONE_ESXI_CONNECTOR   = "vmware.standalone.esxi.connector";
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
    static constexpr const char* SUB_ACTUATOR                           = "actuator";
    static constexpr const char* SUB_KUBERNETES_CONNECTOR               = "kubernetes.connector";
    static constexpr const char* SUB_KUBERNETES_MANAGER                 = "kubernetes.manager";
    static constexpr const char* SUB_KUBERNETES_CLUSTER                 = "kubernetes.cluster";
    static constexpr const char* SUB_KUBERNETES_NODE                    = "kubernetes.node";
    static constexpr const char* SUB_DELL_VXRAIL_VM                     = "dell.vxrail.vm";
    static constexpr const char* SUB_PROXMOX_VE_CONNECTOR               = "proxmox.ve.connector";
    static constexpr const char* SUB_PROXMOX_VE_CLUSTER                 = "proxmox.ve.cluster";
    static constexpr const char* SUB_PROXMOX_VE_NODE                    = "proxmox.ve.node";
    static constexpr const char* SUB_PROXMOX_VE_VM                      = "proxmox.ve.vm";
    static constexpr const char* SUB_DELL_VXRAIL_FAULT_DOMAIN           = "dell.vxrail.fault.domain";
    static constexpr const char* SUB_MICROSOFT_FAILOVER_CONNECTOR       = "microsoft.failover.connector";
    static constexpr const char* SUB_MICROSOFT_FAILOVER_CLUSTER         = "microsoft.failover.cluster";
    static constexpr const char* SUB_MICROSOFT_FAILOVER_SERVER          = "microsoft.failover.server";
    static constexpr const char* SUB_MICROSOFT_FAILOVER_VM              = "microsoft.failover.vm";
} //namespace fty

namespace persist {
// CAUTION: THESE ENUMS REFLECT (PARTS OF) THE DATABASE
// keep consistent with DB table t_bios_asset_device_type @id_asset_device_type

    enum asset_subtype { // IDs
        SUNKNOWN = 0,

        UPS = 1,
        GENSET,     // 2
        EPDU,       // 3
        PDU,        // 4
        SERVER,     // 5
        FEED,       // 6
        STS,        // 7
        SWITCH,     // 8
        STORAGE,    // 9
        VM,         // 10
        VIRTUAL = VM,

        N_A = 11, // CAUTION: don't change N_A id. It is used as default value in initdb.sql for types, that don't have N_A
        ROUTER,                 // 12
        RACKCONTROLLER,         // 13
        SENSOR,                 // 14
        APPLIANCE,              // 15
        CHASSIS,                // 16
        PATCHPANEL,             // 17
        OTHER,                  // 18
        SENSORGPIO,             // 19
        GPO,                    // 20

        IPMINFRA_SERVER = 22,
        IPMINFRA_SERVICE,       // 23
        VMWARE_VCENTER,         // 24
        CITRIX_POOL,            // 25
        VMWARE_CLUSTER,         // 26
        VMWARE_ESXI,            // 27
        MICROSOFT_HYPERV_SERVER,// 28
        VMWARE_VM,              // 29

        CITRIX_VM = 31,

        VMWARE_STANDALONE_ESXI = 33,
        VMWARE_TASK,            // 34
        VMWARE_VAPP,            // 35
        CITRIX_XENSERVER,       // 36
        CITRIX_VAPP,            // 37
        CITRIX_TASK,            // 38
        MICROSOFT_VM,           // 39
        MICROSOFT_TASK,         // 40
        MICROSOFT_SERVER_CONNECTOR, // 41
        MICROSOFT_SERVER,       // 42
        MICROSOFT_CLUSTER,      // 43

        NUTANIX_VM = 51,
        NUTANIX_PRISM_GATEWAY,  // 52
        NUTANIX_NODE,           // 53
        NUTANIX_CLUSTER,        // 54
        NUTANIX_PRISM_CONNECTOR,// 55

        VMWARE_VCENTER_CONNECTOR  = 60,
        VMWARE_STANDALONE_ESXI_CONNECTOR,   // 61

        VMWARE_SRM = 65,
        VMWARE_SRM_PLAN,        // 66
        PCU,                    // 67 (Parallel Control Unit)
        DELL_VXRAIL_CONNECTOR,  // 68
        DELL_VXRAIL_MANAGER,    // 69
        DELL_VXRAIL_CLUSTER,    // 70

        MICROSOFT_HYPERV_SERVICE = 72,
        VMWARE_CLUSTER_FAULT_DOMAIN,        // 73
        MICROSOFT_SCVMM_CONNECTOR,          // 74
        MICROSOFT_SCVMM,                    // 75
        DELL_VXRAIL_EXSI,                   // 76
        MICROSOFT_STANDALONE_HYPERV_SERVER, // 77
        ACTUATOR,                           // 78
        KUBERNETES_CONNECTOR,               // 79
        KUBERNETES_MANAGER,                 // 80
        KUBERNETES_CLUSTER,                 // 81
        KUBERNETES_NODE,                    // 82
        DELL_VXRAIL_VM,                     // 83

        PROXMOX_VE_CONNECTOR, // 84
        PROXMOX_VE_CLUSTER,   // 85
        PROXMOX_VE_NODE,      // 86
        PROXMOX_VE_VM,        // 87

        DELL_VXRAIL_FAULT_DOMAIN, // 88

        MICROSOFT_FAILOVER_CONNECTOR, // 89
        MICROSOFT_FAILOVER_CLUSTER, // 90
        MICROSOFT_FAILOVER_SERVER, // 91
        MICROSOFT_FAILOVER_VM, // 92

        MAX_ASSET_SUBTYPE_ID
    };
} // namespace persist

namespace persist {

enum asset_operation
{
    CREATE = 1,
    DELETE,
    UPDATE,
    GET,
    RETIRE,
    INVENTORY,
    INSERT [[deprecated]] = CREATE
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

bool is_virtual(const std::string& type);

bool is_container(const std::string& type);

bool is_ok_element_type(uint16_t element_type_id);

bool is_ok_name(const char* name);

bool is_ok_keytag(const char* keytag);

bool is_ok_value(const char* value);

bool is_ok_link_type(uint8_t link_type_id);

} // namespace persist
