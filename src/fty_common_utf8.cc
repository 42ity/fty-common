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
#include <stdarg.h>

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
            free (codepoint);
            codepoint = NULL;
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

std::string
s_jsonify_translation_string (const char *key, va_list args)
{
    log_trace ("called");

    // check if 'key' is already in JSON format
    // since we may start/end with {{vari}} and we don't want to go through the whole string,
    // check just first two and last two characters
    size_t length = strlen (key);
    if (length >= 2 && key[0] == '{' && key[1] != '{' && key[length-2] != '}' && key[length-1] == '}') {
        return std::string (key);
    }

    int pos = 0;
    int va_index = 1;
    va_list args2, args3;
    va_copy (args2, args);
    va_copy (args3, args);

    // make std::string copy so we can use insert/append
    std::string key_replaced;
    key_replaced.reserve (length);
    std::string json_format;
    std::string var_entry;

    while (*key != '\0') {
        // start of formatting directive
        if (*key == '%') {
            std::string var_str = "var" + std::to_string (va_index);
            std::string var_str_ref = "{{" + var_str + "}}";
            std::string format;
            // copy the formatting directive
            while (*key != ' ' && *key != '\0') {
                format.append (key, 1);
                key++;
                if (format == "%s")
                    break;
            }

            var_entry = " \"" + var_str + "\": \"" + format + "\",";
            // update the key_replaced with variable string reference
            key_replaced.append (var_str_ref);

            // append JSON format entry for this variable
            // TODO: build objects for some variables (like dates)
            json_format.append (var_entry);
            va_index++;

            format.clear ();
            var_entry.clear ();
            // update the position in key_replaced to correspond to key
            pos += var_str_ref.size ();
        }
        else {
            key_replaced.insert (pos, key, 1);
            key++; pos++;
        }
    }

    if (json_format.empty ()) {
        // no formatting directives, finish up the JSON and return
        return "{ \"key\": \"" + key_replaced + "\" }";
    }

    // finish up the JSON with formatting directives:
    json_format = "{ \"key\": \"" + key_replaced + "\", \"variables\": {" + json_format;
    // remove the trailing comma
    json_format.pop_back ();
    json_format.append (" } }");

    // replace formatting directives with real content
    char *json_attempt = (char *) zmalloc (json_format.length ());
    size_t missing = vsnprintf (json_attempt, json_format.length (), json_format.c_str (), args2);
    va_end (args2);
    if (missing >= json_format.length ()) {
        log_trace ("JSON buffer too small, we have to reallocate");
        char *json = (char *) realloc ((void *) json_attempt, missing + 1);
        if (json != NULL && json != json_attempt) {
            json_attempt = json;
        }
        vsnprintf (json_attempt, missing + 1, json_format.c_str (), args3);
        va_end (args3);
    }
    else
        log_trace ("JSON buffer was sufficient");

    std::string json_str (json_attempt);
    free (json_attempt);

    // drop quotes enclosing inserted variables which are already in JSON format
    // - one from the previous call, second from this one
    size_t insert_start = json_str.find ("\"{");
    size_t object_start = insert_start, object_end;
    while (insert_start != std::string::npos) {
        try {
            std::string res = JSON::readObject (json_str, object_start, object_end);
            log_trace ("JSON object = %s\n", res.c_str ());
        } catch (JSON::NotFoundException &e) {
            log_error ("JSON object not found in %s", json_str.substr (insert_start).c_str ());
        } catch (JSON::CorruptedLineException &e) {
            log_error ("Corrupted line %s", json_str.substr (insert_start).c_str ());
        }
        size_t insert_end = json_str.find ("}\"", insert_start);
        if (insert_end == object_end) {
            json_str.replace (insert_start, 2, " {");
            json_str.replace (insert_end, 2, "} ");
        }
        // move in case match was not replacable
        insert_start++;
        insert_start = json_str.find ("\"{", insert_start);
        object_start = insert_start;
    }
    key_replaced.clear ();
    return json_str;
}

std::string
jsonify_translation_string (const char *key, ...)
{
    va_list args;
    va_start (args, key);
    std::string jsonified_str = UTF8::s_jsonify_translation_string (key, args);
    va_end (args);
    return jsonified_str;
}

