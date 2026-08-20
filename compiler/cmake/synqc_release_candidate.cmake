# Build a non-publishing, CLI-only synqc release candidate.
#
# This script is deliberately runnable with CMake 3.18+ on Linux, macOS, and
# Windows. It creates a local/CI candidate archive plus machine-readable
# manifest and checksum material; it never creates a GitHub Release or uploads
# a package to a registry.

cmake_minimum_required(VERSION 3.18)

foreach(required_var IN ITEMS SYNQ_RELEASE_BINARY SYNQ_RELEASE_SOURCE_DIR SYNQ_RELEASE_OUTPUT_DIR)
    if(NOT DEFINED ${required_var} OR "${${required_var}}" STREQUAL "")
        message(FATAL_ERROR "${required_var} is required")
    endif()
endforeach()

if(NOT EXISTS "${SYNQ_RELEASE_BINARY}")
    message(FATAL_ERROR "SYNQ_RELEASE_BINARY does not exist: ${SYNQ_RELEASE_BINARY}")
endif()

if(NOT EXISTS "${SYNQ_RELEASE_SOURCE_DIR}/LICENSE")
    message(FATAL_ERROR "Missing required release file: ${SYNQ_RELEASE_SOURCE_DIR}/LICENSE")
endif()

if(NOT EXISTS "${SYNQ_RELEASE_SOURCE_DIR}/LICENSE_COMMERCIAL.md")
    message(FATAL_ERROR "Missing required release file: ${SYNQ_RELEASE_SOURCE_DIR}/LICENSE_COMMERCIAL.md")
endif()

function(synq_require_json_safe label value)
    if("${value}" MATCHES "[\"\r\n]")
        message(FATAL_ERROR "${label} contains a JSON-unsafe character")
    endif()
endfunction()

if(NOT DEFINED SYNQ_RELEASE_PLATFORM OR "${SYNQ_RELEASE_PLATFORM}" STREQUAL "")
    string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}" SYNQ_RELEASE_PLATFORM)
endif()

if(NOT DEFINED SYNQ_RELEASE_ARCH OR "${SYNQ_RELEASE_ARCH}" STREQUAL "")
    string(TOLOWER "${CMAKE_HOST_SYSTEM_PROCESSOR}" SYNQ_RELEASE_ARCH)
endif()

if(NOT DEFINED SYNQ_RELEASE_COMMIT OR "${SYNQ_RELEASE_COMMIT}" STREQUAL "")
    set(SYNQ_RELEASE_COMMIT "local")
endif()

if(NOT DEFINED SYNQ_RELEASE_TAG OR "${SYNQ_RELEASE_TAG}" STREQUAL "")
    set(SYNQ_RELEASE_TAG "not-a-release")
endif()

if(NOT DEFINED SYNQ_RELEASE_WORKFLOW_URL OR "${SYNQ_RELEASE_WORKFLOW_URL}" STREQUAL "")
    set(SYNQ_RELEASE_WORKFLOW_URL "local")
endif()

if(NOT DEFINED SYNQ_RELEASE_TRUST_STATE OR "${SYNQ_RELEASE_TRUST_STATE}" STREQUAL "")
    if(SYNQ_RELEASE_PLATFORM STREQUAL "macos" OR SYNQ_RELEASE_PLATFORM STREQUAL "windows")
        set(SYNQ_RELEASE_TRUST_STATE "unsigned")
    else()
        set(SYNQ_RELEASE_TRUST_STATE "not-code-signed")
    endif()
endif()

foreach(json_value IN ITEMS
        SYNQ_RELEASE_PLATFORM
        SYNQ_RELEASE_ARCH
        SYNQ_RELEASE_COMMIT
        SYNQ_RELEASE_TAG
        SYNQ_RELEASE_WORKFLOW_URL
        SYNQ_RELEASE_TRUST_STATE)
    synq_require_json_safe("${json_value}" "${${json_value}}")
endforeach()

execute_process(
    COMMAND "${SYNQ_RELEASE_BINARY}" --version
    RESULT_VARIABLE version_result
    OUTPUT_VARIABLE version_output
    ERROR_VARIABLE version_error
)
if(NOT version_result EQUAL 0)
    message(FATAL_ERROR "synqc --version failed (${version_result}): ${version_error}")
endif()

string(REPLACE "\r\n" "\n" version_output "${version_output}")
string(REGEX MATCH "^synqc ([^\n]+)" version_match "${version_output}")
if(NOT version_match)
    message(FATAL_ERROR "Unexpected synqc --version output: ${version_output}")
endif()
set(SYNQ_RELEASE_CLI_VERSION "${CMAKE_MATCH_1}")
synq_require_json_safe("synqc version" "${SYNQ_RELEASE_CLI_VERSION}")

