// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Token Stream IR

#pragma once

#include "serializable_ir.h"
#include <vector>
#include <string>

namespace synq::compiler::ir {

/**
 * @class Token
 * @brief Represents a single token from lexical analysis
 */
struct Token {
    enum class Type {
        // Literals
        INTEGER,
        FLOAT,
        STRING,
        IDENTIFIER,
        
        // Keywords
        FN,
        LET,
        MUT,
        IF,
        ELSE,
        WHILE,
        FOR,
        RETURN,
        BREAK,
        CONTINUE,
        MATCH,
        STRUCT,
        ENUM,
        TRAIT,
        IMPL,
        USE,
        MOD,
        PUB,
        ASYNC,
        AWAIT,
        
        // Operators
        PLUS,
        MINUS,
        STAR,
        SLASH,
        PERCENT,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        AND,
        OR,
        NOT,
        ASSIGN,
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        STAR_ASSIGN,
        SLASH_ASSIGN,
        
        // Delimiters
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        SEMICOLON,
        COLON,
        COMMA,
        DOT,
        ARROW,
        FAT_ARROW,
        QUESTION,
        
        // Special
        EOF_TOKEN,
        UNKNOWN
    };

    Type type;
    std::string value;
    int line;
    int column;
    int length;

    /**
     * @brief Convert token type to string for debugging
     */
    std::string type_to_string() const;

    /**
     * @brief Serialize token to JSON
     */
    json to_json() const {
        return json{
            {"type", type_to_string()},
            {"value", value},
            {"line", line},
            {"column", column},
            {"length", length}
        };
    }

    /**
     * @brief Deserialize token from JSON
     */
    static Token from_json(const json& data);
};

/**
 * @class TokenStream
 * @brief Represents the output of lexical analysis (tokenization)
 * 
 * This IR contains all tokens extracted from the source code.
 * It is the first stage of compilation and serves as input to the parser.
 */
class TokenStream : public SerializableIR {
public:
    TokenStream() = default;
    explicit TokenStream(const std::string& source_code);

    std::string ir_type() const override { return "TokenStream"; }

    /**
     * @brief Serialize token stream to JSON
     */
    json serialize() const override;

    /**
     * @brief Deserialize token stream from JSON
     */
    void deserialize(const json& data) override;

    /**
     * @brief Add a token to the stream
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
     * @brief Get token at index
     */
    const Token& get_token(size_t index) const {
        return tokens[index];
    }

    /**
     * @brief Get number of tokens
     */
    size_t token_count() const {
        return tokens.size();
    }

    /**
     * @brief Check if stream is empty
     */
    bool is_empty() const {
        return tokens.empty();
    }

    /**
     * @brief Get source code that was tokenized
     */
    const std::string& get_source_code() const {
        return source_code;
    }

    /**
     * @brief Perform lexical analysis on source code
     */
    void tokenize(const std::string& source);

private:
    std::vector<Token> tokens;
    std::string source_code;

    /**
     * @brief Helper to tokenize identifiers and keywords
     */
    Token tokenize_identifier(const std::string& source, size_t& pos, int& line, int& column);

    /**
     * @brief Helper to tokenize numbers
     */
    Token tokenize_number(const std::string& source, size_t& pos, int& line, int& column);

    /**
     * @brief Helper to tokenize strings
     */
    Token tokenize_string(const std::string& source, size_t& pos, int& line, int& column);

    /**
     * @brief Helper to tokenize operators and delimiters
     */
    Token tokenize_operator(const std::string& source, size_t& pos, int& line, int& column);

    /**
     * @brief Check if character is whitespace
     */
    static bool is_whitespace(char c);

    /**
     * @brief Check if character is identifier start
     */
    static bool is_identifier_start(char c);

    /**
     * @brief Check if character is identifier continuation
     */
    static bool is_identifier_continue(char c);

    /**
     * @brief Check if character is digit
     */
    static bool is_digit(char c);

    /**
     * @brief Get keyword type from string
     */
    static Token::Type get_keyword_type(const std::string& word);
};

} // namespace synq::compiler::ir
