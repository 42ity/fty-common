/*  =========================================================================
    fty_common_client - Interfaces for server clients

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

#ifndef FTY_COMMON_CLIENT_H_INCLUDED
#define FTY_COMMON_CLIENT_H_INCLUDED

#include <string>
#include <vector>
#include <functional>

namespace fty
{
    using Callback = std::function<void(const std::vector<std::string> &)>;
    using Payload = std::vector<std::string>;

    /**
    * \brief Interface for basic synchronous client
    * The client will send a request with a payload(vector or string) to the server
    * The server will receive the payload and identify the Sender
    * The server will reply with a payload.
    *
    * \see Interface for basic synchronous server
    */
    class SyncClient
    {
    public:
        virtual ~SyncClient() = default;
        virtual Payload syncRequestWithReply(const Payload & payload) = 0;
    };

    /**
    * \brief Interface for stream producer
    * The interface user has to implement a function which publish payload to a stream
    *
    */
    class StreamPublisher
    {
    public:
        virtual ~StreamPublisher() = default;
        virtual void publish(const Payload & payload) = 0;
    };

    /**
    * \brief Interface for stream listeners
    * The interface user has to implement a function to register a callback.
    * This callback is going to be call each time we receive a message on the stream
    *
    * The user has also to implement a function to de-register a Callback from the stream
    */
    class StreamSubscriber
    {
    public:
        virtual ~StreamSubscriber() = default;
        virtual uint32_t subscribe( Callback callback) = 0;
        virtual void unsubscribe(uint32_t subId) = 0;
    };

} //namespace fty

#endif
