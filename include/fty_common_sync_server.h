/*  =========================================================================
    fty_common_sync_server - Interface for basic synchromous server

    Copyright (C) 2019 - 2020 Eaton

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

#ifndef FTY_COMMON_SYNC_SERVER_H_INCLUDED
#define FTY_COMMON_SYNC_SERVER_H_INCLUDED

#include <string>
#include <vector>

namespace fty
{
    using Sender = std::string;
    using Payload = std::vector<std::string>;

    /**
    * \brief Interface for basic synchronous server
    * The server receive a request from a Sender and with a payload(vector or string)
    * The server will reply with a payload.
    *
    * If return payload is empty, no answer will be returned to the client.
    *
    * \see Interface for basic synchronous client
    */

    class SyncServer
    {
    public:
        virtual ~SyncServer() = default;
        virtual Payload handleRequest(const Sender & sender, const Payload & payload) = 0;
    };

} //namespace fty

#endif
