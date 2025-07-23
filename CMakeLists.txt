# CMakeLists.txt — SynQ Compiler Ecosystem
# SynQ Commercial Attribution License v1.0
# © 2025 SynQ Contributors. All rights reserved.

cmake_minimum_required(VERSION 3.18)
project(SynQ LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE COMPILER_SOURCES CONFIGURE_DEPENDS compiler/*.cpp)
file(GLOB_RECURSE TOOLS_SOURCES CONFIGURE_DEPENDS tools/*.cpp)
file(GLOB_RECURSE REPL_SOURCES CONFIGURE_DEPENDS repl/*.cpp)
file(GLOB_RECURSE PLUGIN_SOURCES CONFIGURE_DEPENDS plugins/*.cpp)
file(GLOB_RECURSE LSP_SOURCES CONFIGURE_DEPENDS lsp/*.cpp)
file(GLOB_RECURSE STD_SOURCES CONFIGURE_DEPENDS stdlib/*.cpp)

# Executable for main compiler CLI
add_executable(synq
  ${SRC_SOURCES}
  ${COMPILER_SOURCES}
  ${TOOLS_SOURCES}
  ${REPL_SOURCES}
  ${PLUGIN_SOURCES}
  ${LSP_SOURCES}
  ${STD_SOURCES}
)

# Required libraries
find_package(Threads REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(synq PRIVATE Threads::Threads)
target_include_directories(synq PRIVATE ${pybind11_INCLUDE_DIRS} ${pybind11_INCLUDE_DIRS}/pybind11)

# Optional: OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(synq PRIVATE OpenMP::OpenMP_CXX)
endif()

# Enable testing
enable_testing()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/unit/*.cpp tests/integration/*.cpp)
add_executable(synq_tests ${TEST_SOURCES})
target_link_libraries(synq_tests PRIVATE Threads::Threads pybind11::embed)
target_include_directories(synq_tests PRIVATE ${pybind11_INCLUDE_DIRS} ${pybind11_INCLUDE_DIRS}/pybind11)
add_test(NAME AllSynQTests COMMAND synq_tests)

# Coverage support
option(SYNQ_ENABLE_COVERAGE "Enable coverage instrumentation" ON)
if(SYNQ_ENABLE_COVERAGE)
  target_compile_options(synq PRIVATE --coverage)
  target_link_libraries(synq PRIVATE --coverage)
  target_compile_options(synq_tests PRIVATE --coverage)
  target_link_libraries(synq_tests PRIVATE --coverage)
endif()

# Installation
install(TARGETS synq DESTINATION bin)
install(DIRECTORY plugins/ DESTINATION share/synq/plugins FILES_MATCHING PATTERN "*.json")

# Packaging
set(CPACK_GENERATOR "ZIP")
include(CPack)
set(CMAKE_MODULE_PATH /home/jules/.pyenv/versions/3.12.11/lib/python3.12/site-packages/pybind11/share/cmake/pybind11 ${CMAKE_MODULE_PATH})
