if(NOT DEFINED SYNQ_BUILD_DIR OR NOT DEFINED SYNQ_SOURCE_DIR)
    message(FATAL_ERROR "SYNQ_BUILD_DIR and SYNQ_SOURCE_DIR are required")
endif()

set(SYNQ_SDK_PREFIX "${SYNQ_BUILD_DIR}/installed-sdk-prefix")
set(SYNQ_CONSUMER_BUILD "${SYNQ_BUILD_DIR}/installed-sdk-consumer-build")
set(SYNQ_CONSUMER_SOURCE "${SYNQ_SOURCE_DIR}/tests/interop/installed_sdk_consumer")

if(NOT DEFINED SYNQ_BUILD_CONFIGURATION)
    set(SYNQ_BUILD_CONFIGURATION "")
endif()

file(REMOVE_RECURSE "${SYNQ_SDK_PREFIX}" "${SYNQ_CONSUMER_BUILD}")

set(install_command "${CMAKE_COMMAND}" --install "${SYNQ_BUILD_DIR}" --prefix "${SYNQ_SDK_PREFIX}")
if(SYNQ_BUILD_CONFIGURATION)
    list(APPEND install_command --config "${SYNQ_BUILD_CONFIGURATION}")
endif()
execute_process(
    COMMAND ${install_command}
    RESULT_VARIABLE install_result
)
if(NOT install_result EQUAL 0)
    message(FATAL_ERROR "Failed to install experimental SynQ SDK into isolated prefix")
endif()

set(consumer_configure_command
    "${CMAKE_COMMAND}" -S "${SYNQ_CONSUMER_SOURCE}" -B "${SYNQ_CONSUMER_BUILD}"
    "-DCMAKE_PREFIX_PATH=${SYNQ_SDK_PREFIX}"
    "-DCMAKE_BUILD_TYPE=Release"
)
if(DEFINED SYNQ_CMAKE_TOOLCHAIN_FILE AND NOT SYNQ_CMAKE_TOOLCHAIN_FILE STREQUAL "")
    list(APPEND consumer_configure_command "-DCMAKE_TOOLCHAIN_FILE=${SYNQ_CMAKE_TOOLCHAIN_FILE}")
endif()
execute_process(
    COMMAND ${consumer_configure_command}
    RESULT_VARIABLE configure_result
)
if(NOT configure_result EQUAL 0)
    message(FATAL_ERROR "Installed SDK consumer configuration failed")
endif()

set(consumer_build_command "${CMAKE_COMMAND}" --build "${SYNQ_CONSUMER_BUILD}" --parallel 2)
if(SYNQ_BUILD_CONFIGURATION)
    list(APPEND consumer_build_command --config "${SYNQ_BUILD_CONFIGURATION}")
endif()
execute_process(
    COMMAND ${consumer_build_command}
    RESULT_VARIABLE build_result
)
if(NOT build_result EQUAL 0)
    message(FATAL_ERROR "Installed SDK consumer build failed")
endif()

set(SYNQ_CONSUMER_EXECUTABLE "${SYNQ_CONSUMER_BUILD}/synq_installed_sdk_consumer${CMAKE_EXECUTABLE_SUFFIX}")
if(SYNQ_BUILD_CONFIGURATION)
    set(SYNQ_CONSUMER_EXECUTABLE "${SYNQ_CONSUMER_BUILD}/${SYNQ_BUILD_CONFIGURATION}/synq_installed_sdk_consumer${CMAKE_EXECUTABLE_SUFFIX}")
endif()
execute_process(
    COMMAND "${SYNQ_CONSUMER_EXECUTABLE}"
    RESULT_VARIABLE run_result
)
if(NOT run_result EQUAL 0)
    message(FATAL_ERROR "Installed SDK consumer failed at runtime")
endif()
