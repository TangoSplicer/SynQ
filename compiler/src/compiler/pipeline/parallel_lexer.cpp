// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Parallel Lexer Implementation

#include "parallel_lexer.h"
#include <chrono>
#include <cctype>

namespace synq::compiler::pipeline {

ParallelLexer::ParallelLexer(size_t num_threads)
    : num_threads(std::max(size_t(1), num_threads)) {
    // Create worker threads
    for (size_t i = 0; i < this->num_threads; ++i) {
        workers.emplace_back(&ParallelLexer::worker_loop, this);
    }
}

ParallelLexer::~ParallelLexer() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        shutdown = true;
    }
    work_available.notify_all();
    
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

std::vector<ir::ParsedIR> ParallelLexer::tokenize_files(
    const std::vector<std::pair<std::string, std::string>>& files) {
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Clear previous results
    {
        std::lock_guard<std::mutex> lock(results_mutex);
        results.clear();
        results.resize(files.size());
    }
    
    // Queue all work
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        for (size_t i = 0; i < files.size(); ++i) {
            work_queue.push(files[i]);
        }
    }
    work_available.notify_all();
    
    // Wait for all work to complete
    while (true) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (work_queue.empty()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    total_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    ).count();
    
    std::lock_guard<std::mutex> lock(results_mutex);
    return results;
}

ir::ParsedIR ParallelLexer::tokenize_file(
    const std::string& file_path,
    const std::string& source_code) {
    
    return lex(file_path, source_code);
}

void ParallelLexer::worker_loop() {
    while (true) {
        std::pair<std::string, std::string> work;
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            work_available.wait(lock, [this] { return !work_queue.empty() || shutdown; });
            
            if (shutdown && work_queue.empty()) {
                break;
            }
            
            if (work_queue.empty()) {
                continue;
            }
            
            work = work_queue.front();
            work_queue.pop();
        }
        
        // Perform lexical analysis
        ir::ParsedIR parsed = lex(work.first, work.second);
        
        // Store result
        {
            std::lock_guard<std::mutex> lock(results_mutex);
            results.push_back(parsed);
            total_tokens += parsed.token_count();
        }
    }
}

