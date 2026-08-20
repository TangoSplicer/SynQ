# Validate a complete candidate archive without relying on its build directory.
# The script accepts exactly the archive, manifest, and release source root.

cmake_minimum_required(VERSION 3.18)

foreach(required_var IN ITEMS
        SYNQ_RELEASE_ARCHIVE
        SYNQ_RELEASE_MANIFEST
        SYNQ_RELEASE_SOURCE_DIR
        SYNQ_RELEASE_SCRATCH_DIR)
    if(NOT DEFINED ${required_var} OR "${${required_var}}" STREQUAL "")
        message(FATAL_ERROR "${required_var} is required")
    endif()
endforeach()

foreach(required_path_var IN ITEMS SYNQ_RELEASE_ARCHIVE SYNQ_RELEASE_MANIFEST)
    if(NOT EXISTS "${${required_path_var}}")
        message(FATAL_ERROR "${required_path_var} does not exist: ${${required_path_var}}")
    endif()
endforeach()

file(READ "${SYNQ_RELEASE_MANIFEST}" manifest_contents)
foreach(required_json IN ITEMS
        "\"schema_version\": 1"
        "\"distribution\": \"synqc-release-candidate\""
        "\"publication_status\": \"not-published\""
        "\"cli_version\": \"0.1.0-experimental\""
        "\"file_name\"")
    string(FIND "${manifest_contents}" "${required_json}" json_index)
    if(json_index EQUAL -1)
        message(FATAL_ERROR "Candidate manifest is missing required field: ${required_json}")
    endif()
endforeach()

string(REGEX MATCH "\"file_name\": \"([^\"]+)\"" archive_name_match "${manifest_contents}")
if(NOT archive_name_match)
    message(FATAL_ERROR "Candidate manifest lacks archive file name")
endif()
set(manifest_archive_name "${CMAKE_MATCH_1}")
get_filename_component(actual_archive_name "${SYNQ_RELEASE_ARCHIVE}" NAME)
if(NOT manifest_archive_name STREQUAL actual_archive_name)
    message(FATAL_ERROR "Candidate manifest archive name does not match archive path")
endif()

string(REGEX MATCH "\"sha256\": \"([0-9a-f]+)\"" archive_sha_match "${manifest_contents}")
if(NOT archive_sha_match)
    message(FATAL_ERROR "Candidate manifest lacks SHA-256")
endif()
set(manifest_sha256 "${CMAKE_MATCH_1}")
file(SHA256 "${SYNQ_RELEASE_ARCHIVE}" actual_sha256)
if(NOT manifest_sha256 STREQUAL actual_sha256)
    message(FATAL_ERROR "Candidate archive SHA-256 does not match manifest")
endif()

string(REGEX MATCH "\"bytes\": ([0-9]+)" archive_bytes_match "${manifest_contents}")
if(NOT archive_bytes_match)
    message(FATAL_ERROR "Candidate manifest lacks archive byte count")
endif()
set(manifest_bytes "${CMAKE_MATCH_1}")
file(SIZE "${SYNQ_RELEASE_ARCHIVE}" actual_bytes)
if(NOT manifest_bytes STREQUAL actual_bytes)
    message(FATAL_ERROR "Candidate archive byte count does not match manifest")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -E tar tf "${SYNQ_RELEASE_ARCHIVE}"
    RESULT_VARIABLE archive_list_result
    OUTPUT_VARIABLE archive_list
    ERROR_VARIABLE archive_list_error
)
if(NOT archive_list_result EQUAL 0)
    message(FATAL_ERROR "Unable to list candidate archive: ${archive_list_error}")
endif()

string(REPLACE "\r\n" "\n" archive_list "${archive_list}")
string(REGEX MATCH "^([^/\n]+)/" root_match "${archive_list}")
if(NOT root_match)
    message(FATAL_ERROR "Candidate archive lacks a single top-level directory")
endif()
set(archive_root "${CMAKE_MATCH_1}")

