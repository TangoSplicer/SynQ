# CTest entry point for the current-platform candidate package smoke test.

if(NOT DEFINED SYNQ_BUILD_DIR OR NOT DEFINED SYNQ_SOURCE_DIR)
    message(FATAL_ERROR "SYNQ_BUILD_DIR and SYNQ_SOURCE_DIR are required")
endif()

if(WIN32)
    set(candidate_binary "${SYNQ_BUILD_DIR}/Release/synqc.exe")
    set(candidate_platform "windows")
elseif(APPLE)
    set(candidate_binary "${SYNQ_BUILD_DIR}/synqc")
    set(candidate_platform "macos")
else()
    set(candidate_binary "${SYNQ_BUILD_DIR}/synqc")
    set(candidate_platform "linux")
endif()

set(candidate_output "${SYNQ_BUILD_DIR}/release-candidate-smoke")
set(candidate_tag "v0.1.0-experimental-local-test")
execute_process(
    COMMAND "${CMAKE_COMMAND}"
            -DSYNQ_RELEASE_BINARY=${candidate_binary}
            -DSYNQ_RELEASE_SOURCE_DIR=${SYNQ_SOURCE_DIR}/..
            -DSYNQ_RELEASE_OUTPUT_DIR=${candidate_output}
            -DSYNQ_RELEASE_PLATFORM=${candidate_platform}
            -DSYNQ_RELEASE_ARCH=test-arch
            -DSYNQ_RELEASE_COMMIT=local-ctest
            -DSYNQ_RELEASE_TAG=${candidate_tag}
            -DSYNQ_RELEASE_WORKFLOW_URL=local
            -P ${SYNQ_SOURCE_DIR}/cmake/synqc_release_candidate.cmake
    RESULT_VARIABLE package_result
)
if(NOT package_result EQUAL 0)
    message(FATAL_ERROR "Candidate archive packaging failed")
endif()

if(candidate_platform STREQUAL "windows")
    set(candidate_extension "zip")
else()
    set(candidate_extension "tar.gz")
endif()
set(candidate_stem "synqc-0.1.0-experimental-${candidate_platform}-test-arch")

execute_process(
    COMMAND "${CMAKE_COMMAND}"
            -DSYNQ_RELEASE_ARCHIVE=${candidate_output}/${candidate_stem}.${candidate_extension}
            -DSYNQ_RELEASE_MANIFEST=${candidate_output}/${candidate_stem}-manifest.json
            -DSYNQ_RELEASE_SOURCE_DIR=${SYNQ_SOURCE_DIR}/..
            -DSYNQ_RELEASE_SCRATCH_DIR=${candidate_output}/extract
            -DSYNQ_EXPECTED_TAG=${candidate_tag}
            -P ${SYNQ_SOURCE_DIR}/tests/cmake/release_candidate_package_smoke.cmake
    RESULT_VARIABLE validation_result
)
if(NOT validation_result EQUAL 0)
    message(FATAL_ERROR "Candidate archive validation failed")
endif()
