add_library(synq_lib STATIC
  ${SRC_SOURCES}
  ${COMPILER_SOURCES}
  ${TOOLS_SOURCES}
  ${REPL_SOURCES}
  ${PLUGIN_SOURCES}
  ${LSP_SOURCES}
  ${STD_SOURCES}
)

find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(synq_lib PRIVATE Threads::Threads nlohmann_json::nlohmann_json OpenSSL::Crypto)
target_include_directories(synq_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_PYTHON_BINDINGS)
  find_package(pybind11 CONFIG REQUIRED)
  target_link_libraries(synq_lib PRIVATE pybind11::embed)
  target_include_directories(synq_lib PRIVATE ${pybind11_INCLUDE_DIRS})
endif()

if(OpenMP_CXX_FOUND)
  target_link_libraries(synq_lib PRIVATE OpenMP::OpenMP_CXX)
endif()

# Skip CLI for now to ensure core builds
# add_subdirectory(cli)
