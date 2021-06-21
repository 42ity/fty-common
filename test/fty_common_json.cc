/*  =========================================================================
    fty_common_json - Functions for parsing JSON strings

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
    fty_common_json - Functions for parsing JSON strings
@discuss
@end
*/

#include "fty_common_json.h"
#include <catch2/catch.hpp>
#include <cxxtools/jsondeserializer.h>
#include <cxxtools/jsonserializer.h>

TEST_CASE("Json")
{
    printf("fty_common_json_test...\n");

    //                   0123456
    std::string input = "{{{{{{test}}}}}";
    std::string exp1  = "{{{{{test}}}}}";
    std::string exp2  = "{{{{test}}}}";
    std::string exp3  = "{test}";
    // at 0 invalid object is located
    size_t i = 1, j;
    CHECK(JSON::readObject(input, i, j) == exp1);
    CHECK(i == 1);
    CHECK(j == exp1.length() - 1 + 1);
    i = 2;
    CHECK(JSON::readObject(input, i, j) == exp2);
    CHECK(i == 2);
    CHECK(j == exp2.length() - 1 + 2);
    i = 5;
    CHECK(JSON::readObject(input, i, j) == exp3);
    CHECK(i == 5);
    CHECK(j == exp3.length() - 1 + 5);
    i = 6;
    try {
        JSON::readObject(input, i, j);
        CHECK(std::string("Exception should have been raised first") == std::string("Code should never get here"));
    } catch (JSON::NotFoundException&) {
        // this is only valid case
    } catch (...) {
        CHECK(std::string("Specific exception expected") == std::string("Code should never get here"));
    }
    i = 0;
    try {
        JSON::readObject(input, i, j);
        CHECK(std::string("Exception should have been raised first") == std::string("Code should never get here"));
    } catch (JSON::CorruptedLineException&) {
        // this is only valid case
    } catch (...) {
        CHECK(std::string("Specific exception expected") == std::string("Code should never get here"));
    }

    // writeToStream
    {
        std::ostringstream          output;
        cxxtools::SerializationInfo si;
        bool                        failed;

        // simple SI
        si.addMember("member").setValue("value");

        failed = false;
        try {
            JSON::writeToStream(output, si, false); // no beautyfication
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("writeToStream \"%s\": %s", output.str().c_str(), (failed ? "FAILED" : "success"));
        CHECK(!failed);
        CHECK(output.str() == "{\"member\":\"value\"}");
    }

    // readFromStream, writeToString
    {
        std::istringstream          in;
        cxxtools::SerializationInfo si;
        std::string                 result;
        bool                        failed;

        in.str("{\"member\":\"value\"}"); // simple paylaod

        failed = false;
        try {
            JSON::readFromStream(in, si);
            result = JSON::writeToString(si, false); // no beautyfication
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromStream: %s", (failed ? "FAILED" : "success"));
        CHECK(!failed);
        CHECK(result == "{\"member\":\"value\"}");
    }

    // readFromString
    {
        std::string                 buffer;
        cxxtools::SerializationInfo si;
        bool                        failed;

        // empty buffer (invalid JSON)
        failed = false;
        try {
            buffer = "";
            JSON::readFromString(buffer, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromString \"%s\": %s", buffer.c_str(), (failed ? "FAILED" : "success"));
        CHECK(failed);

        // empty valid JSON buffer
        failed = false;
        try {
            buffer = "{}";
            JSON::readFromString(buffer, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromString \"%s\": %s", buffer.c_str(), (failed ? "FAILED" : "success"));
        CHECK(!failed);

        // valid JSON buffer
        failed = false;
        try {
            buffer = "{\"member1\": \"value1\", \"member2\": 42, \"array\": [{\"x\": 12}, {\"x\": 13}]}";
            JSON::readFromString(buffer, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromString \"%s\": %s", buffer.c_str(), (failed ? "FAILED" : "success"));
        assert(!failed);

        // invalid JSON buffer
        failed = false;
        try {
            buffer = "{\"member1\": \"value1\", \"member2\": 42, \"array\": [{\"x: 12}, {\"x\": 13}]}";
            JSON::readFromString(buffer, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromString \"%s\": %s", buffer.c_str(), (failed ? "FAILED" : "success"));
        assert(failed);
    }

    // readFromFile
    {
        std::string                 path_name;
        cxxtools::SerializationInfo si;
        bool                        failed = false;

        // file_path empty
        try {
            path_name = "";
            JSON::readFromFile(path_name, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile \"%s\": %s", path_name.c_str(), (failed ? "FAILED" : "success"));
        CHECK(failed);

        // fanc file_pathy
        failed = false;
        try {
            path_name = "test/data/unknown.json";
            JSON::readFromFile(path_name, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile \"%s\": %s", path_name.c_str(), (failed ? "FAILED" : "success"));
        CHECK(failed);

        // file_path exist
        failed = false;
        try {
            path_name = "test/data/example.json";
            JSON::readFromFile(path_name, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile \"%s\": %s", path_name.c_str(), (failed ? "FAILED" : "success"));
        CHECK(!failed);
    }

    // writeToString
    {
        std::string                 buffer;
        cxxtools::SerializationInfo si;
        bool                        failed;

        // empty buffer
        failed = false;
        try {
            si.clear();
            buffer = JSON::writeToString(si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("writeToString %s\nbuffer: \"%s\"", (failed ? "FAILED" : "succeeded"), buffer.c_str());
        CHECK(!failed);
        CHECK(buffer == "null\n");

        // si set from file...
        failed = false;
        try {
            si.clear();
            std::string path_name = "test/data/example.json";
            JSON::readFromFile(path_name, si);
            buffer = JSON::writeToString(si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("writeToString %s\nbuffer: \"%s\"", (failed ? "FAILED" : "succeeded"), buffer.c_str());
        CHECK(!failed);
        CHECK(!buffer.empty());
    }

    // readFromFile, writeToFile
    {
        cxxtools::SerializationInfo si;
        bool                        failed;

        // si set from file... saved in another file
        failed = false;
        try {
            std::string in_file  = "test/data/example.json";
            std::string out_file = "/tmp/example.json";
            si.clear();
            JSON::readFromFile(in_file, si);
            JSON::writeToFile(out_file, si);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile/writeToFile %s", (failed ? "FAILED" : "succeeded"));
        assert(!failed);
    }

    // readFromFile, writeToString
    {
        cxxtools::SerializationInfo si1, si2;
        std::string                 buffer1, buffer2;
        bool                        failed;

        failed = false;
        try {
            std::string file1 = "test/data/example.json";
            std::string file2 = "/tmp/example.json";
            JSON::readFromFile(file1, si1);
            JSON::readFromFile(file2, si2);
            buffer1 = JSON::writeToString(si1);
            buffer2 = JSON::writeToString(si2);
        } catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile/writeToString %s\nbuffer1: %s\nbuffer2: %s", (failed ? "FAILED" : "succeeded"),
            buffer1.c_str(), buffer2.c_str());
        CHECK(!failed);
        CHECK(buffer1 == buffer2);
    }

    printf("fty_common_json_test: OK\n");
}