if(SYNQ_RELEASE_PLATFORM STREQUAL "windows")
    set(SYNQ_RELEASE_ARCHIVE_EXTENSION "zip")
    set(SYNQ_RELEASE_ARCHIVE_FLAGS "cf")
    set(SYNQ_RELEASE_ARCHIVE_FORMAT "--format=zip")
else()
    set(SYNQ_RELEASE_ARCHIVE_EXTENSION "tar.gz")
    set(SYNQ_RELEASE_ARCHIVE_FLAGS "czf")
    set(SYNQ_RELEASE_ARCHIVE_FORMAT "")
endif()

set(SYNQ_RELEASE_STEM
    "synqc-${SYNQ_RELEASE_CLI_VERSION}-${SYNQ_RELEASE_PLATFORM}-${SYNQ_RELEASE_ARCH}")
set(SYNQ_RELEASE_STAGE_PARENT "${SYNQ_RELEASE_OUTPUT_DIR}/stage")
set(SYNQ_RELEASE_STAGE_DIR "${SYNQ_RELEASE_STAGE_PARENT}/${SYNQ_RELEASE_STEM}")
set(SYNQ_RELEASE_ARCHIVE
    "${SYNQ_RELEASE_OUTPUT_DIR}/${SYNQ_RELEASE_STEM}.${SYNQ_RELEASE_ARCHIVE_EXTENSION}")
set(SYNQ_RELEASE_MANIFEST
    "${SYNQ_RELEASE_OUTPUT_DIR}/${SYNQ_RELEASE_STEM}-manifest.json")
set(SYNQ_RELEASE_SUMS
    "${SYNQ_RELEASE_OUTPUT_DIR}/${SYNQ_RELEASE_STEM}-SHA256SUMS.txt")

file(REMOVE_RECURSE "${SYNQ_RELEASE_STAGE_PARENT}")
file(MAKE_DIRECTORY "${SYNQ_RELEASE_STAGE_DIR}/bin")
file(MAKE_DIRECTORY "${SYNQ_RELEASE_STAGE_DIR}/docs")
file(MAKE_DIRECTORY "${SYNQ_RELEASE_OUTPUT_DIR}")

if(SYNQ_RELEASE_PLATFORM STREQUAL "windows")
    set(SYNQ_RELEASE_BINARY_NAME "synqc.exe")
else()
    set(SYNQ_RELEASE_BINARY_NAME "synqc")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${SYNQ_RELEASE_BINARY}"
            "${SYNQ_RELEASE_STAGE_DIR}/bin/${SYNQ_RELEASE_BINARY_NAME}"
    RESULT_VARIABLE copy_binary_result
)
if(NOT copy_binary_result EQUAL 0)
    message(FATAL_ERROR "Failed to stage synqc binary")
endif()

foreach(top_level_file IN ITEMS LICENSE LICENSE_COMMERCIAL.md README.md)
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${SYNQ_RELEASE_SOURCE_DIR}/${top_level_file}"
                "${SYNQ_RELEASE_STAGE_DIR}/${top_level_file}"
        RESULT_VARIABLE copy_top_level_result
    )
    if(NOT copy_top_level_result EQUAL 0)
        message(FATAL_ERROR "Failed to stage ${top_level_file}")
    endif()
endforeach()

foreach(document_file IN ITEMS
        CLI.md
        KNOWN_LIMITS_v0.1.0.md
        SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md
        EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md)
    set(document_source "${SYNQ_RELEASE_SOURCE_DIR}/docs/${document_file}")
    if(NOT EXISTS "${document_source}")
        message(FATAL_ERROR "Missing required release document: ${document_source}")
    endif()
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${document_source}"
                "${SYNQ_RELEASE_STAGE_DIR}/docs/${document_file}"
        RESULT_VARIABLE copy_document_result
    )
    if(NOT copy_document_result EQUAL 0)
        message(FATAL_ERROR "Failed to stage ${document_file}")
    endif()
endforeach()

file(WRITE "${SYNQ_RELEASE_STAGE_DIR}/NOTICE.txt"
"SynQ ${SYNQ_RELEASE_CLI_VERSION} experimental CLI candidate\n\n"
"This archive is a non-published release candidate generated from commit ${SYNQ_RELEASE_COMMIT}.\n"
"It does not establish a stable runtime, stable ABI, provider integration, hardware access, or platform support commitment.\n"
"See docs/KNOWN_LIMITS_v0.1.0.md and docs/SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md.\n")

