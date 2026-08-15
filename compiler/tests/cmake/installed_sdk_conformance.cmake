if(NOT DEFINED SYNQ_BUILD_DIR OR NOT DEFINED SYNQ_SOURCE_DIR)
    message(FATAL_ERROR "SYNQ_BUILD_DIR and SYNQ_SOURCE_DIR are required")
endif()

set(SYNQ_SDK_PREFIX "${SYNQ_BUILD_DIR}/installed-sdk-prefix")
set(SYNQ_CONSUMER_BUILD "${SYNQ_BUILD_DIR}/installed-sdk-consumer-build")
set(SYNQ_CONSUMER_SOURCE "${SYNQ_SOURCE_DIR}/tests/interop/installed_sdk_consumer")

file(REMOVE_RECURSE "${SYNQ_SDK_PREFIX}" "${SYNQ_CONSUMER_BUILD}")

execute_process(
    COMMAND "${CMAKE_COMMAND}" --install "${SYNQ_BUILD_DIR}" --prefix "${SYNQ_SDK_PREFIX}"
    RESULT_VARIABLE install_result
)
if(NOT install_result EQUAL 0)
    message(FATAL_ERROR "Failed to install experimental SynQ SDK into isolated prefix")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -S "${SYNQ_CONSUMER_SOURCE}" -B "${SYNQ_CONSUMER_BUILD}"
            "-DCMAKE_PREFIX_PATH=${SYNQ_SDK_PREFIX}"
            "-DCMAKE_BUILD_TYPE=Release"
    RESULT_VARIABLE configure_result
)
if(NOT configure_result EQUAL 0)
    message(FATAL_ERROR "Installed SDK consumer configuration failed")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${SYNQ_CONSUMER_BUILD}" --parallel 2
    RESULT_VARIABLE build_result
)
if(NOT build_result EQUAL 0)
    message(FATAL_ERROR "Installed SDK consumer build failed")
endif()

execute_process(
    COMMAND "${SYNQ_CONSUMER_BUILD}/synq_installed_sdk_consumer"
    RESULT_VARIABLE run_result
)
if(NOT run_result EQUAL 0)
    message(FATAL_ERROR "Installed SDK consumer failed at runtime")
endif()
