cmake_minimum_required(VERSION 3.18)
project(SynQInstalledSdkConsumer LANGUAGES C CXX)

find_package(SynQ CONFIG REQUIRED)

add_executable(synq_installed_sdk_consumer main.c)
target_link_libraries(synq_installed_sdk_consumer PRIVATE SynQ::synq_lib)
set_target_properties(synq_installed_sdk_consumer PROPERTIES LINKER_LANGUAGE CXX)