file(WRITE "${SYNQ_RELEASE_STAGE_DIR}/VERSION.json"
"{\n"
"  \"schema_version\": 1,\n"
"  \"distribution\": \"synqc-release-candidate\",\n"
"  \"publication_status\": \"not-published\",\n"
"  \"cli_version\": \"${SYNQ_RELEASE_CLI_VERSION}\",\n"
"  \"release_tag\": \"${SYNQ_RELEASE_TAG}\",\n"
"  \"commit\": \"${SYNQ_RELEASE_COMMIT}\",\n"
"  \"platform\": \"${SYNQ_RELEASE_PLATFORM}\",\n"
"  \"architecture\": \"${SYNQ_RELEASE_ARCH}\",\n"
"  \"trust_state\": \"${SYNQ_RELEASE_TRUST_STATE}\",\n"
"  \"workflow_url\": \"${SYNQ_RELEASE_WORKFLOW_URL}\"\n"
"}\n")

if(SYNQ_RELEASE_PLATFORM STREQUAL "windows")
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E tar ${SYNQ_RELEASE_ARCHIVE_FLAGS}
                "${SYNQ_RELEASE_ARCHIVE}" ${SYNQ_RELEASE_ARCHIVE_FORMAT}
                "${SYNQ_RELEASE_STEM}"
        WORKING_DIRECTORY "${SYNQ_RELEASE_STAGE_PARENT}"
        RESULT_VARIABLE archive_result
        ERROR_VARIABLE archive_error
    )
else()
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E tar ${SYNQ_RELEASE_ARCHIVE_FLAGS}
                "${SYNQ_RELEASE_ARCHIVE}" "${SYNQ_RELEASE_STEM}"
        WORKING_DIRECTORY "${SYNQ_RELEASE_STAGE_PARENT}"
        RESULT_VARIABLE archive_result
        ERROR_VARIABLE archive_error
    )
endif()
if(NOT archive_result EQUAL 0)
    message(FATAL_ERROR "Failed to create candidate archive: ${archive_error}")
endif()

if(NOT EXISTS "${SYNQ_RELEASE_ARCHIVE}")
    message(FATAL_ERROR "Candidate archive was not created: ${SYNQ_RELEASE_ARCHIVE}")
endif()

file(SHA256 "${SYNQ_RELEASE_ARCHIVE}" SYNQ_RELEASE_SHA256)
file(SIZE "${SYNQ_RELEASE_ARCHIVE}" SYNQ_RELEASE_BYTES)
get_filename_component(SYNQ_RELEASE_ARCHIVE_NAME "${SYNQ_RELEASE_ARCHIVE}" NAME)

file(WRITE "${SYNQ_RELEASE_SUMS}"
"${SYNQ_RELEASE_SHA256}  ${SYNQ_RELEASE_ARCHIVE_NAME}\n")

file(WRITE "${SYNQ_RELEASE_MANIFEST}"
"{\n"
"  \"schema_version\": 1,\n"
"  \"distribution\": \"synqc-release-candidate\",\n"
"  \"publication_status\": \"not-published\",\n"
"  \"cli_version\": \"${SYNQ_RELEASE_CLI_VERSION}\",\n"
"  \"release_tag\": \"${SYNQ_RELEASE_TAG}\",\n"
"  \"commit\": \"${SYNQ_RELEASE_COMMIT}\",\n"
"  \"platform\": \"${SYNQ_RELEASE_PLATFORM}\",\n"
"  \"architecture\": \"${SYNQ_RELEASE_ARCH}\",\n"
"  \"trust_state\": \"${SYNQ_RELEASE_TRUST_STATE}\",\n"
"  \"workflow_url\": \"${SYNQ_RELEASE_WORKFLOW_URL}\",\n"
"  \"archive\": {\n"
"    \"file_name\": \"${SYNQ_RELEASE_ARCHIVE_NAME}\",\n"
"    \"bytes\": ${SYNQ_RELEASE_BYTES},\n"
"    \"sha256\": \"${SYNQ_RELEASE_SHA256}\"\n"
"  }\n"
"}\n")

file(WRITE "${SYNQ_RELEASE_OUTPUT_DIR}/release-candidate-paths.txt"
"archive=${SYNQ_RELEASE_ARCHIVE}\n"
"manifest=${SYNQ_RELEASE_MANIFEST}\n"
"sha256sums=${SYNQ_RELEASE_SUMS}\n"
"stage=${SYNQ_RELEASE_STAGE_DIR}\n")

message(STATUS "Created non-publishing synqc candidate archive: ${SYNQ_RELEASE_ARCHIVE}")
message(STATUS "Candidate manifest: ${SYNQ_RELEASE_MANIFEST}")
message(STATUS "Candidate SHA-256: ${SYNQ_RELEASE_SHA256}")
