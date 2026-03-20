// Phase 13 Stage 4: Dynamic Language Bindings
#include "language_bindings.h"
#include <sstream>

namespace synq::compiler::ffi {

// Python Binding
std::string PythonBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    oss << "import ctypes\n\n";
    oss << "synq_lib = ctypes.CDLL('libsynq.so')\n\n";
    oss << "def " << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i;
    }
    oss << "):\n";
    oss << "    synq_lib.synq_" << func.name << ".argtypes = [";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << convert_type(func.parameter_types[i]);
    }
    oss << "]\n";
    oss << "    synq_lib.synq_" << func.name << ".restype = " << convert_type(func.return_type) << "\n";
    oss << "    return synq_lib.synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i;
    }
    oss << ")\n";
    return oss.str();
}

std::string PythonBinding::generate_struct(const FFIStruct& s) {
    std::ostringstream oss;
    oss << "class " << s.name << "(ctypes.Structure):\n";
    oss << "    _fields_ = [\n";
    for (const auto& field : s.fields) {
        oss << "        ('" << field.first << "', " << convert_type(field.second) << "),\n";
    }
    oss << "    ]\n";
    return oss.str();
}

std::string PythonBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::BOOL: return "ctypes.c_bool";
        case ValueType::I32: return "ctypes.c_int32";
        case ValueType::I64: return "ctypes.c_int64";
        case ValueType::F64: return "ctypes.c_double";
        case ValueType::STRING: return "ctypes.c_char_p";
        default: return "ctypes.c_void_p";
    }
}

std::string PythonBinding::convert_value(const FFIValue& value) {
    return "/* Python value conversion */";
}

// Ruby Binding
std::string RubyBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    oss << "require 'ffi'\n\n";
    oss << "module SynQ\n";
    oss << "  extend FFI::Library\n";
    oss << "  ffi_lib 'libsynq.so'\n\n";
    oss << "  attach_function :" << func.name << ", :synq_" << func.name << ", [";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << convert_type(func.parameter_types[i]);
    }
    oss << "], " << convert_type(func.return_type) << "\n";
    oss << "end\n";
    return oss.str();
}

std::string RubyBinding::generate_struct(const FFIStruct& s) {
    std::ostringstream oss;
    oss << "class " << s.name << " < FFI::Struct\n";
    oss << "  layout ";
    for (size_t i = 0; i < s.fields.size(); ++i) {
        if (i > 0) oss << ",\n         ";
        oss << ":" << s.fields[i].first << ", " << convert_type(s.fields[i].second);
    }
    oss << "\nend\n";
    return oss.str();
}

std::string RubyBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::I32: return ":int32";
        case ValueType::I64: return ":int64";
        case ValueType::F64: return ":double";
        case ValueType::STRING: return ":string";
        default: return ":pointer";
    }
}

std::string RubyBinding::convert_value(const FFIValue& value) {
    return "/* Ruby value conversion */";
}

// PHP Binding
std::string PHPBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    oss << "<?php\n\n";
    oss << "$ffi = FFI::cdef(\n";
    oss << "    \"" << convert_type(func.return_type) << " synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << convert_type(func.parameter_types[i]);
    }
    oss << ");\",\n";
    oss << "    \"libsynq.so\"\n";
    oss << ");\n\n";
    oss << "function " << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "$arg" << i;
    }
    oss << ") {\n";
    oss << "    global $ffi;\n";
    oss << "    return $ffi->synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "$arg" << i;
    }
    oss << ");\n";
    oss << "}\n";
    return oss.str();
}

std::string PHPBinding::generate_struct(const FFIStruct& s) {
    return "// PHP struct " + s.name;
}

std::string PHPBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::I32: return "int";
        case ValueType::I64: return "long";
        case ValueType::F64: return "double";
        case ValueType::STRING: return "char*";
        default: return "void*";
    }
}

std::string PHPBinding::convert_value(const FFIValue& value) {
    return "/* PHP value conversion */";
}

// Lua Binding
std::string LuaBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    oss << "local ffi = require('ffi')\n\n";
    oss << "ffi.cdef[[\n";
    oss << "    " << convert_type(func.return_type) << " synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << convert_type(func.parameter_types[i]);
    }
    oss << ");\n";
    oss << "]]\n\n";
    oss << "local synq = ffi.load('libsynq.so')\n\n";
    oss << "function " << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i;
    }
    oss << ")\n";
    oss << "    return synq.synq_" << func.name << "(";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "arg" << i;
    }
    oss << ")\n";
    oss << "end\n";
    return oss.str();
}

std::string LuaBinding::generate_struct(const FFIStruct& s) {
    return "-- Lua struct " + s.name;
}

std::string LuaBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::I32: return "int";
        case ValueType::I64: return "int64_t";
        case ValueType::F64: return "double";
        case ValueType::STRING: return "const char*";
        default: return "void*";
    }
}

std::string LuaBinding::convert_value(const FFIValue& value) {
    return "/* Lua value conversion */";
}

// JavaScript Binding
std::string JavaScriptBinding::generate_binding(const FFIFunction& func) {
    std::ostringstream oss;
    oss << "const ffi = require('ffi-napi');\n\n";
    oss << "const synq = ffi.Library('libsynq', {\n";
    oss << "    " << func.name << ": ['" << convert_type(func.return_type) << "', [";
    for (size_t i = 0; i < func.parameter_types.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "'" << convert_type(func.parameter_types[i]) << "'";
    }
    oss << "]]\n";
    oss << "});\n\n";
    oss << "module.exports = synq;\n";
    return oss.str();
}

std::string JavaScriptBinding::generate_struct(const FFIStruct& s) {
    return "// JavaScript struct " + s.name;
}

std::string JavaScriptBinding::convert_type(const FFIType& type) {
    switch (type.base_type) {
        case ValueType::I32: return "int32";
        case ValueType::I64: return "int64";
        case ValueType::F64: return "double";
        case ValueType::STRING: return "string";
        default: return "pointer";
    }
}

std::string JavaScriptBinding::convert_value(const FFIValue& value) {
    return "/* JavaScript value conversion */";
}

} // namespace synq::compiler::ffi
