// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Typed & Optimized IR

#pragma once

#include "binary_format.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <set>

namespace synq::compiler::ir {

/**
 * @class Type
 * @brief Represents a type in the type system
 */
class Type {
public:
    enum class Kind : uint8_t {
        PRIMITIVE = 0,      // i32, f64, str, bool, etc.
        POINTER = 1,        // *T
        REFERENCE = 2,      // &T
        MUT_REFERENCE = 3,  // &mut T
        ARRAY = 4,          // [T; N]
        SLICE = 5,          // [T]
        FUNCTION = 6,       // fn(A, B) -> C
        STRUCT = 7,         // struct Foo { ... }
        ENUM = 8,           // enum Bar { ... }
        TRAIT = 9,          // trait Baz { ... }
        GENERIC = 10,       // T (type variable)
        UNKNOWN = 255
    };

    Kind kind = Kind::UNKNOWN;
    std::string name;
    std::vector<std::shared_ptr<Type>> type_params;

    /**
     * @brief Get string representation
     */
    std::string to_string() const;

    /**
     * @brief Check if numeric type
     */
    bool is_numeric() const;

    /**
     * @brief Check if reference type
     */
    bool is_reference() const;

    /**
     * @brief Serialize to binary
     */
    void write(BinaryWriter& writer) const;

    /**
     * @brief Deserialize from binary
     */
    static std::shared_ptr<Type> read(BinaryReader& reader);

    /**
     * @brief Create primitive type
     */
    static std::shared_ptr<Type> make_primitive(const std::string& name);

    /**
     * @brief Create pointer type
     */
    static std::shared_ptr<Type> make_pointer(const std::shared_ptr<Type>& pointee);

    /**
     * @brief Create reference type
     */
    static std::shared_ptr<Type> make_reference(const std::shared_ptr<Type>& referenced);

    /**
     * @brief Create function type
     */
    static std::shared_ptr<Type> make_function(
        const std::vector<std::shared_ptr<Type>>& params,
        const std::shared_ptr<Type>& return_type
    );

    /**
     * @brief Check type equality
     */
    bool equals(const Type& other) const;
};

/**
 * @class Symbol
 * @brief Represents a symbol in the symbol table
 */
class Symbol {
public:
    enum class Kind : uint8_t {
        VARIABLE = 0,
        FUNCTION = 1,
        TYPE = 2,
        TRAIT = 3,
        MODULE = 4
    };

    Kind kind;
    std::string name;
    std::shared_ptr<Type> type;
    bool is_mutable = false;
    bool is_public = false;
    uint16_t definition_line = 0;

    /**
     * @brief Serialize to binary
     */
    void write(BinaryWriter& writer) const;

    /**
     * @brief Deserialize from binary
     */
    static Symbol read(BinaryReader& reader);
};

/**
 * @class Scope
 * @brief Represents a scope in the program
 */
class Scope {
public:
    /**
     * @brief Define a symbol
     */
    void define(const std::string& name, const Symbol& symbol) {
        symbols[name] = symbol;
    }

    /**
     * @brief Look up symbol in this scope only
     */
    const Symbol* lookup_local(const std::string& name) const {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            return &it->second;
        }
        return nullptr;
    }

    /**
     * @brief Get all symbols
     */
    const std::unordered_map<std::string, Symbol>& get_symbols() const {
        return symbols;
    }

    /**
     * @brief Serialize to binary
     */
    void write(BinaryWriter& writer) const;

    /**
     * @brief Deserialize from binary
     */
    static Scope read(BinaryReader& reader);

private:
    std::unordered_map<std::string, Symbol> symbols;
};

/**
 * @class SymbolTable
 * @brief Manages scopes and symbol resolution
 */
class SymbolTable {
public:
    /**
     * @brief Enter a new scope
     */
    void enter_scope() {
        scopes.push_back(Scope());
    }

    /**
     * @brief Exit current scope
     */
    void exit_scope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }

    /**
     * @brief Define symbol in current scope
     */
    void define(const std::string& name, const Symbol& symbol) {
        if (!scopes.empty()) {
            scopes.back().define(name, symbol);
        }
    }

    /**
     * @brief Look up symbol (searches all scopes)
     */
    const Symbol* lookup(const std::string& name) const {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            const Symbol* sym = it->lookup_local(name);
            if (sym) return sym;
        }
        return nullptr;
    }

    /**
     * @brief Get current scope depth
     */
    size_t scope_depth() const {
        return scopes.size();
    }

    /**
     * @brief Get all symbols (global scope)
     */
    const std::unordered_map<std::string, Symbol>& get_global_symbols() const {
        if (scopes.empty()) {
            static const std::unordered_map<std::string, Symbol> empty;
            return empty;
        }
        return scopes[0].get_symbols();
    }

    /**
     * @brief Serialize to binary
     */
    void write(BinaryWriter& writer) const;

    /**
     * @brief Deserialize from binary
     */
    static SymbolTable read(BinaryReader& reader);

