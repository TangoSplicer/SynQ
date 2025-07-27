enable_testing()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/unit/*.cpp tests/integration/*.cpp)
add_executable(synq_tests ${TEST_SOURCES})
target_link_libraries(synq_tests PRIVATE Theads::Threads pybind11::embed)
target_include_directories(synq_tests PRIVATE ${pybind11_INCLUDE_DIRS} ${pybind11_INCLUDE_DIRS}/pybind11 ${CMAKE_SOURCE_DIR}/src)
add_test(NAME AllSynQTests COMMAND synq_tests)
