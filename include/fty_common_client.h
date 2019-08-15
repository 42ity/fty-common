/*  =========================================================================
    fty_common_client - Interfaces for server clients

    Copyright (C) 2019 Eaton

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

#ifndef FTY_COMMON_CLIENT_H_INCLUDED
#define FTY_COMMON_CLIENT_H_INCLUDED

#include <string>
#include <vector>
#include <functional>

namespace fty
{
    using Callback = std::function<void(const std::vector<std::string> &)>;
     
    class SyncClient
    {
    public:
        virtual std::vector<std::string> syncRequestWithReply(const std::vector<std::string> & payload) = 0;
    };
    
    class StreamPublisher
    {
    public:
        virtual void publish(const std::vector<std::string> & payload) = 0;
    };
    
    class StreamSubscriber
    {
    public:
        virtual uint32_t subscribe( Callback callback) = 0;
        virtual void unsubscribe(uint32_t subId) = 0;
    };
    
} //namespace fty

#endif
