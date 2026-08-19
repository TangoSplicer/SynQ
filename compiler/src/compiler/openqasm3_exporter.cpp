#include "compiler/openqasm3_exporter.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace synq::compiler {
namespace {

void add_diagnostic(OpenQasm3ExportResult& result, std::size_t line, const std::string& message) {
    result.diagnostics.push_back("line " + std::to_string(line) + ": " + message);
}

bool parse_qubit_operand(const std::string& operand, std::size_t& index) {
    if (operand.size() <= 3 || operand.rfind("q[", 0) != 0 || operand.back() != ']') return false;
    index = 0;
    for (std::size_t position = 2; position + 1 < operand.size(); ++position) {
        const char character = operand[position];
        if (character < '0' || character > '9') return false;
        const std::size_t digit = static_cast<std::size_t>(character - '0');
        if (index > (std::numeric_limits<std::size_t>::max() - digit) / 10) return false;
        index = index * 10 + digit;
    }
    return true;
}

bool parse_operands(const std::vector<std::string>& arguments, std::vector<std::size_t>& operands) {
    operands.clear();
    for (std::size_t position = 1; position < arguments.size(); ++position) {
        std::size_t index = 0;
        if (!parse_qubit_operand(arguments[position], index)) return false;
        operands.push_back(index);
    }
    return true;
}

void write_single_qubit_gate(std::ostringstream& body, const std::string& gate,
                             const std::string& register_name, std::size_t qubit) {
    body << gate << " " << register_name << "[" << qubit << "];\n";
}

bool is_decimal_parameter(const std::string& value) {
    std::size_t position = !value.empty() && value.front() == '-' ? 1 : 0;
    if (position == value.size()) return false;
    bool has_digit = false;
    bool has_decimal_point = false;
    for (; position < value.size(); ++position) {
        const unsigned char character = static_cast<unsigned char>(value[position]);
        if (std::isdigit(character) != 0) {
            has_digit = true;
        } else if (value[position] == '.' && !has_decimal_point) {
            has_decimal_point = true;
        } else {
            return false;
        }
    }
    return has_digit;
}

bool is_literal_angle_parameter(const std::string& value) {
    if (value == "pi" || value == "-pi" || is_decimal_parameter(value)) return true;
    const std::string prefix = value.rfind("-pi/", 0) == 0 ? "-pi/" : "pi/";
    const std::string denominator = value.rfind(prefix, 0) == 0 ? value.substr(prefix.size()) : "";
    return !denominator.empty() && denominator != "0" &&
           std::all_of(denominator.begin(), denominator.end(), [](unsigned char character) {
               return std::isdigit(character) != 0;
           });
}

bool is_identifier(const std::string& value) {
    if (value.empty()) return false;
    const unsigned char first = static_cast<unsigned char>(value.front());
    if (!(std::isalpha(first) != 0 || value.front() == '_')) return false;
    return std::all_of(value.begin() + 1, value.end(), [](unsigned char character) {
        return std::isalnum(character) != 0 || character == '_';
    });
}

bool parse_routine_actual_qubit(const std::string& source, std::string& register_name, std::size_t& index) {
    const std::size_t open = source.find('[');
    if (open == std::string::npos || open == 0 || source.back() != ']' ||
        source.find('[', open + 1) != std::string::npos || !is_identifier(source.substr(0, open))) {
        return false;
    }
    const std::string index_text = source.substr(open + 1, source.size() - open - 2);
    if (index_text.empty() || !std::all_of(index_text.begin(), index_text.end(), [](unsigned char character) {
            return std::isdigit(character) != 0;
        })) {
        return false;
    }
    index = 0;
    for (char character : index_text) {
        const std::size_t digit = static_cast<std::size_t>(character - '0');
        if (index > (std::numeric_limits<std::size_t>::max() - digit) / 10) return false;
        index = index * 10 + digit;
    }
    register_name = source.substr(0, open);
    return true;
}

bool is_valid_parameterized_routine(const HybridCallableDeclaration& callable) {
    if (callable.kind != CallableDeclarationKind::Kernel || callable.formals.empty() ||
        !callable.parameterized_body.has_value()) {
        return false;
    }
    std::unordered_set<std::string> names;
    for (const HybridRoutineFormal& formal : callable.formals) {
        if (!is_identifier(formal.name) || !names.emplace(formal.name).second) return false;
    }
    const HybridParameterizedRoutineBody& body = *callable.parameterized_body;
    const bool one_angle_one_qubit = callable.formals.size() == 2 &&
        callable.formals[0].kind == RoutineFormalKind::Angle && callable.formals[1].kind == RoutineFormalKind::Qubit &&
        body.angle_formal.has_value() && *body.angle_formal == callable.formals[0].name &&
        body.qubit_formals.size() == 1 && body.qubit_formals[0] == callable.formals[1].name &&
        (body.kind == QuantumGateKind::Rx || body.kind == QuantumGateKind::Ry ||
         body.kind == QuantumGateKind::Rz || body.kind == QuantumGateKind::Phase);
    const bool one_qubit = callable.formals.size() == 1 && callable.formals[0].kind == RoutineFormalKind::Qubit &&
        !body.angle_formal.has_value() && body.qubit_formals.size() == 1 &&
        body.qubit_formals[0] == callable.formals[0].name &&
        (body.kind == QuantumGateKind::H || body.kind == QuantumGateKind::X ||
         body.kind == QuantumGateKind::Y || body.kind == QuantumGateKind::Z);
    const bool two_qubits = callable.formals.size() == 2 && callable.formals[0].kind == RoutineFormalKind::Qubit &&
        callable.formals[1].kind == RoutineFormalKind::Qubit && !body.angle_formal.has_value() &&
        body.qubit_formals.size() == 2 && body.qubit_formals[0] == callable.formals[0].name &&
        body.qubit_formals[1] == callable.formals[1].name && body.qubit_formals[0] != body.qubit_formals[1] &&
        body.kind == QuantumGateKind::Cx;
    return one_angle_one_qubit || one_qubit || two_qubits;
}

bool is_direct_measurement_feedback_condition(const HybridControlFlow& control, const std::string& result_name) {
    return control.kind == ClassicalControlKind::If &&
           control.condition.kind == ClassicalConditionKind::IdentifierReference &&
           control.condition.expression.kind == ClassicalBooleanExpressionKind::IdentifierReference &&
           control.condition.expression.source_text == result_name &&
           control.condition.expression.operands.empty();
}

bool is_direct_measurement_feedback_correction(const HybridControlFlow& control) {
    const auto* gate = std::get_if<HybridQuantumGate>(&control.body);
    return gate != nullptr && gate->kind == QuantumGateKind::X && !gate->literal_angle.has_value() &&
           gate->qubit_indices.size() == 1 && gate->qubit_register_names.size() == 1;
}

bool split_parameterized_kernel(const std::string& kernel, std::string& gate, std::string& parameter) {
    const std::size_t open = kernel.find('(');
    if (open == std::string::npos || kernel.back() != ')' || kernel.find('(', open + 1) != std::string::npos) return false;
    gate = kernel.substr(0, open);
    parameter = kernel.substr(open + 1, kernel.size() - open - 2);
    return !gate.empty() && is_literal_angle_parameter(parameter);
}

void write_parameterized_single_qubit_gate(
    std::ostringstream& body, const std::string& gate, const std::string& parameter,
    const std::string& register_name, std::size_t qubit) {
    body << gate << "(" << parameter << ") " << register_name << "[" << qubit << "];\n";
}

QuantumGateKind quantum_gate_kind(const std::string& source_name) {
    if (source_name == "h") return QuantumGateKind::H;
    if (source_name == "x") return QuantumGateKind::X;
    if (source_name == "y") return QuantumGateKind::Y;
    if (source_name == "z") return QuantumGateKind::Z;
    if (source_name == "cx") return QuantumGateKind::Cx;
    if (source_name == "bell_pair") return QuantumGateKind::BellPair;
    if (source_name == "rx") return QuantumGateKind::Rx;
    if (source_name == "ry") return QuantumGateKind::Ry;
    if (source_name == "rz") return QuantumGateKind::Rz;
    if (source_name == "p") return QuantumGateKind::Phase;
    return QuantumGateKind::Unknown;
}

std::unique_ptr<QuantumGateNode> typed_legacy_instruction(const InstructionNode& instruction) {
    if (instruction.op != "quantum" || instruction.args.empty()) return nullptr;
    std::vector<std::size_t> operands;
    if (!parse_operands(instruction.args, operands)) return nullptr;

    const std::string& kernel = instruction.args.front();
    std::string source_name;
    std::string parameter;
    if (split_parameterized_kernel(kernel, source_name, parameter)) {
        return std::make_unique<QuantumGateNode>(quantum_gate_kind(source_name), source_name,
                                                 parameter, std::move(operands), instruction.line);
    }
    return std::make_unique<QuantumGateNode>(quantum_gate_kind(kernel), kernel, std::nullopt,
                                             std::move(operands), instruction.line);
}

void lower_quantum_gate(const QuantumGateNode& gate, std::ostringstream& body,
                        std::size_t& qubit_count, OpenQasm3ExportResult& result) {
    const auto register_name = [&gate](std::size_t position) -> const std::string& {
        static const std::string default_register = "q";
        return position < gate.qubit_register_names.size() ? gate.qubit_register_names[position] : default_register;
    };
    if (gate.kind == QuantumGateKind::H || gate.kind == QuantumGateKind::X ||
        gate.kind == QuantumGateKind::Y || gate.kind == QuantumGateKind::Z) {
        if (gate.literal_angle.has_value()) {
            add_diagnostic(result, gate.line,
                           "unsupported parameterized gate `" + gate.source_name + "` for the OpenQASM 3 recovery exporter");
            return;
        }
        if (gate.qubit_indices.size() > 1) {
            add_diagnostic(result, gate.line, "single-qubit kernel `" + gate.source_name + "` accepts at most one operand");
            return;
        }
        const std::size_t qubit = gate.qubit_indices.empty() ? 0 : gate.qubit_indices.front();
        write_single_qubit_gate(body, gate.source_name, register_name(0), qubit);
        qubit_count = std::max(qubit_count, qubit + 1);
        return;
    }

    if (gate.kind == QuantumGateKind::Cx) {
        if (gate.literal_angle.has_value() || gate.qubit_indices.size() != 2) {
            add_diagnostic(result, gate.line, "kernel `cx` requires exactly two explicit qubit operands");
            return;
        }
        body << "cx " << register_name(0) << "[" << gate.qubit_indices[0] << "], "
             << register_name(1) << "[" << gate.qubit_indices[1] << "];\n";
        qubit_count = std::max(qubit_count, std::max(gate.qubit_indices[0], gate.qubit_indices[1]) + 1);
        return;
    }

    if (gate.kind == QuantumGateKind::BellPair) {
        if (gate.literal_angle.has_value() || (gate.qubit_indices.size() != 0 && gate.qubit_indices.size() != 2)) {
            add_diagnostic(result, gate.line, "kernel `bell_pair` accepts zero or exactly two explicit qubit operands");
            return;
        }
        const std::size_t first = gate.qubit_indices.empty() ? 0 : gate.qubit_indices[0];
        const std::size_t second = gate.qubit_indices.empty() ? 1 : gate.qubit_indices[1];
        write_single_qubit_gate(body, "h", register_name(0), first);
        body << "cx " << register_name(0) << "[" << first << "], "
             << register_name(gate.qubit_indices.empty() ? 0 : 1) << "[" << second << "];\n";
        qubit_count = std::max(qubit_count, std::max(first, second) + 1);
        return;
    }

    if (gate.kind == QuantumGateKind::Rx || gate.kind == QuantumGateKind::Ry ||
        gate.kind == QuantumGateKind::Rz || gate.kind == QuantumGateKind::Phase) {
        if (!gate.literal_angle.has_value() || gate.qubit_indices.size() != 1) {
            add_diagnostic(result, gate.line,
                           "parameterized gate `" + gate.source_name + "` requires exactly one explicit qubit operand");
            return;
        }
        write_parameterized_single_qubit_gate(body, gate.source_name, *gate.literal_angle,
                                              register_name(0), gate.qubit_indices.front());
        qubit_count = std::max(qubit_count, gate.qubit_indices.front() + 1);
        return;
    }

    if (gate.literal_angle.has_value()) {
        add_diagnostic(result, gate.line,
                       "unsupported parameterized gate `" + gate.source_name + "` for the OpenQASM 3 recovery exporter");
    } else {
        add_diagnostic(result, gate.line,
                       "unsupported quantum kernel `" + gate.source_name + "` for the OpenQASM 3 recovery exporter");
    }
}

OpenQasm3ExportResult export_extended_hybrid_openqasm3(const HybridProgram& program) {
    OpenQasm3ExportResult result;
    std::ostringstream body;
    std::unordered_map<std::string, std::size_t> declared_qubit_counts;
    std::vector<std::string> declaration_order;
    std::unordered_map<std::string, bool> measured_registers;
    std::vector<std::string> measurement_order;
    std::unordered_map<std::string, std::string> declared_boolean_storage;
    std::unordered_map<std::string, HybridQuantumGate> callable_bodies;
    std::unordered_map<std::string, HybridCallableDeclaration> parameterized_callable_definitions;
    std::size_t parameterized_declaration_count = 0;
    std::size_t parameterized_call_count = 0;
    std::optional<HybridMeasurement> pending_feedback_measurement;
    std::string pending_feedback_storage;
    bool emitted_feedback_storage = false;

    for (const HybridNode& node : program.nodes) {
        if (pending_feedback_measurement.has_value()) {
            const auto* control = std::get_if<HybridControlFlow>(&node);
            if (control == nullptr || !control->feedback_enabled ||
                !is_direct_measurement_feedback_condition(*control, *pending_feedback_measurement->result_name)) {
                add_diagnostic(result, pending_feedback_measurement->span.line,
                               "SYNQ-H004: strict Hybrid export requires an immediate direct conditional x correction after a named U4 measurement");
                return result;
            }
            if (!is_direct_measurement_feedback_correction(*control)) {
                add_diagnostic(result, control->span.line,
                               "SYNQ-H004: strict Hybrid export accepts only one direct conditional x correction for a U4 measurement result");
                return result;
            }
            const auto& correction = std::get<HybridQuantumGate>(control->body);
            const auto declaration = declared_qubit_counts.find(correction.qubit_register_names.front());
            if (declaration == declared_qubit_counts.end() || correction.qubit_indices.front() >= declaration->second) {
                add_diagnostic(result, correction.span.line,
                               "SYNQ-H004: U4 conditional correction requires an earlier declared in-range qubit operand");
                return result;
            }
            QuantumGateNode typed_gate(correction.kind, correction.source_name, correction.literal_angle,
                                       correction.qubit_indices, correction.span.line, correction.span,
                                       correction.qubit_register_names);
            std::ostringstream lowered_body;
            std::size_t inferred_qubit_count = 0;
            lower_quantum_gate(typed_gate, lowered_body, inferred_qubit_count, result);
            if (!result.ok()) return result;
            body << "if (" << pending_feedback_storage << ") " << lowered_body.str();
            pending_feedback_measurement.reset();
            continue;
        }
        if (const auto* qubits = std::get_if<HybridQubitDeclaration>(&node)) {
            if (declared_qubit_counts.find(qubits->name) != declared_qubit_counts.end()) {
                add_diagnostic(result, qubits->span.line,
                               "Hybrid OpenQASM 3 export accepts each qubit register declaration only once");
                continue;
            }
            declared_qubit_counts.emplace(qubits->name, qubits->qubit_count);
            declaration_order.push_back(qubits->name);
            continue;
        }

        if (const auto* callable = std::get_if<HybridCallableDeclaration>(&node)) {
            if (callable->classical_body.has_value() || callable->binary_classical_body.has_value()) {
                add_diagnostic(result, callable->span.line,
                               "strict Hybrid OpenQASM export explicitly rejects Alpha local classical callable runtime declarations");
                continue;
            }
            if (!callable->formals.empty() || callable->parameterized_body.has_value()) {
                ++parameterized_declaration_count;
                if (parameterized_declaration_count > 32) {
                    add_diagnostic(result, callable->span.line,
                                   "SYNQ-H003: strict Hybrid export accepts at most 32 parameterized routine declarations");
                    continue;
                }
                if (!is_valid_parameterized_routine(*callable)) {
                    add_diagnostic(result, callable->span.line,
                                   "strict Hybrid export rejects an unresolved or malformed parameterized routine declaration");
                    continue;
                }
                if (!parameterized_callable_definitions.emplace(callable->name, *callable).second) {
                    add_diagnostic(result, callable->span.line,
                                   "strict Hybrid export accepts each parameterized routine declaration only once");
                }
                continue;
            }
            if (callable->kind != CallableDeclarationKind::Kernel || !callable->body.has_value()) {
                add_diagnostic(result, callable->span.line,
                               "Hybrid OpenQASM 3 export accepts only bounded one-gate kernel definitions before a call");
                continue;
            }
            callable_bodies.emplace(callable->name, *callable->body);
            continue;
        }

        if (const auto* declaration = std::get_if<HybridDeclaration>(&node)) {
            if (declaration->classical_callable_invocation.has_value() ||
                declaration->binary_classical_callable_invocation.has_value()) {
                add_diagnostic(result, declaration->span.line,
                               "strict Hybrid OpenQASM export explicitly rejects Alpha local classical callable runtime invocations");
                continue;
            }
        }

        if (const auto* call = std::get_if<HybridCallableCall>(&node)) {
            if (!call->arguments.empty()) {
                ++parameterized_call_count;
                if (parameterized_call_count > 128) {
                    add_diagnostic(result, call->span.line,
                                   "SYNQ-H003: strict Hybrid export accepts at most 128 parameterized routine calls");
                    continue;
                }
                const auto parameterized_target = parameterized_callable_definitions.find(call->name);
                if (parameterized_target == parameterized_callable_definitions.end() ||
                    !is_valid_parameterized_routine(parameterized_target->second)) {
                    add_diagnostic(result, call->span.line,
                                   "strict Hybrid export requires a resolved earlier parameterized routine definition before expansion");
                    continue;
                }
                const HybridCallableDeclaration& routine = parameterized_target->second;
                if (call->arguments.size() != routine.formals.size()) {
                    add_diagnostic(result, call->span.line,
                                   "strict Hybrid export received parameterized routine actuals with an incompatible signature");
                    continue;
                }
                std::optional<std::string> literal_angle;
                std::vector<std::size_t> qubit_indices;
                std::vector<std::string> register_names;
                bool actuals_valid = true;
                for (std::size_t position = 0; position < routine.formals.size(); ++position) {
                    const HybridRoutineFormal& formal = routine.formals[position];
                    const std::string& actual = call->arguments[position];
                    if (formal.kind == RoutineFormalKind::Angle) {
                        if (!is_literal_angle_parameter(actual)) {
                            actuals_valid = false;
                            break;
                        }
                        literal_angle = actual;
                        continue;
                    }
                    std::string register_name;
                    std::size_t index = 0;
                    if (!parse_routine_actual_qubit(actual, register_name, index)) {
                        actuals_valid = false;
                        break;
                    }
                    const auto declaration = declared_qubit_counts.find(register_name);
                    if (declaration == declared_qubit_counts.end() || index >= declaration->second) {
                        actuals_valid = false;
                        break;
                    }
                    register_names.push_back(std::move(register_name));
                    qubit_indices.push_back(index);
                }
                if (qubit_indices.size() == 2 && register_names[0] == register_names[1] &&
                    qubit_indices[0] == qubit_indices[1]) {
                    actuals_valid = false;
                }
                if (!actuals_valid) {
                    add_diagnostic(result, call->span.line,
                                   "strict Hybrid export rejects unresolved or invalid parameterized routine call actuals");
                    continue;
                }
                const HybridParameterizedRoutineBody& routine_body = *routine.parameterized_body;
                HybridQuantumGate expanded{routine_body.kind, routine_body.source_name, literal_angle,
                                           std::move(qubit_indices), std::move(register_names), call->span};
                QuantumGateNode typed_gate(expanded.kind, expanded.source_name, expanded.literal_angle,
                                           expanded.qubit_indices, expanded.span.line, expanded.span,
                                           expanded.qubit_register_names);
                std::size_t inferred_qubit_count = 0;
                lower_quantum_gate(typed_gate, body, inferred_qubit_count, result);
                continue;
            }
            if (parameterized_callable_definitions.find(call->name) != parameterized_callable_definitions.end()) {
                add_diagnostic(result, call->span.line,
                               "strict Hybrid export received a parameterized routine call without its required actuals");
                continue;
            }
            const auto target = callable_bodies.find(call->name);
            if (target == callable_bodies.end()) {
                add_diagnostic(result, call->span.line,
                               "Hybrid OpenQASM 3 bounded call lowering requires an earlier one-gate kernel definition");
                continue;
            }
            const HybridQuantumGate& gate = target->second;
            bool operands_valid = gate.qubit_register_names.size() == gate.qubit_indices.size();
            for (std::size_t position = 0; operands_valid && position < gate.qubit_indices.size(); ++position) {
                const auto declaration = declared_qubit_counts.find(gate.qubit_register_names[position]);
                operands_valid = declaration != declared_qubit_counts.end() &&
                    gate.qubit_indices[position] < declaration->second;
            }
            if (!operands_valid) {
                add_diagnostic(result, call->span.line,
                               "bounded kernel call requires an earlier declared register and an in-range body operand");
                continue;
            }
            QuantumGateNode typed_gate(gate.kind, gate.source_name, gate.literal_angle,
                                       gate.qubit_indices, gate.span.line, gate.span,
                                       gate.qubit_register_names);
            std::size_t inferred_qubit_count = 0;
            lower_quantum_gate(typed_gate, body, inferred_qubit_count, result);
            continue;
        }

        if (const auto* gate = std::get_if<HybridQuantumGate>(&node)) {
            if (gate->qubit_register_names.size() != gate->qubit_indices.size()) {
                add_diagnostic(result, gate->span.line,
                               "Hybrid OpenQASM 3 export received mismatched quantum register metadata");
                continue;
            }
            bool operands_valid = true;
            for (std::size_t position = 0; position < gate->qubit_indices.size(); ++position) {
                const auto declaration = declared_qubit_counts.find(gate->qubit_register_names[position]);
                if (declaration == declared_qubit_counts.end()) {
                    add_diagnostic(result, gate->span.line,
                                   "Hybrid OpenQASM 3 export requires each declared qubit register before use");
                    operands_valid = false;
                    break;
                }
                if (gate->qubit_indices[position] >= declaration->second) {
                    add_diagnostic(result, gate->span.line,
                                   "quantum operand is outside its explicit qubit declaration range");
                    operands_valid = false;
                    break;
                }
            }
            if (!operands_valid) continue;
            QuantumGateNode typed_gate(gate->kind, gate->source_name, gate->literal_angle,
                                       gate->qubit_indices, gate->span.line, gate->span,
                                       gate->qubit_register_names);
            std::size_t inferred_qubit_count = 0;
            lower_quantum_gate(typed_gate, body, inferred_qubit_count, result);
            continue;
        }

        if (const auto* measurement = std::get_if<HybridMeasurement>(&node)) {
            const auto declaration = declared_qubit_counts.find(measurement->qubit_register_name);
            if (declaration == declared_qubit_counts.end()) {
                add_diagnostic(result, measurement->span.line,
                               "Hybrid OpenQASM 3 export requires each declared qubit register before measurement");
                continue;
            }
            if (measurement->result_name.has_value()) {
                if (!measurement->feedback_enabled) {
                    add_diagnostic(result, measurement->span.line,
                                   "Hybrid OpenQASM 3 export does not lower named SynQ measurement-result declarations");
                    continue;
                }
                if (emitted_feedback_storage) {
                    add_diagnostic(result, measurement->span.line,
                                   "SYNQ-H004: strict Hybrid export accepts at most one U4 named measurement-feedback pair");
                    return result;
                }
                if (measurement->qubit_index >= declaration->second) {
                    add_diagnostic(result, measurement->span.line,
                                   "measurement operand is outside its explicit qubit declaration range");
                    continue;
                }
                pending_feedback_storage = "synq_measure_" + *measurement->result_name;
                body << pending_feedback_storage << " = measure " << measurement->qubit_register_name << "["
                     << measurement->qubit_index << "];\n";
                pending_feedback_measurement = *measurement;
                emitted_feedback_storage = true;
                continue;
            }
            if (measurement->qubit_index >= declaration->second) {
                add_diagnostic(result, measurement->span.line,
                               "measurement operand is outside its explicit qubit declaration range");
                continue;
            }
            body << "c_" << measurement->qubit_register_name << "[" << measurement->qubit_index
                 << "] = measure " << measurement->qubit_register_name << "["
                 << measurement->qubit_index << "];\n";
            if (!measured_registers[measurement->qubit_register_name]) {
                measured_registers[measurement->qubit_register_name] = true;
                measurement_order.push_back(measurement->qubit_register_name);
            }
            continue;
        }

        if (const auto* declaration = std::get_if<HybridDeclaration>(&node)) {
            if (declaration->literal_kind != ClassicalLiteralKind::Boolean ||
                declaration->initializer.kind != ClassicalExpressionKind::BooleanLiteral) {
                add_diagnostic(result, declaration->span.line,
                               "Hybrid OpenQASM 3 export supports only top-level Boolean literal declarations");
                continue;
            }
            const auto inserted = declared_boolean_storage.emplace(
                declaration->name, "synq_bool_" + declaration->name);
            if (!inserted.second) {
                add_diagnostic(result, declaration->span.line,
                               "Hybrid OpenQASM 3 export accepts each Boolean declaration only once");
                continue;
            }
            body << "bool " << inserted.first->second << " = "
                 << (declaration->initializer.source_text == "true" ? "true" : "false") << ";\n";
            continue;
        }

        if (const auto* declaration = std::get_if<HybridMutableDeclaration>(&node)) {
            add_diagnostic(result, declaration->span.line,
                           "Hybrid OpenQASM 3 export does not lower mutable SynQ state declarations");
            continue;
        }

        if (const auto* assignment = std::get_if<HybridAssignment>(&node)) {
            add_diagnostic(result, assignment->span.line,
                           "Hybrid OpenQASM 3 export does not lower mutable SynQ state assignments");
            continue;
        }

        if (const auto* control = std::get_if<HybridControlFlow>(&node)) {
            if (control->kind != ClassicalControlKind::If) {
                add_diagnostic(result, control->span.line,
                               "Hybrid OpenQASM 3 export lowers if conditions only; while-loop lowering is not supported");
                continue;
            }
            std::string lowered_condition;
            if (control->condition.kind == ClassicalConditionKind::BooleanLiteral) {
                lowered_condition = control->condition.boolean_value ? "true" : "false";
            } else if (control->condition.kind == ClassicalConditionKind::IdentifierReference) {
                const auto storage = declared_boolean_storage.find(control->condition.source_text);
                if (storage == declared_boolean_storage.end()) {
                    add_diagnostic(result, control->span.line,
                                   "Hybrid OpenQASM 3 identifier-if lowering requires an earlier top-level Boolean literal declaration; aliases, measurement results, non-Boolean values, and forward bindings are not supported");
                    continue;
                }
                lowered_condition = storage->second;
            } else if (control->condition.kind == ClassicalConditionKind::BooleanExpression &&
                       control->condition.expression.kind == ClassicalBooleanExpressionKind::Not &&
                       control->condition.expression.operands.size() == 1 &&
                       control->condition.expression.operands.front().kind ==
                           ClassicalBooleanExpressionKind::BooleanLiteral) {
                lowered_condition = control->condition.expression.operands.front().boolean_value
                    ? "false" : "true";
            } else if (control->condition.kind == ClassicalConditionKind::BooleanExpression &&
                       control->condition.expression.kind == ClassicalBooleanExpressionKind::Not &&
                       control->condition.expression.operands.size() == 1 &&
                       control->condition.expression.operands.front().kind ==
                           ClassicalBooleanExpressionKind::IdentifierReference) {
                const auto storage = declared_boolean_storage.find(
                    control->condition.expression.operands.front().source_text);
                if (storage == declared_boolean_storage.end()) {
                    add_diagnostic(result, control->span.line,
                                   "Hybrid OpenQASM 3 negated identifier-if lowering requires an earlier top-level Boolean literal declaration; aliases, measurement results, non-Boolean values, and forward bindings are not supported");
                    continue;
                }
                lowered_condition = "!" + storage->second;
            } else {
                add_diagnostic(result, control->span.line,
                               "Hybrid OpenQASM 3 export lowers only literal, not <Boolean literal>, one earlier Boolean-declaration identifier, or not <earlier Boolean-declaration identifier> if conditions; Boolean expressions are otherwise not supported");
                continue;
            }
            const auto* gate = std::get_if<HybridQuantumGate>(&control->body);
            if (gate == nullptr) {
                add_diagnostic(result, control->span.line,
                               "Hybrid OpenQASM 3 if lowering supports one typed quantum gate body only");
                continue;
            }
            if (gate->qubit_register_names.size() != gate->qubit_indices.size()) {
                add_diagnostic(result, gate->span.line,
                               "Hybrid OpenQASM 3 export received mismatched quantum register metadata");
                continue;
            }
            bool operands_valid = true;
            for (std::size_t position = 0; position < gate->qubit_indices.size(); ++position) {
                const auto declaration = declared_qubit_counts.find(gate->qubit_register_names[position]);
                if (declaration == declared_qubit_counts.end() || gate->qubit_indices[position] >= declaration->second) {
                    add_diagnostic(result, gate->span.line,
                                   "if quantum operand requires an earlier declared register and an in-range index");
                    operands_valid = false;
                    break;
                }
            }
            if (!operands_valid) continue;
            QuantumGateNode typed_gate(gate->kind, gate->source_name, gate->literal_angle,
                                       gate->qubit_indices, gate->span.line, gate->span,
                                       gate->qubit_register_names);
            std::ostringstream lowered_body;
            std::size_t inferred_qubit_count = 0;
            lower_quantum_gate(typed_gate, lowered_body, inferred_qubit_count, result);
            if (!result.ok()) continue;
            body << "if (" << lowered_condition << ") " << lowered_body.str();
            continue;
        }

        const auto* declaration = std::get_if<HybridDeclaration>(&node);
        add_diagnostic(result, declaration == nullptr ? 0 : declaration->span.line,
                       "Hybrid OpenQASM 3 export supports only qubit declarations, typed quantum gates, and unnamed measurements");
    }

    if (pending_feedback_measurement.has_value()) {
        add_diagnostic(result, pending_feedback_measurement->span.line,
                       "SYNQ-H004: strict Hybrid export requires an immediate direct conditional x correction after a named U4 measurement");
        return result;
    }
    if (!result.ok()) return result;
    if (declaration_order.empty()) {
        add_diagnostic(result, 0, "Hybrid OpenQASM 3 export requires at least one explicit qubit declaration");
        return result;
    }

    std::ostringstream output;
    output << "OPENQASM 3.0;\n";
    output << "include \"stdgates.inc\";\n";
    for (const std::string& name : declaration_order) {
        output << "qubit[" << declared_qubit_counts.at(name) << "] " << name << ";\n";
    }
    if (emitted_feedback_storage) output << "bit " << pending_feedback_storage << ";\n";
    for (const std::string& name : measurement_order) {
        output << "bit[" << declared_qubit_counts.at(name) << "] c_" << name << ";\n";
    }
    output << body.str();
    result.program = output.str();
    return result;
}

}  // namespace

