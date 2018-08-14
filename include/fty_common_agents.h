/*  =========================================================================
    fty_common_agents - Common definitions for agents

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

#ifndef FTY_COMMON_AGENTS_H_INCLUDED
#define FTY_COMMON_AGENTS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define AGENT_FTY_ALERT_ENGINE      "fty-alert-engine"
#define AGENT_FTY_ALERT_FLEXIBLE    "fty-alert-flexible"
#define AGENT_FTY_ALERT_LIST        "fty-alert-list"
#define AGENT_FTY_ASSET             "asset-agent"        // FIXME: to be renamed
#define AGENT_FTY_DISCOVERY         "fty-discovery"
#define AGENT_FTY_EMAIL             "fty-email"
#define AGENT_FTY_INFO              "fty-info"
#define AGENT_FTY_KPI_POWER_UPTIME  "uptime"             // FIXME: to be renamed
#define AGENT_FTY_MDNS_SD           "fty-mdns-sd"
#define AGENT_FTY_METRIC_CACHE      "fty-metric-cache"

// FIXME: needs audit
/* fty-metric-composite:
#define AGENT_FTY_METRIC_COMPOSITE  "" */

#define AGENT_FTY_METRIC_COMPUTE    "fty-metric-compute"
#define AGENT_FTY_METRIC_STORE      "fty-metric-store"
#define AGENT_FTY_METRIC_TPOWER     "fty-metric-tpower"
#define AGENT_FTY_NUT               "fty-nut"
#define AGENT_FTY_OUTAGE            "fty-outage"
#define AGENT_FTY_SENSOR_ENV        "fty-sensor-env"
#define AGENT_FTY_SENSOR_GPIO       "fty-sensor-gpio"
#define AGENT_FTY_WARRANTY          "warranty-metric"    // FIXME: to be renamed

#ifdef __cplusplus
}
#endif

#endif
