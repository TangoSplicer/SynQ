# CMakeLists.txt — SynQ Compiler Ecosystem
# MIT License
# © 2025 SynQ Contributors

cmake_minimum_required(VERSION 3.18)
project(SynQ LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# The historical test and Python-binding targets are incomplete. Keep the
# default build focused on the compiler library until those targets are repaired.
option(BUILD_TESTS "Build test suite" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python REPL bindings" OFF)
option(BUILD_CLI_TOOLS "Build standalone CLI tool sources" OFF)
option(BUILD_COMPILER_EXECUTABLE "Build the unfinished compiler command-line entry point" OFF)
option(BUILD_EXPERIMENTAL_COMPONENTS "Build experimental debugger, network, optimizer, and plugin subsystems" OFF)
option(BUILD_REPL "Build experimental REPL command sources" OFF)
option(BUILD_CORE_SMOKE_TESTS "Build focused smoke tests for the compiler core profile" ON)

file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS src/*.cpp)
if(NOT BUILD_PYTHON_BINDINGS)
    list(FILTER SRC_SOURCES EXCLUDE REGEX ".*/bindings/.*\\.cpp$")
endif()
if(NOT BUILD_CLI_TOOLS)
    list(FILTER SRC_SOURCES EXCLUDE REGEX ".*/cli/.*\\.cpp$")
endif()
if(NOT BUILD_COMPILER_EXECUTABLE)
    list(FILTER SRC_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")
endif()
if(NOT BUILD_EXPERIMENTAL_COMPONENTS)
    list(FILTER SRC_SOURCES EXCLUDE REGEX ".*/(debugger|net|optimizer|plugins|runtime)/.*\\.cpp$")
endif()
if(NOT BUILD_REPL)
    list(FILTER SRC_SOURCES EXCLUDE REGEX ".*/repl/.*\\.cpp$")
endif()
add_subdirectory(src)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


include(dependencies)
include(packaging)

# Only include tests if building with tests enabled.
if(BUILD_TESTS)
    include(gtest)
    include(coverage)
endif()

if(BUILD_CORE_SMOKE_TESTS)
    enable_testing()
    add_executable(synq_core_smoke tests/smoke/core_smoke.cpp)
    target_include_directories(synq_core_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_core_smoke PRIVATE synq_lib)
    add_test(NAME synq_core_smoke COMMAND synq_core_smoke)

    add_executable(synq_parser_runtime_smoke tests/smoke/parser_runtime_smoke.cpp)
    target_include_directories(synq_parser_runtime_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_parser_runtime_smoke PRIVATE synq_lib)
    add_test(NAME synq_parser_runtime_smoke COMMAND synq_parser_runtime_smoke)

    add_executable(synq_feature_gate_smoke tests/smoke/feature_gate_smoke.cpp)
    target_include_directories(synq_feature_gate_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_feature_gate_smoke PRIVATE synq_lib)
    add_test(NAME synq_feature_gate_smoke COMMAND synq_feature_gate_smoke)

    add_executable(synq_diagnostics_smoke tests/smoke/diagnostic_smoke.cpp)
    target_include_directories(synq_diagnostics_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_diagnostics_smoke PRIVATE synq_lib)
    add_test(NAME synq_diagnostics_smoke COMMAND synq_diagnostics_smoke)

    add_executable(synq_gate_validation_smoke tests/smoke/gate_validation_smoke.cpp)
    target_include_directories(synq_gate_validation_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_gate_validation_smoke PRIVATE synq_lib)
    add_test(NAME synq_gate_validation_smoke COMMAND synq_gate_validation_smoke)

    add_executable(synq_c_abi_smoke tests/interop/c_abi_smoke.c)
    target_include_directories(synq_c_abi_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(synq_c_abi_smoke PRIVATE synq_lib)
    set_target_properties(synq_c_abi_smoke PROPERTIES LINKER_LANGUAGE CXX)
    add_test(NAME synq_c_abi_smoke COMMAND synq_c_abi_smoke)

    add_library(synq_ffi_shared SHARED tests/interop/synq_ffi_shared_anchor.cpp)
    target_include_directories(synq_ffi_shared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(synq_ffi_shared PRIVATE synq_lib)
    set_target_properties(synq_ffi_shared PROPERTIES OUTPUT_NAME synq_ffi)

    find_program(SYNQ_SBCL_EXECUTABLE sbcl REQUIRED)
    add_test(
        NAME synq_common_lisp_abi_smoke
        COMMAND ${SYNQ_SBCL_EXECUTABLE} --noinform --script
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/common_lisp_abi_smoke.lisp
                $<TARGET_FILE:synq_ffi_shared>
    )

    find_program(SYNQ_CLOJURE_EXECUTABLE clojure REQUIRED)
    find_file(SYNQ_JNA_JAR NAMES jna.jar PATHS /usr/share/java REQUIRED)
    add_test(
        NAME synq_clojure_jna_abi_smoke
        COMMAND ${SYNQ_CLOJURE_EXECUTABLE} -cp ${SYNQ_JNA_JAR}
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/clojure_jna_abi_smoke.clj
                $<TARGET_FILE:synq_ffi_shared>
    )

    find_program(SYNQ_MMC_EXECUTABLE mmc REQUIRED)
    get_filename_component(SYNQ_MMC_DIRECTORY ${SYNQ_MMC_EXECUTABLE} DIRECTORY)
    set(SYNQ_MERCURY_ABI_SMOKE ${CMAKE_CURRENT_BINARY_DIR}/mercury_abi_smoke)
    set(SYNQ_MERCURY_ABI_SMOKE_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/mercury_abi_smoke.m)
    add_custom_command(
        OUTPUT ${SYNQ_MERCURY_ABI_SMOKE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/mercury_abi_smoke.m
                ${SYNQ_MERCURY_ABI_SMOKE_SOURCE}
        COMMAND ${CMAKE_COMMAND} -E env "PATH=${SYNQ_MMC_DIRECTORY}:$ENV{PATH}"
                ${SYNQ_MMC_EXECUTABLE} --make --grade asm_fast.gc
                --c-include-directory ${CMAKE_CURRENT_SOURCE_DIR}/include
                --link-object $<TARGET_FILE:synq_ffi_shared>
                --ld-flag=-Wl,-rpath,$<TARGET_FILE_DIR:synq_ffi_shared>
                mercury_abi_smoke
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS synq_ffi_shared ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/mercury_abi_smoke.m
        VERBATIM
    )
    add_custom_target(synq_mercury_abi_smoke_target ALL DEPENDS ${SYNQ_MERCURY_ABI_SMOKE})
    add_test(NAME synq_mercury_abi_smoke COMMAND ${SYNQ_MERCURY_ABI_SMOKE})

    find_program(SYNQ_RUSTC_EXECUTABLE rustc REQUIRED)
    set(SYNQ_RUST_ABI_SMOKE ${CMAKE_CURRENT_BINARY_DIR}/synq_rust_abi_smoke)
    add_custom_command(
        OUTPUT ${SYNQ_RUST_ABI_SMOKE}
        COMMAND ${SYNQ_RUSTC_EXECUTABLE} --edition=2021 ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/rust_abi_smoke.rs
                -L native=$<TARGET_FILE_DIR:synq_lib>
                -l static=synq_lib
                -l dylib=stdc++
                -l dylib=crypto
                -l dylib=pthread
                -o ${SYNQ_RUST_ABI_SMOKE}
        DEPENDS synq_lib ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/rust_abi_smoke.rs
        VERBATIM
    )
    add_custom_target(synq_rust_abi_smoke_target ALL DEPENDS ${SYNQ_RUST_ABI_SMOKE})
    add_test(NAME synq_rust_abi_smoke COMMAND ${SYNQ_RUST_ABI_SMOKE})

    add_executable(synq_openqasm3_exporter_smoke tests/smoke/openqasm3_exporter_smoke.cpp)
    target_include_directories(synq_openqasm3_exporter_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_openqasm3_exporter_smoke PRIVATE synq_lib)
    set(SYNQ_OPENQASM3_REFERENCE_FIXTURE ${CMAKE_CURRENT_BINARY_DIR}/synq_openqasm3_reference.qasm)
    add_test(NAME synq_openqasm3_exporter_smoke COMMAND synq_openqasm3_exporter_smoke ${SYNQ_OPENQASM3_REFERENCE_FIXTURE})

    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_test(
        NAME synq_openqasm3_reference_parse
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/validate_openqasm3.py ${SYNQ_OPENQASM3_REFERENCE_FIXTURE}
    )
    set_tests_properties(synq_openqasm3_reference_parse PROPERTIES DEPENDS synq_openqasm3_exporter_smoke)

    add_test(
        NAME synq_openqasm3_qiskit_import
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/interop/validate_qiskit_qasm3_import.py ${SYNQ_OPENQASM3_REFERENCE_FIXTURE}
    )
    set_tests_properties(synq_openqasm3_qiskit_import PROPERTIES DEPENDS synq_openqasm3_exporter_smoke)
endif()

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
