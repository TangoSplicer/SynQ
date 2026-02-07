// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Abstract Syntax Tree IR

#pragma once

#include "serializable_ir.h"
#include <vector>
#include <string>
#include <memory>

namespace synq::compiler::ir {

// Forward declarations
class ASTNode;
class Expression;
class Statement;
class Declaration;

using ASTNodePtr = std::shared_ptr<ASTNode>;
using ExpressionPtr = std::shared_ptr<Expression>;
using StatementPtr = std::shared_ptr<Statement>;
using DeclarationPtr = std::shared_ptr<Declaration>;

/**
 * @class ASTNode
 * @brief Base class for all AST nodes
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;

    int line = 0;
    int column = 0;

    virtual std::string node_type() const = 0;
    virtual json to_json() const = 0;
    virtual void from_json(const json& data) = 0;
};

/**
 * @class Expression
 * @brief Base class for expression nodes
 */
class Expression : public ASTNode {
public:
    // Expression types
    enum class Type {
        LITERAL,
        IDENTIFIER,
        BINARY_OP,
        UNARY_OP,
        CALL,
        ARRAY,
        STRUCT,
        FIELD_ACCESS,
        INDEX_ACCESS,
        CAST,
        LAMBDA,
        IF_EXPR,
        MATCH_EXPR,
        BLOCK
    };

    Type expr_type;
};

/**
 * @class LiteralExpr
 * @brief Represents a literal value (number, string, boolean)
 */
class LiteralExpr : public Expression {
public:
    enum class LiteralType { INTEGER, FLOAT, STRING, BOOLEAN, NULL_VAL };

    LiteralType literal_type;
    std::string value;

    std::string node_type() const override { return "LiteralExpr"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class IdentifierExpr
 * @brief Represents an identifier (variable or function name)
 */
class IdentifierExpr : public Expression {
public:
    std::string name;

    std::string node_type() const override { return "IdentifierExpr"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class BinaryOpExpr
 * @brief Represents a binary operation (a + b, a * b, etc.)
 */
class BinaryOpExpr : public Expression {
public:
    std::string op;
    ExpressionPtr left;
    ExpressionPtr right;

    std::string node_type() const override { return "BinaryOpExpr"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class UnaryOpExpr
 * @brief Represents a unary operation (!a, -a, etc.)
 */
class UnaryOpExpr : public Expression {
public:
    std::string op;
    ExpressionPtr operand;

    std::string node_type() const override { return "UnaryOpExpr"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class CallExpr
 * @brief Represents a function call
 */
class CallExpr : public Expression {
public:
    ExpressionPtr callee;
    std::vector<ExpressionPtr> arguments;

    std::string node_type() const override { return "CallExpr"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class Statement
 * @brief Base class for statement nodes
 */
class Statement : public ASTNode {
public:
    enum class Type {
        EXPRESSION,
        DECLARATION,
        ASSIGNMENT,
        IF,
        WHILE,
        FOR,
        RETURN,
        BREAK,
        CONTINUE,
        BLOCK
    };

    Type stmt_type;
};

/**
 * @class ExpressionStmt
 * @brief Represents an expression statement
 */
class ExpressionStmt : public Statement {
public:
    ExpressionPtr expression;

    std::string node_type() const override { return "ExpressionStmt"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class IfStmt
 * @brief Represents an if statement
 */
class IfStmt : public Statement {
public:
    ExpressionPtr condition;
    std::vector<StatementPtr> then_branch;
    std::vector<StatementPtr> else_branch;

    std::string node_type() const override { return "IfStmt"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class WhileStmt
 * @brief Represents a while loop
 */
class WhileStmt : public Statement {
public:
    ExpressionPtr condition;
    std::vector<StatementPtr> body;

    std::string node_type() const override { return "WhileStmt"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class Declaration
 * @brief Base class for declarations (functions, structs, etc.)
 */
class Declaration : public ASTNode {
public:
    enum class Type {
        FUNCTION,
        STRUCT,
        ENUM,
        TRAIT,
        IMPL,
        MODULE,
        USE
    };

    Type decl_type;
    std::string name;
    std::string visibility;  // "pub" or ""
};

/**
 * @class FunctionDecl
 * @brief Represents a function declaration
 */
class FunctionDecl : public Declaration {
public:
    struct Parameter {
        std::string name;
        std::string type_name;
        bool is_mutable = false;

        json to_json() const {
            return json{
                {"name", name},
                {"type", type_name},
                {"mutable", is_mutable}
            };
        }
    };

    std::vector<Parameter> parameters;
    std::string return_type;
    std::vector<StatementPtr> body;

    std::string node_type() const override { return "FunctionDecl"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class StructDecl
 * @brief Represents a struct declaration
 */
class StructDecl : public Declaration {
public:
    struct Field {
        std::string name;
        std::string type_name;

        json to_json() const {
            return json{
                {"name", name},
                {"type", type_name}
            };
        }
    };

    std::vector<Field> fields;

    std::string node_type() const override { return "StructDecl"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class Program
 * @brief Represents the entire parsed program (AST root)
 */
class Program : public ASTNode {
public:
    std::vector<DeclarationPtr> declarations;
    std::vector<StatementPtr> statements;

    std::string node_type() const override { return "Program"; }
    json to_json() const override;
    void from_json(const json& data) override;
};

/**
 * @class AST_IR
 * @brief Represents the output of parsing (Abstract Syntax Tree)
 * 
 * This IR contains the complete AST of the source code.
 * It is produced by the parser and serves as input to semantic analysis.
 */
class AST_IR : public SerializableIR {
public:
    AST_IR() = default;

    std::string ir_type() const override { return "AST"; }

    /**
     * @brief Serialize AST to JSON
     */
    json serialize() const override;

    /**
     * @brief Deserialize AST from JSON
     */
    void deserialize(const json& data) override;

    /**
     * @brief Set the program (root of AST)
     */
    void set_program(const std::shared_ptr<Program>& prog) {
        program = prog;
    }

    /**
     * @brief Get the program (root of AST)
     */
    const std::shared_ptr<Program>& get_program() const {
        return program;
    }

    /**
     * @brief Add a syntax error
     */
    void add_error(const std::string& message, int line, int column) {
        errors.push_back({message, line, column});
    }

    /**
     * @brief Get all syntax errors
     */
    const std::vector<std::tuple<std::string, int, int>>& get_errors() const {
        return errors;
    }

    /**
     * @brief Check if parsing had errors
     */
    bool has_errors() const {
        return !errors.empty();
    }

private:
    std::shared_ptr<Program> program;
    std::vector<std::tuple<std::string, int, int>> errors;  // (message, line, column)
};

} // namespace synq::compiler::ir
