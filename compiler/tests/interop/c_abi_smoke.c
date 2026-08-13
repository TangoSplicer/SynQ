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
    const char* path = "/tmp/synq_c_abi_smoke.synq";
    const char* semantic_path = "/tmp/synq_c_abi_semantic_smoke.synq";
    const char* duplicate_path = "/tmp/synq_c_abi_duplicate_smoke.synq";
    const char* measurement_path = "/tmp/synq_c_abi_measurement_smoke.synq";
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
    status = synq_parse_file("/tmp/synq_missing_c_abi_file.synq", &program, &diagnostic);
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

    puts("SynQ C ABI smoke test passed");
    return 0;
}