OpenQasm3ExportResult export_openqasm3(const ProgramNode& program) {
    OpenQasm3ExportResult result;
    std::ostringstream body;
    std::size_t qubit_count = 0;
    bool has_measurements = false;

    for (const ASTNode* statement : program.statements) {
        const auto* measurement = dynamic_cast<const MeasurementNode*>(statement);
        if (measurement != nullptr) {
            if (measurement->result_name.has_value()) {
                add_diagnostic(result, measurement->line,
                               "OpenQASM 3 export does not lower named SynQ measurement-result declarations");
                continue;
            }
            body << "c[" << measurement->qubit_index << "] = measure q[" << measurement->qubit_index << "];\n";
            qubit_count = std::max(qubit_count, measurement->qubit_index + 1);
            has_measurements = true;
            continue;
        }
        const auto* typed_gate = dynamic_cast<const QuantumGateNode*>(statement);
        std::unique_ptr<QuantumGateNode> legacy_gate;
        if (typed_gate == nullptr) {
            const auto* instruction = dynamic_cast<const InstructionNode*>(statement);
            if (instruction != nullptr) {
                if (instruction->op != "quantum" || instruction->args.empty()) {
                    add_diagnostic(result, instruction->line,
                                   "only `quantum <kernel> [q[index](, q[index])*]` instructions can be exported to OpenQASM 3");
                    continue;
                }
                legacy_gate = typed_legacy_instruction(*instruction);
                if (legacy_gate == nullptr) {
                    add_diagnostic(result, instruction->line,
                                   "OpenQASM 3 export requires explicit operands in the form `q[index]`");
                    continue;
                }
                typed_gate = legacy_gate.get();
            }
        }
        if (typed_gate == nullptr) {
            const auto* declaration = dynamic_cast<const DeclarationNode*>(statement);
            add_diagnostic(result, declaration == nullptr ? 0 : declaration->line,
                           "only supported quantum instructions can be exported to OpenQASM 3");
            continue;
        }
        lower_quantum_gate(*typed_gate, body, qubit_count, result);
    }

    if (!result.ok()) {
        return result;
    }
    if (qubit_count == 0) {
        add_diagnostic(result, 0, "no supported quantum instructions were available for OpenQASM 3 export");
        return result;
    }

    std::ostringstream output;
    output << "OPENQASM 3.0;\n";
    output << "include \"stdgates.inc\";\n";
    output << "qubit[" << qubit_count << "] q;\n";
    if (has_measurements) output << "bit[" << qubit_count << "] c;\n";
    output << body.str();
    result.program = output.str();
    return result;
}

