/*  =========================================================================
    fty_commons - Singleton object and common functions

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
    fty_commons - Singleton object and common functions
@discuss
@end
*/

#include "fty_common_classes.h"
#include <stdio.h>

void
fty_common_base_test (bool verbose)
{
    printf (" * fty_commons: \n");

    printf("test: typeid/subtypeid enum. milestones\n");
    {
        assert(persist::asset_type::TUNKNOWN == 0);
        assert(persist::asset_type::GROUP == 1);
        assert(persist::asset_type::TSERVER == 15);
        assert(persist::asset_type::COPS == 20);

        assert(persist::asset_subtype::SUNKNOWN == 0);
        assert(persist::asset_subtype::UPS == 1);
        assert(persist::asset_subtype::N_A == 11);
        assert(persist::asset_subtype::VMWARE_VCENTER_CONNECTOR == 60);
        assert(persist::asset_subtype::VMWARE_SRM == 65);
        assert(persist::asset_subtype::PCU == 75);
    }

    printf("test: type/typeid unknown\n");
    {
        std::vector<std::string> types({
            "rackcontroller",
            "42ity",
            ""
        });

        const uint16_t idUnknown = persist::asset_type::TUNKNOWN;
        const std::string sUnknown = persist::typeid_to_type(idUnknown);

        bool success = true;
        for (auto type : types) {
            uint16_t id = persist::type_to_typeid(type);
            std::string s = persist::typeid_to_type(id);

            bool ok = (id == idUnknown) && (s == sUnknown);
            if (!ok) printf("ERROR: type: %s, id: %d, s: %s\n", type.c_str(), id, s.c_str());
            success &= ok;
        }
        assert(success);
    }

    printf("test: type/typeid\n");
    {
        std::vector<std::string> types({
            "unknown",      "UnkNOwN",
            "group",        "Group",
            "datacenter",   "DATACENTER",
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
            "server",
            "planner",
            "plan",
            "cops"
        });

        bool success = true;
        for (auto type : types) {
            uint16_t id = persist::type_to_typeid(type);
            std::string s = persist::typeid_to_type(id);

            std::string attempt(type);
            std::transform(attempt.begin(), attempt.end(), attempt.begin(), ::tolower);

            bool ok = (s == attempt);
            if (!ok) printf("ERROR: type: %s, id: %d, s: %s, attempt: %s\n", type.c_str(), id, s.c_str(), attempt.c_str());
            success &= ok;
        }
        assert(success);
    }

    printf("test: subtype/subtypeid unknown\n");
    {
        std::vector<std::string> types({
            "42ity"
        });

        const uint16_t idUnknown = persist::asset_subtype::SUNKNOWN;
        const std::string sUnknown = persist::subtypeid_to_subtype(idUnknown);

        bool success = true;
        for (auto type : types) {
            uint16_t id = persist::subtype_to_subtypeid(type);
            std::string s = persist::subtypeid_to_subtype(id);

            bool ok = (id == idUnknown) && (s == sUnknown);
            if (!ok) printf("ERROR: subtype: %s, id: %d, s: %s\n", type.c_str(), id, s.c_str());
            success &= ok;
        }
        assert(success);
    }

    printf("test: subtype/subtypeid\n");
    {
        std::vector<std::string> types({
            "",  // "N_A"
            "unknown", "UnkNOwN",

            "ups",      "UPS",
            "genset",   "Genset",
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
            "rackcontroller", // "rack controller"
            "rack controller",
            "sensor",
            "appliance",
            "chassis",
            "patchpanel", // "patch panel"
            "patch panel",
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

            "vmware.vcenter.connector",
            "vmware.standalone.esxi.connector",
            "netapp.ontap",

            "vmware.srm",
            "vmware.srm.plan",

            "pcu"
        });

        bool success = true;
        for (auto type : types) {
            uint16_t id = persist::subtype_to_subtypeid(type);
            std::string s = persist::subtypeid_to_subtype(id);

            std::string attempt(type);
            if (attempt != "N_A")
                std::transform(attempt.begin(), attempt.end(), attempt.begin(), ::tolower);

            // handle exceptions
            if (type == "") attempt = "N_A";
            else if (type == "rackcontroller") attempt = "rack controller";
            else if (type == "patchpanel") attempt = "patch panel";

            bool ok = (attempt == s);
            if (!ok) printf("ERROR: subtype: %s, id: %d, s: %s, attempt: %s\n", type.c_str(), id, s.c_str(), attempt.c_str());
            success &= ok;
        }
        assert(success);
    }

    //  @end
    printf (" * fty_commons: OK\n");
}
