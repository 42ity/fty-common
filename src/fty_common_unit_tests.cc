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

#include "fty_common_unit_tests.h"
#include "fty_common.h"
#include <chrono>
#include <condition_variable>
#include <thread>

namespace fty {
Payload EchoServer::handleRequest(const Sender& sender, const Payload& payload)
{
    m_lastSender = sender;
    return payload;
}

const Sender& EchoServer::getLastSender() const
{
    return m_lastSender;
}

/*---------------------------------------------------------------*/

SyncClientTest::SyncClientTest(const Sender& sender, SyncServer& server)
    : m_sender(sender)
    , m_syncServer(server)
{
}

Payload SyncClientTest::syncRequestWithReply(const Payload& payload)
{
    return m_syncServer.handleRequest(m_sender, payload);
}

/*---------------------------------------------------------------*/
StreamClientTest::~StreamClientTest()
{
    if (m_listernerThread.joinable()) {
        m_listernerThread.join();
    }
}

void StreamClientTest::publish(const Payload& payload)
{
    if (m_callback) {
        if (m_listernerThread.joinable()) {
            m_listernerThread.join();
        }

        m_lastPayload = payload;

        m_listernerThread = std::thread(m_callback, m_lastPayload);
    }
}

uint32_t StreamClientTest::subscribe(Callback callback)
{
    if (m_listernerThread.joinable()) {
        m_listernerThread.join();
    }

    m_callback = callback;
    return 0;
}

void StreamClientTest::unsubscribe(uint32_t /*subId*/)
{
    if (m_listernerThread.joinable()) {
        m_listernerThread.join();
    }

    m_callback = nullptr;
}


} // namespace fty
