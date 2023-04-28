/*
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
*/

#include "fty_common_filesystem.h"
#include <catch2/catch.hpp>

TEST_CASE("File system")
{
    using namespace shared;

    CHECK(path_separator() != nullptr);

    CHECK(file_mode(nullptr) == 0);
    CHECK(file_mode("") == 0);
    CHECK((file_mode("./test/data/") & S_IFMT) == S_IFDIR);
    CHECK((file_mode("./test/data/example.json") & S_IFMT) == S_IFREG);

    CHECK(is_file(nullptr) == false);
    CHECK(is_file("") == false);
    CHECK(is_file("test/data/") == false);
    CHECK(is_file("test/data/example.json") == true);

    CHECK(is_dir(nullptr) == false);
    CHECK(is_dir("") == false);
    CHECK(is_dir("test/data/") == true);
    CHECK(is_dir("test/data/example.json") == false);

    CHECK(items_in_directory(nullptr).size() == 0);
    CHECK(items_in_directory("").size() == 0);
    CHECK(items_in_directory("test/data/").size() > 2); // at least . & ..
    CHECK(items_in_directory("test/data/example.json").size() == 0);

    std::vector<std::string> v;
    v.clear();
    CHECK(((is_item_in_directory("", v) == false) && (v.size() == 0)));
    v.clear();
    CHECK(((is_item_in_directory("hello", v) == false) && (v.size() == 0)));
    v.clear();
    CHECK(((is_item_in_directory("test/data/", v) == true) && (v.size() != 0)));

    v = files_in_directory(nullptr);
    CHECK(v.size() == 0);
    v = files_in_directory("");
    CHECK(v.size() == 0);
    v = files_in_directory("test/data/");
    CHECK(v.size() != 0);

    v.clear();
    CHECK(((is_file_in_directory("", v) == false) && (v.size() == 0)));
    v.clear();
    CHECK(((is_file_in_directory("hello", v) == false) && (v.size() == 0)));
    v.clear();
    CHECK(((is_file_in_directory("test/data/", v) == true) && (v.size() != 0)));

    system("rm -fr test/data/d0");
    CHECK(mkdir_if_needed(nullptr) == false);
    CHECK(mkdir_if_needed("") == false);
    CHECK(mkdir_if_needed("test/data/d0") == false);
    CHECK(is_dir("test/data/d0") == true);
    CHECK(mkdir_if_needed("test/data/d0") == true); //exist

    CHECK(basename("") == "");
    CHECK(basename("/") == "");
    CHECK(basename("test/") == "");
    CHECK(basename("test/data/") == "");
    CHECK(basename("test/data") == "data");
    CHECK(basename("test/data/hello") == "hello");
    CHECK(basename("test/data/hello.json") == "hello.json");
}
