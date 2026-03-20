// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 13: Language Interoperability - Universal FFI Architecture

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <variant>
#include <cstdint>
#include <functional>

namespace synq::compiler::ffi {

/**
 * @enum TargetLanguage
 * @brief Supported target languages for FFI
 */
enum class TargetLanguage {
    // Systems languages
    C,
    CPP,
    RUST,
    GO,
    ZIG,
    D,
    NIM,
    V,

    // JVM languages
    JAVA,
    KOTLIN,
    SCALA,
    CLOJURE,

    // Dynamic languages
    PYTHON,
    RUBY,
    PHP,
    LUA,
    JAVASCRIPT,

    // Functional languages
    HASKELL,
    OCAML,
    LISP,
    SCHEME,
    ELIXIR,

    // .NET languages
    CSHARP,
    FSHARP,

    // Modern languages
    SWIFT,
    DART,
    CRYSTAL,

    // Web/Scripting
    TYPESCRIPT,
    GOLANG_WASM,

    // Other
    WEBASSEMBLY,
};

/**
 * @enum CallingConvention
 * @brief Calling conventions for FFI
 */
enum class CallingConvention {
    CDECL,           // C declaration (default)
    STDCALL,         // Standard call (Windows)
    FASTCALL,        // Fast call
    THISCALL,        // This call (C++)
    SYSCALL,         // System call
    CUSTOM,          // Custom convention
};

/**
 * @enum ValueType
 * @brief FFI value types
 */
enum class ValueType {
    VOID,
    BOOL,
    I8, I16, I32, I64,
    U8, U16, U32, U64,
    F32, F64,
    STRING,
    BYTES,
    POINTER,
    STRUCT,
    ARRAY,
    FUNCTION,
    OPAQUE,
};

/**
 * @struct FFIType
 * @brief Type information for FFI
 */
struct FFIType {
    ValueType base_type;
    std::string name;
    size_t size;
    size_t alignment;
    bool is_pointer;
    bool is_array;
    size_t array_size;
    std::vector<std::shared_ptr<FFIType>> fields;  // For structs
};

/**
 * @struct FFIValue
 * @brief Value wrapper for FFI
 */
struct FFIValue {
    FFIType type;
    std::variant<
        void*,
        bool,
        int8_t, int16_t, int32_t, int64_t,
        uint8_t, uint16_t, uint32_t, uint64_t,
        float, double,
        std::string,
        std::vector<uint8_t>
    > data;
};

/**
 * @struct FFIFunction
 * @brief Function signature for FFI
 */
struct FFIFunction {
    std::string name;
    std::string mangled_name;
    FFIType return_type;
    std::vector<FFIType> parameter_types;
    CallingConvention calling_convention;
    bool is_variadic;
    std::string documentation;
};

/**
 * @struct FFIStruct
 * @brief Struct definition for FFI
 */
struct FFIStruct {
    std::string name;
    std::vector<std::pair<std::string, FFIType>> fields;
    size_t size;
    size_t alignment;
    bool is_packed;
};

/**
 * @class FFIBinding
 * @brief Base class for language-specific FFI bindings
 */
class FFIBinding {
public:
    explicit FFIBinding(TargetLanguage language);
    virtual ~FFIBinding() = default;

    /**
     * @brief Generate FFI binding code
     */
    virtual std::string generate_binding(const FFIFunction& func) = 0;

    /**
     * @brief Generate struct definition
     */
    virtual std::string generate_struct(const FFIStruct& s) = 0;

    /**
     * @brief Convert SynQ type to target language type
     */
    virtual std::string convert_type(const FFIType& type) = 0;

    /**
     * @brief Convert value from SynQ to target language
     */
    virtual std::string convert_value(const FFIValue& value) = 0;

    /**
     * @brief Get language name
     */
    std::string get_language_name() const;

protected:
    TargetLanguage language;
};

/**
 * @class UniversalFFI
 * @brief Universal FFI coordinator for all languages
 */
class UniversalFFI {
public:
    UniversalFFI();

    /**
     * @brief Register a function for FFI
     */
    void register_function(const FFIFunction& func);

