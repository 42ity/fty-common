/*  =========================================================================
    fty_common_unit_tests - Implementation of interface useful for unit test

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

#ifndef FTY_COMMON_UNIT_TESTS_H_INCLUDED
#define FTY_COMMON_UNIT_TESTS_H_INCLUDED

#include "fty_common_sync_server.h"
#include "fty_common_client.h"

namespace fty
{
    /**
    * \brief Helper class to test synchronous client and server
    * The server is replaying with the same payload as received
    * 
    */
    class EchoServer final : public SyncServer
    {
    public:
        explicit EchoServer();
        Payload handleRequest(const Sender & sender, const Payload & payload) override;
    };
    
    /**
    * \brief Helper class to test synchronous client with a server
    * the client will use the handle request of the server to execute the request
    * 
    */
    class SyncClientTest final : public SyncClient
    {
    public:
        explicit SyncClientTest(const Sender & sender, SyncServer & server);
        Payload syncRequestWithReply(const Payload & payload) override;
        
    private:
        Sender m_sender;
        SyncServer & m_syncServer;
    };
    
    /**
    * \brief Helper class to test stream client with only one subscriber and one publisher
    * When publish, the call back will be called if exist.
    * 
    */
    class StreamClientTest final : public StreamPublisher, public StreamSubscriber
    {
    public:
        virtual void publish(const Payload & payload) override;
        virtual uint32_t subscribe( Callback callback) override;
        virtual void unsubscribe(uint32_t subId) override;
        
    private:
        Callback m_callback;
    };
} // namespace fty

//  Self test of this class
void
    fty_common_unit_tests_test (bool verbose);


#endif