ir::ParsedIR ParallelLexer::lex(const std::string& file_path, const std::string& source_code) {
    ir::ParsedIR parsed;
    parsed.set_file_path(file_path);
    
    size_t pos = 0;
    int line = 1;
    int column = 1;
    
    while (pos < source_code.length()) {
        char c = source_code[pos];
        
        // Skip whitespace
        if (is_whitespace(c)) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        // Skip comments
        if (c == '/' && pos + 1 < source_code.length() && source_code[pos + 1] == '/') {
            // Line comment
            while (pos < source_code.length() && source_code[pos] != '\n') {
                pos++;
            }
            continue;
        }
        
        if (c == '/' && pos + 1 < source_code.length() && source_code[pos + 1] == '*') {
            // Block comment
            pos += 2;
            while (pos + 1 < source_code.length()) {
                if (source_code[pos] == '*' && source_code[pos + 1] == '/') {
                    pos += 2;
                    break;
                }
                if (source_code[pos] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                pos++;
            }
            continue;
        }
        
        // Tokenize
        ir::Token token;
        
        if (is_identifier_start(c)) {
            token = tokenize_identifier(source_code, pos, line, column);
        } else if (is_digit(c)) {
            token = tokenize_number(source_code, pos, line, column);
        } else if (c == '"') {
            token = tokenize_string(source_code, pos, line, column);
        } else {
            token = tokenize_operator(source_code, pos, line, column);
        }
        
        parsed.add_token(token);
    }
    
    // Add EOF token
    ir::Token eof_token;
    eof_token.type = ir::Token::Type::EOF_TOKEN;
    eof_token.line = line;
    eof_token.column = column;
    parsed.add_token(eof_token);
    
    return parsed;
}

bool ParallelLexer::is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool ParallelLexer::is_identifier_start(char c) {
    return std::isalpha(c) || c == '_';
}

bool ParallelLexer::is_identifier_continue(char c) {
    return std::isalnum(c) || c == '_';
}

bool ParallelLexer::is_digit(char c) {
    return std::isdigit(c);
}

ir::Token::Type ParallelLexer::get_keyword_type(const std::string& word) {
    static const std::unordered_map<std::string, ir::Token::Type> keywords = {
        {"fn", ir::Token::Type::FN},
        {"let", ir::Token::Type::LET},
        {"mut", ir::Token::Type::MUT},
        {"if", ir::Token::Type::IF},
        {"else", ir::Token::Type::ELSE},
        {"while", ir::Token::Type::WHILE},
        {"for", ir::Token::Type::FOR},
        {"return", ir::Token::Type::RETURN},
        {"break", ir::Token::Type::BREAK},
        {"continue", ir::Token::Type::CONTINUE},
        {"match", ir::Token::Type::MATCH},
        {"struct", ir::Token::Type::STRUCT},
        {"enum", ir::Token::Type::ENUM},
        {"trait", ir::Token::Type::TRAIT},
        {"impl", ir::Token::Type::IMPL},
        {"use", ir::Token::Type::USE},
        {"mod", ir::Token::Type::MOD},
        {"pub", ir::Token::Type::PUB},
        {"async", ir::Token::Type::ASYNC},
        {"await", ir::Token::Type::AWAIT},
    };
    
    auto it = keywords.find(word);
    if (it != keywords.end()) {
        return it->second;
    }
    return ir::Token::Type::IDENTIFIER;
}

ir::Token ParallelLexer::tokenize_identifier(
    const std::string& source, size_t& pos, int& line, int& column) {
    
    int start_column = column;
    size_t start_pos = pos;
    
    while (pos < source.length() && is_identifier_continue(source[pos])) {
        column++;
        pos++;
    }
    
    std::string word = source.substr(start_pos, pos - start_pos);
    
    ir::Token token;
    token.type = get_keyword_type(word);
    token.value = word;
    token.line = line;
    token.column = start_column;
    token.length = word.length();
    
    return token;
}

ir::Token ParallelLexer::tokenize_number(
    const std::string& source, size_t& pos, int& line, int& column) {
    
    int start_column = column;
    size_t start_pos = pos;
    
    while (pos < source.length() && is_digit(source[pos])) {
        column++;
        pos++;
    }
    
    // Check for float
    if (pos < source.length() && source[pos] == '.') {
        column++;
        pos++;
        while (pos < source.length() && is_digit(source[pos])) {
            column++;
            pos++;
        }
    }
    
    std::string number = source.substr(start_pos, pos - start_pos);
    
    ir::Token token;
    token.type = number.find('.') != std::string::npos ? 
                 ir::Token::Type::FLOAT : ir::Token::Type::INTEGER;
    token.value = number;
    token.line = line;
    token.column = start_column;
    token.length = number.length();
    
    return token;
}

ir::Token ParallelLexer::tokenize_string(
    const std::string& source, size_t& pos, int& line, int& column) {
    
    int start_column = column;
    column++;
    pos++;  // Skip opening quote
    
    std::string str;
    while (pos < source.length() && source[pos] != '"') {
        if (source[pos] == '\\' && pos + 1 < source.length()) {
            str += source[pos];
            str += source[pos + 1];
            pos += 2;
            column += 2;
        } else {
            str += source[pos];
            if (source[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
    }
    
    if (pos < source.length()) {
        column++;
        pos++;  // Skip closing quote
    }
    
    ir::Token token;
    token.type = ir::Token::Type::STRING;
    token.value = str;
    token.line = line;
    token.column = start_column;
    token.length = str.length() + 2;
    
    return token;
}

ir::Token ParallelLexer::tokenize_operator(
    const std::string& source, size_t& pos, int& line, int& column) {
    
    int start_column = column;
    char c = source[pos];
    
    ir::Token token;
    token.line = line;
    token.column = start_column;
    
    // Two-character operators
    if (pos + 1 < source.length()) {
        std::string two_char = source.substr(pos, 2);
        if (two_char == "==") { token.type = ir::Token::Type::EQ; pos += 2; column += 2; }
        else if (two_char == "!=") { token.type = ir::Token::Type::NE; pos += 2; column += 2; }
        else if (two_char == "<=") { token.type = ir::Token::Type::LE; pos += 2; column += 2; }
        else if (two_char == ">=") { token.type = ir::Token::Type::GE; pos += 2; column += 2; }
        else if (two_char == "&&") { token.type = ir::Token::Type::AND; pos += 2; column += 2; }
        else if (two_char == "||") { token.type = ir::Token::Type::OR; pos += 2; column += 2; }
        else if (two_char == "+=") { token.type = ir::Token::Type::PLUS_ASSIGN; pos += 2; column += 2; }
        else if (two_char == "-=") { token.type = ir::Token::Type::MINUS_ASSIGN; pos += 2; column += 2; }
        else if (two_char == "*=") { token.type = ir::Token::Type::STAR_ASSIGN; pos += 2; column += 2; }
        else if (two_char == "/=") { token.type = ir::Token::Type::SLASH_ASSIGN; pos += 2; column += 2; }
        else if (two_char == "=>") { token.type = ir::Token::Type::FAT_ARROW; pos += 2; column += 2; }
        else if (two_char == "->") { token.type = ir::Token::Type::ARROW; pos += 2; column += 2; }
        else goto single_char;
    } else {
        single_char:
        // Single-character operators
        switch (c) {
            case '+': token.type = ir::Token::Type::PLUS; break;
            case '-': token.type = ir::Token::Type::MINUS; break;
            case '*': token.type = ir::Token::Type::STAR; break;
            case '/': token.type = ir::Token::Type::SLASH; break;
            case '%': token.type = ir::Token::Type::PERCENT; break;
            case '<': token.type = ir::Token::Type::LT; break;
            case '>': token.type = ir::Token::Type::GT; break;
            case '!': token.type = ir::Token::Type::NOT; break;
            case '=': token.type = ir::Token::Type::ASSIGN; break;
            case '(': token.type = ir::Token::Type::LPAREN; break;
            case ')': token.type = ir::Token::Type::RPAREN; break;
            case '{': token.type = ir::Token::Type::LBRACE; break;
            case '}': token.type = ir::Token::Type::RBRACE; break;
            case '[': token.type = ir::Token::Type::LBRACKET; break;
            case ']': token.type = ir::Token::Type::RBRACKET; break;
            case ';': token.type = ir::Token::Type::SEMICOLON; break;
            case ':': token.type = ir::Token::Type::COLON; break;
            case ',': token.type = ir::Token::Type::COMMA; break;
            case '.': token.type = ir::Token::Type::DOT; break;
            case '?': token.type = ir::Token::Type::QUESTION; break;
            default: token.type = ir::Token::Type::UNKNOWN; break;
        }
        pos++;
        column++;
    }
    
    token.value = source.substr(start_column - 1, pos - (start_column - 1));
    token.length = token.value.length();
    
    return token;
}

} // namespace synq::compiler::pipeline