    /**
     * @brief Register a struct for FFI
     */
    void register_struct(const FFIStruct& s);

    /**
     * @brief Generate bindings for a language
     */
    std::string generate_bindings(TargetLanguage language);

    /**
     * @brief Generate bindings for all languages
     */
    std::unordered_map<std::string, std::string> generate_all_bindings();

    /**
     * @brief Call a foreign function
     */
    FFIValue call_function(const std::string& func_name, const std::vector<FFIValue>& args);

    /**
     * @brief Get supported languages
     */
    static std::vector<TargetLanguage> get_supported_languages();

    /**
     * @brief Get language name
     */
    static std::string get_language_name(TargetLanguage language);

private:
    std::vector<FFIFunction> functions;
    std::vector<FFIStruct> structs;
    std::unordered_map<TargetLanguage, std::shared_ptr<FFIBinding>> bindings;

    /**
     * @brief Create binding for language
     */
    std::shared_ptr<FFIBinding> create_binding(TargetLanguage language);
};

/**
 * @class FFITypeConverter
 * @brief Converts between SynQ and foreign types
 */
class FFITypeConverter {
public:
    /**
     * @brief Convert SynQ type to C type
     */
    static std::string synq_to_c(const FFIType& type);

    /**
     * @brief Convert SynQ type to Python type
     */
    static std::string synq_to_python(const FFIType& type);

    /**
     * @brief Convert SynQ type to Java type
     */
    static std::string synq_to_java(const FFIType& type);

    /**
     * @brief Convert SynQ type to Rust type
     */
    static std::string synq_to_rust(const FFIType& type);

    /**
     * @brief Convert SynQ type to Go type
     */
    static std::string synq_to_go(const FFIType& type);

    /**
     * @brief Convert SynQ type to C# type
     */
    static std::string synq_to_csharp(const FFIType& type);

    /**
     * @brief Convert SynQ type to JavaScript type
     */
    static std::string synq_to_javascript(const FFIType& type);

    /**
     * @brief Convert SynQ type to generic language type
     */
    static std::string synq_to_generic(const FFIType& type, TargetLanguage language);
};

/**
 * @class FFIMarshalling
 * @brief Marshalling/unmarshalling between SynQ and foreign values
 */
class FFIMarshalling {
public:
    /**
     * @brief Marshal SynQ value to C
     */
    static void* marshal_to_c(const FFIValue& value);

    /**
     * @brief Unmarshal C value to SynQ
     */
    static FFIValue unmarshal_from_c(void* ptr, const FFIType& type);

    /**
     * @brief Marshal SynQ value to Python
     */
    static void* marshal_to_python(const FFIValue& value);

    /**
     * @brief Unmarshal Python value to SynQ
     */
    static FFIValue unmarshal_from_python(void* ptr, const FFIType& type);

    /**
     * @brief Marshal SynQ value to Java
     */
    static void* marshal_to_java(const FFIValue& value);

    /**
     * @brief Unmarshal Java value to SynQ
     */
    static FFIValue unmarshal_from_java(void* ptr, const FFIType& type);

    /**
     * @brief Marshal SynQ value to generic language
     */
    static void* marshal_to_language(const FFIValue& value, TargetLanguage language);

    /**
     * @brief Unmarshal value from generic language
     */
    static FFIValue unmarshal_from_language(void* ptr, const FFIType& type,
                                           TargetLanguage language);
};

/**
 * @class FFIValidator
 * @brief Validates FFI bindings for correctness
 */
class FFIValidator {
public:
    /**
     * @brief Validate function signature
     */
    static bool validate_function(const FFIFunction& func);

    /**
     * @brief Validate struct definition
     */
    static bool validate_struct(const FFIStruct& s);

    /**
     * @brief Validate type compatibility
     */
    static bool validate_type_compatibility(const FFIType& from, const FFIType& to);

    /**
     * @brief Check for ABI compatibility
     */
    static bool check_abi_compatibility(TargetLanguage from, TargetLanguage to);

    /**
     * @brief Get validation errors
     */
    static const std::vector<std::string>& get_errors();
};

} // namespace synq::compiler::ffi
