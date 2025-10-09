# CMakeLists.txt — SynQ Compiler Ecosystem
# MIT License
# © 2025 SynQ Contributors

cmake_minimum_required(VERSION 3.18)
project(SynQ LANGUAGES C CXX)

file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_subdirectory(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


include(dependencies)
include(packaging)
include(gtest)
include(coverage)

# Global includes
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/compiler)
include_directories(${CMAKE_SOURCE_DIR}/tools)
include_directories(${CMAKE_SOURCE_DIR}/repl)
include_directories(${CMAKE_SOURCE_DIR}/plugins)
include_directories(${CMAKE_SOURCE_DIR}/stdlib)
include_directories(${CMAKE_SOURCE_DIR}/lsp)
include_directories(${CMAKE_SOURCE_DIR}/src/backend)
include_directories(${CMAKE_SOURCE_DIR}/src/optimizer)
include_directories(${CMAKE_SOURCE_DIR}/src/bindings)

# Helper macro to collect sources
file(GLOB_RECURSE COMPILER_SOURCES CONFIGURE_DEPENDS compiler/*.cpp)
file(GLOB_RECURSE TOOLS_SOURCES CONFIGURE_DEPENDS tools/*.cpp)
file(GLOB_RECURSE REPL_SOURCES CONFIGURE_DEPENDS repl/*.cpp)
file(GLOB_RECURSE PLUGIN_SOURCES CONFIGURE_DEPENDS plugins/*.cpp)
file(GLOB_RECURSE LSP_SOURCES CONFIGURE_DEPENDS lsp/*.cpp)
file(GLOB_RECURSE STD_SOURCES CONFIGURE_DEPENDS stdlib/*.cpp)