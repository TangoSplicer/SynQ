// Copyright (c) 2025 SynQ Contributors
//
// C ABI bridge for the recovery-profile parser and OpenQASM 3 exporter. Keep
// C++ implementation details private; callers only receive opaque handles and
// library-owned UTF-8 strings.

#include "synq/synq_ffi.h"

#include <cstdlib>
#include <cstring>
#include <exception>
#include <memory>
#include <new>
#include <string>

#include "compiler/ast.h"
#include "compiler/openqasm3_exporter.h"
#include "compiler/parser.h"

struct synq_program {
    std::unique_ptr<ProgramNode> program;
};

namespace {

char* copy_utf8_string(const std::string& value) {
    char* copy = static_cast<char*>(std::malloc(value.size() + 1));
    if (copy == nullptr) return nullptr;
    std::memcpy(copy, value.c_str(), value.size() + 1);
    return copy;
}

void clear_output(char** output) {
    if (output != nullptr) {
        *output = nullptr;
    }
}

synq_status return_error(synq_status status, const std::string& message, char** out_diagnostic) {
    if (out_diagnostic != nullptr) {
        *out_diagnostic = copy_utf8_string(message);
        if (*out_diagnostic == nullptr) return SYNQ_STATUS_INTERNAL_ERROR;
    }
    return status;
}

bool contains_parameterized_routine_node(const ProgramNode& program) {
    for (const ASTNode* statement : program.statements) {
        const auto* callable = dynamic_cast<const CallableDeclarationNode*>(statement);
        if (callable != nullptr && (!callable->formals.empty() || callable->parameterized_body.has_value())) {
            return true;
        }
        const auto* call = dynamic_cast<const CallableCallNode*>(statement);
        if (call != nullptr && !call->arguments.empty()) return true;
    }
    return false;
}

bool contains_measurement_feedback_node(const ProgramNode& program) {
    for (const ASTNode* statement : program.statements) {
        const auto* measurement = dynamic_cast<const MeasurementNode*>(statement);
        if (measurement != nullptr && measurement->feedback_enabled && measurement->result_name.has_value()) {
            return true;
        }
    }
    return false;
}

bool contains_classical_callable_runtime_node(const ProgramNode& program) {
    for (const ASTNode* statement : program.statements) {
        const auto* callable = dynamic_cast<const CallableDeclarationNode*>(statement);
        if (callable != nullptr && (callable->classical_body.has_value() ||
                                    callable->binary_classical_body.has_value())) return true;
        const auto* declaration = dynamic_cast<const DeclarationNode*>(statement);
        if (declaration != nullptr && (declaration->classical_callable_invocation.has_value() ||
                                       declaration->binary_classical_callable_invocation.has_value())) return true;
    }
    return false;
}

}  // namespace

extern "C" unsigned int synq_abi_version(void) {
    return SYNQ_ABI_VERSION;
}

extern "C" const char* synq_version(void) {
    return "synq-c-abi/1";
}

extern "C" synq_status synq_parse_file(const char* utf8_path,
                                        synq_program** out_program,
                                        char** out_diagnostic) {
    clear_output(out_diagnostic);
    if (out_program == nullptr) {
        return return_error(SYNQ_STATUS_INVALID_ARGUMENT, "out_program must not be NULL", out_diagnostic);
    }
    *out_program = nullptr;
    if (utf8_path == nullptr || utf8_path[0] == '\0') {
        return return_error(SYNQ_STATUS_INVALID_ARGUMENT, "utf8_path must be a non-empty UTF-8 path", out_diagnostic);
    }

    try {
        Parser parser;
        synq::compiler::ParseResult result = parser.parseFileWithDiagnostics(utf8_path);
        if (!result.ok()) {
            const std::string diagnostic = result.diagnostics.empty()
                ? "SynQ parser rejected the source file without a diagnostic"
                : synq::compiler::format_diagnostic(utf8_path, result.diagnostics.front());
            return return_error(SYNQ_STATUS_PARSE_ERROR, diagnostic, out_diagnostic);
        }

        std::unique_ptr<synq_program> handle(new synq_program());
        handle->program = result.take_program();
        *out_program = handle.release();
        return SYNQ_STATUS_OK;
    } catch (const std::bad_alloc&) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ could not allocate parser state", out_diagnostic);
    } catch (const std::exception&) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ parser raised an internal exception", out_diagnostic);
    } catch (...) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ parser raised an unknown internal exception", out_diagnostic);
    }
}

