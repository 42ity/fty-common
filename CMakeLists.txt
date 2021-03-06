cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

########################################################################################################################

project(fty_common
    VERSION 1.0.0
    DESCRIPTION "Provides common tools for agents"
)

########################################################################################################################
find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR}/fty-cmake)
########################################################################################################################

etn_target(shared ${PROJECT_NAME}
    PUBLIC_INCLUDE_DIR include
    PUBLIC
        fty_common_agents.h
        fty_common_base.h
        fty_common_filesystem.h
        fty_common_json.h
        fty_common_macros.h
        fty_common_str_defs.h
        fty_common_asset_types.h
        fty_common_client.h
        fty_common.h
        fty_common_nut_types.h
        fty_common_sync_server.h
        fty_common_utf8.h
        fty_common_unit_tests.h
    SOURCES
        src/fty_common_asset_types.cc
        src/fty_common_filesystem.cc
        src/fty_common_json.cc
        src/fty_common_str_defs.cc
        src/fty_common_utf8.cc
        src/fty_common_unit_tests.cc
    FLAGS
        -Wno-format-nonliteral
    USES
        czmq
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
        test/main.cpp
        test/fty_common_base.cc
        test/fty_common_json.cc
        test/fty_common_unit_tests.cc
        test/fty_common_utf8.cc
    USES
        pthread
    SUBDIR
        test
)

########################################################################################################################

