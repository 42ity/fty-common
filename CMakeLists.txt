cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

########################################################################################################################

project(fty_common
    VERSION 1.0.0
    DESCRIPTION "Common tools library"
)

########################################################################################################################
find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR}/fty-cmake)
########################################################################################################################

etn_target(shared ${PROJECT_NAME}
    PUBLIC_INCLUDE_DIR include
    PUBLIC
        fty_common.h
        fty_common_agents.h
        fty_common_asset_types.h
        fty_common_base.h
        fty_common_client.h
        fty_common_filesystem.h
        fty_common_json.h
        fty_common_macros.h
        fty_common_nut_types.h
        fty_common_quote_codec.h
        fty_common_str_defs.h
        fty_common_sync_server.h
        fty_common_unit_tests.h
        fty_common_utf8.h
    SOURCES
        src/fty_common_asset_types.cc
        src/fty_common_filesystem.cc
        src/fty_common_json.cc
        src/fty_common_quote_codec.cc
        src/fty_common_str_defs.cc
        src/fty_common_unit_tests.cc
        src/fty_common_utf8.cc
    FLAGS
        -Wno-format-nonliteral
    USES
        cxxtools
        fty_common_logging
        pthread
)

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

########################################################################################################################

etn_test_target(${PROJECT_NAME}
    CONFIGS
        test/data/example.json
        test/data/test-config.conf
    SOURCES
        test/main.cc
        test/fty_common_base.cc
        test/fty_common_asset_types.cc
        test/fty_common_json.cc
        test/fty_common_filesystem.cc
        test/fty_common_unit_tests.cc
        test/fty_common_utf8.cc
        test/fty_common_quote_codec.cc
    USES
        pthread
    SUBDIR
        test
)

########################################################################################################################

