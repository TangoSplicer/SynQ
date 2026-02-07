// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - LLVM IR Wrapper

#pragma once

#include "serializable_ir.h"
#include <string>
#include <vector>

namespace synq::compiler::ir {

/**
 * @class LLVM_IR
 * @brief Wrapper for LLVM Intermediate Representation
 * 
 * This IR contains:
 * - LLVM IR code as text (can be serialized)
 * - Target triple (e.g., "x86_64-unknown-linux-gnu")
 * - Data layout information
 * - Metadata about the module
 * 
 * LLVM IR is a low-level, target-independent representation that can be:
 * - Optimized by LLVM's 50+ optimization passes
 * - Compiled to machine code for any target
 * - Interpreted by LLVM JIT compiler
 * 
 * It is produced by code generation and serves as input to backend compilation.
 */
class LLVM_IR : public SerializableIR {
public:
    LLVM_IR() = default;
    explicit LLVM_IR(const std::string& ir_code);

    std::string ir_type() const override { return "LLVM_IR"; }

    /**
     * @brief Serialize LLVM IR to JSON
     */
    json serialize() const override;

    /**
     * @brief Deserialize LLVM IR from JSON
     */
    void deserialize(const json& data) override;

    /**
     * @brief Get the LLVM IR code
     */
    const std::string& get_ir_code() const {
        return ir_code;
    }

    /**
     * @brief Set the LLVM IR code
     */
    void set_ir_code(const std::string& code) {
        ir_code = code;
    }

    /**
     * @brief Append to LLVM IR code
     */
    void append_ir(const std::string& code) {
        ir_code += code;
    }

    /**
     * @brief Get target triple (e.g., "x86_64-unknown-linux-gnu")
     */
    const std::string& get_target_triple() const {
        return target_triple;
    }

    /**
     * @brief Set target triple
     */
    void set_target_triple(const std::string& triple) {
        target_triple = triple;
    }

    /**
     * @brief Get data layout
     */
    const std::string& get_data_layout() const {
        return data_layout;
    }

    /**
     * @brief Set data layout
     */
    void set_data_layout(const std::string& layout) {
        data_layout = layout;
    }

    /**
     * @brief Get target CPU
     */
    const std::string& get_target_cpu() const {
        return target_cpu;
    }

    /**
     * @brief Set target CPU
     */
    void set_target_cpu(const std::string& cpu) {
        target_cpu = cpu;
    }

    /**
     * @brief Get target features
     */
    const std::vector<std::string>& get_target_features() const {
        return target_features;
    }

    /**
     * @brief Add target feature
     */
    void add_target_feature(const std::string& feature) {
        target_features.push_back(feature);
    }

    /**
     * @brief Get optimization level (0-3)
     */
    int get_optimization_level() const {
        return optimization_level;
    }

    /**
     * @brief Set optimization level (0-3)
     */
    void set_optimization_level(int level) {
        optimization_level = std::max(0, std::min(3, level));
    }

    /**
     * @brief Check if debug info is enabled
     */
    bool has_debug_info() const {
        return debug_info_enabled;
    }

    /**
     * @brief Enable/disable debug info
     */
    void set_debug_info(bool enabled) {
        debug_info_enabled = enabled;
    }

    /**
     * @brief Get size of LLVM IR code (in bytes)
     */
    size_t get_size() const {
        return ir_code.size();
    }

    /**
     * @brief Validate LLVM IR syntax
     * @return true if IR is valid, false otherwise
     */
    bool validate() const;

    /**
     * @brief Get line count of LLVM IR
     */
    size_t get_line_count() const {
        size_t count = 1;
        for (char c : ir_code) {
            if (c == '\n') count++;
        }
        return count;
    }

    /**
     * @brief Get function count
     */
    size_t get_function_count() const;

    /**
     * @brief Get global variable count
     */
    size_t get_global_count() const;

private:
    std::string ir_code;                        ///< LLVM IR code as text
    std::string target_triple;                  ///< Target triple (e.g., "x86_64-unknown-linux-gnu")
    std::string data_layout;                    ///< Data layout string
    std::string target_cpu;                     ///< Target CPU (e.g., "generic", "native")
    std::vector<std::string> target_features;   ///< Target features (e.g., "+avx2", "+sse4.1")
    int optimization_level = 2;                 ///< Optimization level (0-3)
    bool debug_info_enabled = false;            ///< Whether debug info is included
};

} // namespace synq::compiler::ir
