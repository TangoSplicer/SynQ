// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Incremental Type Checker

#pragma once

#include "../ir/parsed_ir.h"
#include "../ir/typed_optimized_ir.h"
#include "../dependency/dependency_tracker.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <chrono>

namespace synq::compiler::pipeline {

/**
 * @class IncrementalTypeChecker
 * @brief Type checker with incremental compilation support
 * 
 * Features:
 * - Only type-checks changed files and their dependents
 * - Reuses cached symbol tables from previous builds
 * - Detects type errors incrementally
 * - 5-10x speedup for single-file changes
 * 
 * Algorithm:
 * 1. Load cached symbol tables and dependency graph
 * 2. Identify changed files (via file hash)
 * 3. Identify files that depend on changed files (transitive)
 * 4. Type-check only affected files
 * 5. Cache symbol tables and type information
 * 
 * Usage:
 * ```cpp
 * IncrementalTypeChecker checker(cache);
 * auto results = checker.check_files(parsed_irs);
 * ```
 */
class IncrementalTypeChecker {
public:
    explicit IncrementalTypeChecker(dependency::CompilationCache& cache);

    /**
     * @brief Type-check multiple files (with incremental optimization)
     */
    std::vector<ir::TypedOptimizedIR> check_files(
        const std::vector<ir::ParsedIR>& parsed_irs
    );

    /**
     * @brief Type-check a single file
     */
    ir::TypedOptimizedIR check_file(const ir::ParsedIR& parsed_ir);

    /**
     * @brief Get total type-check time (milliseconds)
     */
    uint64_t get_total_time_ms() const { return total_time_ms; }

    /**
     * @brief Get number of files that were recompiled
     */
    size_t get_recompiled_count() const { return recompiled_count; }

    /**
     * @brief Get number of files that used cache
     */
    size_t get_cached_count() const { return cached_count; }

private:
    dependency::CompilationCache& cache;
    uint64_t total_time_ms = 0;
    size_t recompiled_count = 0;
    size_t cached_count = 0;

    /**
     * @brief Perform type checking on parsed IR
     */
    ir::TypedOptimizedIR type_check(const ir::ParsedIR& parsed_ir);

    /**
     * @brief Check if file needs recompilation
     */
    bool needs_recompilation(const ir::ParsedIR& parsed_ir) const;

    /**
     * @brief Get cached type information
     */
    ir::TypedOptimizedIR get_cached_result(const ir::ParsedIR& parsed_ir) const;

    /**
     * @brief Build symbol table from AST
     */
    ir::SymbolTable build_symbol_table(const std::shared_ptr<ir::ASTNode>& ast_root);

    /**
     * @brief Infer types for AST nodes
     */
    void infer_types(
        const std::shared_ptr<ir::ASTNode>& node,
        ir::TypedOptimizedIR& result
    );

    /**
     * @brief Perform optimizations
     */
    ir::OptimizationMetrics perform_optimizations(
        const std::shared_ptr<ir::ASTNode>& ast_root
    );

    /**
     * @brief Collect dependencies from AST
     */
    std::vector<std::string> collect_dependencies(
        const std::shared_ptr<ir::ASTNode>& ast_root
    );

    /**
     * @brief Check type compatibility
     */
    bool types_compatible(
        const std::shared_ptr<ir::Type>& expected,
        const std::shared_ptr<ir::Type>& actual
    ) const;

    /**
     * @brief Infer type of expression
     */
    std::shared_ptr<ir::Type> infer_expression_type(
        const std::shared_ptr<ir::ASTNode>& expr_node,
        const ir::SymbolTable& symbols
    );

    /**
     * @brief Infer type of statement
     */
    void infer_statement_type(
        const std::shared_ptr<ir::ASTNode>& stmt_node,
        ir::TypedOptimizedIR& result
    );
};

/**
 * @class TypeCheckingContext
 * @brief Context for type checking operations
 */
struct TypeCheckingContext {
    ir::SymbolTable symbol_table;
    std::unordered_map<uintptr_t, std::shared_ptr<ir::Type>> node_types;
    std::vector<std::tuple<std::string, uint16_t, uint16_t>> errors;

    /**
     * @brief Add type error
     */
    void add_error(const std::string& message, uint16_t line, uint16_t column) {
        errors.push_back({message, line, column});
    }

    /**
     * @brief Attach type to node
     */
    void attach_type(const ir::ASTNode* node, const std::shared_ptr<ir::Type>& type) {
        node_types[reinterpret_cast<uintptr_t>(node)] = type;
    }

    /**
     * @brief Get type of node
     */
    std::shared_ptr<ir::Type> get_type(const ir::ASTNode* node) const {
        auto it = node_types.find(reinterpret_cast<uintptr_t>(node));
        if (it != node_types.end()) {
            return it->second;
        }
        return nullptr;
    }
};

} // namespace synq::compiler::pipeline
