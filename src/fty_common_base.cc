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
    printf ("fty_common_base_test...\n");

    // type/typeid

    assert(persist::type_to_typeid("unknown") == persist::asset_type::TUNKNOWN);
    assert(persist::typeid_to_type(persist::asset_type::TUNKNOWN) == "unknown");
    assert(persist::asset_type::TUNKNOWN == 0);

    assert(persist::type_to_typeid("group") == persist::asset_type::GROUP);
    assert(persist::typeid_to_type(persist::asset_type::GROUP) == "group");
    assert(persist::asset_type::GROUP == 1);

    assert(persist::type_to_typeid("server") == persist::asset_type::TSERVER);
    assert(persist::typeid_to_type(persist::asset_type::TSERVER) == "server");
    assert(persist::asset_type::TSERVER == 15);

    assert(persist::type_to_typeid("cops") == persist::asset_type::COPS);
    assert(persist::typeid_to_type(persist::asset_type::COPS) == "cops");

    // subtype/subtypeid

    assert(persist::subtype_to_subtypeid("unknown") == persist::asset_subtype::SUNKNOWN);
    assert(persist::subtypeid_to_subtype(persist::asset_subtype::SUNKNOWN) == "unknown");
    assert(persist::asset_subtype::SUNKNOWN == 0);

    assert(persist::subtype_to_subtypeid("ups") == persist::asset_subtype::UPS);
    assert(persist::subtypeid_to_subtype(persist::asset_subtype::UPS) == "ups");
    assert(persist::asset_subtype::UPS == 1);

    assert(persist::subtype_to_subtypeid("N_A") == persist::asset_subtype::N_A);
    assert(persist::subtypeid_to_subtype(persist::asset_subtype::N_A) == "N_A");
    assert(persist::asset_subtype::N_A == 11);

    assert(persist::subtype_to_subtypeid("ups.multiplexer") == persist::asset_subtype::UPS_MULTIPLEXER);
    assert(persist::subtypeid_to_subtype(persist::asset_subtype::UPS_MULTIPLEXER) == "ups.multiplexer");
    assert(persist::asset_subtype::UPS_MULTIPLEXER == 70);

    //  @end
    printf ("fty_common_base_test: OK\n");
}