OpenQasm3ExportResult export_hybrid_openqasm3(const HybridProgram& program) {
    const bool requires_extended_lowering = std::any_of(program.nodes.begin(), program.nodes.end(),
        [](const HybridNode& node) {
            const auto* qubits = std::get_if<HybridQubitDeclaration>(&node);
            const auto* callable = std::get_if<HybridCallableDeclaration>(&node);
            const auto* declaration = std::get_if<HybridDeclaration>(&node);
            return (qubits != nullptr && qubits->name != "q") || std::holds_alternative<HybridControlFlow>(node) ||
                std::holds_alternative<HybridCallableCall>(node) ||
                (callable != nullptr && (callable->body.has_value() || callable->parameterized_body.has_value() ||
                                         callable->classical_body.has_value() ||
                                         callable->binary_classical_body.has_value())) ||
                (declaration != nullptr && (declaration->classical_callable_invocation.has_value() ||
                                             declaration->binary_classical_callable_invocation.has_value()));
        });
    if (requires_extended_lowering) return export_extended_hybrid_openqasm3(program);

    OpenQasm3ExportResult result;
    std::ostringstream body;
    std::optional<std::size_t> declared_qubit_count;
    bool has_measurements = false;

    for (const HybridNode& node : program.nodes) {
        if (const auto* qubits = std::get_if<HybridQubitDeclaration>(&node)) {
            if (qubits->name != "q") {
                add_diagnostic(result, qubits->span.line,
                               "Hybrid OpenQASM 3 export supports only the default qubit declaration qubit q[n]");
                continue;
            }
            if (declared_qubit_count.has_value()) {
                add_diagnostic(result, qubits->span.line,
                               "Hybrid OpenQASM 3 export accepts exactly one default qubit declaration");
                continue;
            }
            declared_qubit_count = qubits->qubit_count;
            continue;
        }

        if (const auto* gate = std::get_if<HybridQuantumGate>(&node)) {
            if (!declared_qubit_count.has_value()) {
                add_diagnostic(result, gate->span.line,
                               "Hybrid OpenQASM 3 export requires qubit q[n] before quantum operations");
                continue;
            }
            for (std::size_t index : gate->qubit_indices) {
                if (index >= *declared_qubit_count) {
                    add_diagnostic(result, gate->span.line,
                                   "quantum operand is outside the explicit qubit q[n] declaration range");
                    break;
                }
            }
            if (!result.ok()) continue;
            QuantumGateNode typed_gate(gate->kind, gate->source_name, gate->literal_angle,
                                       gate->qubit_indices, gate->span.line, gate->span);
            std::size_t inferred_qubit_count = 0;
            lower_quantum_gate(typed_gate, body, inferred_qubit_count, result);
            if (inferred_qubit_count > *declared_qubit_count) {
                add_diagnostic(result, gate->span.line,
                               "quantum kernel requires operands outside the explicit qubit q[n] declaration range");
            }
            continue;
        }

        if (const auto* measurement = std::get_if<HybridMeasurement>(&node)) {
            if (!declared_qubit_count.has_value()) {
                add_diagnostic(result, measurement->span.line,
                               "Hybrid OpenQASM 3 export requires qubit q[n] before measurements");
                continue;
            }
            if (measurement->result_name.has_value()) {
                add_diagnostic(result, measurement->span.line,
                               "Hybrid OpenQASM 3 export does not lower named SynQ measurement-result declarations");
                continue;
            }
            if (measurement->qubit_index >= *declared_qubit_count) {
                add_diagnostic(result, measurement->span.line,
                               "measurement operand is outside the explicit qubit q[n] declaration range");
                continue;
            }
            body << "c[" << measurement->qubit_index << "] = measure q[" << measurement->qubit_index << "];\n";
            has_measurements = true;
            continue;
        }

        if (const auto* control = std::get_if<HybridControlFlow>(&node)) {
            add_diagnostic(result, control->span.line,
                           "Hybrid OpenQASM 3 export does not lower bounded classical control-flow nodes");
            continue;
        }

        if (const auto* declaration = std::get_if<HybridMutableDeclaration>(&node)) {
            add_diagnostic(result, declaration->span.line,
                           "Hybrid OpenQASM 3 export does not lower mutable SynQ state declarations");
            continue;
        }

        if (const auto* assignment = std::get_if<HybridAssignment>(&node)) {
            add_diagnostic(result, assignment->span.line,
                           "Hybrid OpenQASM 3 export does not lower mutable SynQ state assignments");
            continue;
        }

        const auto* declaration = std::get_if<HybridDeclaration>(&node);
        add_diagnostic(result, declaration == nullptr ? 0 : declaration->span.line,
                       "Hybrid OpenQASM 3 export supports only qubit declarations, typed quantum gates, and unnamed measurements");
    }

    if (!result.ok()) return result;
    if (!declared_qubit_count.has_value()) {
        add_diagnostic(result, 0, "Hybrid OpenQASM 3 export requires exactly one explicit qubit q[n] declaration");
        return result;
    }

    std::ostringstream output;
    output << "OPENQASM 3.0;\n";
    output << "include \"stdgates.inc\";\n";
    output << "qubit[" << *declared_qubit_count << "] q;\n";
    if (has_measurements) output << "bit[" << *declared_qubit_count << "] c;\n";
    output << body.str();
    result.program = output.str();
    return result;
}

}  // namespace synq::compiler
