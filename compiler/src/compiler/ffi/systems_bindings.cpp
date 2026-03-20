// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 13: Language Interoperability - Systems Language Bindings

#include "language_bindings.h"
#include <sstream>

namespace synq::compiler::ffi {

// ============================================================================
// C Binding Implementation
// ============================================================================

std::string CBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    
    oss << "// Generated C binding for: " << func.name << "\n";
    oss << "#ifndef SYNQ_" << func.name << "_H\n";
    oss << "#define SYNQ_" << func.name << "_H\n\n";
    oss << "#include <stdint.h>\n";
    oss << "#include <stddef.h>\n";
    oss << "#include <stdbool.h>\n\n";
    
    oss << "#ifdef __cplusplus\n";
    oss << "extern \"C\" {\n";
    oss << "#endif\n\n";
    
    // Function declaration
    oss << convert_type(func.return_type) << " synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << convert_type(func.parameter_types[i]) << " arg" << i;
    }
    oss << ");\n\n";
    
    oss << "#ifdef __cplusplus\n";
    oss << "}\n";
    oss << "#endif\n\n";
    oss << "#endif  // SYNQ_" << func.name << "_H\n";
    
    return oss.str();
}

std::string CBinding::generate_struct(const FFIStruct& s) {
    std::ostringstream oss;
    
    oss << "typedef struct {\n";
    for (const auto& field : s.fields) {
        oss << "    " << convert_type(field.second) << " " << field.first << ";\n";
    }
    oss << "} synq_" << s.name << ";\n";
    
    return oss.str();
}

std::string CBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::VOID: return "void";
        case ValueType::BOOL: return "bool";
        case ValueType::I8: return "int8_t";
        case ValueType::I16: return "int16_t";
        case ValueType::I32: return "int32_t";
        case ValueType::I64: return "int64_t";
        case ValueType::U8: return "uint8_t";
        case ValueType::U16: return "uint16_t";
        case ValueType::U32: return "uint32_t";
        case ValueType::U64: return "uint64_t";
        case ValueType::F32: return "float";
        case ValueType::F64: return "double";
        case ValueType::STRING: return "const char*";
        case ValueType::BYTES: return "uint8_t*";
        case ValueType::POINTER: return "void*";
        default: return "void*";
    }
}

std::string CBinding::convert_value(const FFIValue& value) {
    return "/* C value conversion */";
}

// ============================================================================
// C++ Binding Implementation
// ============================================================================

std::string CppBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    
    oss << "// Generated C++ binding for: " << func.name << "\n";
    oss << "#pragma once\n\n";
    oss << "#include <cstdint>\n";
    oss << "#include <string>\n";
    oss << "#include <vector>\n\n";
    
    oss << "namespace synq {\n\n";
    
    // Function declaration
    oss << "extern \"C\" " << convert_type(func.return_type) << " synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << convert_type(func.parameter_types[i]) << " arg" << i;
    }
    oss << ");\n\n";
    
    // C++ wrapper
    oss << "inline " << convert_type(func.return_type) << " " << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << convert_type(func.parameter_types[i]) << " arg" << i;
    }
    oss << ") {\n";
    oss << "    return synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i;
    }
    oss << ");\n";
    oss << "}\n\n";
    
    oss << "}  // namespace synq\n";
    
    return oss.str();
}

std::string CppBinding::generate_struct(const FFIStruct& s) {
    std::ostringstream oss;
    
    oss << "struct " << s.name << " {\n";
    for (const auto& field : s.fields) {
        oss << "    " << convert_type(field.second) << " " << field.first << ";\n";
    }
    oss << "};\n";
    
    return oss.str();
}

std::string CppBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::VOID: return "void";
        case ValueType::BOOL: return "bool";
        case ValueType::I8: return "int8_t";
        case ValueType::I16: return "int16_t";
        case ValueType::I32: return "int32_t";
        case ValueType::I64: return "int64_t";
        case ValueType::U8: return "uint8_t";
        case ValueType::U16: return "uint16_t";
        case ValueType::U32: return "uint32_t";
        case ValueType::U64: return "uint64_t";
        case ValueType::F32: return "float";
        case ValueType::F64: return "double";
        case ValueType::STRING: return "std::string";
        case ValueType::BYTES: return "std::vector<uint8_t>";
        case ValueType::POINTER: return "void*";
        default: return "void*";
    }
}

