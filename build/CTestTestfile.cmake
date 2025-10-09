# CMake generated Testfile for 
# Source directory: /workspace/SynQ
# Build directory: /workspace/SynQ/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllSynQTests "/workspace/SynQ/build/synq_tests")
set_tests_properties(AllSynQTests PROPERTIES  _BACKTRACE_TRIPLES "/workspace/SynQ/cmake/gtest.cmake;9;add_test;/workspace/SynQ/cmake/gtest.cmake;0;;/workspace/SynQ/CMakeLists.txt;20;include;/workspace/SynQ/CMakeLists.txt;0;")
subdirs("src")
