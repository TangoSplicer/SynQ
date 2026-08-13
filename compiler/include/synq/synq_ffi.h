/*
 * SynQ C ABI, version 1.
 *
 * This header is the supported native interoperability boundary for the
 * recovered compiler profile. It deliberately exposes neither C++ types nor a
 * complete SynQ language runtime.
 */
#ifndef SYNQ_SYNQ_FFI_H
#define SYNQ_SYNQ_FFI_H

#ifdef __cplusplus
extern "C" {
#endif

#define SYNQ_ABI_VERSION 1u

typedef struct synq_program synq_program;

typedef enum synq_status {
    SYNQ_STATUS_OK = 0,
    SYNQ_STATUS_INVALID_ARGUMENT = 1,
    SYNQ_STATUS_PARSE_ERROR = 2,
    SYNQ_STATUS_EXPORT_ERROR = 3,
    SYNQ_STATUS_INTERNAL_ERROR = 4
} synq_status;

/* Returns the ABI major version implemented by this library. */
unsigned int synq_abi_version(void);

/*
 * Returns a static, NUL-terminated ABI identifier. The caller must not modify
 * or free this pointer.
 */
const char* synq_version(void);

/*
 * Parses a UTF-8 source-file path into an opaque program handle.
 *
 * On success, `*out_program` receives a handle that must be released with
 * `synq_program_free`. On failure, it remains NULL. If `out_diagnostic` is
 * non-NULL, the function stores either NULL on success or an allocated UTF-8
 * diagnostic string on failure. Release an allocated diagnostic with
 * `synq_string_free`.
 */
synq_status synq_parse_file(const char* utf8_path,
                            synq_program** out_program,
                            char** out_diagnostic);

/*
 * Exports the bounded, supported quantum subset of an opaque parsed program as
 * UTF-8 OpenQASM 3 source. Output and diagnostic ownership follow the same
 * rules as `synq_parse_file`.
 */
synq_status synq_export_openqasm3(const synq_program* program,
                                  char** out_openqasm3,
                                  char** out_diagnostic);

/* Releases a string allocated by this library. NULL is accepted. */
void synq_string_free(char* value);

/* Releases an opaque program handle allocated by `synq_parse_file`. NULL is accepted. */
void synq_program_free(synq_program* program);

#ifdef __cplusplus
}
#endif

#endif
