/*
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
*/

#include <dirent.h>
#include <string.h>

#include "fty_common_filesystem.h"


namespace shared {

const char *path_separator() {
    static const char * sep = "/";
    return sep;
}

mode_t file_mode( const char *path ) {
    struct stat st;

    if( stat( path, &st ) == -1 ) return 0;
    return st.st_mode;
}

bool is_file( const char  *path ) {
    return S_ISREG(file_mode( path ));
}

bool is_dir( const char  *path ) {
    return S_ISDIR( file_mode( path ) );
}

std::vector<std::string> items_in_directory( const char *path ) {
    std::vector<std::string> result;

    DIR * dir = opendir( path );
    if(dir) {
        struct dirent* entry;
        while( ( entry = readdir(dir) ) != NULL ) {
            result.push_back(entry->d_name);
        }
        closedir(dir);
    }
    return result;
}

bool
is_item_in_directory (
        const std::string& path,
        std::vector <std::string>& items
        ) {

    DIR *dir = opendir (path.c_str ());
    if (!dir) {
        return false;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp (entry->d_name, ".") == 0 ||
            strcmp (entry->d_name, "..") == 0) {
            continue;
        }
        items.push_back (entry->d_name);
    }
    closedir (dir);
    return true;
}

std::vector<std::string> files_in_directory( const char *path ) {
    std::vector<std::string> result;
    std::string spath = path; spath += path_separator();

    for( auto it : items_in_directory( path ) ) {
        if( is_file( (spath + it).c_str() ) ) result.push_back(it);
    }
    return result;
}

bool
is_file_in_directory (
        const std::string& path,
        std::vector <std::string>& files
        ) {
    std::string spath = path;
    spath += path_separator();

    std::vector <std::string> items;
    if (is_item_in_directory (spath, items) == false) {
        return false;
    }

    for (const auto& item : items) {
        std::string path = spath + item;
        if (is_file (path.c_str()))
            files.push_back (item);
    }
    return true;
}

bool mkdir_if_needed(const char *path, mode_t mode, bool create_parent ) {
    if( ! path || strlen(path) == 0 ) return false;
    if( is_dir( path ) ) return true;

    if( create_parent ) {
        std::string parent = path;
        size_t i = parent.find_last_of( path_separator() );
        if( i != std::string::npos ) {
            parent = parent.substr(0,i);
            mkdir_if_needed( parent.c_str(), mode, create_parent );
        }
    }
    mkdir(path,mode);
    return false;
}

// basename from libgen.h does not play nice with const char*
std::string basename(const std::string& path) {
    auto pos = path.rfind(path_separator());
    if (pos == std::string::npos)
        return std::string{path};

    return path.substr(pos+1);
}


} // namespace shared
