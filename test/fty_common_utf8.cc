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
#include <catch2/catch.hpp>
#include <czmq.h>
#include <fty_log.h>

#define SELFTEST_DIR_RO "src/selftest-ro"
#define SELFTEST_DIR_RW "src/selftest-rw"

TEST_CASE("utf8")
{
    printf(" * fty_common_utf8: ");

    {
        CHECK(UTF8::utf8eq("ŽlUťOUčKý kůň", "\u017dlu\u0165ou\u010dk\xc3\xbd K\u016f\xc5\x88") == 1);
        CHECK(UTF8::utf8eq("Žluťou\u0165ký kůň", "ŽLUťou\u0165Ký kůň") == 1);
        CHECK(UTF8::utf8eq("Žluťou\u0165ký kůň", "ŽLUťou\u0165Ký kůň ") == 0);
        CHECK(UTF8::utf8eq("Ka\xcc\x81rol", "K\xc3\xa1rol") == 0);
        CHECK(UTF8::utf8eq("супер test", "\u0441\u0443\u043f\u0435\u0440 Test") == 1);
        CHECK(UTF8::utf8eq("ŽlUťOUčKý kůň", "ŽlUťOUčKý kůn") == 0);
        log_debug("utf8eq: OK");
    }

    // clang-format off
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
    // clang-format on

    {
        log_debug("fty-common-utf8:escape: Test #1");
        log_debug("Manual comparison");

        std::string escaped;
        for (auto const& item : tests) {
            escaped = UTF8::escape(item.first);
            CHECK(escaped.compare(item.second) == 0);
        }
        printf("OK\n");
    }

    {
        log_debug("fty-common-utf8:escape: Test #2");
        log_debug("Manual comparison - C wrapper");

        char* escaped;
        for (auto const& item : tests) {
            escaped = utf8_escape(item.first.c_str());
            assert(streq(escaped, item.second.c_str()));
            free(escaped);
        }
        printf("OK\n");
    }

    const char* translation_string1 = "Text used as a key with %s and %d";
    const char* translation_string2 = "Text used as a key with %'.2f and %lld";
    const char* translation_string3 = "Text used as a key";
    const char* translation_string4 = "Text used as a key,%s and (%s)";
    const char* translation_string5 =
        "{ \"key\": \"Text used as a key,{{var1}} and ({{var2}})\", \"variables\": { \"var1\": \"foo\", \"var2\": "
        "\"bar\" } }";
    const char* translation_string6       = "%s. Text used as a key: %s";
    const char* translation_string7       = "Internal Server Error. %s";
    const char* translation_string8       = "Internal Server Error. %s %s";
    const char* translation_string_badval = "Parameter %s has bad value. Received %s. Expected %s";

    std::string output1(
        "{ \"key\": \"Text used as a key with {{var1}} and {{var2}}\", \"variables\": { \"var1\": \"foo\", \"var2\": "
        "\"5\" } }");
    std::string output2(
        "{ \"key\": \"Text used as a key with {{var1}} and {{var2}}\", \"variables\": { \"var1\": \"10.25\", \"var2\": "
        "\"256\" } }");
    std::string output_arch("{ \"key\": \"Text used as a key with {{var1}}\", \"variables\": { \"var1\": \"5\" } }");
    std::string output3("{ \"key\": \"Text used as a key\" }");
    std::string output4(
        "{ \"key\": \"Text used as a key,{{var1}} and ({{var2}})\", \"variables\": { \"var1\": \"foo\", \"var2\": "
        "\"bar\" } }");
    std::string output5(
        "{ \"key\": \"Text used as a key,{{var1}} and ({{var2}})\", \"variables\": { \"var1\": \"foo\", \"var2\": "
        "\"bar\" } }");
    std::string output6(
        "{ \"key\": \"{{var1}}. Text used as a key: {{var2}}\", \"variables\": { \"var1\": \"foo\", \"var2\": \"bar\" "
        "} }");
    std::string output7(
        "{ \"key\": \"Internal Server Error. {{var1}}\", \"variables\": { \"var1\":  { \"key\": \"Error: client-> recv "
        "(timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": \"60')\" } }  } }");
    std::string output8(
        "{ \"key\": \"Internal Server Error. {{var1}} {{var2}}\", \"variables\": { \"var1\":  { \"key\": \"Error: "
        "client-> recv (timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": \"60')\" } } , \"var2\":  { "
        "\"key\": \"Unexpected param\" }  } }");
    std::string output9(
        "{ \"key\": \"Internal Server Error. {{var1}}\", \"variables\": { \"var1\":  { \"key\": \"Timed out waiting "
        "for message.\" }  } }");
    std::string output_badval(
        "{ \"key\": \"Parameter {{var1}} has bad value. Received {{var2}}. Expected {{var3}}\", \"variables\": { "
        "\"var1\": \"state\", \"var2\":  { \"key\": \"value '{{var1}}'\", \"variables\": { \"var1\": \"XYZ\" } } , "
        "\"var3\":  { \"key\": \"one of the following values {{var1}}\", \"variables\": { \"var1\": \"[ ALL | "
        "ALL-ACTIVE | ACTIVE | ACK-WIP | ACK-IGNORE | ACK-PAUSE | ACK-SILENCE | RESOLVED ]\" } }  } }");

    {
        log_debug("fty-common-utf8:jsonify_translation_string: Test #1");
        log_debug("Manual comparison");
        std::string json;
        json = UTF8::jsonify_translation_string(translation_string1, "foo", 5);
        CHECK(json == output1);

        int64_t val = 256;
        json        = UTF8::jsonify_translation_string(translation_string2, 10.25, val);
        CHECK(json == output2);

        json = UTF8::jsonify_translation_string("Text used as a key with %" PRIu32, 5);
        CHECK(json == output_arch);

        json = UTF8::jsonify_translation_string(translation_string3);
        CHECK(json == output3);

        json = UTF8::jsonify_translation_string(translation_string4, "foo", "bar");
        CHECK(json == output4);

        json = UTF8::jsonify_translation_string(translation_string5, "foo", "bar");
        CHECK(json == output5);

        json = UTF8::jsonify_translation_string(translation_string6, "foo", "bar");
        CHECK(json == output6);

        const char* param1 =
            "{ \"key\": \"Error: client-> recv (timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": "
            "\"60')\" } }";
        json = UTF8::jsonify_translation_string(translation_string7, param1);
        CHECK(json == output7);

        const char* param2 = "{ \"key\": \"Unexpected param\" }";
        json               = UTF8::jsonify_translation_string(translation_string8, param1, param2);
        CHECK(json == output8);

        json = UTF8::jsonify_translation_string(translation_string7, "{ \"key\": \"Timed out waiting for message.\" }");
        CHECK(json == output9);

        json = UTF8::jsonify_translation_string(translation_string_badval, "state",
            "{ \"key\": \"value '{{var1}}'\", \"variables\": { \"var1\": \"XYZ\" } }",
            "{ \"key\": \"one of the following values {{var1}}\", \"variables\": { \"var1\": \"[ ALL | ALL-ACTIVE | "
            "ACTIVE | ACK-WIP | ACK-IGNORE | ACK-PAUSE | ACK-SILENCE | RESOLVED ]\" } }");
        CHECK(json == output_badval);
        printf("OK\n");
    }

    {
        log_debug("fty-common-utf8:jsonify_translation_string: Test #2");
        log_debug("Manual comparison - C wrapper");

        char* json;
        json = utf8_jsonify_translation_string(translation_string1, "foo", 5);
        CHECK(streq(json, output1.c_str()));
        free(json);

        int64_t val = 256;
        json        = utf8_jsonify_translation_string(translation_string2, 10.25, val);
        CHECK(streq(json, output2.c_str()));
        free(json);

        json = utf8_jsonify_translation_string("Text used as a key with %" PRIu32, 5);
        CHECK(streq(json, output_arch.c_str()));
        free(json);

        json = utf8_jsonify_translation_string(translation_string3);
        CHECK(streq(json, output3.c_str()));
        free(json);

        json = utf8_jsonify_translation_string(translation_string4, "foo", "bar");
        CHECK(streq(json, output4.c_str()));
        free(json);

        json = utf8_jsonify_translation_string(translation_string5, "foo", "bar");
        CHECK(streq(json, output5.c_str()));
        free(json);

        json = utf8_jsonify_translation_string(translation_string6, "foo", "bar");
        assert(streq(json, output6.c_str()));
        free(json);

        const char* param1 =
            "{ \"key\": \"Error: client-> recv (timeout = '{{var1}} returned NULL\", \"variables\": { \"var1\": "
            "\"60')\" } }";
        json = utf8_jsonify_translation_string(translation_string7, param1);
        assert(streq(json, output7.c_str()));
        free(json);

        const char* param2 = "{ \"key\": \"Unexpected param\" }";
        json               = utf8_jsonify_translation_string(translation_string8, param1, param2);
        CHECK(streq(json, output8.c_str()));
        free(json);

        json = utf8_jsonify_translation_string(translation_string7, "{ \"key\": \"Timed out waiting for message.\" }");
        CHECK(streq(json, output9.c_str()));
        free(json);

        json = utf8_jsonify_translation_string(translation_string_badval, "state",
            "{ \"key\": \"value '{{var1}}'\", \"variables\": { \"var1\": \"XYZ\" } }",
            "{ \"key\": \"one of the following values {{var1}}\", \"variables\": { \"var1\": \"[ ALL | ALL-ACTIVE | "
            "ACTIVE | ACK-WIP | ACK-IGNORE | ACK-PAUSE | ACK-SILENCE | RESOLVED ]\" } }");
        CHECK(streq(json, output_badval.c_str()));
        free(json);
        printf("OK\n");
    }
}

TEST_CASE("string_quote_codec", "[quote]")
{


   struct {
        const std::string in; //input
    } testVector[] = {
        { "" },
        { "\"" },
        { "\"\"\"\"" },
        { "\"hello\"" },
        { "hello" },
        { "привет" }, //Russian
        { "你好" }, //Chinese simplified
        { "صباح الخير" }, //Arab
    };



   int cnt = 0;
    for (auto& it : testVector) {
        std::string input = it.in;
        std::string enc = UTF8::quoteEncode(input);
        std::string dec = UTF8::quoteDecode(enc);



       std::cout << cnt << ": " << "input('" << input << "'), enc('" << enc << "'), dec('" << dec << "')" << std::endl;
        cnt++;



       bool hasQuote = (input.find('"') != std::string::npos);



       if (hasQuote) { CHECK(input != enc); }
        else { CHECK(input == enc); }



       CHECK(enc.find('"') == std::string::npos);
        CHECK(input == dec);



       std::string enc2 = UTF8::quoteEncode(enc);
        CHECK(enc == enc2);
    }
}