private:
    std::vector<Scope> scopes;
};

/**
 * @class OptimizationMetrics
 * @brief Tracks optimization statistics
 */
struct OptimizationMetrics {
    uint32_t dead_code_removed = 0;
    uint32_t constants_folded = 0;
    uint32_t redundant_loads_eliminated = 0;
    uint32_t inlined_functions = 0;
    uint32_t loop_optimizations = 0;

    /**
     * @brief Serialize to binary
     */
    void write(BinaryWriter& writer) const {
        writer.write_u32(dead_code_removed);
        writer.write_u32(constants_folded);
        writer.write_u32(redundant_loads_eliminated);
        writer.write_u32(inlined_functions);
        writer.write_u32(loop_optimizations);
    }

    /**
     * @brief Deserialize from binary
     */
    static OptimizationMetrics read(BinaryReader& reader) {
        OptimizationMetrics metrics;
        metrics.dead_code_removed = reader.read_u32();
        metrics.constants_folded = reader.read_u32();
        metrics.redundant_loads_eliminated = reader.read_u32();
        metrics.inlined_functions = reader.read_u32();
        metrics.loop_optimizations = reader.read_u32();
        return metrics;
    }
};

/**
 * @class TypedOptimizedIR
 * @brief Represents the output of combined Type Checking + Optimization
 * 
 * This IR contains:
 * - Type information for all expressions
 * - Symbol table with all defined symbols
 * - Optimization metadata
 * - Type errors (if any)
 * - Incremental compilation metadata
 * 
 * Benefits:
 * - Single cache file instead of two
 * - Persistent symbol table (reuse across builds)
 * - Incremental type checking (only changed files)
 * - 5-10x speedup for single-file changes
 * 
 * This is the output of Stage 2 (Type Checking + Optimization)
 */
class TypedOptimizedIR {
public:
    TypedOptimizedIR() = default;

    /**
     * @brief Serialize to custom binary format
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserialize from custom binary format
     */
    static TypedOptimizedIR deserialize(const std::vector<uint8_t>& data);

    /**
     * @brief Get symbol table
     */
    SymbolTable& get_symbol_table() {
        return symbol_table;
    }

    /**
     * @brief Get symbol table (const)
     */
    const SymbolTable& get_symbol_table() const {
        return symbol_table;
    }

    /**
     * @brief Add type error
     */
    void add_error(const std::string& message, uint16_t line, uint16_t column) {
        errors.push_back({message, line, column});
    }

    /**
     * @brief Get all type errors
     */
    const std::vector<std::tuple<std::string, uint16_t, uint16_t>>& get_errors() const {
        return errors;
    }

    /**
     * @brief Check if type checking had errors
     */
    bool has_errors() const {
        return !errors.empty();
    }

    /**
     * @brief Set file metadata
     */
    void set_file_path(const std::string& path) { file_path = path; }
    void set_file_hash(const std::string& hash) { file_hash = hash; }
    void set_timestamp(uint64_t ts) { timestamp = ts; }
    void add_dependency(const std::string& dep) { dependencies.push_back(dep); }

    /**
     * @brief Get file metadata
     */
    const std::string& get_file_path() const { return file_path; }
    const std::string& get_file_hash() const { return file_hash; }
    uint64_t get_timestamp() const { return timestamp; }
    const std::vector<std::string>& get_dependencies() const { return dependencies; }

    /**
     * @brief Set optimization metrics
     */
    void set_metrics(const OptimizationMetrics& m) { metrics = m; }

    /**
     * @brief Get optimization metrics
     */
    const OptimizationMetrics& get_metrics() const { return metrics; }

    /**
     * @brief Mark as incremental (only changed files recompiled)
     */
    void set_incremental(bool inc) { is_incremental = inc; }

    /**
     * @brief Check if this is an incremental build
     */
    bool get_incremental() const { return is_incremental; }

    /**
     * @brief Get size in bytes (serialized)
     */
    size_t get_size() const;

private:
    SymbolTable symbol_table;
    std::vector<std::tuple<std::string, uint16_t, uint16_t>> errors;
    OptimizationMetrics metrics;
    
    std::string file_path;
    std::string file_hash;
    uint64_t timestamp = 0;
    std::vector<std::string> dependencies;
    bool is_incremental = false;
};

} // namespace synq::compiler::ir
