add_library(synq_lib STATIC
  ${SRC_SOURCES}
  ${COMPILER_SOURCES}
  ${TOOLS_SOURCES}
  ${REPL_SOURCES}
  ${PLUGIN_SOURCES}
  ${LSP_SOURCES}
  ${STD_SOURCES}
)

target_link_libraries(synq_lib PRIVATE pybind11::all)
target_include_directories(synq_lib PRIVATE ${pybind11_INCLUDE_DIRS} ${pybind11_INCLUDE_DIRS}/pybind11)

if(OpenMP_CXX_FOUND)
  target_link_libraries(synq_lib PRIVATE OpenMP::OpenMP_CXX)
endif()

add_subdirectory(cli)
