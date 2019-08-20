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

/*
@header
    fty_common_unit_tests - Implementation of interface useful for unit test
@discuss
@end
*/

#include "fty_common_classes.h"

namespace fty
{
    EchoServer::EchoServer(){}

    Payload EchoServer::handleRequest(const Sender & /*sender*/, const Payload & payload)
    {
        return payload;
    }
    
/*---------------------------------------------------------------*/
    
    SyncClientTest::SyncClientTest(const Sender & sender, SyncServer & server)
        : m_sender(sender), m_syncServer(server) 
    {}

    Payload SyncClientTest::syncRequestWithReply(const Payload & payload)
    {
        return m_syncServer.handleRequest(m_sender, payload);
    }
    
/*---------------------------------------------------------------*/
    
    void StreamClientTest::publish(const Payload & payload)
    {
        if(m_callback)
        {
            m_callback(payload);
        }
    }

    uint32_t StreamClientTest::subscribe( Callback callback)
    {
        m_callback = callback;
        return 0;
    }

    void StreamClientTest::unsubscribe(uint32_t subId)
    {
        m_callback = nullptr;
    }


} // namespace fty


//  --------------------------------------------------------------------------
//  Self test of this class

// If your selftest reads SCMed fixture data, please keep it in
// src/selftest-ro; if your test creates filesystem objects, please
// do so under src/selftest-rw.
// The following pattern is suggested for C selftest code:
//    char *filename = NULL;
//    filename = zsys_sprintf ("%s/%s", SELFTEST_DIR_RO, "mytemplate.file");
//    assert (filename);
//    ... use the "filename" for I/O ...
//    zstr_free (&filename);
// This way the same "filename" variable can be reused for many subtests.
#define SELFTEST_DIR_RO "src/selftest-ro"
#define SELFTEST_DIR_RW "src/selftest-rw"

fty::Payload g_payload;

static void callback(const fty::Payload & payload)
{
    g_payload = payload;
}

void
fty_common_unit_tests_test (bool verbose)
{
    printf (" * fty_common_unit_tests: \n");

    //  @selftest Test EchoServer
    {
        printf (" * * * EchoServer: ");
        fty::EchoServer server;

        fty::Payload expectedPayload = {"This", "is", "a", "test"};
        fty::Payload receivedPayload = server.handleRequest("test", expectedPayload);

        assert (expectedPayload == receivedPayload);
        printf ("OK\n");
    }
    //  @end
    
    //  @selftest Test SyncClientTest
    {
        printf (" * * * SyncClientTest: ");
        fty::EchoServer server;
        fty::SyncClientTest client("test", server);

        fty::Payload expectedPayload = {"This", "is", "a", "test"};
        fty::Payload receivedPayload = client.syncRequestWithReply(expectedPayload);

        assert (expectedPayload == receivedPayload);
        printf ("OK\n");
    }
    //  @end
    
    //  @selftest Test StreamClientTest
    {
        printf (" * * * StreamClientTest: ");
        fty::StreamClientTest client;
        
        fty::Payload expectedPayload = {"This", "is", "a", "test"};
        g_payload = {};
        
        uint32_t registrationId = client.subscribe(callback);
        client.publish(expectedPayload);
        
        assert (expectedPayload == g_payload);
        
        client.unsubscribe(registrationId);
        
        g_payload = {};
        client.publish(expectedPayload);
                
        assert (g_payload.empty());
        printf ("OK\n");
    }
    //  @end
    
    
}
