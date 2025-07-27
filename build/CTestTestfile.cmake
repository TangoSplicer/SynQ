# CMake generated Testfile for
# Source directory: /app
# Build directory: /app/build
#
# This file includes the relevant testing commands required for
# testing this directory and lists subdirectories to be tested as well.
add_test(AllSynQTests "/app/build/synq_tests")
set_tests_properties(AllSynQTests PROPERTIES  _BACKTRACE_TRIPLES "/app/cmake/gtest.cmake;7;add_test;/app/cmake/gtest.cmake;0;;/app/CMakeLists.txt;20;include;/app/CMakeLists.txt;0;")
subdirs("src")