std::string
vajsonify_translation_string (const char *key, va_list args)
{
    std::string jsonified_str = UTF8::s_jsonify_translation_string (key, args);
    return jsonified_str;
}
} // namespace UTF8

char *
utf8_escape (const char *string)
{
    std::string escaped_str = UTF8::escape (string);
    size_t length = escaped_str.length ();
    char *escaped = (char *) zmalloc (length + 1);
    strcpy (escaped, escaped_str.c_str ());
    return escaped;
}

char *
utf8_jsonify_translation_string (const char *key, ...)
{
    va_list args;
    va_start (args, key);
    std::string jsonified_str = UTF8::s_jsonify_translation_string (key, args);
    va_end (args);
    size_t length = jsonified_str.length ();
    char *jsonified = (char *) zmalloc (length + 1);
    strcpy (jsonified, jsonified_str.c_str ());
    return jsonified;
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
    //  utf8eq test
    //  @end
    {
        assert(UTF8::utf8eq("ŽlUťOUčKý kůň", "\u017dlu\u0165ou\u010dk\xc3\xbd K\u016f\xc5\x88") == 1);
        assert(UTF8::utf8eq("Žluťou\u0165ký kůň", "ŽLUťou\u0165Ký kůň") == 1);
        assert(UTF8::utf8eq("Žluťou\u0165ký kůň", "ŽLUťou\u0165Ký kůň ") == 0);
        assert(UTF8::utf8eq("Ka\xcc\x81rol", "K\xc3\xa1rol") == 0);
        assert(UTF8::utf8eq("супер test", "\u0441\u0443\u043f\u0435\u0440 Test") == 1);
        assert(UTF8::utf8eq("ŽlUťOUčKý kůň", "ŽlUťOUčKý kůn") == 0);
        log_debug("utf8eq: OK");
    }

    // utils::json::escape (<first>) should equal <second>
    std::vector <std::pair <std::string, std::string>> tests {
        {"'jednoduche ' uvozovky'",                                     "'jednoduche ' uvozovky'"},
        {"'jednoduche '' uvozovky'",                                    "'jednoduche '' uvozovky'"},
        {"dvojite \" uvozovky",                                         R"(dvojite \" uvozovky)"},
        {"dvojite \\\" uvozovky",                                       R"(dvojite \\\" uvozovky)"},
        {"dvojite \\\\\" uvozovky",                                     R"(dvojite \\\\\" uvozovky)"},
        {"dvojite \\\\\\\" uvozovky",                                   R"(dvojite \\\\\\\" uvozovky)"},
        {"dvojite \\\\\\\\\" uvozovky",                                 R"(dvojite \\\\\\\\\" uvozovky)"},
        {"'",                                                           R"(')"},
        {"\"",                                                          R"(\")"},
        {"'\"",                                                         R"('\")"},
        {"\"\"",                                                        R"(\"\")"},
        {"\"\"\"",                                                      R"(\"\"\")"},
        {"\\\"\"\"",                                                    R"(\\\"\"\")"},
        {"\"\\\"\"",                                                    R"(\"\\\"\")"},
        {"\"\"\\\"",                                                    R"(\"\"\\\")"},
        {"\\\"\\\"\\\"",                                                R"(\\\"\\\"\\\")"},
        {"\" dvojite \\\\\" uvozovky \"",                               R"(\" dvojite \\\\\" uvozovky \")"},
        {"dvojite \"\\\"\" uvozovky",                                   R"(dvojite \"\\\"\" uvozovky)"},
        {"dvojite \\\\\"\\\\\"\\\\\" uvozovky",                         R"(dvojite \\\\\"\\\\\"\\\\\" uvozovky)"},

        {"\b",                                                          R"(\\b)"},
        {"\\b",                                                         R"(\\b)"},
        {"\\\b",                                                        R"(\\\\b)"},
        {"\\\\b",                                                       R"(\\\\b)"},
        {"\\\\\b",                                                      R"(\\\\\\b)"},
        {"\\\\\\b",                                                     R"(\\\\\\b)"},
        {"\\\\\\\b",                                                    R"(\\\\\\\\b)"},
        {"\\\\\\\\b",                                                   R"(\\\\\\\\b)"},
        {"\\\\\\\\\b",                                                  R"(\\\\\\\\\\b)"},

        {"\\",                                                          R"(\\)"},
        {"\\\\",                                                        R"(\\\\)"},
        {"\\\\\\",                                                      R"(\\\\\\)"},
        {"\\\\\\\\",                                                    R"(\\\\\\\\)"},
        {"\\\\\\\\\\",                                                  R"(\\\\\\\\\\)"},
        // tests for version which does not escape UTF-8
        //{"\uA66A",                                                    "\uA66A"},
        //{"Ꙫ",                                                         "Ꙫ"},
        //{"\uA66A Ꙫ",                                                  "\uA66A Ꙫ"},

        {"\\uA66A",                                                     R"(\\uA66A)"},
        {"Ꙫ",                                                           R"(\ua66a)"},
        {"\\Ꙫ",                                                         R"(\\\ua66a)"},
        {"\u040A Њ",                                                    R"(\u040a \u040a)"},
        // do not escape control chars yet
        //{"\u0002\u0005\u0018\u001B",                                  R"(\u0002\u0005\u0018\u001b)"},

        {"\\\uA66A",                                                    R"(\\\ua66a)"},
        {"\\\\uA66A",                                                   R"(\\\\uA66A)"},
        {"\\\\\uA66A",                                                  R"(\\\\\ua66a)"},

        {"\\\\Ꙫ",                                                       R"(\\\\\ua66a)"},
        {"\\\\\\Ꙫ",                                                     R"(\\\\\\\ua66a)"},

        {"first second \n third\n\n \\n \\\\\n fourth",                 R"(first second \\n third\\n\\n \\n \\\\\\n fourth)"},
        // do not escape control chars yet
        //{"first second \n third\n\"\n \\n \\\\\"\f\\\t\\u\u0007\\\n fourth", R"(first second \\n third\\n\"\\n \\n \\\\\"\\f\\\\t\\u\u0007\\\\n fourth)"}
    };

    {
        log_debug ("fty-common-utf8:escape: Test #1");
        log_debug ("Manual comparison");

        std::string escaped;
        for (auto const& item : tests) {
            escaped = UTF8::escape (item.first);
            assert ( escaped.compare (item.second) == 0);
        }
        printf ("OK\n");
    }

    {
        log_debug ("fty-common-utf8:escape: Test #2");
        log_debug ("Manual comparison - C wrapper");

        char *escaped;
        for (auto const& item : tests) {
            escaped = utf8_escape (item.first.c_str ());
            assert (streq (escaped, item.second.c_str ()));
            free (escaped);
        }
        printf ("OK\n");
    }

    const char *translation_string1 = "Text used as a key with %s and %d";
    const char *translation_string2 = "Text used as a key with %'.2f and %lld";
    const char *translation_string3 = "Text used as a key";
    const char *translation_string4 = "Text used as a key,%s and (%s)";
    const char *translation_string5 = "{ \"key\": \"Text used as a key,{{var1}} and ({{var2}})\", \"variables\": { \"var1\": \"foo\", \"var2\": \"bar\" } }";
    const char *translation_string6 = "%s. Text used as a key: %s";
    const char *translation_string7 = "Internal Server Error. %s";
    const char *translation_string8 = "Internal Server Error. %s %s";

    std::string output1 ("{ \"key\": \"Text used as a key with {{var1}} and {{var2}}\", \"variables\": { \"var1\": \"foo\", \"var2\": \"5\" } }");
    std::string output2 ("{ \"key\": \"Text used as a key with {{var1}} and {{var2}}\", \"variables\": { \"var1\": \"10.25\", \"var2\": \"256\" } }");
    std::string output_arch ("{ \"key\": \"Text used as a key with {{var1}}\", \"variables\": { \"var1\": \"5\" } }");
    std::string output3 ("{ \"key\": \"Text used as a key\" }");
    std::string output4 ("{ \"key\": \"Text used as a key,{{var1}} and ({{var2}})\", \"variables\": { \"var1\": \"foo\", \"var2\": \"bar\" } }");
    std::string output5 ("{ \"key\": \"Text used as a key,{{var1}} and ({{var2}})\", \"variables\": { \"var1\": \"foo\", \"var2\": \"bar\" } }");
    std::string output6 ("{ \"key\": \"{{var1}}. Text used as a key: {{var2}}\", \"variables\": { \"var1\": \"foo\", \"var2\": \"bar\" } }");
    std::string output7 ("{ \"key\": \"Internal Server Error. {{var1}}\", \"variables\": { \"var1\":  { \"key\": \"Error: client-> recv (timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": \"60')\" } }  } }");
    std::string output8 ("{ \"key\": \"Internal Server Error. {{var1}} {{var2}}\", \"variables\": { \"var1\":  { \"key\": \"Error: client-> recv (timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": \"60')\" } } , \"var2\":  { \"key\": \"Unexpected param\" }  } }");
    std::string output9 ("{ \"key\": \"Internal Server Error. {{var1}}\", \"variables\": { \"var1\":  { \"key\": \"Timed out waiting for message.\" }  } }");
    {
        log_debug ("fty-common-utf8:jsonify_translation_string: Test #1");
        log_debug ("Manual comparison");
        std::string json;
        json = UTF8::jsonify_translation_string (translation_string1, "foo", 5);
        assert (json == output1);

        int64_t val = 256;
        json = UTF8::jsonify_translation_string (translation_string2, 10.25, val);
        assert (json == output2);

        json = UTF8::jsonify_translation_string ("Text used as a key with %" PRIu32, 5);
        assert (json == output_arch);

        json = UTF8::jsonify_translation_string (translation_string3);
        assert (json == output3);

        json = UTF8::jsonify_translation_string (translation_string4, "foo", "bar");
        assert (json == output4);

        json = UTF8::jsonify_translation_string (translation_string5, "foo", "bar");
        assert (json == output5);

        json = UTF8::jsonify_translation_string (translation_string6, "foo", "bar");
        assert (json == output6);

        const char *param1 = "{ \"key\": \"Error: client-> recv (timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": \"60')\" } }";
        json = UTF8::jsonify_translation_string (translation_string7, param1);
        assert (json == output7);

        const char *param2 = "{ \"key\": \"Unexpected param\" }";
        json = UTF8::jsonify_translation_string (translation_string8, param1, param2);
        assert (json == output8);

        json = UTF8::jsonify_translation_string (translation_string7, "{ \"key\": \"Timed out waiting for message.\" }");
        assert (json == output9);
        printf ("OK\n");
    }

    {
        log_debug ("fty-common-utf8:jsonify_translation_string: Test #2");
        log_debug ("Manual comparison - C wrapper");

        char *json;
        json = utf8_jsonify_translation_string (translation_string1, "foo", 5);
        assert (streq (json, output1.c_str ()));
        free (json);

        int64_t val = 256;
        json = utf8_jsonify_translation_string (translation_string2, 10.25, val);
        assert (streq (json, output2.c_str ()));
        free (json);

        json = utf8_jsonify_translation_string ("Text used as a key with %" PRIu32, 5);
        assert (streq (json, output_arch.c_str ()));
        free (json);

        json = utf8_jsonify_translation_string (translation_string3);
        assert (streq (json, output3.c_str ()));
        free (json);

        json = utf8_jsonify_translation_string (translation_string4, "foo", "bar");
        assert (streq (json, output4.c_str ()));
        free (json);

        json = utf8_jsonify_translation_string (translation_string5, "foo", "bar");
        assert (streq (json, output5.c_str ()));
        free (json);

        json = utf8_jsonify_translation_string (translation_string6, "foo", "bar");
        assert (streq (json, output6.c_str ()));
        free (json);

        const char *param1 = "{ \"key\": \"Error: client-> recv (timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": \"60')\" } }";
        json = utf8_jsonify_translation_string (translation_string7, param1);
        assert (streq (json, output7.c_str ()));
        free (json);

        const char *param2 = "{ \"key\": \"Unexpected param\" }";
        json = utf8_jsonify_translation_string (translation_string8, param1, param2);
        assert (streq (json, output8.c_str ()));
        free (json);

        json = utf8_jsonify_translation_string (translation_string7, "{ \"key\": \"Timed out waiting for message.\" }");
        assert (streq (json, output9.c_str ()));
        free (json);
        printf ("OK\n");
    }
}
