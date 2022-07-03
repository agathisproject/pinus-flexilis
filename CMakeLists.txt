cmake_minimum_required (VERSION 3.0)
project (pinus-flexilis LANGUAGES C)
#set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options("-Os" "-Wall" "-Wextra" "-Wpedantic"
        "-Wformat=2" "-Wformat-truncation" "-Wformat-overflow"
        "-Winit-self" "-Wfloat-equal" "-Winline" "-Wundef" "-Wshadow"
        "-Wredundant-decls" "-Wunreachable-code" "-Wmissing-include-dirs"
        "-Wunused" "-Wuninitialized" "-Wconversion" "-Wwrite-strings"
        "-Wswitch" "-Wparentheses" "-Wformat" "-Wstrict-overflow=5"
        "-Wstrict-prototypes" "-Wno-unused-parameter"
        "-fno-common" "-Wstack-usage=255"
)

add_library(ini STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opt/ini/ini.c
)

add_library(cli STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/cli.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/cmd.c
)

add_library(agathis STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/agathis/base.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/agathis/comm.c
)
#target_link_libraries(agathis nng)

add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sim/state.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sim/misc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sim/top.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)
target_link_libraries(${PROJECT_NAME}
        pthread rt
        ini cli agathis
)
