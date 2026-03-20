// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 13: Language Interoperability - Language-Specific Bindings

#pragma once

#include "universal_ffi.h"
#include <string>
#include <vector>

namespace synq::compiler::ffi {

// ============================================================================
// Systems Languages
// ============================================================================

/**
 * @class CBinding
 * @brief C language FFI binding
 */
class CBinding : public FFIBinding {
public:
    CBinding() : FFIBinding(TargetLanguage::C) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class CppBinding
 * @brief C++ language FFI binding
 */
class CppBinding : public FFIBinding {
public:
    CppBinding() : FFIBinding(TargetLanguage::CPP) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class RustBinding
 * @brief Rust language FFI binding
 */
class RustBinding : public FFIBinding {
public:
    RustBinding() : FFIBinding(TargetLanguage::RUST) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class GoBinding
 * @brief Go language FFI binding
 */
class GoBinding : public FFIBinding {
public:
    GoBinding() : FFIBinding(TargetLanguage::GO) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class ZigBinding
 * @brief Zig language FFI binding
 */
class ZigBinding : public FFIBinding {
public:
    ZigBinding() : FFIBinding(TargetLanguage::ZIG) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class DBinding
 * @brief D language FFI binding
 */
class DBinding : public FFIBinding {
public:
    DBinding() : FFIBinding(TargetLanguage::D) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class NimBinding
 * @brief Nim language FFI binding
 */
class NimBinding : public FFIBinding {
public:
    NimBinding() : FFIBinding(TargetLanguage::NIM) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class VBinding
 * @brief V language FFI binding
 */
class VBinding : public FFIBinding {
public:
    VBinding() : FFIBinding(TargetLanguage::V) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

// ============================================================================
// JVM Languages
// ============================================================================

/**
 * @class JavaBinding
 * @brief Java language FFI binding
 */
class JavaBinding : public FFIBinding {
public:
    JavaBinding() : FFIBinding(TargetLanguage::JAVA) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class KotlinBinding
 * @brief Kotlin language FFI binding
 */
class KotlinBinding : public FFIBinding {
public:
    KotlinBinding() : FFIBinding(TargetLanguage::KOTLIN) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class ScalaBinding
 * @brief Scala language FFI binding
 */
class ScalaBinding : public FFIBinding {
public:
    ScalaBinding() : FFIBinding(TargetLanguage::SCALA) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class ClojureBinding
 * @brief Clojure language FFI binding
 */
class ClojureBinding : public FFIBinding {
public:
    ClojureBinding() : FFIBinding(TargetLanguage::CLOJURE) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

// ============================================================================
// Dynamic Languages
// ============================================================================

/**
 * @class PythonBinding
 * @brief Python language FFI binding
 */
class PythonBinding : public FFIBinding {
public:
    PythonBinding() : FFIBinding(TargetLanguage::PYTHON) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class RubyBinding
 * @brief Ruby language FFI binding
 */
class RubyBinding : public FFIBinding {
public:
    RubyBinding() : FFIBinding(TargetLanguage::RUBY) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class PHPBinding
 * @brief PHP language FFI binding
 */
class PHPBinding : public FFIBinding {
public:
    PHPBinding() : FFIBinding(TargetLanguage::PHP) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class LuaBinding
 * @brief Lua language FFI binding
 */
class LuaBinding : public FFIBinding {
public:
    LuaBinding() : FFIBinding(TargetLanguage::LUA) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class JavaScriptBinding
 * @brief JavaScript language FFI binding
 */
class JavaScriptBinding : public FFIBinding {
public:
    JavaScriptBinding() : FFIBinding(TargetLanguage::JAVASCRIPT) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

// ============================================================================
// Functional Languages
// ============================================================================

/**
 * @class HaskellBinding
 * @brief Haskell language FFI binding
 */
class HaskellBinding : public FFIBinding {
public:
    HaskellBinding() : FFIBinding(TargetLanguage::HASKELL) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class OCamlBinding
 * @brief OCaml language FFI binding
 */
class OCamlBinding : public FFIBinding {
public:
    OCamlBinding() : FFIBinding(TargetLanguage::OCAML) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class LispBinding
 * @brief Lisp language FFI binding
 */
class LispBinding : public FFIBinding {
public:
    LispBinding() : FFIBinding(TargetLanguage::LISP) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class ElixirBinding
 * @brief Elixir language FFI binding
 */
class ElixirBinding : public FFIBinding {
public:
    ElixirBinding() : FFIBinding(TargetLanguage::ELIXIR) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

// ============================================================================
// .NET and Modern Languages
// ============================================================================

/**
 * @class CSharpBinding
 * @brief C# language FFI binding
 */
class CSharpBinding : public FFIBinding {
public:
    CSharpBinding() : FFIBinding(TargetLanguage::CSHARP) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class SwiftBinding
 * @brief Swift language FFI binding
 */
class SwiftBinding : public FFIBinding {
public:
    SwiftBinding() : FFIBinding(TargetLanguage::SWIFT) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class DartBinding
 * @brief Dart language FFI binding
 */
class DartBinding : public FFIBinding {
public:
    DartBinding() : FFIBinding(TargetLanguage::DART) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

/**
 * @class CrystalBinding
 * @brief Crystal language FFI binding
 */
class CrystalBinding : public FFIBinding {
public:
    CrystalBinding() : FFIBinding(TargetLanguage::CRYSTAL) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

// ============================================================================
// WebAssembly
// ============================================================================

/**
 * @class WebAssemblyBinding
 * @brief WebAssembly FFI binding
 */
class WebAssemblyBinding : public FFIBinding {
public:
    WebAssemblyBinding() : FFIBinding(TargetLanguage::WEBASSEMBLY) {}
    std::string generate_binding(const FFIFunction& func) override;
    std::string generate_struct(const FFIStruct& s) override;
    std::string convert_type(const FFIType& type) override;
    std::string convert_value(const FFIValue& value) override;
};

} // namespace synq::compiler::ffi
