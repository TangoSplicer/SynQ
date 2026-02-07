// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Optimized IR

#pragma once

#include "serializable_ir.h"
#include <vector>
#include <string>
#include <memory>

namespace synq::compiler::ir {

/**
 * @class Instruction
 * @brief Represents a low-level instruction in the optimized IR
 */
class Instruction {
public:
    enum class OpCode {
        // Literals and moves
        LOAD_CONST,
        LOAD_VAR,
        STORE_VAR,
        MOVE,
        
        // Arithmetic
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        NEG,
        
        // Comparison
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        
        // Logical
        AND,
        OR,
        NOT,
        
        // Control flow
        JUMP,
        JUMP_IF_TRUE,
        JUMP_IF_FALSE,
        CALL,
        RETURN,
        
        // Memory
        ALLOC,
        FREE,
        LOAD_PTR,
        STORE_PTR,
        
        // Quantum
        QUANTUM_GATE,
        QUANTUM_MEASURE,
        
        // Other
        NOP,
        UNKNOWN
    };

    OpCode opcode;
    std::vector<std::string> operands;
    int line = 0;
    int column = 0;

    /**
     * @brief Get string representation of opcode
     */
    std::string opcode_to_string() const;

    /**
     * @brief Serialize instruction to JSON
     */
    json to_json() const {
        return json{
            {"opcode", opcode_to_string()},
            {"operands", operands},
            {"line", line},
            {"column", column}
        };
    }

    /**
     * @brief Deserialize instruction from JSON
     */
    static Instruction from_json(const json& data);
};

/**
 * @class BasicBlock
 * @brief Represents a basic block (sequence of instructions with single entry/exit)
 */
class BasicBlock {
public:
    std::string label;
    std::vector<Instruction> instructions;
    std::vector<std::string> successors;  // Labels of successor blocks
    std::vector<std::string> predecessors;  // Labels of predecessor blocks

    /**
     * @brief Add instruction to block
     */
    void add_instruction(const Instruction& instr) {
        instructions.push_back(instr);
    }

    /**
     * @brief Serialize block to JSON
     */
    json to_json() const {
        json instrs = json::array();
        for (const auto& instr : instructions) {
            instrs.push_back(instr.to_json());
        }
        return json{
            {"label", label},
            {"instructions", instrs},
            {"successors", successors},
            {"predecessors", predecessors}
        };
    }

    /**
     * @brief Deserialize block from JSON
     */
    static BasicBlock from_json(const json& data);
};

/**
 * @class Function
 * @brief Represents a function in optimized IR
 */
class Function {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::string return_type;
    std::vector<BasicBlock> basic_blocks;

    /**
     * @brief Add basic block
     */
    void add_block(const BasicBlock& block) {
        basic_blocks.push_back(block);
    }

    /**
     * @brief Get entry block (first block)
     */
    BasicBlock* get_entry_block() {
        return basic_blocks.empty() ? nullptr : &basic_blocks[0];
    }

    /**
     * @brief Serialize function to JSON
     */
    json to_json() const {
        json blocks = json::array();
        for (const auto& block : basic_blocks) {
            blocks.push_back(block.to_json());
        }
        return json{
            {"name", name},
            {"parameters", parameters},
            {"return_type", return_type},
            {"basic_blocks", blocks}
        };
    }

    /**
     * @brief Deserialize function from JSON
     */
    static Function from_json(const json& data);
};

/**
 * @class OptimizedIR
 * @brief Represents the output of optimization (Optimized Intermediate Representation)
 * 
 * This IR contains:
 * - Functions represented as control flow graphs (basic blocks)
 * - Low-level instructions
 * - Optimization metadata
 * 
 * Optimizations applied:
 * - Dead code elimination
 * - Constant folding
 * - Loop optimization
 * - Inlining
 * - Register allocation hints
 * 
 * It is produced by the optimizer and serves as input to code generation.
 */
class OptimizedIR : public SerializableIR {
public:
    OptimizedIR() = default;

    std::string ir_type() const override { return "OptimizedIR"; }

    /**
     * @brief Serialize optimized IR to JSON
     */
    json serialize() const override;

    /**
     * @brief Deserialize optimized IR from JSON
     */
    void deserialize(const json& data) override;

    /**
     * @brief Add a function
     */
    void add_function(const Function& func) {
        functions.push_back(func);
    }

    /**
     * @brief Get all functions
     */
    const std::vector<Function>& get_functions() const {
        return functions;
    }

    /**
     * @brief Get function by name
     */
    Function* get_function(const std::string& name) {
        for (auto& func : functions) {
            if (func.name == name) {
                return &func;
            }
        }
        return nullptr;
    }

    /**
     * @brief Add global variable
     */
    void add_global(const std::string& name, const std::string& type) {
        globals[name] = type;
    }

    /**
     * @brief Get global variables
     */
    const std::unordered_map<std::string, std::string>& get_globals() const {
        return globals;
    }

    /**
     * @brief Add optimization metric
     */
    void add_metric(const std::string& name, double value) {
        metrics[name] = value;
    }

    /**
     * @brief Get optimization metrics
     */
    const std::unordered_map<std::string, double>& get_metrics() const {
        return metrics;
    }

private:
    std::vector<Function> functions;
    std::unordered_map<std::string, std::string> globals;
    std::unordered_map<std::string, double> metrics;  // e.g., "dead_code_removed", "constants_folded"
};

} // namespace synq::compiler::ir
