add_library(synq_lib STATIC
  ${SRC_SOURCES}
  ${COMPILER_SOURCES}
  ${TOOLS_SOURCES}
  ${REPL_SOURCES}
  ${PLUGIN_SOURCES}
  ${LSP_SOURCES}
  ${STD_SOURCES}
)

find_package(pybind11 CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(synq_lib PRIVATE Threads::Threads pybind11::embed)
target_include_directories(synq_lib PRIVATE ${pybind11_INCLUDE_DIRS})

if(OpenMP_CXX_FOUND)
  target_link_libraries(synq_lib PRIVATE OpenMP::OpenMP_CXX)
endif()

# Skip CLI for now to ensure core builds
# add_subdirectory(cli)