// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Typed AST IR

#pragma once

#include "ast_ir.h"
#include <unordered_map>
#include <set>

namespace synq::compiler::ir {

/**
 * @class Type
 * @brief Represents a type in the type system
 */
class Type {
public:
    enum class Kind {
        PRIMITIVE,      // i32, f64, str, bool, etc.
        POINTER,        // *T
        REFERENCE,      // &T
        MUT_REFERENCE,  // &mut T
        ARRAY,          // [T; N]
        SLICE,          // [T]
        FUNCTION,       // fn(A, B) -> C
        STRUCT,         // struct Foo { ... }
        ENUM,           // enum Bar { ... }
        TRAIT,          // trait Baz { ... }
        GENERIC,        // T (type variable)
        UNKNOWN
    };

    Kind kind = Kind::UNKNOWN;
    std::string name;
    std::vector<std::shared_ptr<Type>> type_params;  // For generics, function params, etc.

    /**
     * @brief Get string representation of type
     */
    std::string to_string() const;

    /**
     * @brief Check if type is numeric
     */
    bool is_numeric() const;

    /**
     * @brief Check if type is reference
     */
    bool is_reference() const;

    /**
     * @brief Serialize type to JSON
     */
    json to_json() const;

    /**
     * @brief Deserialize type from JSON
     */
    static std::shared_ptr<Type> from_json(const json& data);

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
};

/**
 * @class Symbol
 * @brief Represents a symbol in the symbol table
 */
class Symbol {
public:
    enum class Kind {
        VARIABLE,
        FUNCTION,
        TYPE,
        TRAIT,
        MODULE
    };

    Kind kind;
    std::string name;
    std::shared_ptr<Type> type;
    bool is_mutable = false;
    bool is_public = false;
    int definition_line = 0;

    /**
     * @brief Serialize symbol to JSON
     */
    json to_json() const;

    /**
     * @brief Deserialize symbol from JSON
     */
    static Symbol from_json(const json& data);
};

/**
 * @class Scope
 * @brief Represents a scope in the program
 */
class Scope {
public:
    /**
     * @brief Define a symbol in this scope
     */
    void define(const std::string& name, const Symbol& symbol) {
        symbols[name] = symbol;
    }

    /**
     * @brief Look up a symbol in this scope (not parent scopes)
     */
    const Symbol* lookup_local(const std::string& name) const {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            return &it->second;
        }
        return nullptr;
    }

    /**
     * @brief Get all symbols in this scope
     */
    const std::unordered_map<std::string, Symbol>& get_symbols() const {
        return symbols;
    }

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
     * @brief Define a symbol in current scope
     */
    void define(const std::string& name, const Symbol& symbol) {
        if (!scopes.empty()) {
            scopes.back().define(name, symbol);
        }
    }

    /**
     * @brief Look up a symbol (searches current scope and parents)
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

private:
    std::vector<Scope> scopes;
};

/**
 * @class TypedAST_IR
 * @brief Represents the output of semantic analysis (Type-checked AST)
 * 
 * This IR contains:
 * - The AST with type information attached to nodes
 * - Symbol table with all defined symbols
 * - Type information for all expressions
 * - Semantic errors (if any)
 * 
 * It is produced by semantic analysis and serves as input to optimization.
 */
class TypedAST_IR : public SerializableIR {
public:
    TypedAST_IR() = default;

    std::string ir_type() const override { return "TypedAST"; }

    /**
     * @brief Serialize typed AST to JSON
     */
    json serialize() const override;

    /**
     * @brief Deserialize typed AST from JSON
     */
    void deserialize(const json& data) override;

    /**
     * @brief Set the program
     */
    void set_program(const std::shared_ptr<Program>& prog) {
        program = prog;
    }

    /**
     * @brief Get the program
     */
    const std::shared_ptr<Program>& get_program() const {
        return program;
    }

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
     * @brief Attach type to an AST node
     */
    void attach_type(const ASTNode* node, const std::shared_ptr<Type>& type) {
        node_types[reinterpret_cast<uintptr_t>(node)] = type;
    }

    /**
     * @brief Get type of an AST node
     */
    std::shared_ptr<Type> get_type(const ASTNode* node) const {
        auto it = node_types.find(reinterpret_cast<uintptr_t>(node));
        if (it != node_types.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
     * @brief Add a semantic error
     */
    void add_error(const std::string& message, int line, int column) {
        errors.push_back({message, line, column});
    }

    /**
     * @brief Get all semantic errors
     */
    const std::vector<std::tuple<std::string, int, int>>& get_errors() const {
        return errors;
    }

    /**
     * @brief Check if semantic analysis had errors
     */
    bool has_errors() const {
        return !errors.empty();
    }

private:
    std::shared_ptr<Program> program;
    SymbolTable symbol_table;
    std::unordered_map<uintptr_t, std::shared_ptr<Type>> node_types;
    std::vector<std::tuple<std::string, int, int>> errors;  // (message, line, column)
};

} // namespace synq::compiler::ir
