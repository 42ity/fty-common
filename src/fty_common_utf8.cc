/*  =========================================================================
    fty_common_utf8 - Functions for handling UTF-8 encoded strings

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
    fty_common_utf8 - Functions for handling UTF-8 encoded strings
@discuss
@end
*/

#include "fty_common_classes.h"

namespace UTF8 {

int
codepoint_size(const char *uchar) {
    //ASCII
    if ((((unsigned char) uchar[0]) & 0x80) == 0)
        return 0;
    unsigned char start = ((unsigned char) uchar[0]) & 0xF0;
    if ((start & 0xF0) == 0xF0)
        return 7;
    if (((start & 0xE0) == 0xE0) || ((start & 0xC0) == 0xC0))
        return 6;
    return -1;
}

size_t
utf8_len(const char *s) {
    size_t len = 0;
    for (; *s; ++s) if ((*s & 0xC0) != 0x80) ++len;
    return len;
}

//returns a pointer to the beginning of the pos'th utf8 codepoint

const char
*utf8_index(const char *s, size_t pos) {
    ++pos;
    for (; *s; ++s) {
        if ((*s & 0xC0) != 0x80) --pos;
        if (pos == 0) return s;
    }
    return NULL;
}

int
utf8_to_codepoint(const char *uchar, char **codepoint) {
    //codepoint is NOT null-terminated because it makes comparison too cumbersome
    assert(codepoint);

    const unsigned char *uuchar = (unsigned char *) uchar;
    static const char hex[] = "0123456789abcdef";
    unsigned char start = uuchar[0] & 0xF0;
    unsigned char ubytes[4] = {0, 0, 0, 0};
    unsigned int codepoint_int = 0;
    int mask = 0xf;

    // 4-byte character - 7 bytes for codepoint
    if ((start & 0xF0) == 0xF0) {
        ubytes[0] = uuchar[0] & 0x7;
        for (int i = 1; i < 4; i++)
            ubytes[i] = uuchar[i] & 0x3f;

        codepoint_int = (ubytes[0] << 18) + (ubytes[1] << 12) + (ubytes[2] << 6) + ubytes[3];
        if (codepoint_int <= 0x10fff) {
            for (int i = 6; i > 1; i--) {
                (*codepoint)[i] = hex[codepoint_int & mask];
                codepoint_int >>= 4;
            }
            (*codepoint)[1] = 'u';
            (*codepoint)[0] = '\\';
            return 4;
        }            // everything else is unassigned character
        else
            return -1;
    }
    // 3-byte character
    if ((start & 0xE0) == 0xE0) {
        ubytes[0] = uuchar[0] & 0xf;
        for (int i = 1; i < 3; i++)
            ubytes[i] = uuchar[i] & 0x3f;
        codepoint_int = (ubytes[0] << 12) + (ubytes[1] << 6) + ubytes[2];
        for (int i = 5; i > 1; i--) {
            (*codepoint)[i] = hex[codepoint_int & mask];
            codepoint_int >>= 4;
        }
        (*codepoint)[1] = 'u';
        (*codepoint)[0] = '\\';
        return 3;
    }
    //2-byte character
    if ((start & 0xC0) == 0xC0) {
        ubytes[0] = uuchar[0] & 0x1f;
        ubytes[1] = uuchar[1] & 0x3f;
        codepoint_int = (ubytes[0] << 6) + ubytes[1];
        for (int i = 5; i > 2; i--) {
            (*codepoint)[i] = hex[codepoint_int & mask];
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

int
compare_utf8_codepoint(const char *str_utf8, const char *str_codepoint) {
    assert(str_utf8);
    assert(str_codepoint);
    size_t len = utf8_len(str_utf8);

    int j = 0;
    for (size_t i = 0; i < len; i++) {
        const char *pos = utf8_index(str_utf8, i);
        if (codepoint_size(pos) == 0) {
            log_debug("Comparing '%c' with '%c'\n", *pos, str_codepoint[j]);
            if (*pos != str_codepoint[j])
                return 0;
            j++;
        } else {
            char *codepoint = (char *) malloc(codepoint_size(pos) * sizeof (char));
            int rv = utf8_to_codepoint(pos, &codepoint);
            if (rv == -1)
                log_error("Error while converting alert name '%s' for comparison with alert name '%s'\n", str_utf8, str_codepoint);
            for (int k = 0; k < codepoint_size(pos); k++) {
                log_debug("codepoint : Comparing '%c' with '%c'\n", codepoint[k], str_codepoint[j]);
                if (tolower(codepoint[k]) != tolower(str_codepoint[j]))
                    return 0;
                j++;
            }
            free(codepoint);
        }
    }
    return 1;
}

// 1, ..., 4 - # of utf8 octets
// -1 - error

int8_t
utf8_octets(const char *c) {
    assert(c);
    if ((*c & 0x80) == 0) // lead bit is zero, must be a single ascii
        return 1;
    else
        if ((*c & 0xE0) == 0xC0) // 110x xxxx (2 octets)
        return 2;
    else
        if ((*c & 0xF0) == 0xE0) // 1110 xxxx (3 octets)
        return 3;
    else
        if ((*c & 0xF8) == 0xF0) // 1111 0xxx (4 octets)
        return 4;
    else
        log_error("Unrecognized utf8 lead byte '%x' in string '%s'", *c, c);
    return -1;
}

// ignores case on 1 octet bytes
// 0 - same
// 1 - different

static int
utf8_compare_octets(const char *s1, const char *s2, size_t pos, size_t length, uint8_t count) {
    assert(count >= 1 && count <= 4);
    assert(pos + count <= length);

    for (int i = 0; i < count; i++) {
        const char c1 = s1[pos + i];
        const char c2 = s2[pos + i];

        if ((count == 1 && tolower(c1) != tolower(c2)) ||
                (count > 1 && c1 != c2))
            return 1;
    }
    return 0;
}

// compare utf8 strings for equality
// ignore case on ascii (i.e on 1 byte chars)

int
utf8eq(const char *s1, const char *s2) {
    assert(s1);
    assert(s2);

    if (strlen(s1) != strlen(s2))
        return 0;

    size_t pos = 0;
    size_t length = strlen(s1);


    while (pos < length) {
        uint8_t s1_octets = utf8_octets(s1 + pos);
        uint8_t s2_octets = utf8_octets(s2 + pos);

        if (s1_octets == -1 || s2_octets == -1)
            return -1;

        if (s1_octets != s2_octets)
            return 0;

        if (utf8_compare_octets(s1, s2, pos, length, s1_octets) == 1)
            return 0;

        pos = pos + s1_octets;
    }
    return 1;
}

std::string
escape (const char *string) {
    if (!string)
        return "(null_ptr)";

    std::string after;
    std::string::size_type length = strlen (string);
    after.reserve (length * 2);

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
        char c = string[i];
        int8_t width = UTF8::utf8_octets (string + i);
        if (c == '"') {
            after.append ("\\\"");
        }
        else if (c =='\b') {
            after.append ("\\\\b");
        }
        else if (c =='\f') {
            after.append ("\\\\f");
        }
        else if (c == '\n') {
            after.append ("\\\\n");
        }
        else if (c == '\r') {
            after.append ("\\\\r");
        }
        else if (c == '\t') {
            after.append ("\\\\t");
        }
        else if (c == '\\') {
            after.append ("\\\\");
        }
        // escape UTF-8 chars which have more than 1 byte
        else if (width > 1) {
            // allocate memory for "\u" + 4 hex digits + terminator
            // allocating 8 bytes just for performance doesn't make sense
            char *codepoint = (char *) calloc (7, sizeof (char));
            // calloc() takes care of zero termination, which utf8_to_codepoint() doesn't do
            UTF8::utf8_to_codepoint (string + i, &codepoint);

            std::string codepoint_str (codepoint);
            zstr_free (&codepoint);
            after.append (codepoint_str);
        }
        else {
            after += c;
        }
        i += width;
    }
    return after;
}

std::string
escape (const std::string& before) {
    return escape (before.c_str ());
}

}
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

void
fty_common_utf8_test (bool verbose)
{
    printf (" * fty_common_utf8: ");

    //  @selftest
    //  Simple create/destroy test
    //  @end
    assert(UTF8::utf8eq("ŽlUťOUčKý kůň", "\u017dlu\u0165ou\u010dk\xc3\xbd K\u016f\xc5\x88") == 1);
    assert(UTF8::utf8eq("Žluťou\u0165ký kůň", "ŽLUťou\u0165Ký kůň") == 1);
    assert(UTF8::utf8eq("Žluťou\u0165ký kůň", "ŽLUťou\u0165Ký kůň ") == 0);
    assert(UTF8::utf8eq("Ka\xcc\x81rol", "K\xc3\xa1rol") == 0);
    assert(UTF8::utf8eq("супер test", "\u0441\u0443\u043f\u0435\u0440 Test") == 1);
    assert(UTF8::utf8eq("ŽlUťOUčKý kůň", "ŽlUťOUčKý kůn") == 0);
    log_debug("utf8eq: OK");
}
