:- module mercury_abi_smoke.
:- interface.

:- import_module io.

:- pred main(io::di, io::uo) is det.

:- implementation.

:- import_module require.

:- pragma foreign_decl("C", "
    #include <string.h>
    #include \"synq/synq_ffi.h\"
").

:- pred abi_identity is semidet.
:- pragma foreign_proc("C",
    abi_identity,
    [will_not_call_mercury, promise_pure, will_not_modify_trail,
        does_not_affect_liveness],
"
    SUCCESS_INDICATOR =
        synq_abi_version() == SYNQ_ABI_VERSION &&
        strcmp(synq_version(), \"synq-c-abi/1\") == 0;
").

:- pred in_memory_measurement_flow is semidet.
:- pragma foreign_proc("C",
    in_memory_measurement_flow,
    [will_not_call_mercury, promise_pure, will_not_modify_trail,
        does_not_affect_liveness],
"
    synq_program *program = NULL;
    char *diagnostic = NULL;
    char *openqasm = NULL;
    synq_status status;

    SUCCESS_INDICATOR = MR_FALSE;
    status = synq_parse_source(\"quantum h q[0]\\nmeasure q[0]\\n\",
        &program, &diagnostic);
    if (status == SYNQ_STATUS_OK && program != NULL && diagnostic == NULL) {
        status = synq_export_openqasm3(program, &openqasm, &diagnostic);
        if (status == SYNQ_STATUS_OK && openqasm != NULL && diagnostic == NULL &&
            strstr(openqasm, \"bit[1] c;\") != NULL &&
            strstr(openqasm, \"c[0] = measure q[0];\") != NULL) {
            SUCCESS_INDICATOR = MR_TRUE;
        }
    }

    synq_string_free(openqasm);
    synq_string_free(diagnostic);
    synq_program_free(program);
").

:- pred malformed_measurement_diagnostic is semidet.
:- pragma foreign_proc("C",
    malformed_measurement_diagnostic,
    [will_not_call_mercury, promise_pure, will_not_modify_trail,
        does_not_affect_liveness],
"
    synq_program *program = NULL;
    char *diagnostic = NULL;
    synq_status status;

    status = synq_parse_source(\"measure q[0], q[1]\", &program, &diagnostic);
    SUCCESS_INDICATOR =
        status == SYNQ_STATUS_PARSE_ERROR && program == NULL && diagnostic != NULL &&
        strstr(diagnostic, \"<memory>:1\") != NULL &&
        strstr(diagnostic, \"SYNQ-P008\") != NULL;

    synq_string_free(diagnostic);
    synq_program_free(program);
").

main(!IO) :-
    ( if abi_identity then
        true
    else
        require.error("Mercury consumer did not observe SynQ C ABI v1 identity")
    ),
    ( if in_memory_measurement_flow then
        true
    else
        require.error("Mercury consumer did not complete SynQ in-memory parse/export ownership flow")
    ),
    ( if malformed_measurement_diagnostic then
        true
    else
        require.error("Mercury consumer did not receive SynQ malformed-measurement diagnostic")
    ).
