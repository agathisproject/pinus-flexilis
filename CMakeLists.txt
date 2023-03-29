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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opt/inih/ini.c
)

add_library(utils STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opt/utils.c
)

add_library(cli STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/cli.c
)

add_library(hw STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hw/platform_sim/platform.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hw/platform_sim/state.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hw/platform_sim/top.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hw/storage.c
        )
target_link_libraries(hw utils)

add_library(agathis STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/agathis/base.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/agathis/comm.c
)
target_link_libraries(agathis hw)

add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cmd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)
target_link_libraries(${PROJECT_NAME}
        pthread rt
        ini utils cli hw
        agathis
)
