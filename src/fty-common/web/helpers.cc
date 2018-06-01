/*
 *
 * Copyright (C) 2015 - 2018 Eaton
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <cassert>
#include <cxxtools/regex.h>
#include <unistd.h> // make "readlink" available on ARM
#include <tntdb.h>

#include "fty-common/web/utils_web.h"
#include "fty-common/web/helpers.h"
#include "fty-common/db/dbpath.h"
#include "fty-common/common/str_defs.h" // EV_LICENSE_DIR, EV_DATA_DIR
//#include "shared/log.h"

/**
 * TODO: This list should not be precompiled once and forever in the
 * REST API component that can not know about future added components
 * (e.g. bundling with third-party connectors), but be derived from
 * data delivered by packages that provide each piece of our system.
 * Also, the wrapper's `systemctl list-ipm-units` is not that expensive
 * to call (e.g. once to initialize this map after webserver startup)
 * and get a list of allowed services for this particular installation,
 * and allows to leave the decision point in one place rather than
 * scatter this list (inevitably inconsistently at some point).
 * Alternately, this can be done during the webserver service unit
 * startup, to generate a file under /tmp with needed current data.
 **/
static
std::map <std::string, std::string> systemctl_service_names = {
    // external (copied from 'fty-core.git/tools/systemctl' wrapper)
    { "mariadb", "" },
    { "mysql", "" },
    { "mysqld", "" },
    { "ntp", "" },
    { "ntpd", "" },
    { "ntpdate", "" },
    { "sntp", "" },
    { "networking", "" },
    { "network", "" },
    { "nut-monitor", "" },
    { "nut-server", "" },
    { "malamute", "" },
    { "saslauthd", "" },
    { "rsyslog", "" },
    { "rsyslogd", "" },
    // internal (copied from 'fty-core.git/tools/systemctl' wrapper)
    // note that at Karol's discretion, some units could be omitted
    { "bios", "" },
    { "tntnet@bios", "" },
    { "fty-outage", "" },
    { "fty-metric-store", ""  },
    { "fty-kpi-power-uptime", "" },
    { "fty-alert-list", "" },
    { "fty-asset", "" },
    { "fty-metric-store-cleaner", "" },
    { "fty-alert-engine", "" },
    { "fty-email", "" },
    { "fty-metric-composite-configurator", "" },
    { "fty-metric-tpower", "" },
    { "bios-agent-inventory", "" },
    { "fty-discovery", "" },
    { "fty-info", "" },
    { "fty-mdns-sd", "" },
    { "fty-metric-snmp", "" },
    { "bios-db-init", "" },
    { "fty-db-init", "" },
    { "fty-db-engine", "" },
    { "fty-db-upgrade", "" },
    { "bios-fake-th", "" },
    { "bios-networking", "" },
    { "bios-reset-button", "" },
    { "bios-ssh-last-resort", "" },
    { "biostimer-compress-logs", "" },
    { "biostimer-verify-fs", "" },
    { "biostimer-loghost-rsyslog-netconsole", "" },
    { "fty-nut", "" },
    { "fty-license-accepted", "" },
    { "biostimer-warranty-metric", "" },
    { "ifplug-dhcp-autoconf", "" },
    { "ipc-meta-setup", "" },
    { "fty-sensor-env", "" },
    { "fty-sensor-gpio", "" },
    { "fty-nut-configurator", "" },
    { "fty-metric-compute", "" },
    { "fty-metric-cache", "" },
    { "fty-alert-flexible", "" },
    // legacy compatibility
    { "bios-agent-smtp", "fty-email" },
    { "bios-agent-rt", "fty-metric-cache" }
};

bool
systemctl_valid_service_name (std::string& service_name)
{
    if (service_name.empty ())
        return true; // for 'list' operation service name is empty

    auto find = systemctl_service_names.find (service_name);
    if (find != systemctl_service_names.end ()) {
        if (!find->second.empty ())
            service_name.assign (find->second);
        return true;
    }
    return false;
}

