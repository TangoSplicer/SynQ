// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Parsed IR Implementation

#include "parsed_ir.h"
#include <algorithm>

namespace synq::compiler::ir {

/**
 * @brief Serialize ASTNode to binary
 */
void ASTNode::write(BinaryWriter& writer) const {
    writer.write_u8(static_cast<uint8_t>(node_type));
    writer.write_u16(line);
    writer.write_u16(column);
    writer.write_string(name);
    
    // Write attributes
    writer.write_u32(static_cast<uint32_t>(attributes.size()));
    for (const auto& [key, value] : attributes) {
        writer.write_string(key);
        writer.write_string(value);
    }
    
    // Write children
    writer.write_u32(static_cast<uint32_t>(children.size()));
    for (const auto& child : children) {
        if (child) {
            child->write(writer);
        }
    }
}

/**
 * @brief Deserialize ASTNode from binary
 */
std::shared_ptr<ASTNode> ASTNode::read(BinaryReader& reader) {
    auto node = std::make_shared<ASTNode>();
    
    node->node_type = static_cast<Type>(reader.read_u8());
    node->line = reader.read_u16();
    node->column = reader.read_u16();
    node->name = reader.read_string();
    
    // Read attributes
    uint32_t attr_count = reader.read_u32();
    for (uint32_t i = 0; i < attr_count; ++i) {
        std::string key = reader.read_string();
        std::string value = reader.read_string();
        node->attributes[key] = value;
    }
    
    // Read children
    uint32_t child_count = reader.read_u32();
    for (uint32_t i = 0; i < child_count; ++i) {
        node->children.push_back(ASTNode::read(reader));
    }
    
    return node;
}

/**
 * @brief Serialize ParsedIR to custom binary format
 */
std::vector<uint8_t> ParsedIR::serialize() const {
    BinaryWriter writer;
    
    // Write header
    BinaryFormatHeader header;
    header.ir_type = BinaryFormatHeader::IRType::PARSED;
    header.file_hash = file_hash;
    header.timestamp = timestamp;
    header.dependencies = dependencies;
    header.write(writer);
    
    // Write tokens
    writer.write_u32(static_cast<uint32_t>(tokens.size()));
    for (const auto& token : tokens) {
        token.write(writer);
    }
    
    // Write AST
    if (ast_root) {
        writer.write_u8(1);  // AST present
        ast_root->write(writer);
    } else {
        writer.write_u8(0);  // No AST
    }
    
    // Write errors
    writer.write_u32(static_cast<uint32_t>(errors.size()));
    for (const auto& [msg, line, col] : errors) {
        writer.write_string(msg);
        writer.write_u16(line);
        writer.write_u16(col);
    }
    
    // Write file metadata
    writer.write_string(file_path);
    
    // Calculate and write checksum
    const auto& buffer = writer.get_buffer();
    uint32_t checksum = crc32(buffer.data(), buffer.size());
    writer.write_u32(checksum);
    
    return writer.get_buffer();
}

/**
 * @brief Deserialize ParsedIR from custom binary format
 */
ParsedIR ParsedIR::deserialize(const std::vector<uint8_t>& data) {
    BinaryReader reader(data);
    ParsedIR ir;
    
    // Read header
    auto header = BinaryFormatHeader::read(reader);
    ir.file_hash = header.file_hash;
    ir.timestamp = header.timestamp;
    ir.dependencies = header.dependencies;
    
    // Read tokens
    uint32_t token_count = reader.read_u32();
    for (uint32_t i = 0; i < token_count; ++i) {
        ir.tokens.push_back(Token::read(reader));
    }
    
    // Read AST
    uint8_t has_ast = reader.read_u8();
    if (has_ast) {
        ir.ast_root = ASTNode::read(reader);
    }
    
    // Read errors
    uint32_t error_count = reader.read_u32();
    for (uint32_t i = 0; i < error_count; ++i) {
        std::string msg = reader.read_string();
        uint16_t line = reader.read_u16();
        uint16_t col = reader.read_u16();
        ir.errors.push_back({msg, line, col});
    }
    
    // Read file metadata
    ir.file_path = reader.read_string();
    
    // Verify checksum
    uint32_t stored_checksum = reader.read_u32();
    // Note: In production, verify checksum here
    
    return ir;
}

/**
 * @brief Get serialized size
 */
size_t ParsedIR::get_size() const {
    return serialize().size();
}

/**
 * @brief Count AST nodes recursively
 */
size_t ParsedIR::count_nodes(const std::shared_ptr<ASTNode>& node) {
    if (!node) return 0;
    size_t count = 1;
    for (const auto& child : node->children) {
        count += count_nodes(child);
    }
    return count;
}

/**
 * @brief Get total AST node count
 */
size_t ParsedIR::ast_node_count() const {
    return count_nodes(ast_root);
}

} // namespace synq::compiler::ir
