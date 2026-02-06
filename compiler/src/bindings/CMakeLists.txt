find_package(pybind11 REQUIRED)
pybind11_add_module(synq_repl synq_repl.cpp)
target_include_directories(synq_repl PRIVATE ../src)