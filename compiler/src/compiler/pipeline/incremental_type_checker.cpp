// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Incremental Type Checker Implementation

#include "incremental_type_checker.h"
#include <chrono>
#include <algorithm>

namespace synq::compiler::pipeline {

IncrementalTypeChecker::IncrementalTypeChecker(dependency::CompilationCache& cache)
    : cache(cache) {
}

std::vector<ir::TypedOptimizedIR> IncrementalTypeChecker::check_files(
    const std::vector<ir::ParsedIR>& parsed_irs) {
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<ir::TypedOptimizedIR> results;
    
    // Identify which files need recompilation
    std::unordered_set<std::string> changed_files;
    for (const auto& parsed_ir : parsed_irs) {
        if (needs_recompilation(parsed_ir)) {
            changed_files.insert(parsed_ir.get_file_path());
        }
    }
    
    // Get all files that need recompilation (including dependents)
    auto& dep_graph = cache.get_dependency_graph();
    auto files_to_recompile = dep_graph.get_files_to_recompile(changed_files);
    
    // Type-check files
    for (const auto& parsed_ir : parsed_irs) {
        if (files_to_recompile.find(parsed_ir.get_file_path()) != files_to_recompile.end()) {
            // Recompile this file
            auto typed_ir = type_check(parsed_ir);
            typed_ir.set_incremental(true);
            results.push_back(typed_ir);
            recompiled_count++;
        } else {
            // Use cached result
            auto cached_ir = get_cached_result(parsed_ir);
            cached_ir.set_incremental(true);
            results.push_back(cached_ir);
            cached_count++;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    total_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    ).count();
    
    return results;
}

ir::TypedOptimizedIR IncrementalTypeChecker::check_file(const ir::ParsedIR& parsed_ir) {
    if (needs_recompilation(parsed_ir)) {
        return type_check(parsed_ir);
    } else {
        return get_cached_result(parsed_ir);
    }
}

bool IncrementalTypeChecker::needs_recompilation(const ir::ParsedIR& parsed_ir) const {
    const auto& file_path = parsed_ir.get_file_path();
    const auto& file_hash = parsed_ir.get_file_hash();
    
    return !cache.has_valid_cache(file_path, file_hash);
}

ir::TypedOptimizedIR IncrementalTypeChecker::get_cached_result(
    const ir::ParsedIR& parsed_ir) const {
    
    auto cached_data = cache.get_cached(parsed_ir.get_file_path());
    if (cached_data.empty()) {
        // Fallback to type checking if cache is invalid
        return type_check(parsed_ir);
    }
    
    try {
        return ir::TypedOptimizedIR::deserialize(cached_data);
    } catch (...) {
        // Fallback to type checking if deserialization fails
        return type_check(parsed_ir);
    }
}

ir::TypedOptimizedIR IncrementalTypeChecker::type_check(const ir::ParsedIR& parsed_ir) {
    ir::TypedOptimizedIR result;
    result.set_file_path(parsed_ir.get_file_path());
    result.set_file_hash(parsed_ir.get_file_hash());
    result.set_timestamp(parsed_ir.get_timestamp());
    
    const auto& ast_root = parsed_ir.get_ast_root();
    if (!ast_root) {
        result.add_error("No AST available", 0, 0);
        return result;
    }
    
    // Build symbol table
    auto symbol_table = build_symbol_table(ast_root);
    
    // Infer types
    infer_types(ast_root, result);
    
    // Perform optimizations
    auto metrics = perform_optimizations(ast_root);
    result.set_metrics(metrics);
    
    // Collect dependencies
    auto deps = collect_dependencies(ast_root);
    for (const auto& dep : deps) {
        result.add_dependency(dep);
    }
    
    // Update dependency graph
    for (const auto& dep : deps) {
        cache.get_dependency_graph().add_dependency(parsed_ir.get_file_path(), dep);
    }
    
    // Cache the result
    auto serialized = result.serialize();
    cache.cache(parsed_ir.get_file_path(), serialized);
    cache.get_file_hasher().update_hash(parsed_ir.get_file_path(), parsed_ir.get_file_hash());
    
    return result;
}

ir::SymbolTable IncrementalTypeChecker::build_symbol_table(
    const std::shared_ptr<ir::ASTNode>& ast_root) {
    
    ir::SymbolTable symbol_table;
    symbol_table.enter_scope();  // Global scope
    
    if (!ast_root) return symbol_table;
    
    // Walk AST and collect declarations
    std::vector<std::shared_ptr<ir::ASTNode>> queue;
    queue.push_back(ast_root);
    
    while (!queue.empty()) {
        auto node = queue.back();
        queue.pop_back();
        
        if (!node) continue;
        
        // Check if this is a declaration
        if (node->node_type == ir::ASTNode::Type::FUNCTION) {
            ir::Symbol symbol;
            symbol.kind = ir::Symbol::Kind::FUNCTION;
            symbol.name = node->name;
            symbol.type = ir::Type::make_primitive("function");
            symbol.definition_line = node->line;
            symbol_table.define(symbol.name, symbol);
        } else if (node->node_type == ir::ASTNode::Type::STRUCT) {
            ir::Symbol symbol;
            symbol.kind = ir::Symbol::Kind::TYPE;
            symbol.name = node->name;
            symbol.type = ir::Type::make_primitive("struct");
            symbol.definition_line = node->line;
            symbol_table.define(symbol.name, symbol);
        } else if (node->node_type == ir::ASTNode::Type::VARIABLE) {
            ir::Symbol symbol;
            symbol.kind = ir::Symbol::Kind::VARIABLE;
            symbol.name = node->name;
            symbol.type = ir::Type::make_primitive("unknown");
            symbol.definition_line = node->line;
            auto it = node->attributes.find("mutable");
            symbol.is_mutable = (it != node->attributes.end() && it->second == "true");
            symbol_table.define(symbol.name, symbol);
        }
        
        // Add children to queue
        for (const auto& child : node->children) {
            queue.push_back(child);
        }
    }
    
    return symbol_table;
}

void IncrementalTypeChecker::infer_types(
    const std::shared_ptr<ir::ASTNode>& node,
    ir::TypedOptimizedIR& result) {
    
    if (!node) return;
    
    // Recursively infer types
    std::vector<std::shared_ptr<ir::ASTNode>> queue;
    queue.push_back(node);
    
    while (!queue.empty()) {
        auto current = queue.back();
        queue.pop_back();
        
        if (!current) continue;
        
        // Infer type based on node type
        std::shared_ptr<ir::Type> inferred_type;
        
        switch (current->node_type) {
            case ir::ASTNode::Type::LITERAL: {
                auto it = current->attributes.find("literal_type");
                if (it != current->attributes.end()) {
                    inferred_type = ir::Type::make_primitive(it->second);
                }
                break;
            }
            case ir::ASTNode::Type::IDENTIFIER: {
                inferred_type = ir::Type::make_primitive("unknown");
                break;
            }
            case ir::ASTNode::Type::BINARY_OP: {
                inferred_type = ir::Type::make_primitive("i32");  // Simplified
                break;
            }
            default:
                inferred_type = ir::Type::make_primitive("void");
                break;
        }
        
        if (inferred_type) {
            result.get_symbol_table().define(current->name, {
                ir::Symbol::Kind::VARIABLE,
                current->name,
                inferred_type,
                false,
                false,
                current->line
            });
        }
        
        // Add children to queue
        for (const auto& child : current->children) {
            queue.push_back(child);
        }
    }
}

ir::OptimizationMetrics IncrementalTypeChecker::perform_optimizations(
    const std::shared_ptr<ir::ASTNode>& ast_root) {
    
    ir::OptimizationMetrics metrics;
    
    if (!ast_root) return metrics;
    
    // Simple optimizations (can be expanded)
    // For now, just count nodes as a placeholder
    std::vector<std::shared_ptr<ir::ASTNode>> queue;
    queue.push_back(ast_root);
    
    while (!queue.empty()) {
        auto node = queue.back();
        queue.pop_back();
        
        if (!node) continue;
        
        // Example: detect dead code
        if (node->node_type == ir::ASTNode::Type::STATEMENT) {
            auto it = node->attributes.find("dead_code");
            if (it != node->attributes.end() && it->second == "true") {
                metrics.dead_code_removed++;
            }
        }
        
        // Example: detect constant folding opportunities
        if (node->node_type == ir::ASTNode::Type::BINARY_OP) {
            auto it = node->attributes.find("constant_folding");
            if (it != node->attributes.end() && it->second == "true") {
                metrics.constants_folded++;
            }
        }
        
        // Add children to queue
        for (const auto& child : node->children) {
            queue.push_back(child);
        }
    }
    
    return metrics;
}

std::vector<std::string> IncrementalTypeChecker::collect_dependencies(
    const std::shared_ptr<ir::ASTNode>& ast_root) {
    
    std::vector<std::string> dependencies;
    
    if (!ast_root) return dependencies;
    
    // Walk AST and collect imports/uses
    std::vector<std::shared_ptr<ir::ASTNode>> queue;
    queue.push_back(ast_root);
    
    while (!queue.empty()) {
        auto node = queue.back();
        queue.pop_back();
        
        if (!node) continue;
        
        // Check for use/import statements
        if (node->node_type == ir::ASTNode::Type::STATEMENT) {
            auto it = node->attributes.find("import");
            if (it != node->attributes.end()) {
                dependencies.push_back(it->second);
            }
        }
        
        // Add children to queue
        for (const auto& child : node->children) {
            queue.push_back(child);
        }
    }
    
    return dependencies;
}

bool IncrementalTypeChecker::types_compatible(
    const std::shared_ptr<ir::Type>& expected,
    const std::shared_ptr<ir::Type>& actual) const {
    
    if (!expected || !actual) return true;  // Unknown types are compatible
    
    return expected->equals(*actual);
}

std::shared_ptr<ir::Type> IncrementalTypeChecker::infer_expression_type(
    const std::shared_ptr<ir::ASTNode>& expr_node,
    const ir::SymbolTable& symbols) {
    
    if (!expr_node) {
        return ir::Type::make_primitive("unknown");
    }
    
    switch (expr_node->node_type) {
        case ir::ASTNode::Type::LITERAL: {
            auto it = expr_node->attributes.find("literal_type");
            if (it != expr_node->attributes.end()) {
                return ir::Type::make_primitive(it->second);
            }
            return ir::Type::make_primitive("unknown");
        }
        case ir::ASTNode::Type::IDENTIFIER: {
            const auto* symbol = symbols.lookup(expr_node->name);
            if (symbol && symbol->type) {
                return symbol->type;
            }
            return ir::Type::make_primitive("unknown");
        }
        case ir::ASTNode::Type::BINARY_OP: {
            return ir::Type::make_primitive("i32");  // Simplified
        }
        default:
            return ir::Type::make_primitive("void");
    }
}

void IncrementalTypeChecker::infer_statement_type(
    const std::shared_ptr<ir::ASTNode>& stmt_node,
    ir::TypedOptimizedIR& result) {
    
    if (!stmt_node) return;
    
    // Type check statements
    switch (stmt_node->node_type) {
        case ir::ASTNode::Type::VARIABLE: {
            auto it = stmt_node->attributes.find("type");
            if (it != stmt_node->attributes.end()) {
                auto type = ir::Type::make_primitive(it->second);
                result.get_symbol_table().define(stmt_node->name, {
                    ir::Symbol::Kind::VARIABLE,
                    stmt_node->name,
                    type,
                    false,
                    false,
                    stmt_node->line
                });
            }
            break;
        }
        default:
            break;
    }
}

} // namespace synq::compiler::pipeline
