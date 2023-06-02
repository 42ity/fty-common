/*  =========================================================================
    fty_common_utf8 - Functions for handling UTF-8 encoded strings

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
    fty_common_utf8 - Functions for handling UTF-8 encoded strings
@discuss
@end
*/

#include "fty_common_utf8.h"
#include "fty_common_json.h"
#include <cassert>
#include <fty_log.h>

namespace UTF8 {

[[maybe_unused]] static int codepoint_size(const char* uchar)
{
    // ASCII
    if (((static_cast<unsigned char>(uchar[0])) & 0x80) == 0)
        return 0;
    unsigned char start = (static_cast<unsigned char>(uchar[0])) & 0xF0;
    if ((start & 0xF0) == 0xF0)
        return 7;
    if (((start & 0xE0) == 0xE0) || ((start & 0xC0) == 0xC0))
        return 6;
    return -1;
}

[[maybe_unused]] static size_t utf8_len(const char* s)
{
    size_t len = 0;
    for (; *s; ++s)
        if ((*s & 0xC0) != 0x80)
            ++len;
    return len;
}

// returns a pointer to the beginning of the pos'th utf8 codepoint

[[maybe_unused]] static const char* utf8_index(const char* s, size_t pos)
{
    ++pos;
    for (; *s; ++s) {
        if ((*s & 0xC0) != 0x80)
            --pos;
        if (pos == 0)
            return s;
    }
    return nullptr;
}

int utf8_to_codepoint(const char* uchar, char** codepoint)
{
    // codepoint is NOT null-terminated because it makes comparison too cumbersome
    assert(codepoint);

    const unsigned char* uuchar        = reinterpret_cast<const unsigned char*>(uchar);
    static const char    hex[]         = "0123456789abcdef";
    unsigned char        start         = uuchar[0] & 0xF0;
    unsigned char        ubytes[4]     = {0, 0, 0, 0};
    unsigned int         codepoint_int = 0;
    int                  mask          = 0xf;

    // 4-byte character - 7 bytes for codepoint
    if ((start & 0xF0) == 0xF0) {
        ubytes[0] = uuchar[0] & 0x7;
        for (int i = 1; i < 4; i++)
            ubytes[i] = uuchar[i] & 0x3f;

        codepoint_int = uint(ubytes[0] << 18) + uint(ubytes[1] << 12) + uint(ubytes[2] << 6) + ubytes[3];
        if (codepoint_int <= 0x10fff) {
            for (int i = 6; i > 1; i--) {
                (*codepoint)[i] = hex[codepoint_int & uint(mask)];
                codepoint_int >>= 4;
            }
            (*codepoint)[1] = 'u';
            (*codepoint)[0] = '\\';
            return 4;
        } // everything else is unassigned character
        else
            return -1;
    }
    // 3-byte character
    if ((start & 0xE0) == 0xE0) {
        ubytes[0] = uuchar[0] & 0xf;
        for (int i = 1; i < 3; i++)
            ubytes[i] = uuchar[i] & 0x3f;
        codepoint_int = uint(ubytes[0] << 12) + uint(ubytes[1] << 6) + ubytes[2];
        for (int i = 5; i > 1; i--) {
            (*codepoint)[i] = hex[codepoint_int & uint(mask)];
            codepoint_int >>= 4;
        }
        (*codepoint)[1] = 'u';
        (*codepoint)[0] = '\\';
        return 3;
    }
    // 2-byte character
    if ((start & 0xC0) == 0xC0) {
        ubytes[0]     = uuchar[0] & 0x1f;
        ubytes[1]     = uuchar[1] & 0x3f;
        codepoint_int = uint(ubytes[0] << 6) + ubytes[1];
        for (int i = 5; i > 2; i--) {
            (*codepoint)[i] = hex[codepoint_int & uint(mask)];
            codepoint_int >>= 4;
        }
        (*codepoint)[2] = '0';
        (*codepoint)[1] = 'u';
        (*codepoint)[0] = '\\';
        return 2;
    }
    // ASCII; don't do anything
    if ((uuchar[0] & 0x80) == 0)
        return 0;
    // in any other case, this is not a unicode character
    return -1;
}

// How many 8-bit bytes of the input comprise the next UTF-8 logical character?
// 1, ..., 4 - # of utf8 octets
// -1 - error

int8_t utf8_octets(const char* c)
{
    assert(c);
    const uint8_t b = uint8_t(*c); // UTF is defined in terms of 8-bit octets, do not equate those to potentially
                                   // varied-width platform defined char's
    if ((b & 0x80) == 0)           // lead bit is zero, must be a single ascii
        return 1;
    else if ((b & 0xE0) == 0xC0) // 110x xxxx (2 octets)
        return 2;
    else if ((b & 0xF0) == 0xE0) // 1110 xxxx (3 octets)
        return 3;
    else if ((b & 0xF8) == 0xF0) // 1111 0xxx (4 octets)
        return 4;
    else
        log_error("Unrecognized utf8 lead byte '%" PRIx8 "' in string '%s'", b, c);
    return -1;
}

// ignores case on 1 octet bytes
// 0 - same
// 1 - different

static int utf8_compare_octets(const char* s1, const char* s2, size_t pos, [[maybe_unused]] size_t length, int8_t count)
{
    assert(s1);
    assert(s2);

    // FIXME: Should the process really die with invalid UTF strings?
    assert(count >= 1 && count <= 4);
    assert(pos + size_t(count) <= length);

    // FIXME: When assert is a no-op (production) should this proceed with errors then ignored above?
    // e.g. a count==-1 would yield "return 0" below, and so perceived-equal utf8 chars...

    for (size_t i = 0; i < size_t(count); i++) {
        const char c1 = s1[pos + i];
        const char c2 = s2[pos + i];

        if ((count == 1 && tolower(c1) != tolower(c2)) || (count > 1 && c1 != c2))
            return 1;
    }
    return 0;
}

// compare utf8 strings for equality
// ignore case on ascii (i.e on 1 byte chars)
// 0 - different
// 1 - same
// -1 - error

int utf8eq(const char* s1, const char* s2)
{
    // FIXME: Should we really crash if one string pointer
    //        is NULL, or return an equal/not-equal verdict?
    // Also, are two NULLs considered equal strings?
    assert(s1);
    assert(s2);

    size_t length = strlen(s1);
    if (length != strlen(s2))
        return 0;

    size_t pos = 0;
    while (pos < length) {
        int8_t s1_octets = utf8_octets(s1 + pos);
        int8_t s2_octets = utf8_octets(s2 + pos);

        if (s1_octets == -1 || s2_octets == -1) {
            log_debug("Strings '%s' and '%s' are not equal because of invalid UTF-8 sequences", s1, s2);
            return -1;
        }

        // Different octet lengths at position "pos"
        if (s1_octets != s2_octets)
            return 0;

        // Same length for both inputs, should be safe...
        if (utf8_compare_octets(s1, s2, pos, length, s1_octets) == 1)
            return 0;

        // Try next logical char...
        pos = pos + size_t(s1_octets);
    }
    return 1;
}

std::string escape(const char* string)
{
    if (!string)
        return "(null_ptr)";

    std::string            after;
    std::string::size_type length = strlen(string);
    after.reserve(length * 2);

    /*
        Quote from http://www.json.org/
        -------------------------------
        Char
        any-Unicode-character-except-"-or-\-or-control-character:
            \"
            \\
            \/
            \b
            \f
            \n
            \r
            \t
            \u four-hex-digits
        ------------------------------
    */
    std::string::size_type i = 0;
    while (i < length) {
        char   c     = string[i];
        int8_t width = UTF8::utf8_octets(string + i);
        switch (width) {
            case -1:
                log_debug("Cannot escape string '%s' because of invalid UTF-8 sequences at offset %ju", string, i);
                return "(invalid_utf8)";
            case 1:
                switch (c) {
                    case '"':
                        after.append("\\\"");
                        break;
                    case '\b':
                        after.append("\\\\b");
                        break;
                    case '\f':
                        after.append("\\\\f");
                        break;
                    case '\n':
                        after.append("\\\\n");
                        break;
                    case '\r':
                        after.append("\\\\r");
                        break;
                    case '\t':
                        after.append("\\\\t");
                        break;
                    case '\\':
                        after.append("\\\\");
                        break;
                    default:
                        after += c;
                }      // cases of single-byte codepoint "c"
                break; // case width==1
            case 2:
            case 3:
            case 4: {
                // escape UTF-8 chars which have more than 1 byte
                // allocate memory for "\u" + 4 hex digits + terminator
                // allocating 8 bytes just for performance doesn't make sense
                char* codepoint = static_cast<char*>(calloc(7, sizeof(char)));
                // calloc () takes care of zero termination, which utf8_to_codepoint () doesn't do
                UTF8::utf8_to_codepoint(string + i, &codepoint);

                std::string codepoint_str(codepoint);
                free(codepoint);
                codepoint = nullptr;
                after.append(codepoint_str);
            } // scope codepoint_str and codepoint
            break;
            default:
                log_debug(
                    "Cannot escape string '%s' because utf8_octets() returned "
                    "unexpected byte length %" PRIi8 " for logical character at offset %ju",
                    string, width, i);
                return "(invalid_utf8)";
        } // cases of width

        // We should not have width==0 ever, and -1 is filtered above
        i += size_t(width);
    }
    return after;
}

std::string escape(const std::string& before)
{
    return escape(before.c_str());
}

std::string bash_escape(const std::string& param)
{
    std::string escape_chars(" \t!\"#$&'()*,;<=>?[\\]^`{|}~");
    std::string escaped;
    size_t      start = 0;
    while (true) {
        size_t end = param.find_first_of(escape_chars, start);
        if (end == std::string::npos) {
            escaped += param.substr(start);
            break;
        }
        size_t length = end - start;
        escaped += param.substr(start, length) + "\\" + param[end];
        start = end + 1;
    }

    return escaped;
}

// This function converts string expressed as ("Text used as a key with %s and %d", var1, var2, ...) into JSON format:
// {
//  "key" : "Text used as a key with $var1$ and $var2$",
//  "variables" : {
//      "var1" : "foo";
//      "var2" : 5
//  }
//}
//
// It makes the following assumptions:
// * we are not using a 'space' flag in formatting directives
// * every conversion is either %s, or a sequence of non-space characters started with % and ended with a space

static std::string s_jsonify_translation_string(const char* key, va_list args)
{
    // log_trace ("called");
    if (!key) return "";

    // check if 'key' is already in JSON format
    // since we may start/end with {{vari}} and we don't want to go through the whole string,
    // check just first two and last two characters
    size_t length = strlen(key);
    if (length >= 2 && key[0] == '{' && key[1] != '{' && key[length - 2] != '}' && key[length - 1] == '}') {
        return std::string(key);
    }

    size_t  pos      = 0;
    int     va_index = 1;
    va_list args2, args3;
    va_copy(args2, args);
    va_copy(args3, args);

    // make std::string copy so we can use insert/append
    std::string key_replaced;
    key_replaced.reserve(length);
    std::string json_format;
    std::string var_entry;

    while (*key != '\0') {
        // start of formatting directive
        if (*key == '%') {
            std::string var_str     = "var" + std::to_string(va_index);
            std::string var_str_ref = "{{" + var_str + "}}";
            std::string format;
            // copy the formatting directive
            bool loop_control = true;
            while (loop_control) {
                switch (*key) {
                    case ' ':
                    case '\0':
                    case '"':
                    case '(':
                    case ')':
                    case '[':
                    case ']':
                        loop_control = false;
                        break;
                    case '\'':
                        if (*(key - 1) != '%') {
                            loop_control = false;
                            break;
                        } else {
                            [[fallthrough]];
                        }
                    default:
                        format.append(key, 1);
                        key++;
                        if (format == "%s")
                            loop_control = false;
                        break;
                }
            }

            var_entry = " \"" + var_str + "\": \"" + format + "\",";
            // update the key_replaced with variable string reference
            key_replaced.append(var_str_ref);

            // append JSON format entry for this variable
            // TODO: build objects for some variables (like dates)
            json_format.append(var_entry);
            va_index++;

            format.clear();
            var_entry.clear();
            // update the position in key_replaced to correspond to key
            pos += var_str_ref.size();
        } else {
            key_replaced.insert(pos, key, 1);
            key++;
            pos++;
        }
    }

    if (json_format.empty()) {
        va_end(args2);
        va_end(args3);
        // no formatting directives, finish up the JSON and return
        return "{ \"key\": \"" + key_replaced + "\" }";
    }

    // finish up the JSON with formatting directives:
    json_format = "{ \"key\": \"" + key_replaced + "\", \"variables\": {" + json_format;
    // remove the trailing comma
    json_format.pop_back();
    json_format.append(" } }");

    // replace formatting directives with real content
    char* json_attempt = static_cast<char*>(calloc(json_format.length(), sizeof(char)));
    if (json_attempt == nullptr) {
        va_end(args2);
        va_end(args3);
        log_error("JSON buffer allocate has failed (%zu bytes)", json_format.length());
        return "";
    }
    size_t missing = size_t(vsnprintf(json_attempt, json_format.length(), json_format.c_str(), args2));
    va_end(args2);

    if (missing >= json_format.length()) {
        char* json = static_cast<char*>(realloc(json_attempt, missing + 1));
        if (json == nullptr) {
            log_error("JSON buffer reallocate has failed (%zu bytes)", (missing + 1));
            free(json_attempt);
            va_end(args3);
            return "";
        }
        json_attempt = json;
        vsnprintf(json_attempt, missing + 1, json_format.c_str(), args3);
    } else {
        // log_trace ("JSON buffer was sufficient");
    }
    va_end(args3);

    std::string json_str(json_attempt);
    free(json_attempt);

    // drop quotes enclosing inserted variables which are already in JSON format
    // - one from the previous call, second from this one
    size_t insert_start = json_str.find("\"{");
    size_t object_start = insert_start, object_end;
    while (insert_start != std::string::npos) {
        try {
            std::string res = JSON::readObject(json_str, object_start, object_end);
            log_trace("JSON object = %s\n", res.c_str());
        } catch (JSON::NotFoundException&) {
            log_trace("JSON object not found in %s", json_str.substr(insert_start).c_str());
            object_end = std::string::npos;
        } catch (JSON::CorruptedLineException&) {
            log_trace("Corrupted line %s", json_str.substr(insert_start).c_str());
            object_end = std::string::npos;
        }
        size_t insert_end = json_str.find("}\"", insert_start);
        while (insert_end != std::string::npos && insert_end != object_end) {
            insert_end = json_str.find("}\"", insert_end + 1);
        }
        if (insert_end == object_end) {
            json_str.replace(insert_start, 2, " {");
            json_str.replace(insert_end, 2, "} ");
        }
        // move in case match was not replacable
        insert_start++;
        insert_start = json_str.find("\"{", insert_start);
        object_start = insert_start;
    }
    key_replaced.clear();
    return json_str;
}

std::string jsonify_translation_string(const char* key, ...)
{
    va_list args;
    va_start(args, key);
    std::string jsonified_str = UTF8::s_jsonify_translation_string(key, args);
    va_end(args);
    return jsonified_str;
}

std::string vajsonify_translation_string(const char* key, va_list args)
{
    std::string jsonified_str = UTF8::s_jsonify_translation_string(key, args);
    return jsonified_str;
}

} // namespace UTF8

char* utf8_escape(const char* string)
{
    std::string escaped_str = UTF8::escape(string ? string : "");
    return strdup(escaped_str.c_str());
}

char* utf8_bash_escape(const char* string)
{
    std::string escaped_str = UTF8::bash_escape(string ? string : "");
    return strdup(escaped_str.c_str());
}

char* utf8_jsonify_translation_string(const char* key, ...)
{
    if (!key) return strdup("");

    va_list args;
    va_start(args, key);
    std::string jsonified_str = UTF8::s_jsonify_translation_string(key, args);
    va_end(args);
    return strdup(jsonified_str.c_str());
}