void
systemctl_get_service_names (std::vector <std::string>& v) 
{
    for (const auto& i : systemctl_service_names) {
        v.push_back (i.first);
    }
}




const char* UserInfo::toString() {
    switch (_profile) {
        case  BiosProfile::Dashboard: return "Dashboard";
        case  BiosProfile::Admin: return "Administrator";
        case  BiosProfile::Anonymous: return "Anonymous";
    }
    /* Currently one use-case is to return a string in profiles.
       This routine could reasonably return NULL as an error here,
       but it should not reach this point anyway (return is here
       mostly for syntax purposes and to quiesce compiler warnings.
     */
    return "N/A";
}


int64_t
iname_to_dbid (const std::string& url, const std::string& asset_name)
{
    try
    {
        int64_t id = 0;

        tntdb::Connection conn = tntdb::connectCached(url);
        // MVY: This makes sure that passing of asset (display) name will work too
        tntdb::Statement st = conn.prepareCached(
        " SELECT DISTINCT (asset.id_asset_element) "
        " FROM "
        "   t_bios_asset_element AS asset "
        " LEFT JOIN t_bios_asset_ext_attributes AS ext "
        " ON "
        "   asset.id_asset_element = ext.id_asset_element "
        " WHERE asset.name = :asset_name "
        " OR (ext.keytag = \"name\" AND ext.value = :asset_name) "
        );

        tntdb::Row row = st.set("asset_name", asset_name).selectRow();
        log_debug("[t_bios_asset_element]: were selected %" PRIu32 " rows", 1);

        row [0].get(id);
        return id;
    }
    catch (const std::exception &e)
    {
        if (!asset_name.empty())
            log_error ("%s not found. exception caught %s", asset_name.c_str(), e.what ());
        else
            log_error ("asset_name empty. exception caught %s", e.what ());
        return -1;
    }
}

bool
check_element_identifier (const char *param_name, const std::string& param_value, uint32_t& element_id, http_errors_t& errors) {
    assert (param_name);
    if (param_value.empty ()) {
        http_add_error ("", errors,"request-param-required", param_name);
        return false;
    }


    int64_t eid = 0;
    const char *prohibited = "_@%;\"";
    for (unsigned int a = 0; a < strlen (prohibited); ++a) {
        if (param_value.find (prohibited[a]) != std::string::npos) {
            http_add_error ("", errors, "request-param-bad", param_name,
                            std::string ("value '").append (param_value).append ("'").append (" contains prohibited characters (").append (prohibited).append(")").c_str (),
                            "valid identificator");
            return false;
        }
    }
    eid =iname_to_dbid (url, param_value);
    if (eid == -1) {
        http_add_error (
            "", errors, "request-param-bad", param_name,
            std::string ("value '").append (param_value).append ("' is not valid identificator").c_str (),
            "existing identificator"
            );
        return false;
    }
    element_id = eid;
    return true;
}

typedef int (t_check_func)(int letter);

bool
check_func_text (const char *param_name, const std::string& param_value, http_errors_t& errors,  size_t minlen, size_t maxlen, t_check_func func) {
    if (param_value.size () < minlen) {
        http_add_error ("", errors, "request-param-bad", param_name,
                        std::string ("value '").append (param_value).append ("'").append (" is too short").c_str (),
                        std::string ("string from ").append (std::to_string (minlen)).append (" to ").append (std::to_string(maxlen)).append (" characters.").c_str ());
        return false;
    }
    if (param_value.size () > maxlen) {
        http_add_error ("", errors, "request-param-bad", param_name,
                        std::string ("value '").append (param_value).append ("'").append (" is too long").c_str (),
                        std::string ("string from ").append (std::to_string (minlen)).append (" to ").append (std::to_string(maxlen)).append (" characters.").c_str ());
        return false;
    }
    for (const auto letter : param_value) {
        if (!func (letter)) {
        http_add_error ("", errors, "request-param-bad", param_name,
                        std::string ("value '").append (param_value).append ("'").append (" contains invalid characters").c_str (),
                        "valid string");
        return false;

        }

    }
    return true;
}