extern "C" synq_status synq_parse_source(const char* utf8_source,
                                          synq_program** out_program,
                                          char** out_diagnostic) {
    clear_output(out_diagnostic);
    if (out_program == nullptr) {
        return return_error(SYNQ_STATUS_INVALID_ARGUMENT, "out_program must not be NULL", out_diagnostic);
    }
    *out_program = nullptr;
    if (utf8_source == nullptr) {
        return return_error(SYNQ_STATUS_INVALID_ARGUMENT, "utf8_source must not be NULL", out_diagnostic);
    }

    try {
        Parser parser;
        synq::compiler::ParseResult result = parser.parseSourceWithDiagnostics(utf8_source);
        if (!result.ok()) {
            const std::string diagnostic = result.diagnostics.empty()
                ? "SynQ parser rejected the in-memory source without a diagnostic"
                : synq::compiler::format_diagnostic("<memory>", result.diagnostics.front());
            return return_error(SYNQ_STATUS_PARSE_ERROR, diagnostic, out_diagnostic);
        }

        std::unique_ptr<synq_program> handle(new synq_program());
        handle->program = result.take_program();
        *out_program = handle.release();
        return SYNQ_STATUS_OK;
    } catch (const std::bad_alloc&) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ could not allocate parser state", out_diagnostic);
    } catch (const std::exception&) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ parser raised an internal exception", out_diagnostic);
    } catch (...) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ parser raised an unknown internal exception", out_diagnostic);
    }
}

extern "C" synq_status synq_export_openqasm3(const synq_program* program,
                                              char** out_openqasm3,
                                              char** out_diagnostic) {
    clear_output(out_openqasm3);
    clear_output(out_diagnostic);
    if (out_openqasm3 == nullptr) {
        return return_error(SYNQ_STATUS_INVALID_ARGUMENT, "out_openqasm3 must not be NULL", out_diagnostic);
    }
    if (program == nullptr || program->program == nullptr) {
        return return_error(SYNQ_STATUS_INVALID_ARGUMENT, "program must not be NULL", out_diagnostic);
    }

    try {
        if (contains_classical_callable_runtime_node(*program->program)) {
            return return_error(SYNQ_STATUS_EXPORT_ERROR,
                                "experimental C ABI explicitly rejects Alpha classical callable runtime nodes; use the compiler CLI --eval-runtime path instead",
                                out_diagnostic);
        }
        if (contains_measurement_feedback_node(*program->program)) {
            return return_error(SYNQ_STATUS_EXPORT_ERROR,
                                "experimental C ABI explicitly rejects Alpha measurement-feedback nodes; use the compiler CLI strict Hybrid OpenQASM or bounded local simulation paths instead",
                                out_diagnostic);
        }
        if (contains_parameterized_routine_node(*program->program)) {
            return return_error(SYNQ_STATUS_EXPORT_ERROR,
                                "experimental C ABI explicitly rejects Alpha parameterized quantum routine nodes; use the compiler CLI strict Hybrid OpenQASM path instead",
                                out_diagnostic);
        }
        const synq::compiler::OpenQasm3ExportResult result = synq::compiler::export_openqasm3(*program->program);
        if (!result.ok()) {
            std::string diagnostic;
            for (std::size_t index = 0; index < result.diagnostics.size(); ++index) {
                if (index != 0) diagnostic += '\n';
                diagnostic += result.diagnostics[index];
            }
            return return_error(SYNQ_STATUS_EXPORT_ERROR, diagnostic, out_diagnostic);
        }

        *out_openqasm3 = copy_utf8_string(result.program);
        if (*out_openqasm3 == nullptr) {
            return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ could not allocate OpenQASM output", out_diagnostic);
        }
        return SYNQ_STATUS_OK;
    } catch (const std::bad_alloc&) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ could not allocate exporter state", out_diagnostic);
    } catch (const std::exception&) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ exporter raised an internal exception", out_diagnostic);
    } catch (...) {
        return return_error(SYNQ_STATUS_INTERNAL_ERROR, "SynQ exporter raised an unknown internal exception", out_diagnostic);
    }
}

extern "C" void synq_string_free(char* value) {
    std::free(value);
}

extern "C" void synq_program_free(synq_program* program) {
    delete program;
}
