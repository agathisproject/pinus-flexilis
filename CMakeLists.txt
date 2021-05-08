cmake_minimum_required (VERSION 3.0)
project (pinus-flexilis LANGUAGES C)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Winit-self"
    "-Winline" "-Wundef" "-Wshadow" "-Wredundant-decls" "-Wunreachable-code"
    "-Wmissing-include-dirs" "-Wunused" "-Wuninitialized" "-Wconversion"
    "-Wswitch" "-Wparentheses" "-Wformat" "-Wno-unused-parameter")

add_library(cli STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/cli.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/cmd.c)

add_executable(${PROJECT_NAME}
    "src/main.c")
target_link_libraries(${PROJECT_NAME}
        cli)