bool
check_regex_text (const char *param_name, const std::string& param_value, const std::string& regex, http_errors_t& errors)
{
    cxxtools::Regex R (regex, REG_EXTENDED | REG_ICASE);
    if (! R.match (param_value)) {
        http_add_error ("", errors, "request-param-bad", param_name,
                        std::string ("value '").append (param_value).append ("'").append (" is not valid").c_str (),
                        std::string ("string matching ").append (regex).append (" regular expression").c_str ());
        return false;
    }
    return true;
}

// 1    contains chars from 'exclude'
// 0    does not
// -1   error (not a utf8 string etc...)
int
utf8_contains_chars (const std::string& input, const std::vector <char>& exclude)
{
    // for now works with (excludes) 1 byte (ascii) chars only (_@% etc...)
    // easily extendable to exclude >1 byte characters (unicode chars) if needed

    unsigned int pos = 0;

    while (pos < input.length ()) {
        const char c = input [pos];
        if ((c & 0x80 ) == 0) {     // lead bit is zero, must be a single ascii
            for (const auto& item : exclude)
                if (c == item)
                    return 1;
            pos = pos + 1;
        }
        else
        if ((c & 0xE0 ) == 0xC0 ) {  // 110x xxxx (2 octets)
            pos = pos + 2;
        }
        else
        if ((c & 0xF0 ) == 0xE0 ) { // 1110 xxxx (3 octets)
            pos = pos + 3;
        }
        else
        if ((c & 0xF8 ) == 0xF0 ) { // 1111 0xxx (4 octets)
            pos = pos + 4;
        }
        else {
            log_error ("Unrecognized utf8 lead byte '%x' in string '%s'", c, input.c_str ());
            return -1;
        }
    }

    return 0;
}

static const std::vector <char> single_byte_excludes = {
    '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09',
    '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0e', '\x0f',
};

bool check_asset_name (const std::string& param_name, const std::string& name, http_errors_t &errors) {
    if (utf8_contains_chars (name, single_byte_excludes) == 1) {
        http_add_error ("", errors, "request-param-bad", param_name.c_str (), name.c_str (), "valid asset name (characters not allowed: \\x00 ... \\x0f");
        return false;
    }
    return true;
}

bool
check_alert_rule_name (const std::string& param_name, const std::string& rule_name, http_errors_t& errors)
{
    // assumption: rule name == rule_name@asset_name, where rule_name is always plain old ASCII
    std::string::size_type index = rule_name.find ("@");
    if (index == std::string::npos) {
        bool old_way = check_regex_text (param_name.c_str (), rule_name, _ALERT_RULE_NAME_RE_STR, errors);
        if (!old_way)
            return false;
        return true;
    }

    std::string rule = rule_name.substr (0, index);
    log_debug ("rule == '%s'", rule.c_str ());
    bool is_rule = check_regex_text (param_name.c_str (), rule, _ALERT_RULE_NAME_RE_STR, errors);
    if (!is_rule)
        return false;

    std::string asset_name = rule_name.substr (index + 1, std::string::npos);
    log_debug ("asset name == '%s'", asset_name.c_str ());
    bool is_asset_name = check_asset_name (param_name, asset_name, errors);
    if (!is_asset_name)
        return false;

    return true;
}

bool
check_alert_just_rule_part (const std::string& param_name, const std::string& rule_name, http_errors_t& errors)
{
    bool is_rule = check_regex_text (param_name.c_str (), rule_name, _ALERT_RULE_NAME_RE_STR, errors);
    if (!is_rule)
        return false;

    return true;
}

static bool
s_isDELETE (const tnt::HttpRequest &request)
{
    return request.getMethod () == "DELETE";
}

