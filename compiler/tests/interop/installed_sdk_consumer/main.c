#include <stdio.h>
#include <string.h>

#include <synq/synq_ffi.h>

static int require(int condition, const char* message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main(void) {
    const char* source = "quantum h q[0]\nmeasure q[0]\n";
    synq_program* program = NULL;
    char* diagnostic = NULL;
    char* openqasm = NULL;
    synq_status status;

    if (!require(synq_abi_version() == SYNQ_ABI_VERSION,
                 "installed consumer observes ABI v1")) return 1;
    if (!require(strcmp(synq_version(), "synq-c-abi/1") == 0,
                 "installed consumer observes ABI identity")) return 1;

    status = synq_parse_source(source, &program, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && program != NULL && diagnostic == NULL,
                 "installed consumer parses bounded in-memory source")) {
        synq_string_free(diagnostic);
        synq_program_free(program);
        return 1;
    }

    status = synq_export_openqasm3(program, &openqasm, &diagnostic);
    if (!require(status == SYNQ_STATUS_OK && openqasm != NULL && diagnostic == NULL &&
                 strstr(openqasm, "c[0] = measure q[0];") != NULL,
                 "installed consumer exports bounded OpenQASM and owns its output")) {
        synq_string_free(openqasm);
        synq_string_free(diagnostic);
        synq_program_free(program);
        return 1;
    }

    synq_string_free(openqasm);
    synq_program_free(program);
    return 0;
}
