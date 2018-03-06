/*
 *
 * Copyright (C) 2015-2017 Eaton
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

#include "fty-common/db/dbpath.h"
#include "fty-common/log/fty_log.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "fty-common/common/filesystem.h"

static std::string
s_get_dbpath() {
    std::string s_url =
            std::string("mysql:db=box_utf8;user=") +
                  ((getenv("DB_USER")   == NULL) ? "root" : getenv("DB_USER")) +
                  ((getenv("DB_PASSWD") == NULL) ? ""     :
                      std::string(";password=") + getenv("DB_PASSWD"));
    log_debug("s_get_dbpath() : generated DB_URL=%s", s_url.c_str());
    return s_url;
}

std::string url = s_get_dbpath();

void dbpath () {
    log_info("Updating db url with DB_USER=%s ..",(getenv("DB_USER")   == NULL) ? "root" : getenv("DB_USER"));
    url = s_get_dbpath();
}

// drop double quotes from a string
// needed for reading of db passwd file
// DB_USER="user" -> DB_USER=user
// modify buffer in place
static void
s_dropdq (char* buffer) {
    char* dq_ptr = NULL;
    char* buf = buffer;
    while ((dq_ptr = strchr (buf, '"')) != NULL) {

        buf = dq_ptr;
        while (*dq_ptr) {
            *dq_ptr = *(dq_ptr+1);
            dq_ptr++;
        }
    }
}

// read /etc/default/bios-db-rw and update url global variable
// @return 0 if OK, -1 if KO
bool dbreadcredentials(){
    if(!shared::is_file (PASSWD_FILE))return false;
    // and setup db username/password
    log_debug("dbreadcredentials : Reading %s ..",PASSWD_FILE);
    std::ifstream dbpasswd {PASSWD_FILE};
    static char db_user[256];
    memset (db_user, '\0', 256);
    dbpasswd.getline (db_user, 256);
    s_dropdq (db_user);
    static char db_passwd[256];
    memset (db_passwd, '\0', 256);
    dbpasswd.getline (db_passwd, 256);
    s_dropdq (db_passwd);
    dbpasswd.close ();
    if(db_user==NULL || db_passwd==NULL)return false;
    log_debug("dbreadcredentials : setting envvars...");
    putenv (db_user);
    putenv (db_passwd);
    dbpath();
    return true;
}
