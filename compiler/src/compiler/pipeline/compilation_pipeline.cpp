// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Compilation Pipeline Implementation

#include "compilation_pipeline.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace synq::compiler::pipeline {

void CompilationStats::print() const {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         Phase 11: Compilation Pipeline Statistics          ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "📊 Timing:\n";
    std::cout << "  Total:       " << std::setw(6) << total_time_ms << " ms\n";
    std::cout << "  Lexing:      " << std::setw(6) << lex_time_ms << " ms\n";
    std::cout << "  Type Check:  " << std::setw(6) << type_check_time_ms << " ms\n";
    std::cout << "  Codegen:     " << std::setw(6) << codegen_time_ms << " ms\n\n";
    
    std::cout << "📁 Files:\n";
    std::cout << "  Processed:   " << std::setw(6) << files_processed << "\n";
    std::cout << "  Cached:      " << std::setw(6) << files_cached << "\n";
    std::cout << "  Recompiled:  " << std::setw(6) << files_recompiled << "\n";
    std::cout << "  Cache Hit:   " << std::setw(5) << std::fixed << std::setprecision(1) 
              << cache_hit_rate() << "%\n\n";
    
    std::cout << "🔧 Compilation:\n";
    std::cout << "  Tokens:      " << std::setw(6) << tokens_generated << "\n";
    std::cout << "  AST Nodes:   " << std::setw(6) << ast_nodes_generated << "\n";
    std::cout << "  Symbols:     " << std::setw(6) << symbols_defined << "\n\n";
}

CompilationPipeline::CompilationPipeline(const std::string& cache_dir)
    : cache(cache_dir) {
}

CompilationOutput CompilationPipeline::compile(const CompilationInput& input) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    CompilationStats stats;
    stats.files_processed = input.files.size();
    
    // Stage 1: Lexing + Parsing
    auto lex_output = stage_lex_parse(input, stats);
    if (!lex_output.success) {
        return lex_output;
    }
    
    // Stage 2: Type Checking + Optimization
    auto type_output = stage_type_check(lex_output, stats);
    if (!type_output.success) {
        return type_output;
    }
    
    // Stage 3: Code Generation
    auto codegen_output = stage_codegen(type_output, stats);
    if (!codegen_output.success) {
        return codegen_output;
    }
    
    // Merge outputs
    auto final_output = merge_outputs(lex_output, type_output, codegen_output);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    stats.total_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    ).count();
    
    final_output.stats = stats;
    last_stats = stats;
    
    // Save cache metadata
    cache.save_metadata();
    
    return final_output;
}

CompilationOutput CompilationPipeline::stage_lex_parse(
    const CompilationInput& input,
    CompilationStats& stats) {
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    CompilationOutput output;
    
    // Create parallel lexer
    ParallelLexer lexer(input.parallel ? input.num_threads : 1);
    
    // Tokenize all files in parallel
    auto parsed_irs = lexer.tokenize_files(input.files);
    
    output.parsed_irs = parsed_irs;
    stats.tokens_generated = lexer.get_total_tokens();
    
    // Count AST nodes
    for (const auto& parsed_ir : parsed_irs) {
        stats.ast_nodes_generated += parsed_ir.ast_node_count();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    stats.lex_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    ).count();
    
    output.success = true;
    return output;
}

CompilationOutput CompilationPipeline::stage_type_check(
    const CompilationOutput& lex_output,
    CompilationStats& stats) {
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    CompilationOutput output;
    
    // Create incremental type checker
    IncrementalTypeChecker checker(cache);
    
    // Type check all files
    auto typed_irs = checker.check_files(lex_output.parsed_irs);
    
    output.typed_irs = typed_irs;
    stats.files_recompiled = checker.get_recompiled_count();
    stats.files_cached = checker.get_cached_count();
    
    // Count symbols
    for (const auto& typed_ir : typed_irs) {
        const auto& symbols = typed_ir.get_symbol_table().get_global_symbols();
        stats.symbols_defined += symbols.size();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    stats.type_check_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    ).count();
    
    output.success = true;
    return output;
}

CompilationOutput CompilationPipeline::stage_codegen(
    const CompilationOutput& type_output,
    CompilationStats& stats) {
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    CompilationOutput output;
    
    // Generate LLVM IR for each typed IR
    for (const auto& typed_ir : type_output.typed_irs) {
        auto llvm_ir = generate_llvm_ir(typed_ir);
        output.llvm_irs.push_back(llvm_ir);
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    stats.codegen_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    ).count();
    
    output.success = true;
    return output;
}

ir::LLVM_IR CompilationPipeline::generate_llvm_ir(const ir::TypedOptimizedIR& typed_ir) {
    ir::LLVM_IR llvm_ir;
    llvm_ir.set_file_path(typed_ir.get_file_path());
    llvm_ir.set_file_hash(typed_ir.get_file_hash());
    llvm_ir.set_timestamp(typed_ir.get_timestamp());
    
    // Set target information
    llvm_ir.set_target_triple("x86_64-unknown-linux-gnu");
    llvm_ir.set_target_cpu("generic");
    llvm_ir.set_data_layout("e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128");
    llvm_ir.set_optimization_level(2);
    
    // Generate minimal LLVM IR
    std::string ir_code;
    ir_code += "; ModuleID = '" + typed_ir.get_file_path() + "'\n";
    ir_code += "target triple = \"x86_64-unknown-linux-gnu\"\n";
    ir_code += "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n\n";
    
    // Add function declarations from symbol table
    const auto& symbols = typed_ir.get_symbol_table().get_global_symbols();
    for (const auto& [name, symbol] : symbols) {
        if (symbol.kind == ir::Symbol::Kind::FUNCTION) {
            ir_code += "declare void @" + name + "()\n";
        }
    }
    
    ir_code += "\ndefine i32 @main() {\n";
    ir_code += "  ret i32 0\n";
    ir_code += "}\n";
    
    llvm_ir.set_ir_code(ir_code);
    
    return llvm_ir;
}

bool CompilationPipeline::validate_output(const CompilationOutput& output) {
    // Basic validation
    if (output.parsed_irs.empty() && output.typed_irs.empty()) {
        return false;
    }
    
    // Check for errors
    for (const auto& parsed_ir : output.parsed_irs) {
        if (parsed_ir.has_errors()) {
            return false;
        }
    }
    
    for (const auto& typed_ir : output.typed_irs) {
        if (typed_ir.has_errors()) {
            return false;
        }
    }
    
    return true;
}

CompilationOutput CompilationPipeline::merge_outputs(
    const CompilationOutput& stage1,
    const CompilationOutput& stage2,
    const CompilationOutput& stage3) {
    
    CompilationOutput merged;
    merged.parsed_irs = stage1.parsed_irs;
    merged.typed_irs = stage2.typed_irs;
    merged.llvm_irs = stage3.llvm_irs;
    merged.success = stage1.success && stage2.success && stage3.success;
    
    // Merge errors
    merged.errors.insert(merged.errors.end(), stage1.errors.begin(), stage1.errors.end());
    merged.errors.insert(merged.errors.end(), stage2.errors.begin(), stage2.errors.end());
    merged.errors.insert(merged.errors.end(), stage3.errors.begin(), stage3.errors.end());
    
    return merged;
}

} // namespace synq::compiler::pipeline