string(REPLACE "\n" ";" archive_entries "${archive_list}")
foreach(archive_entry IN LISTS archive_entries)
    if(archive_entry STREQUAL "")
        continue()
    endif()
    if(NOT archive_entry MATCHES "^${archive_root}(/|$)")
        message(FATAL_ERROR "Candidate archive contains a second top-level entry: ${archive_entry}")
    endif()
    if(NOT archive_entry MATCHES
            "^${archive_root}/$|^${archive_root}/bin/$|^${archive_root}/bin/synqc(\\.exe)?$|^${archive_root}/docs/$|^${archive_root}/docs/(CLI\\.md|KNOWN_LIMITS_v0\\.1\\.0\\.md|SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0\\.1\\.0\\.md|EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY\\.md)$|^${archive_root}/(LICENSE|LICENSE_COMMERCIAL\\.md|NOTICE\\.txt|README\\.md|VERSION\\.json)$")
        message(FATAL_ERROR "Candidate archive violates the CLI-only allowlist: ${archive_entry}")
    endif()
endforeach()

file(REMOVE_RECURSE "${SYNQ_RELEASE_SCRATCH_DIR}")
file(MAKE_DIRECTORY "${SYNQ_RELEASE_SCRATCH_DIR}")
execute_process(
    COMMAND "${CMAKE_COMMAND}" -E tar xf "${SYNQ_RELEASE_ARCHIVE}"
    WORKING_DIRECTORY "${SYNQ_RELEASE_SCRATCH_DIR}"
    RESULT_VARIABLE extract_result
    ERROR_VARIABLE extract_error
)
if(NOT extract_result EQUAL 0)
    message(FATAL_ERROR "Unable to extract candidate archive: ${extract_error}")
endif()

if(EXISTS "${SYNQ_RELEASE_SCRATCH_DIR}/${archive_root}/bin/synqc.exe")
    set(candidate_cli "${SYNQ_RELEASE_SCRATCH_DIR}/${archive_root}/bin/synqc.exe")
else()
    set(candidate_cli "${SYNQ_RELEASE_SCRATCH_DIR}/${archive_root}/bin/synqc")
endif()
if(NOT EXISTS "${candidate_cli}")
    message(FATAL_ERROR "Candidate archive does not contain synqc")
endif()

execute_process(
    COMMAND "${candidate_cli}" --version
    RESULT_VARIABLE candidate_version_result
    OUTPUT_VARIABLE candidate_version_output
    ERROR_VARIABLE candidate_version_error
)
if(NOT candidate_version_result EQUAL 0 OR
   NOT candidate_version_output MATCHES "^synqc 0\\.1\\.0-experimental")
    message(FATAL_ERROR "Extracted synqc --version failed: ${candidate_version_error}${candidate_version_output}")
endif()

execute_process(
    COMMAND "${candidate_cli}" --help
    RESULT_VARIABLE candidate_help_result
    OUTPUT_VARIABLE candidate_help_output
    ERROR_VARIABLE candidate_help_error
)
if(NOT candidate_help_result EQUAL 0 OR
   NOT candidate_help_output MATCHES "--validate")
    message(FATAL_ERROR "Extracted synqc --help failed: ${candidate_help_error}${candidate_help_output}")
endif()

set(candidate_source "${SYNQ_RELEASE_SCRATCH_DIR}/candidate_validate.synq")
file(WRITE "${candidate_source}"
"#[experimental(feature = \"qubit-declarations\")]\n"
"qubit q[2]\n"
"quantum h q[0]\n"
"quantum cx q[0], q[1]\n")
execute_process(
    COMMAND "${candidate_cli}" "${candidate_source}" --validate
    RESULT_VARIABLE candidate_validate_result
    OUTPUT_VARIABLE candidate_validate_output
    ERROR_VARIABLE candidate_validate_error
)
if(NOT candidate_validate_result EQUAL 0)
    message(FATAL_ERROR "Extracted synqc --validate failed: ${candidate_validate_error}${candidate_validate_output}")
endif()

message(STATUS "Validated non-publishing synqc candidate archive: ${SYNQ_RELEASE_ARCHIVE}")
