/*  =========================================================================
    fty_common_base64 - Functions for base64 codec

    Copyright (C) 2014 - 2024 Eaton

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

#pragma once

#include <string>

namespace Base64 {

/**
 * \brief Base64 encode message.
 * \param[in]   message (plaintext)
 * \param[in]   len message length (bytes)
 * \return      the encoded message
 */
std::string encode(const std::string& message);
std::string encode(const char* message, size_t len);

/**
 * \brief Base64 decode message.
 * \param[in]   message (encoded)
 * \return      the decoded message
 */
std::string decode(const std::string& message);

}
