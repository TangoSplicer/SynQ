# CMakeLists.txt — SynQ Compiler Ecosystem
# MIT License
# © 2025 SynQ Contributors

cmake_minimum_required(VERSION 3.18)
project(SynQ LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(SYNQ_RECOVERY_CLI_VERSION "0.1.0-experimental")

# The historical test and Python-binding targets are incomplete. Keep the
# default build focused on the compiler library until those targets are repaired.
option(BUILD_TESTS "Build test suite" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python REPL bindings" OFF)
option(BUILD_CLI_TOOLS "Build standalone CLI tool sources" OFF)
option(BUILD_COMPILER_EXECUTABLE "Build the unfinished compiler command-line entry point" OFF)
option(BUILD_EXPERIMENTAL_COMPONENTS "Build experimental debugger, network, optimizer, and plugin subsystems" OFF)
option(BUILD_REPL "Build experimental REPL command sources" OFF)
option(BUILD_CORE_SMOKE_TESTS "Build focused smoke tests for the compiler core profile" ON)
option(BUILD_RECOVERY_INTEROP_SMOKE_TESTS "Build optional cross-language interoperability smoke fixtures" ON)
option(BUILD_OPENQASM_REFERENCE_CHECKS "Run optional Python OpenQASM parser and importer reference checks" ON)
option(BUILD_RECOVERY_CLI "Build the supported recovery-profile synqc executable" ON)
option(BUILD_RECOVERY_NATIVE_SDK "Install and test the experimental static C ABI SDK path" ON)
option(SYNQ_ENABLE_BUILD_HARDENING "Enable supported compiler warnings and ELF linker hardening for recovery targets" ON)
option(BUILD_RECOVERY_BENCHMARKS "Build deterministic local recovery-profile benchmark utilities" OFF)

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

if(BUILD_RECOVERY_CLI)
    add_executable(synqc tools/recovery_cli.cpp)
    target_include_directories(synqc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synqc PRIVATE synq_lib)
    target_compile_definitions(synqc PRIVATE SYNQ_RECOVERY_CLI_VERSION="${SYNQ_RECOVERY_CLI_VERSION}")
endif()

if(BUILD_RECOVERY_BENCHMARKS)
    add_executable(synq_benchmark tools/recovery_benchmark.cpp)
    target_include_directories(synq_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_benchmark PRIVATE synq_lib)
endif()

if(SYNQ_ENABLE_BUILD_HARDENING AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(synq_lib PRIVATE -Wall -Wextra -Wpedantic)
    if(BUILD_RECOVERY_CLI)
        target_compile_options(synqc PRIVATE -Wall -Wextra -Wpedantic)
        if(UNIX AND NOT APPLE)
            target_link_options(synqc PRIVATE -Wl,-z,relro,-z,now)
        endif()
    endif()
endif()

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

    add_executable(synq_hybrid_ir_smoke tests/smoke/hybrid_ir_smoke.cpp)
    target_include_directories(synq_hybrid_ir_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_hybrid_ir_smoke PRIVATE synq_lib)
    add_test(NAME synq_hybrid_ir_smoke COMMAND synq_hybrid_ir_smoke)

    add_executable(synq_name_resolution_smoke tests/smoke/name_resolution_smoke.cpp)
    target_include_directories(synq_name_resolution_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_name_resolution_smoke PRIVATE synq_lib)
    add_test(NAME synq_name_resolution_smoke COMMAND synq_name_resolution_smoke)

    add_executable(synq_semantic_kernel_smoke tests/smoke/semantic_kernel_smoke.cpp)
    target_include_directories(synq_semantic_kernel_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_semantic_kernel_smoke PRIVATE synq_lib)
    add_test(NAME synq_semantic_kernel_smoke COMMAND synq_semantic_kernel_smoke)

    add_executable(synq_classical_control_flow_smoke tests/smoke/classical_control_flow_smoke.cpp)
    target_include_directories(synq_classical_control_flow_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_classical_control_flow_smoke PRIVATE synq_lib)
    add_test(NAME synq_classical_control_flow_smoke COMMAND synq_classical_control_flow_smoke)

    add_executable(synq_classical_expression_smoke tests/smoke/classical_expression_smoke.cpp)
    target_include_directories(synq_classical_expression_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_classical_expression_smoke PRIVATE synq_lib)
    add_test(NAME synq_classical_expression_smoke COMMAND synq_classical_expression_smoke)

    add_executable(synq_boolean_expression_smoke tests/smoke/boolean_expression_smoke.cpp)
    target_include_directories(synq_boolean_expression_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_boolean_expression_smoke PRIVATE synq_lib)
    add_test(NAME synq_boolean_expression_smoke COMMAND synq_boolean_expression_smoke)

    add_executable(synq_integer_arithmetic_expression_smoke tests/smoke/integer_arithmetic_expression_smoke.cpp)
    target_include_directories(synq_integer_arithmetic_expression_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_integer_arithmetic_expression_smoke PRIVATE synq_lib)
    add_test(NAME synq_integer_arithmetic_expression_smoke COMMAND synq_integer_arithmetic_expression_smoke)

    add_executable(synq_qubit_declaration_smoke tests/smoke/qubit_declaration_smoke.cpp)
    target_include_directories(synq_qubit_declaration_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_qubit_declaration_smoke PRIVATE synq_lib)
    add_test(NAME synq_qubit_declaration_smoke COMMAND synq_qubit_declaration_smoke)

    add_executable(synq_measurement_result_smoke tests/smoke/measurement_result_smoke.cpp)
    target_include_directories(synq_measurement_result_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_measurement_result_smoke PRIVATE synq_lib)
    add_test(NAME synq_measurement_result_smoke COMMAND synq_measurement_result_smoke)

    add_executable(synq_hybrid_openqasm3_exporter_smoke tests/smoke/hybrid_openqasm3_exporter_smoke.cpp)
    target_include_directories(synq_hybrid_openqasm3_exporter_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_hybrid_openqasm3_exporter_smoke PRIVATE synq_lib)
    add_test(NAME synq_hybrid_openqasm3_exporter_smoke COMMAND synq_hybrid_openqasm3_exporter_smoke)

    add_executable(synq_callable_declaration_smoke tests/smoke/callable_declaration_smoke.cpp)
    target_include_directories(synq_callable_declaration_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_callable_declaration_smoke PRIVATE synq_lib)
    add_test(NAME synq_callable_declaration_smoke COMMAND synq_callable_declaration_smoke)

    add_executable(synq_bounded_evaluator_smoke tests/smoke/bounded_evaluator_smoke.cpp)
    target_include_directories(synq_bounded_evaluator_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_bounded_evaluator_smoke PRIVATE synq_lib)
    add_test(NAME synq_bounded_evaluator_smoke COMMAND synq_bounded_evaluator_smoke)

    add_executable(synq_bounded_simulator_smoke tests/smoke/bounded_simulator_smoke.cpp)
    target_include_directories(synq_bounded_simulator_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_bounded_simulator_smoke PRIVATE synq_lib)
    add_test(NAME synq_bounded_simulator_smoke COMMAND synq_bounded_simulator_smoke)

    if(BUILD_RECOVERY_CLI)
        add_executable(synq_cli_smoke tests/smoke/cli_smoke.cpp)
        add_test(NAME synq_cli_smoke COMMAND synq_cli_smoke $<TARGET_FILE:synqc>)
    endif()

    add_executable(synq_c_abi_smoke tests/interop/c_abi_smoke.c)
    target_include_directories(synq_c_abi_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(synq_c_abi_smoke PRIVATE synq_lib)
    set_target_properties(synq_c_abi_smoke PROPERTIES LINKER_LANGUAGE CXX)
    add_test(NAME synq_c_abi_smoke COMMAND synq_c_abi_smoke)

    if(BUILD_RECOVERY_NATIVE_SDK)
        add_test(
            NAME synq_installed_sdk_conformance
            COMMAND ${CMAKE_COMMAND}
                    -DSYNQ_BUILD_DIR=${CMAKE_BINARY_DIR}
                    -DSYNQ_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                    -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/cmake/installed_sdk_conformance.cmake
        )
    endif()

    if(BUILD_RECOVERY_INTEROP_SMOKE_TESTS)
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

        find_program(SYNQ_CARGO_EXECUTABLE cargo REQUIRED)
        set(SYNQ_RUST_ALPHA_WRAPPER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/bindings/rust/synq-alpha)
        set(SYNQ_RUST_ALPHA_WRAPPER_TARGET ${CMAKE_CURRENT_BINARY_DIR}/synq_rust_alpha_wrapper_target)
        set(SYNQ_RUST_ALPHA_WRAPPER_STAMP ${CMAKE_CURRENT_BINARY_DIR}/synq_rust_alpha_wrapper.stamp)
        add_custom_command(
            OUTPUT ${SYNQ_RUST_ALPHA_WRAPPER_STAMP}
            COMMAND ${CMAKE_COMMAND} -E env
                    "SYNQ_FFI_DIR=$<TARGET_FILE_DIR:synq_ffi_shared>"
                    "LD_LIBRARY_PATH=$<TARGET_FILE_DIR:synq_ffi_shared>:$ENV{LD_LIBRARY_PATH}"
                    "CARGO_TARGET_DIR=${SYNQ_RUST_ALPHA_WRAPPER_TARGET}"
                    ${SYNQ_CARGO_EXECUTABLE} test --manifest-path ${SYNQ_RUST_ALPHA_WRAPPER_SOURCE}/Cargo.toml --offline
            COMMAND ${CMAKE_COMMAND} -E touch ${SYNQ_RUST_ALPHA_WRAPPER_STAMP}
            WORKING_DIRECTORY ${SYNQ_RUST_ALPHA_WRAPPER_SOURCE}
            DEPENDS synq_ffi_shared
                    ${SYNQ_RUST_ALPHA_WRAPPER_SOURCE}/Cargo.toml
                    ${SYNQ_RUST_ALPHA_WRAPPER_SOURCE}/build.rs
                    ${SYNQ_RUST_ALPHA_WRAPPER_SOURCE}/src/lib.rs
                    ${SYNQ_RUST_ALPHA_WRAPPER_SOURCE}/tests/ffi_contract.rs
            VERBATIM
        )
        add_custom_target(synq_rust_alpha_wrapper_target ALL DEPENDS ${SYNQ_RUST_ALPHA_WRAPPER_STAMP})
        add_test(
            NAME synq_rust_alpha_wrapper
            COMMAND ${CMAKE_COMMAND} -E env
                    "SYNQ_FFI_DIR=$<TARGET_FILE_DIR:synq_ffi_shared>"
                    "LD_LIBRARY_PATH=$<TARGET_FILE_DIR:synq_ffi_shared>:$ENV{LD_LIBRARY_PATH}"
                    "CARGO_TARGET_DIR=${SYNQ_RUST_ALPHA_WRAPPER_TARGET}"
                    ${SYNQ_CARGO_EXECUTABLE} test --manifest-path ${SYNQ_RUST_ALPHA_WRAPPER_SOURCE}/Cargo.toml --offline
        )
    endif()

    add_executable(synq_openqasm3_exporter_smoke tests/smoke/openqasm3_exporter_smoke.cpp)
    target_include_directories(synq_openqasm3_exporter_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(synq_openqasm3_exporter_smoke PRIVATE synq_lib)
    set(SYNQ_OPENQASM3_REFERENCE_FIXTURE ${CMAKE_CURRENT_BINARY_DIR}/synq_openqasm3_reference.qasm)
    add_test(NAME synq_openqasm3_exporter_smoke COMMAND synq_openqasm3_exporter_smoke ${SYNQ_OPENQASM3_REFERENCE_FIXTURE})

    if(BUILD_OPENQASM_REFERENCE_CHECKS)
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