std::string CppBinding::convert_value(const FFIValue& value) {
    return "/* C++ value conversion */";
}

// ============================================================================
// Rust Binding Implementation
// ============================================================================

std::string RustBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    
    oss << "// Generated Rust binding for: " << func.name << "\n\n";
    oss << "#[link(name = \"synq\")]\n";
    oss << "extern \"C\" {\n";
    oss << "    pub fn synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i << ": " << convert_type(func.parameter_types[i]);
    }
    oss << ") -> " << convert_type(func.return_type) << ";\n";
    oss << "}\n\n";
    
    // Safe wrapper
    oss << "pub unsafe fn " << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i << ": " << convert_type(func.parameter_types[i]);
    }
    oss << ") -> " << convert_type(func.return_type) << " {\n";
    oss << "    synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i;
    }
    oss << ")\n";
    oss << "}\n";
    
    return oss.str();
}

std::string RustBinding::generate_struct(const FFIStruct& s) {
    std::ostringstream oss;
    
    oss << "#[repr(C)]\n";
    oss << "pub struct " << s.name << " {\n";
    for (const auto& field : s.fields) {
        oss << "    pub " << field.first << ": " << convert_type(field.second) << ",\n";
    }
    oss << "}\n";
    
    return oss.str();
}

std::string RustBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::VOID: return "()";
        case ValueType::BOOL: return "bool";
        case ValueType::I8: return "i8";
        case ValueType::I16: return "i16";
        case ValueType::I32: return "i32";
        case ValueType::I64: return "i64";
        case ValueType::U8: return "u8";
        case ValueType::U16: return "u16";
        case ValueType::U32: return "u32";
        case ValueType::U64: return "u64";
        case ValueType::F32: return "f32";
        case ValueType::F64: return "f64";
        case ValueType::STRING: return "*const c_char";
        case ValueType::BYTES: return "*mut u8";
        case ValueType::POINTER: return "*mut c_void";
        default: return "*mut c_void";
    }
}

std::string RustBinding::convert_value(const FFIValue& value) {
    return "/* Rust value conversion */";
}

// ============================================================================
// Go Binding Implementation
// ============================================================================

std::string GoBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    
    oss << "// Generated Go binding for: " << func.name << "\n\n";
    oss << "package synq\n\n";
    oss << "import \"C\"\n\n";
    
    oss << "func " << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i << " " << convert_type(func.parameter_types[i]);
    }
    oss << ") " << convert_type(func.return_type) << " {\n";
    oss << "    return " << convert_type(func.return_type) << "(C.synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "C." << convert_type(func.parameter_types[i]) << "(arg" << i << ")";
    }
    oss << "))\n";
    oss << "}\n";
    
    return oss.str();
}

std::string GoBinding::generate_struct(const FFIStruct& s) {
    std::ostringstream oss;
    
    oss << "type " << s.name << " struct {\n";
    for (const auto& field : s.fields) {
        oss << "    " << field.first << " " << convert_type(field.second) << "\n";
    }
    oss << "}\n";
    
    return oss.str();
}

std::string GoBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::VOID: return "struct{}";
        case ValueType::BOOL: return "bool";
        case ValueType::I8: return "int8";
        case ValueType::I16: return "int16";
        case ValueType::I32: return "int32";
        case ValueType::I64: return "int64";
        case ValueType::U8: return "uint8";
        case ValueType::U16: return "uint16";
        case ValueType::U32: return "uint32";
        case ValueType::U64: return "uint64";
        case ValueType::F32: return "float32";
        case ValueType::F64: return "float64";
        case ValueType::STRING: return "string";
        case ValueType::BYTES: return "[]byte";
        case ValueType::POINTER: return "unsafe.Pointer";
        default: return "unsafe.Pointer";
    }
}

std::string GoBinding::convert_value(const FFIValue& value) {
    return "/* Go value conversion */";
}

