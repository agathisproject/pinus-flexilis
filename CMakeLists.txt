cmake_minimum_required (VERSION 3.0)
project (pinus-flexilis LANGUAGES C)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options("-Os" "-Wall" "-Wextra" "-Wpedantic"
        "-Wformat=2" "-Wformat-truncation" "-Wformat-overflow"
        "-Winit-self" "-Wfloat-equal" "-Winline" "-Wundef" "-Wshadow"
        "-Wredundant-decls" "-Wunreachable-code" "-Wmissing-include-dirs"
        "-Wunused" "-Wuninitialized" "-Wconversion" "-Wwrite-strings"
        "-Wswitch" "-Wparentheses" "-Wformat" "-Wstrict-overflow=5"
        "-Wstrict-prototypes" "-Wno-unused-parameter"
        "-fno-common" "-Wstack-usage=255"
)

add_library(cli STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/cli.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/cmd.c
)

add_library(agathis STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/agathis/base.c
)

add_library(sim STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sim/state.c
        )

add_executable(${PROJECT_NAME}
    "src/main.c"
)
target_link_libraries(${PROJECT_NAME}
        cli agathis sim
)
