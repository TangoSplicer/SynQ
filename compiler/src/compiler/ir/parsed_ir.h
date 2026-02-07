// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Parsed IR (Combined Lex+Parse)

#pragma once

#include "binary_format.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace synq::compiler::ir {

/**
 * @class Token
 * @brief Represents a single token from lexical analysis
 */
struct Token {
    enum class Type : uint16_t {
        // Literals
        INTEGER = 0,
        FLOAT = 1,
        STRING = 2,
        IDENTIFIER = 3,
        
        // Keywords
        FN = 10,
        LET = 11,
        MUT = 12,
        IF = 13,
        ELSE = 14,
        WHILE = 15,
        FOR = 16,
        RETURN = 17,
        BREAK = 18,
        CONTINUE = 19,
        MATCH = 20,
        STRUCT = 21,
        ENUM = 22,
        TRAIT = 23,
        IMPL = 24,
        USE = 25,
        MOD = 26,
        PUB = 27,
        ASYNC = 28,
        AWAIT = 29,
        
        // Operators
        PLUS = 40,
        MINUS = 41,
        STAR = 42,
        SLASH = 43,
        PERCENT = 44,
        EQ = 45,
        NE = 46,
        LT = 47,
        GT = 48,
        LE = 49,
        GE = 50,
        AND = 51,
        OR = 52,
        NOT = 53,
        ASSIGN = 54,
        PLUS_ASSIGN = 55,
        MINUS_ASSIGN = 56,
        STAR_ASSIGN = 57,
        SLASH_ASSIGN = 58,
        
        // Delimiters
        LPAREN = 70,
        RPAREN = 71,
        LBRACE = 72,
        RBRACE = 73,
        LBRACKET = 74,
        RBRACKET = 75,
        SEMICOLON = 76,
        COLON = 77,
        COMMA = 78,
        DOT = 79,
        ARROW = 80,
        FAT_ARROW = 81,
        QUESTION = 82,
        
        // Special
        EOF_TOKEN = 255,
        UNKNOWN = 254
    };

    Type type;
    std::string value;
    uint16_t line;
    uint16_t column;
    uint16_t length;

    /**
     * @brief Serialize token to binary
     */
    void write(BinaryWriter& writer) const {
        writer.write_u16(static_cast<uint16_t>(type));
        writer.write_string(value);
        writer.write_u16(line);
        writer.write_u16(column);
        writer.write_u16(length);
    }

    /**
     * @brief Deserialize token from binary
     */
    static Token read(BinaryReader& reader) {
        Token token;
        token.type = static_cast<Type>(reader.read_u16());
        token.value = reader.read_string();
        token.line = reader.read_u16();
        token.column = reader.read_u16();
        token.length = reader.read_u16();
        return token;
    }
};

/**
 * @class ASTNode
 * @brief Base class for AST nodes (simplified for ParsedIR)
 */
struct ASTNode {
    enum class Type : uint8_t {
        PROGRAM = 0,
        FUNCTION = 1,
        STRUCT = 2,
        ENUM = 3,
        VARIABLE = 4,
        EXPRESSION = 5,
        STATEMENT = 6,
        BLOCK = 7,
        IF_STMT = 8,
        WHILE_STMT = 9,
        FOR_STMT = 10,
        RETURN_STMT = 11,
        CALL_EXPR = 12,
        BINARY_OP = 13,
        UNARY_OP = 14,
        LITERAL = 15,
        IDENTIFIER = 16,
        UNKNOWN = 255
    };

    Type node_type;
    uint16_t line;
    uint16_t column;
    std::string name;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::unordered_map<std::string, std::string> attributes;  // For storing metadata

    /**
     * @brief Serialize node to binary (simplified, recursive)
     */
    void write(BinaryWriter& writer) const;

    /**
     * @brief Deserialize node from binary
     */
    static std::shared_ptr<ASTNode> read(BinaryReader& reader);
};

/**
 * @class ParsedIR
 * @brief Represents the output of combined Lexing + Parsing
 * 
 * This IR contains:
 * - All tokens from lexical analysis
 * - Complete AST from parsing
 * - Syntax errors (if any)
 * - Metadata (file hash, dependencies, timestamp)
 * 
 * Benefits:
 * - Single cache file instead of two
 * - Faster I/O (binary format)
 * - Smaller size (5-10x vs JSON)
 * - Enables parallel lexing of multiple files
 * 
 * This is the output of Stage 1 (Lexing + Parsing)
 */
class ParsedIR {
public:
    ParsedIR() = default;

    /**
     * @brief Serialize to custom binary format
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserialize from custom binary format
     */
    static ParsedIR deserialize(const std::vector<uint8_t>& data);

    /**
     * @brief Add a token
     */
    void add_token(const Token& token) {
        tokens.push_back(token);
    }

    /**
     * @brief Get all tokens
     */
    const std::vector<Token>& get_tokens() const {
        return tokens;
    }

    /**
     * @brief Set the AST root
     */
    void set_ast_root(const std::shared_ptr<ASTNode>& root) {
        ast_root = root;
    }

    /**
     * @brief Get the AST root
     */
    const std::shared_ptr<ASTNode>& get_ast_root() const {
        return ast_root;
    }

    /**
     * @brief Add a syntax error
     */
    void add_error(const std::string& message, uint16_t line, uint16_t column) {
        errors.push_back({message, line, column});
    }

    /**
     * @brief Get all syntax errors
     */
    const std::vector<std::tuple<std::string, uint16_t, uint16_t>>& get_errors() const {
        return errors;
    }

    /**
     * @brief Check if parsing had errors
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
     * @brief Get size in bytes (serialized)
     */
    size_t get_size() const;

    /**
     * @brief Get token count
     */
    size_t token_count() const { return tokens.size(); }

    /**
     * @brief Get AST node count (recursive)
     */
    size_t ast_node_count() const;

private:
    std::vector<Token> tokens;
    std::shared_ptr<ASTNode> ast_root;
    std::vector<std::tuple<std::string, uint16_t, uint16_t>> errors;
    
    std::string file_path;
    std::string file_hash;
    uint64_t timestamp = 0;
    std::vector<std::string> dependencies;

    /**
     * @brief Helper to count AST nodes recursively
     */
    static size_t count_nodes(const std::shared_ptr<ASTNode>& node);
};

} // namespace synq::compiler::ir