// ============================================================================
// Zig Binding Implementation
// ============================================================================

std::string ZigBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    
    oss << "// Generated Zig binding for: " << func.name << "\n\n";
    oss << "pub extern \"c\" fn synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i << ": " << convert_type(func.parameter_types[i]);
    }
    oss << ") " << convert_type(func.return_type) << ";\n\n";
    
    oss << "pub fn " << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i << ": " << convert_type(func.parameter_types[i]);
    }
    oss << ") " << convert_type(func.return_type) << " {\n";
    oss << "    return synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i;
    }
    oss << ");\n";
    oss << "}\n";
    
    return oss.str();
}

std::string ZigBinding::generate_struct(const FFIStruct& s) {
    std::ostringstream oss;
    
    oss << "pub const " << s.name << " = extern struct {\n";
    for (const auto& field : s.fields) {
        oss << "    " << field.first << ": " << convert_type(field.second) << ",\n";
    }
    oss << "};\n";
    
    return oss.str();
}

std::string ZigBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::VOID: return "void";
        case ValueType::BOOL: return "bool";
        case ValueType::I8: return "i8";
        case ValueType::I16: return "i16";
        case ValueType::I32: return "i32";
        case ValueType::I64: return "i64";
        case ValueType::U8: return "u8";
        case ValueType::U16: return "u16";
        case ValueType::U32: return "u32";
        case ValueType::U64: return "u64";
        case ValueType::F32: return "f32";
        case ValueType::F64: return "f64";
        case ValueType::STRING: return "[*:0]const u8";
        case ValueType::BYTES: return "[*]u8";
        case ValueType::POINTER: return "?*anyopaque";
        default: return "?*anyopaque";
    }
}

std::string ZigBinding::convert_value(const FFIValue& value) {
    return "/* Zig value conversion */";
}

// ============================================================================
// D, Nim, V Bindings (Simplified)
// ============================================================================

std::string DBinding::generate_binding(const FFIFunction& func) {
    return "// D binding for " + func.name + "\nextern(C) " + convert_type(func.return_type) + 
           " synq_" + func.name + "(...);";
}

std::string DBinding::generate_struct(const FFIStruct& s) {
    return "extern(C) struct " + s.name + " { }";
}

std::string DBinding::convert_type(const FFIType& type) {
    return "void*";  // Simplified
}

std::string DBinding::convert_value(const FFIValue& value) {
    return "/* D value conversion */";
}

std::string NimBinding::generate_binding(const FFIFunction& func) {
    return "# Nim binding for " + func.name + "\nproc synq_" + func.name + 
           "{.importc, cdecl.}(...)";
}

std::string NimBinding::generate_struct(const FFIStruct& s) {
    return "type " + s.name + " {.importc.} = object";
}

std::string NimBinding::convert_type(const FFIType& type) {
    return "pointer";  // Simplified
}

std::string NimBinding::convert_value(const FFIValue& value) {
    return "/* Nim value conversion */";
}

std::string VBinding::generate_binding(const FFIFunction& func) {
    return "// V binding for " + func.name + "\nfn C.synq_" + func.name + "(...) " + 
           convert_type(func.return_type);
}

std::string VBinding::generate_struct(const FFIStruct& s) {
    return "struct " + s.name + " { }";
}

std::string VBinding::convert_type(const FFIType& type) {
    return "voidptr";  // Simplified
}

std::string VBinding::convert_value(const FFIValue& value) {
    return "/* V value conversion */";
}

// ============================================================================
// FFIBinding Base Implementation
// ============================================================================

FFIBinding::FFIBinding(TargetLanguage language) : language(language) {
}

std::string FFIBinding::get_language_name() const {
    switch (language) {
        case TargetLanguage::C: return "C";
        case TargetLanguage::CPP: return "C++";
        case TargetLanguage::RUST: return "Rust";
        case TargetLanguage::GO: return "Go";
        case TargetLanguage::ZIG: return "Zig";
        case TargetLanguage::D: return "D";
        case TargetLanguage::NIM: return "Nim";
        case TargetLanguage::V: return "V";
        default: return "Unknown";
    }
}

} // namespace synq::compiler::ffi
