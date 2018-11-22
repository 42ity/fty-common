/*  =========================================================================
    fty_commons - Singleton object and common functions

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

/*
@header
    fty_commons - Singleton object and common functions
@discuss
@end
*/

#include "fty_common_classes.h"
#include <stdio.h>


int fty_get_polling_interval()
{
    zconfig_t *config = zconfig_load("/etc/fty-nut/fty-nut.cfg");
    int val = strtol(zconfig_get(config, "nut/polling_interval", "30"), NULL, 10);
    zconfig_destroy(&config);
    return val;
}

void
fty_common_base_test (bool verbose)
{
    printf (" * fty_commons: ");

    
    //  @end
    printf ("OK\n");
}
