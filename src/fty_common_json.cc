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

#include "fty_common_classes.h"

namespace JSON {

JSON_TYPE getNextObject (const std::string &line, size_t &start_pos) {
    start_pos = line.find_first_not_of ("\t :,", start_pos);
    if (start_pos == std::string::npos) {
        return JT_None;
    }
    switch (line.at (start_pos)) {
        case '{':
            return JT_Object;
            break;
        case '}':
            return JT_Object_End;
            break;
        case '"':
            return JT_String;
            break;
        default:
            return JT_Invalid;
            break;
    }
}

std::string readObject (const std::string &line, size_t &start_pos, size_t &end_pos) {
    size_t temp = 0;
    end_pos = 0;
    start_pos = line.find_first_of ('{', start_pos);
    if (std::string::npos == start_pos) {
        throw NotFoundException ();
    }
    int count = 1;
    temp = start_pos; // searching at temp+1, so no need to add 1 here
    while (end_pos == 0) {
        temp = line.find_first_of ("{}", temp + 1); // always searching at pos > 0
        if (std::string::npos == temp) {
            throw CorruptedLineException ();
        } else if (line.at (temp) == '{') {
            ++count;
        } else {
            --count; // closing curly bracket
        }
        if (count == 0) {
            end_pos = temp;
        }
    }
    return line.substr (start_pos, end_pos - start_pos + 1);
}

std::string readString (const std::string &line, size_t &start_pos, size_t &end_pos) {
    size_t temp = 0;
    end_pos = 0;
    start_pos = line.find_first_of ('"', start_pos);
    if (std::string::npos == start_pos) {
        throw NotFoundException ();
    }
    temp = start_pos + 1;
    while (end_pos == 0) {
        temp = line.find_first_of ('"', temp + 1); // always searching at pos > 0
        if (std::string::npos == temp) {
            throw CorruptedLineException ();
        }
        if (line[temp - 1] != '\\') {
            end_pos = temp;
        }
    }
    return line.substr (start_pos + 1, end_pos - start_pos - 1);
}

//
// cxxtools SerializationInfo simple interface
//

// write SI to JSON ostringstream
void writeToStream (std::ostringstream& output, cxxtools::SerializationInfo& si, bool beautify)
{
    cxxtools::JsonSerializer serializer;
    serializer.beautify(beautify);
    serializer.begin(output);
    serializer.serialize(si);
    serializer.finish();
}

// write SI to JSON string
std::string writeToString (cxxtools::SerializationInfo& si, bool beautify)
{
    std::ostringstream output;
    writeToStream(output, si, beautify);
    return output.str();
}

// write SI to JSON file
void writeToFile (const std::string path_name, cxxtools::SerializationInfo& si, bool beautify)
{
    std::ofstream output;
    output.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    output.open(path_name);
    cxxtools::JsonSerializer serializer;
    serializer.beautify(beautify);
    serializer.begin(output);
    serializer.serialize(si);
    serializer.finish();
    output.close();
}

// read/set SI from JSON istringstream
void readFromStream (std::istringstream& input, cxxtools::SerializationInfo& si)
{
    cxxtools::JsonDeserializer deserializer(input);
    deserializer.deserialize(si);
}

// read/set SI from JSON string
void readFromString (const std::string string, cxxtools::SerializationInfo& si)
{
    std::istringstream input(string);
    readFromStream(input, si);
}

// read/set SI from JSON file
void readFromFile (const std::string path_name, cxxtools::SerializationInfo& si)
{
    std::ifstream input;
    input.open(path_name);
    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    cxxtools::JsonDeserializer deserializer(input);
    deserializer.deserialize(si);
    input.close();
}

} // namespace JSON

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
fty_common_json_test (bool verbose)
{
    printf("fty_common_json_test...\n");

    //                   0123456
    std::string input = "{{{{{{test}}}}}";
    std::string exp1 = "{{{{{test}}}}}";
    std::string exp2 = "{{{{test}}}}";
    std::string exp3 = "{test}";
    // at 0 invalid object is located
    size_t i = 1, j;
    assert (JSON::readObject (input, i, j) == exp1);
    assert (i == 1);
    assert (j == exp1.length () - 1 + 1);
    i = 2;
    assert (JSON::readObject (input, i, j) == exp2);
    assert (i == 2);
    assert (j == exp2.length () - 1 + 2);
    i = 5;
    assert (JSON::readObject (input, i, j) == exp3);
    assert (i == 5);
    assert (j == exp3.length () - 1 + 5);
    i = 6;
    try {
        JSON::readObject (input, i, j);
        assert (std::string ("Exception should have been raised first") == std::string ("Code should never get here"));
    } catch (JSON::NotFoundException &e) {
        // this is only valid case
    } catch (...) {
        assert (std::string ("Specific exception expected") == std::string ("Code should never get here"));
    }
    i = 0;
    try {
        JSON::readObject (input, i, j);
        assert (std::string ("Exception should have been raised first") == std::string ("Code should never get here"));
    } catch (JSON::CorruptedLineException &e) {
        // this is only valid case
    } catch (...) {
        assert (std::string ("Specific exception expected") == std::string ("Code should never get here"));
    }

    // writeToStream
    {
        std::ostringstream output;
        cxxtools::SerializationInfo si;
        bool failed;

        // simple SI
        si.addMember("member").setValue("value");

        failed = false;
        try {
            JSON::writeToStream(output, si, false); // no beautyfication
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("writeToStream \"%s\": %s", output.str().c_str(), (failed ? "FAILED" : "success"));
        assert(!failed);
        assert(output.str() == "{\"member\":\"value\"}");
    }

    // readFromStream, writeToString
    {
        std::istringstream input;
        cxxtools::SerializationInfo si;
        std::string result;
        bool failed;

        input.str("{\"member\":\"value\"}"); // simple paylaod

        failed = false;
        try {
            JSON::readFromStream(input, si);
            result = JSON::writeToString(si, false); // no beautyfication
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromStream: %s", (failed ? "FAILED" : "success"));
        assert(!failed);
        assert(result == "{\"member\":\"value\"}");
    }

    // readFromString
    {
        std::string buffer;
        cxxtools::SerializationInfo si;
        bool failed;

        // empty buffer (invalid JSON)
        failed = false;
        try {
            buffer = "";
            JSON::readFromString(buffer, si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromString \"%s\": %s", buffer.c_str(), (failed ? "FAILED" : "success"));
        assert(failed);

        // empty valid JSON buffer
        failed = false;
        try {
            buffer = "{}";
            JSON::readFromString(buffer, si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromString \"%s\": %s", buffer.c_str(), (failed ? "FAILED" : "success"));
        assert(!failed);

        // valid JSON buffer
        failed = false;
        try {
            buffer = "{\"member1\": \"value1\", \"member2\": 42, \"array\": [{\"x\": 12}, {\"x\": 13}]}";
            JSON::readFromString(buffer, si);
        }
        catch (const std::exception& e) {
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
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromString \"%s\": %s", buffer.c_str(), (failed ? "FAILED" : "success"));
        assert(failed);
    }

    // readFromFile
    {
        std::string path_name;
        cxxtools::SerializationInfo si;
        bool failed;

        // file_path empty
        failed = false;
        try {
            path_name = "";
            JSON::readFromFile(path_name, si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile \"%s\": %s", path_name.c_str(), (failed ? "FAILED" : "success"));
        assert(failed);

        // fanc file_pathy
        failed = false;
        try {
            path_name = SELFTEST_DIR_RO "/unknown.json";
            JSON::readFromFile(path_name, si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile \"%s\": %s", path_name.c_str(), (failed ? "FAILED" : "success"));
        assert(failed);

        // file_path exist
        failed = false;
        try {
            path_name = SELFTEST_DIR_RO "/example.json";
            JSON::readFromFile(path_name, si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile \"%s\": %s", path_name.c_str(), (failed ? "FAILED" : "success"));
        assert(!failed);
    }

    // writeToString
    {
        std::string buffer;
        cxxtools::SerializationInfo si;
        bool failed;

        // empty buffer
        failed = false;
        try {
            si.clear();
            buffer = JSON::writeToString(si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("writeToString %s\nbuffer: \"%s\"", (failed ? "FAILED" : "succeeded"), buffer.c_str());
        assert(!failed);
        assert(buffer == "null\n");

        // si set from file...
        failed = false;
        try {
            si.clear();
            std::string path_name = SELFTEST_DIR_RO "/example.json";
            JSON::readFromFile(path_name, si);
            buffer = JSON::writeToString(si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("writeToString %s\nbuffer: \"%s\"", (failed ? "FAILED" : "succeeded"), buffer.c_str());
        assert(!failed);
        assert(!buffer.empty());
    }

    // readFromFile, writeToFile
    {
        cxxtools::SerializationInfo si;
        bool failed;

        // si set from file... saved in another file
        failed = false;
        try {
            std::string in_file = SELFTEST_DIR_RO "/example.json";
            std::string out_file = SELFTEST_DIR_RW "/example.json";
            si.clear();
            JSON::readFromFile(in_file, si);
            JSON::writeToFile(out_file, si);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile/writeToFile %s", (failed ? "FAILED" : "succeeded"));
        assert(!failed);
    }

    // readFromFile, writeToString
    {
        cxxtools::SerializationInfo si1, si2;
        std::string buffer1, buffer2;
        bool failed;

        failed = false;
        try {
            std::string file1 = SELFTEST_DIR_RO "/example.json";
            std::string file2 = SELFTEST_DIR_RW "/example.json";
            JSON::readFromFile(file1, si1);
            JSON::readFromFile(file2, si2);
            buffer1 = JSON::writeToString(si1);
            buffer2 = JSON::writeToString(si2);
        }
        catch (const std::exception& e) {
            log_error("Exception reached (%s)", e.what());
            failed = true;
        }
        log_info("readFromFile/writeToString %s\nbuffer1: %s\nbuffer2: %s", (failed ? "FAILED" : "succeeded"),
            buffer1.c_str(), buffer2.c_str());
        assert(!failed);
        assert(buffer1 == buffer2);
    }

    printf("fty_common_json_test: OK\n");
}

