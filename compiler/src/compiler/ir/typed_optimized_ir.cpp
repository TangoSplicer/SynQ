// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Typed & Optimized IR Implementation

#include "typed_optimized_ir.h"
#include <algorithm>

namespace synq::compiler::ir {

// ============================================================================
// Type Implementation
// ============================================================================

std::string Type::to_string() const {
    switch (kind) {
        case Kind::PRIMITIVE: return name;
        case Kind::POINTER: return "*" + (type_params.empty() ? "void" : type_params[0]->to_string());
        case Kind::REFERENCE: return "&" + (type_params.empty() ? "void" : type_params[0]->to_string());
        case Kind::MUT_REFERENCE: return "&mut " + (type_params.empty() ? "void" : type_params[0]->to_string());
        case Kind::ARRAY: {
            if (type_params.size() >= 2) {
                return "[" + type_params[0]->to_string() + "; " + type_params[1]->to_string() + "]";
            }
            return "[]";
        }
        case Kind::SLICE: return "[" + (type_params.empty() ? "void" : type_params[0]->to_string()) + "]";
        case Kind::FUNCTION: {
            std::string result = "fn(";
            for (size_t i = 0; i < type_params.size() - 1; ++i) {
                if (i > 0) result += ", ";
                result += type_params[i]->to_string();
            }
            result += ") -> ";
            if (!type_params.empty()) {
                result += type_params.back()->to_string();
            }
            return result;
        }
        case Kind::STRUCT: return "struct " + name;
        case Kind::ENUM: return "enum " + name;
        case Kind::TRAIT: return "trait " + name;
        case Kind::GENERIC: return name;
        case Kind::UNKNOWN: return "unknown";
    }
    return "unknown";
}

bool Type::is_numeric() const {
    if (kind != Kind::PRIMITIVE) return false;
    return name == "i32" || name == "i64" || name == "f32" || name == "f64" ||
           name == "u32" || name == "u64" || name == "u8" || name == "u16";
}

bool Type::is_reference() const {
    return kind == Kind::REFERENCE || kind == Kind::MUT_REFERENCE;
}

void Type::write(BinaryWriter& writer) const {
    writer.write_u8(static_cast<uint8_t>(kind));
    writer.write_string(name);
    writer.write_u32(static_cast<uint32_t>(type_params.size()));
    for (const auto& param : type_params) {
        if (param) {
            param->write(writer);
        }
    }
}

std::shared_ptr<Type> Type::read(BinaryReader& reader) {
    auto type = std::make_shared<Type>();
    type->kind = static_cast<Kind>(reader.read_u8());
    type->name = reader.read_string();
    uint32_t param_count = reader.read_u32();
    for (uint32_t i = 0; i < param_count; ++i) {
        type->type_params.push_back(Type::read(reader));
    }
    return type;
}

std::shared_ptr<Type> Type::make_primitive(const std::string& name) {
    auto type = std::make_shared<Type>();
    type->kind = Kind::PRIMITIVE;
    type->name = name;
    return type;
}

std::shared_ptr<Type> Type::make_pointer(const std::shared_ptr<Type>& pointee) {
    auto type = std::make_shared<Type>();
    type->kind = Kind::POINTER;
    type->type_params.push_back(pointee);
    return type;
}

std::shared_ptr<Type> Type::make_reference(const std::shared_ptr<Type>& referenced) {
    auto type = std::make_shared<Type>();
    type->kind = Kind::REFERENCE;
    type->type_params.push_back(referenced);
    return type;
}

std::shared_ptr<Type> Type::make_function(
    const std::vector<std::shared_ptr<Type>>& params,
    const std::shared_ptr<Type>& return_type) {
    auto type = std::make_shared<Type>();
    type->kind = Kind::FUNCTION;
    type->type_params = params;
    type->type_params.push_back(return_type);
    return type;
}

bool Type::equals(const Type& other) const {
    if (kind != other.kind || name != other.name) return false;
    if (type_params.size() != other.type_params.size()) return false;
    for (size_t i = 0; i < type_params.size(); ++i) {
        if (!type_params[i] || !other.type_params[i]) return false;
        if (!type_params[i]->equals(*other.type_params[i])) return false;
    }
    return true;
}

// ============================================================================
// Symbol Implementation
// ============================================================================

void Symbol::write(BinaryWriter& writer) const {
    writer.write_u8(static_cast<uint8_t>(kind));
    writer.write_string(name);
    if (type) {
        writer.write_u8(1);
        type->write(writer);
    } else {
        writer.write_u8(0);
    }
    writer.write_u8(is_mutable ? 1 : 0);
    writer.write_u8(is_public ? 1 : 0);
    writer.write_u16(definition_line);
}

Symbol Symbol::read(BinaryReader& reader) {
    Symbol symbol;
    symbol.kind = static_cast<Kind>(reader.read_u8());
    symbol.name = reader.read_string();
    uint8_t has_type = reader.read_u8();
    if (has_type) {
        symbol.type = Type::read(reader);
    }
    symbol.is_mutable = reader.read_u8() != 0;
    symbol.is_public = reader.read_u8() != 0;
    symbol.definition_line = reader.read_u16();
    return symbol;
}

// ============================================================================
// Scope Implementation
// ============================================================================

void Scope::write(BinaryWriter& writer) const {
    writer.write_u32(static_cast<uint32_t>(symbols.size()));
    for (const auto& [name, symbol] : symbols) {
        writer.write_string(name);
        symbol.write(writer);
    }
}

Scope Scope::read(BinaryReader& reader) {
    Scope scope;
    uint32_t count = reader.read_u32();
    for (uint32_t i = 0; i < count; ++i) {
        std::string name = reader.read_string();
        Symbol symbol = Symbol::read(reader);
        scope.define(name, symbol);
    }
    return scope;
}

// ============================================================================
// SymbolTable Implementation
// ============================================================================

void SymbolTable::write(BinaryWriter& writer) const {
    writer.write_u32(static_cast<uint32_t>(scopes.size()));
    for (const auto& scope : scopes) {
        scope.write(writer);
    }
}

SymbolTable SymbolTable::read(BinaryReader& reader) {
    SymbolTable table;
    uint32_t scope_count = reader.read_u32();
    for (uint32_t i = 0; i < scope_count; ++i) {
        table.scopes.push_back(Scope::read(reader));
    }
    return table;
}

// ============================================================================
// TypedOptimizedIR Implementation
// ============================================================================

std::vector<uint8_t> TypedOptimizedIR::serialize() const {
    BinaryWriter writer;
    
    // Write header
    BinaryFormatHeader header;
    header.ir_type = BinaryFormatHeader::IRType::TYPED_OPTIMIZED;
    header.file_hash = file_hash;
    header.timestamp = timestamp;
    header.dependencies = dependencies;
    if (is_incremental) {
        header.flags |= static_cast<uint32_t>(BinaryFormatHeader::Flags::INCREMENTAL);
    }
    header.write(writer);
    
    // Write symbol table
    symbol_table.write(writer);
    
    // Write optimization metrics
    metrics.write(writer);
    
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

TypedOptimizedIR TypedOptimizedIR::deserialize(const std::vector<uint8_t>& data) {
    BinaryReader reader(data);
    TypedOptimizedIR ir;
    
    // Read header
    auto header = BinaryFormatHeader::read(reader);
    ir.file_hash = header.file_hash;
    ir.timestamp = header.timestamp;
    ir.dependencies = header.dependencies;
    ir.is_incremental = (header.flags & static_cast<uint32_t>(BinaryFormatHeader::Flags::INCREMENTAL)) != 0;
    
    // Read symbol table
    ir.symbol_table = SymbolTable::read(reader);
    
    // Read optimization metrics
    ir.metrics = OptimizationMetrics::read(reader);
    
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

size_t TypedOptimizedIR::get_size() const {
    return serialize().size();
}

} // namespace synq::compiler::ir