static bool
s_isPUT (const tnt::HttpRequest &request)
{
    return request.getMethod () == "PUT";
}

static bool
s_in (const std::string &haystack, char needle)
{
    return haystack.find (needle) != std::string::npos;
}

void
check_user_permissions (
        const UserInfo &user,
        const tnt::HttpRequest &request,
        const std::map <BiosProfile, std::string> &permissions,
        const std::string debug,
        http_errors_t &errors
        )
{
    http_errors_t error;

    if (permissions.count (user.profile ()) != 1) {
        // actually it is not an error :)
        log_info ("Permission not defined for given profile");
        http_add_error (debug, errors, "not-authorized", "");
        return;
    }

    const std::string perm = permissions.at (user.profile ());

    if (  (request.isMethodGET  () && s_in (perm, 'R'))
        ||(request.isMethodPOST () && (s_in (perm, 'C') || s_in (perm, 'E')))
        ||(s_isPUT (request)       && s_in (perm, 'U'))
        ||(s_isDELETE (request)    && s_in (perm, 'D'))
       )
    {
        errors.http_code = HTTP_OK;
        return;
    }

    http_add_error (debug, errors, "not-authorized", "");
    return;
}

char*
get_current_db_initialized_file (void)
{
    char *current_db_initalized_file = NULL;
    char *env = getenv (EV_DB_INITIALIZED_DIR);

    int rv = asprintf (&current_db_initalized_file, "%s/fty-db-ready", env ? env : "/var/run");
    if ( rv == -1 ) {
        if (current_db_initalized_file) { // should asprintf failed due to other reasons than OOM
            free(current_db_initalized_file);
        }
        return NULL;
    }
    return current_db_initalized_file;
}

char*
get_current_license_file (void)
{
    char *current_license = NULL;
    char *env = getenv (EV_LICENSE_DIR);

    int rv = asprintf (&current_license, "%s/current", env ? env : "/usr/share/fty/license");
    if ( rv == -1 ) {
        return NULL;
    }
    return current_license;
}

char*
get_accepted_license_file (void)
{
    char *accepted_license = NULL;
    char *env = getenv (EV_DATA_DIR);

    if (asprintf (&accepted_license, "%s/license", env ? env : "/var/lib/fty" ) == -1) {
        return NULL;
    }
    return accepted_license;
}
char*
get_current_license_version (const char* license_file)
{
    // ASSUMPTION: the symlink to the text of the licence is: /XXX
    // $ ls -l /XXX
    // lrwxrwxrwx. 1 achernikava achernikava 3 Sep 25  2015 /XXX -> 1.0
    //
    // FYI:
    // readlink() places the contents of the symbolic link pathname in the
    // buffer buf, which has size bufsiz.  readlink() does not append a null
    // byte to buf.  It will truncate the contents (to a length of bufsiz
    // characters), in case the buffer is too small to hold all of the
    // contents.
    //
    // ssize_t readlink(const char *pathname, char *buf, size_t bufsiz);

    char *buff = (char *) malloc (sizeof(char)*513);
    memset(buff, 0, sizeof(char)*513);
    int rv = readlink (license_file, buff, sizeof(char)*512);
    //
    // On success, these calls return the number of bytes placed in buf.
    // On error, -1 is returned and errno is set to indicate the error.
    //
    if ( rv == -1 ) {
        log_error ("Cannot read symlink for license");
        free (buff);
        return NULL;
    }
    buff[rv] = '\0';
    return buff;
}
// drop the last / in a developer friendly way
// this is intended to fix issue we've on rhel
// "version" : "/usr/share/fty/license/1.0"
// if there's no / in inp, then it's noop
// if so, then it returns character AFTER last /
const char*
basename2 (const char *inp)
{
    const char *sep = strrchr (inp, '/');
    if (!sep)
        return inp;
    return sep + 1;
}
