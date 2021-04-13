/*  =========================================================================
    fty_common_unit_tests - Implementation of interface useful for unit test

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
    fty_common_unit_tests - Implementation of interface useful for unit test
@discuss
@end
*/

#include "fty_common_unit_tests.h"
#include "fty_common.h"
#include <catch2/catch.hpp>
#include <chrono>
#include <condition_variable>
#include <thread>


static fty::Payload g_payload;

static std::mutex              g_lock;
static std::condition_variable g_condvar;

static void callback(const fty::Payload& payload)
{
    std::unique_lock<std::mutex> lock(g_lock);
    g_payload = payload;
    g_condvar.notify_all();
}


TEST_CASE("Test")
{
    printf(" * fty_common_unit_tests: \n");

    //  @selftest Test EchoServer
    {
        printf(" * * * EchoServer: ");
        fty::EchoServer server;

        fty::Payload expectedPayload = {"This", "is", "a", "test"};
        fty::Payload receivedPayload = server.handleRequest("test", expectedPayload);

        CHECK(expectedPayload == receivedPayload);
        CHECK(server.getLastSender() == "test");
        printf("OK\n");
    }
    //  @end

    //  @selftest Test SyncClientTest
    {
        printf(" * * * SyncClientTest: ");
        fty::EchoServer     server;
        fty::SyncClientTest client("test", server);

        fty::Payload expectedPayload = {"This", "is", "a", "test"};
        fty::Payload receivedPayload = client.syncRequestWithReply(expectedPayload);

        CHECK(expectedPayload == receivedPayload);
        CHECK(server.getLastSender() == "test");
        printf("OK\n");
    }
    //  @end

    //  @selftest Test StreamClientTest
    {
        printf(" * * * StreamClientTest: ");
        fty::StreamClientTest client;

        fty::Payload expectedPayload = {"This", "is", "a", "test"};
        g_payload                    = {};

        {
            uint32_t registrationId = client.subscribe(callback);

            std::unique_lock<std::mutex> lock(g_lock);

            client.publish(expectedPayload);

            if (g_condvar.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::timeout) {
                FAIL("Timeout");
            }

            CHECK(expectedPayload == g_payload);

            client.unsubscribe(registrationId);
        }

        {
            g_payload = {};

            std::unique_lock<std::mutex> lock(g_lock);

            client.publish(expectedPayload);

            if (g_condvar.wait_for(lock, std::chrono::seconds(5)) != std::cv_status::timeout) {
                FAIL("Timeout");
            }

            CHECK(g_payload.empty());
        }

        printf("OK\n");
    }
    //  @end
}
