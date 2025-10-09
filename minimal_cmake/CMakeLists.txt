cmake_minimum_required(VERSION 3.18)
project(SynQCore LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find required packages
find_package(Threads REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(OpenMP)

# Core library sources (minimal set)
set(CORE_SOURCES
    src/backend/simulator.cpp
    src/backend/ai_provider.cpp
    src/compiler/ast.cpp
    src/compiler/parser.cpp
    src/compiler/codegen.cpp
    src/compiler/runtime.cpp
    src/runtime/runtime.cpp
)

# Create core library
add_library(synq_core STATIC ${CORE_SOURCES})

# Link libraries
target_link_libraries(synq_core PRIVATE Threads::Threads pybind11::embed)
target_include_directories(synq_core PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${pybind11_INCLUDE_DIRS}
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(synq_core PRIVATE OpenMP::OpenMP_CXX)
endif()