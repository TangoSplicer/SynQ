#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "synq/synq_ffi.h"

static int require(int condition, const char* message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main(void) {
    const char* path = "synq_c_abi_smoke.synq";
    const char* semantic_path = "synq_c_abi_semantic_smoke.synq";
    const char* duplicate_path = "synq_c_abi_duplicate_smoke.synq";
    const char* measurement_path = "synq_c_abi_measurement_smoke.synq";
    const char* in_memory_source = "quantum h q[0]\nmeasure q[0]\n";
    const char* typed_in_memory_source =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "#[experimental(feature = \"callable-declarations\")]\n"
        "qubit q[1]\n"
        "measure q[0] as observed\n"
        "if observed then quantum h q[0]\n"
        "fn prepare()\n"
        "kernel ansatz()\n";
    const char* parameterized_routine_in_memory_source =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"callable-declarations\")]\n"
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "#[experimental(feature = \"parameterized-quantum-routines\")]\n"
        "qubit q[1]\n"
        "kernel rotate(angle theta, qubit target) { quantum rz(theta) target }\n"
        "call rotate(pi/2, q[0])\n";
    const char* measurement_feedback_in_memory_source =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "#[experimental(feature = \"measurement-feedback\")]\n"
        "qubit q[2]\n"
        "measure q[0] as observed\n"
        "if observed then quantum x q[1]\n";
    const char* classical_callable_runtime_in_memory_source =
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "fn increment(value: Integer) -> value + 1\n"
        "let answer = increment(41)\n";
    FILE* source = fopen(path, "w");
    synq_program* program = NULL;
    char* diagnostic = NULL;
    char* openqasm = NULL;
    synq_status status;

    if (!require(synq_abi_version() == SYNQ_ABI_VERSION, "consumer observes ABI version 1")) return 1;
    if (!require(strcmp(synq_version(), "synq-c-abi/1") == 0, "consumer observes stable ABI identifier")) return 1;
    if (!require(source != NULL, "consumer opens a source fixture")) return 1;
    fputs("#[experimental(feature = \"parameterized-quantum-gates\")]\n", source);
    fputs("quantum h q[0]\n", source);
    fputs("quantum rx(pi/2) q[1]\n", source);
    fputs("measure q[1]\n", source);
    fclose(source);

    status = synq_parse_file(path, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK, "C consumer parses a supported source file")) {
        synq_string_free(diagnostic);
        remove(path);
        return 1;
    }
    if (!require(program != NULL, "C consumer receives an opaque program handle")) {
        remove(path);
        return 1;
    }
    if (!require(diagnostic == NULL, "successful parse has no allocated diagnostic")) {
        synq_program_free(program);
        synq_string_free(diagnostic);
        remove(path);
        return 1;
    }

    status = synq_export_openqasm3(program, &openqasm, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK, "C consumer exports supported OpenQASM 3")) {
        synq_program_free(program);
        synq_string_free(diagnostic);
        remove(path);
        return 1;
    }
    if (!require(openqasm != NULL, "C consumer receives library-owned OpenQASM source")) {
        synq_program_free(program);
        remove(path);
        return 1;
    }
    if (!require(strstr(openqasm, "qubit[2] q;") != NULL, "OpenQASM output preserves qubit extent")) {
        synq_string_free(openqasm);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    if (!require(strstr(openqasm, "rx(pi/2) q[1];") != NULL, "OpenQASM output preserves parameterized gate")) {
        synq_string_free(openqasm);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    if (!require(strstr(openqasm, "bit[2] c;") != NULL &&
                 strstr(openqasm, "c[1] = measure q[1];") != NULL,
                 "OpenQASM output preserves bounded measurement lowering")) {
        synq_string_free(openqasm);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    synq_string_free(openqasm);
    synq_string_free(diagnostic);
    synq_program_free(program);

    program = NULL;
    diagnostic = NULL;
    status = synq_parse_source(in_memory_source, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && program != NULL && diagnostic == NULL,
                 "C consumer parses bounded source text directly from memory")) {
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    openqasm = NULL;
    status = synq_export_openqasm3(program, &openqasm, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && openqasm != NULL && diagnostic == NULL &&
                 strstr(openqasm, "c[0] = measure q[0];") != NULL,
                 "C consumer exports a program parsed from in-memory source")) {
        synq_string_free(openqasm);
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    synq_string_free(openqasm);
    synq_program_free(program);

    program = NULL;
    diagnostic = NULL;
    status = synq_parse_source(typed_in_memory_source, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && program != NULL && diagnostic == NULL,
                 "C consumer parses typed Alpha declarations through the opaque ABI")) {
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    openqasm = NULL;
    diagnostic = NULL;
    status = synq_export_openqasm3(program, &openqasm, &diagnostic);
    if (!require(status == SYNQ_STATUS_EXPORT_ERROR && openqasm == NULL && diagnostic != NULL,
                 "C consumer receives an explicit export error for unsupported typed constructs")) {
        synq_string_free(openqasm);
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    synq_string_free(diagnostic);
    synq_program_free(program);

    program = NULL;
    diagnostic = NULL;
    status = synq_parse_source(parameterized_routine_in_memory_source, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && program != NULL && diagnostic == NULL,
                 "C consumer parses an Alpha parameterized routine as an opaque source artifact")) {
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    openqasm = NULL;
    diagnostic = NULL;
    status = synq_export_openqasm3(program, &openqasm, &diagnostic);
    if (!require(status == SYNQ_STATUS_EXPORT_ERROR && openqasm == NULL && diagnostic != NULL &&
                 strstr(diagnostic, "explicitly rejects Alpha parameterized quantum routine") != NULL,
                 "C ABI explicitly rejects Alpha parameterized routine execution/export nodes")) {
        synq_string_free(openqasm);
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    synq_string_free(diagnostic);
    synq_program_free(program);

    program = NULL;
    diagnostic = NULL;
    status = synq_parse_source(measurement_feedback_in_memory_source, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && program != NULL && diagnostic == NULL,
                 "C consumer parses an Alpha measurement-feedback source artifact")) {
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    openqasm = NULL;
    diagnostic = NULL;
    status = synq_export_openqasm3(program, &openqasm, &diagnostic);
    if (!require(status == SYNQ_STATUS_EXPORT_ERROR && openqasm == NULL && diagnostic != NULL &&
                 strstr(diagnostic, "explicitly rejects Alpha measurement-feedback") != NULL,
                 "C ABI explicitly rejects Alpha measurement-feedback execution/export nodes")) {
        synq_string_free(openqasm);
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    synq_string_free(diagnostic);
    synq_program_free(program);

    program = NULL;
    diagnostic = NULL;
    status = synq_parse_source(classical_callable_runtime_in_memory_source, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && program != NULL && diagnostic == NULL,
                 "C consumer parses an Alpha classical callable-runtime source artifact")) {
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    openqasm = NULL;
    diagnostic = NULL;
    status = synq_export_openqasm3(program, &openqasm, &diagnostic);
    if (!require(status == SYNQ_STATUS_EXPORT_ERROR && openqasm == NULL && diagnostic != NULL &&
                 strstr(diagnostic, "explicitly rejects Alpha classical callable runtime") != NULL,
                 "C ABI explicitly rejects Alpha classical callable runtime execution/export nodes")) {
        synq_string_free(openqasm);
        synq_string_free(diagnostic);
        synq_program_free(program);
        remove(path);
        return 1;
    }
    synq_string_free(diagnostic);
    synq_program_free(program);

    program = NULL;
    diagnostic = NULL;
    status = synq_parse_file("synq_missing_c_abi_file.synq", &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_PARSE_ERROR, "C consumer receives a parse error for a missing file")) {
        synq_string_free(diagnostic);
        remove(path);
        return 1;
    }
    if (!require(program == NULL, "failed parse does not create an opaque handle")) {
        synq_string_free(diagnostic);
        remove(path);
        return 1;
    }
    if (!require(diagnostic != NULL, "failed parse produces a library-owned diagnostic")) {
        remove(path);
        return 1;
    }
    if (!require(strstr(diagnostic, "SYNQ-P001") != NULL, "C consumer receives the structured parser diagnostic code")) {
        synq_string_free(diagnostic);
        remove(path);
        return 1;
    }
    synq_string_free(diagnostic);
    remove(path);

    source = fopen(semantic_path, "w");
    if (!require(source != NULL, "consumer opens a semantic-error source fixture")) return 1;
    fputs("quantum cx q[0]\n", source);
    fclose(source);
    program = NULL;
    diagnostic = NULL;
    status = synq_parse_file(semantic_path, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_PARSE_ERROR && program == NULL,
                 "C consumer receives a parse error for a malformed known gate shape")) {
        synq_string_free(diagnostic);
        remove(semantic_path);
        return 1;
    }
    if (!require(diagnostic != NULL && strstr(diagnostic, "SYNQ-S002") != NULL,
                 "C consumer receives the structured semantic diagnostic code")) {
        synq_string_free(diagnostic);
        remove(semantic_path);
        return 1;
    }
    synq_string_free(diagnostic);
    remove(semantic_path);

    source = fopen(duplicate_path, "w");
    if (!require(source != NULL, "consumer opens a duplicate-declaration source fixture")) return 1;
    fputs("let theta = 0.5\n", source);
    fputs("let theta = 1.0\n", source);
    fclose(source);
    program = NULL;
    diagnostic = NULL;
    status = synq_parse_file(duplicate_path, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_PARSE_ERROR && program == NULL,
                 "C consumer receives a parse error for a duplicate declaration")) {
        synq_string_free(diagnostic);
        remove(duplicate_path);
        return 1;
    }
    if (!require(diagnostic != NULL && strstr(diagnostic, "SYNQ-S004") != NULL,
                 "C consumer receives the duplicate-declaration semantic diagnostic code")) {
        synq_string_free(diagnostic);
        remove(duplicate_path);
        return 1;
    }
    synq_string_free(diagnostic);
    remove(duplicate_path);

    source = fopen(measurement_path, "w");
    if (!require(source != NULL, "consumer opens a malformed-measurement source fixture")) return 1;
    fputs("measure q[0], q[1]\n", source);
    fclose(source);
    program = NULL;
    diagnostic = NULL;
    status = synq_parse_file(measurement_path, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_PARSE_ERROR && program == NULL,
                 "C consumer receives a parse error for a malformed measurement")) {
        synq_string_free(diagnostic);
        remove(measurement_path);
        return 1;
    }
    if (!require(diagnostic != NULL && strstr(diagnostic, "SYNQ-P008") != NULL,
                 "C consumer receives the malformed-measurement diagnostic code")) {
        synq_string_free(diagnostic);
        remove(measurement_path);
        return 1;
    }
    synq_string_free(diagnostic);
    remove(measurement_path);

    program = NULL;
    diagnostic = NULL;
    status = synq_parse_source("measure q[0], q[1]", &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_PARSE_ERROR && program == NULL,
                 "C consumer receives a parse error for malformed in-memory source")) {
        synq_string_free(diagnostic);
        return 1;
    }
    if (!require(diagnostic != NULL && strstr(diagnostic, "<memory>:1") != NULL &&
                 strstr(diagnostic, "SYNQ-P008") != NULL,
                 "C consumer receives a source-labelled in-memory diagnostic")) {
        synq_string_free(diagnostic);
        return 1;
    }
    synq_string_free(diagnostic);

    puts("SynQ C ABI smoke test passed");
    return 0;
}
