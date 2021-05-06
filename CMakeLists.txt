cmake_minimum_required (VERSION 3.0)
project (pinus-flexilis LANGUAGES C)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Winit-self"
    "-Winline" "-Wundef" "-Wshadow" "-Wredundant-decls" "-Wunreachable-code"
    "-Wmissing-include-dirs" "-Wunused" "-Wuninitialized" "-Wconversion"
    "-Wswitch" "-Wparentheses" "-Wformat" "-Wno-unused-parameter")

add_executable(${PROJECT_NAME}
    "src/main.c")