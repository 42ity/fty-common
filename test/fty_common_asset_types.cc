/*  =========================================================================
    fty_common_base - Singleton object and common functions

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

#include "fty_common_asset_types.h"
#include <catch2/catch.hpp>
#include <cstring>

TEST_CASE("Asset types")
{
    printf(" * fty_common_asset_types: \n");

    printf("test: typeid/subtypeid enum. milestones\n");
    {
        CHECK(persist::asset_type::TUNKNOWN == 0);
        CHECK(persist::asset_type::GROUP == 1);
        CHECK(persist::asset_type::TSERVER == 15);

        CHECK(persist::asset_subtype::SUNKNOWN == 0);
        CHECK(persist::asset_subtype::UPS == 1);
        CHECK(persist::asset_subtype::N_A == 11);
        CHECK(persist::asset_subtype::VMWARE_VCENTER_CONNECTOR == 60);
        CHECK(persist::asset_subtype::VMWARE_SRM == 65);
        CHECK(persist::asset_subtype::MICROSOFT_HYPERV_SERVICE == 72);
        CHECK(persist::asset_subtype::ACTUATOR == 78);
        CHECK(persist::asset_subtype::KUBERNETES_CONNECTOR == 79);
        CHECK(persist::asset_subtype::DELL_VXRAIL_VM == 83);
    }

    printf("test: type/typeid unknown\n");
    {
        std::vector<std::string> types({"rackcontroller", "42ity", ""});

        const uint16_t    idUnknown = persist::asset_type::TUNKNOWN;
        const std::string sUnknown  = persist::typeid_to_type(idUnknown);

        bool success = true;
        for (auto type : types) {
            uint16_t    id = persist::type_to_typeid(type);
            std::string s  = persist::typeid_to_type(id);

            bool ok = (id == idUnknown) && (s == sUnknown);
            if (!ok)
                printf("ERROR: type: %s, id: %d, s: %s\n", type.c_str(), id, s.c_str());
            success &= ok;
        }
        CHECK(success);
    }

    printf("test: type/typeid\n");
    {
        std::vector<std::string> types({"unknown", "UnkNOwN", "group", "Group", "datacenter", "DATACENTER", "room",
            "row", "rack", "device", "infra-service", "cluster", "hypervisor", "virtual-machine", "storage-service",
            "vapp", "connector", "server", "planner", "plan", "cops",
            "operating-system", "host-group", "container-cluster", "container-node"});

        bool success = true;
        for (auto type : types) {
            uint16_t    id = persist::type_to_typeid(type);
            std::string s  = persist::typeid_to_type(id);

            std::string attempt(type);
            std::transform(attempt.begin(), attempt.end(), attempt.begin(), ::tolower);

            bool ok = (s == attempt);
            if (!ok)
                printf("ERROR: type: %s, id: %d, s: %s, attempt: %s\n", type.c_str(), id, s.c_str(), attempt.c_str());
            success &= ok;
        }
        CHECK(success);
    }

    printf("test: subtype/subtypeid unknown\n");
    {
        const uint16_t    idUnknown = persist::asset_subtype::SUNKNOWN;
        const std::string sUnknown  = persist::subtypeid_to_subtype(idUnknown);

        std::vector<std::string> types({"hello", "world", sUnknown});

        bool success = true;
        for (auto type : types) {
            uint16_t    id = persist::subtype_to_subtypeid(type);
            std::string s  = persist::subtypeid_to_subtype(id);

            bool ok = (id == idUnknown) && (s == sUnknown);
            if (!ok)
                printf("ERROR: subtype: %s, id: %d, s: %s\n", type.c_str(), id, s.c_str());
            success &= ok;
        }
        CHECK(success);
    }

    printf("test: subtype/subtypeid\n");
    {
        std::vector<std::string> types({
            "", // "N_A"
            "unknown", "UnkNOwN",

            "ups", "UPS", "genset", "Genset", "unknown", "epdu", "pdu", "server", "feed", "sts", "switch", "storage",
            "vm", "N_A", "router", "rack controller", "sensor", "appliance", "chassis", "patch panel", "other",
            "sensorgpio", "gpo", "netapp.ontap.node", "ipminfra.server", "ipminfra.service", "vmware.vcenter",
            "citrix.pool", "vmware.cluster", "vmware.esxi", "microsoft.hyperv.server", "vmware.vm", "citrix.vm",
            "netapp.node", "vmware.standalone.esxi", "vmware.task", "vmware.vapp", "citrix.xenserver", "citrix.vapp",
            "citrix.task", "microsoft.vm", "microsoft.task", "microsoft.server.connector", "microsoft.server",
            "microsoft.cluster", "hp.oneview.connector", "hp.oneview", "hp.it.server", "hp.it.rack", "netapp.server",
            "netapp.ontap.connector", "netapp.ontap.cluster", "nutanix.vm", "nutanix.prism.gateway", "nutanix.node",
            "nutanix.cluster", "nutanix.prism.connector", "vmware.vcenter.connector",
            "vmware.standalone.esxi.connector", "netapp.ontap", "vmware.srm", "vmware.srm.plan", "pcu",
            "dell.vxrail.connector", "dell.vxrail.manager", "dell.vxrail.cluster", "microsoft.hyperv.service",
            "vmware.cluster.fault.domain", "microsoft.scvmm.connector", "microsoft.scvmm", "actuator",
            "kubernetes.connector", "kubernetes.manager", "kubernetes.cluster", "kubernetes.node",
            "dell.vxrail.vm"
        });

        bool success = true;
        for (auto type : types) {
            uint16_t    id = persist::subtype_to_subtypeid(type);
            std::string s  = persist::subtypeid_to_subtype(id);

            std::string attempt(type);
            if (attempt != "N_A")
                std::transform(attempt.begin(), attempt.end(), attempt.begin(), ::tolower);

            // handle exceptions
            if (type == "")
                attempt = fty::SUB_N_A;
            else if (type == "rack controller")
                attempt = fty::SUB_RACK_CONTROLLER;
            else if (type == "patch panel")
                attempt = fty::SUB_PATCH_PANEL;

            bool ok = (attempt == s);
            if (!ok)
                printf(
                    "ERROR: subtype: %s, id: %d, s: %s, attempt: %s\n", type.c_str(), id, s.c_str(), attempt.c_str());
            success &= ok;
        }
        CHECK(success);
    }

    printf("test: is_virtual\n");
    {
        CHECK(!persist::is_virtual("hello world"));
        CHECK(!persist::is_virtual("unknown"));
        CHECK(!persist::is_virtual("feed"));
        CHECK(!persist::is_virtual("device"));
        CHECK(!persist::is_virtual("ups"));
        CHECK(!persist::is_virtual("epdu"));
        CHECK(!persist::is_virtual("sts"));

        const std::vector<const char*>types = {
            fty::TYPE_INFRA_SERVICE,
            fty::TYPE_CLUSTER,
            fty::TYPE_HYPERVISOR,
            fty::TYPE_VIRTUAL_MACHINE,
            fty::TYPE_STORAGE_SERVICE,
            fty::TYPE_VAPP,
            fty::TYPE_CONNECTOR,
            fty::TYPE_SERVER,
            fty::TYPE_PLANNER,
            fty::TYPE_OPERATING_SYSTEM,
            fty::TYPE_PLAN,
            fty::TYPE_CONTAINER_CLUSTER,
            fty::TYPE_CONTAINER_NODE,
        };
        for (auto& type : types) {
            CHECK(persist::is_virtual(type));
        }
    }

    {
        using namespace persist;
        int st;

        st = asset_subtype::EPDU;
        CHECK(is_epdu(st) == true);
        CHECK(is_pdu(st) == false);
        CHECK(is_rack(st) == false);
        CHECK(is_dc(st) == false);
        CHECK(is_ups(st) == false);

        st = asset_subtype::PDU;
        CHECK(is_epdu(st) == false);
        CHECK(is_pdu(st) == true);
        CHECK(is_rack(st) == false);
        CHECK(is_dc(st) == false);
        CHECK(is_ups(st) == false);

        st = asset_type::RACK;
        CHECK(is_epdu(st) == false);
        CHECK(is_pdu(st) == false);
        CHECK(is_rack(st) == true);
        CHECK(is_dc(st) == false);
        CHECK(is_ups(st) == false);

        st = asset_type::DATACENTER;
        CHECK(is_epdu(st) == false);
        CHECK(is_pdu(st) == false);
        CHECK(is_rack(st) == false);
        CHECK(is_dc(st) == true);
        CHECK(is_ups(st) == false);

        st = asset_subtype::UPS;
        CHECK(is_epdu(st) == false);
        CHECK(is_pdu(st) == false);
        CHECK(is_rack(st) == false);
        CHECK(is_dc(st) == false);
        CHECK(is_ups(st) == true);

        st = asset_subtype::STS;
        CHECK(is_epdu(st) == false);
        CHECK(is_pdu(st) == false);
        CHECK(is_rack(st) == false);
        CHECK(is_dc(st) == false);
        CHECK(is_ups(st) == false);

        st = 0;
        CHECK(is_epdu(st) == false);
        CHECK(is_pdu(st) == false);
        CHECK(is_rack(st) == false);
        CHECK(is_dc(st) == false);
        CHECK(is_ups(st) == false);
    }

    {
        using namespace persist;

        CHECK(is_container("") == false);
        CHECK(is_container("ups") == false);
        CHECK(is_container("datacenter") == true);
        CHECK(is_container("room") == true);
        CHECK(is_container("row") == true);
        CHECK(is_container("rack") == true);

        CHECK(is_ok_element_type(0) == false);
        CHECK(is_ok_element_type(1) == true);
        CHECK(is_ok_element_type(2) == true);
        CHECK(is_ok_element_type(9999) == false);

        CHECK(is_ok_name(nullptr) == false);
        CHECK(is_ok_name("") == false);
        CHECK(is_ok_name("a") == true);
        CHECK(is_ok_name("a_") == false);
        CHECK(is_ok_name("a%") == false);
        CHECK(is_ok_name("a@") == false);
        CHECK(is_ok_name("abc") == true);

        CHECK(is_ok_keytag(nullptr) == false);
        CHECK(is_ok_keytag("") == false);
        CHECK(is_ok_keytag("a") == true);
        CHECK(is_ok_keytag("0123456789012345678901234567890123456789") == true); // max len 40
        CHECK(is_ok_keytag("01234567890123456789012345678901234567890") == false);

        CHECK(is_ok_value(nullptr) == false);
        CHECK(is_ok_value("") == false);
        CHECK(is_ok_value("a") == true);
        char p[300]; memset(p, 'a', sizeof(p));
        p[256] = 0;
        CHECK(is_ok_value(p) == false); // max len 255
        p[255] = 0;
        CHECK(is_ok_value(p) == true);

        CHECK(is_ok_link_type(0) == false);
        CHECK(is_ok_link_type(1) == true);
        CHECK(is_ok_link_type(255) == true);
    }

    //  @end
    printf(" * fty_common_asset_types: OK\n");
}
