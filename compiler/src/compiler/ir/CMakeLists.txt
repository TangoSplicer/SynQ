# Phase 11: Performance & Compilation - IR Module
# Intermediate Representation types for modular compilation pipeline

add_library(synq_ir
    serializable_ir.cpp
    token_stream.cpp
    ast_ir.cpp
    typed_ast_ir.cpp
    optimized_ir.cpp
    llvm_ir.cpp
)

target_include_directories(synq_ir PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${NLOHMANN_JSON_INCLUDE_DIR}
)

target_link_libraries(synq_ir PUBLIC
    nlohmann_json::nlohmann_json
)

# Enable C++17 features
target_compile_features(synq_ir PUBLIC cxx_std_17)

# Add compiler warnings
if(MSVC)
    target_compile_options(synq_ir PRIVATE /W4)
else()
    target_compile_options(synq_ir PRIVATE -Wall -Wextra -Wpedantic)
endif()
