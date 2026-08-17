use synq_alpha::{abi_identifier, parse_source, Status, ABI_VERSION};

#[test]
fn parses_and_exports_the_bounded_c_abi_subset() {
    assert_eq!(ABI_VERSION, 1);
    assert_eq!(abi_identifier(), "synq-c-abi/1");

    let program = parse_source("quantum h q[0]\nmeasure q[0]\n")
        .expect("supported source should produce a Rust-owned opaque program");
    let qasm = program
        .export_openqasm3()
        .expect("Rust-owned opaque program should export the bounded OpenQASM subset");
    assert!(qasm.contains("OPENQASM 3.0;"));
    assert!(qasm.contains("c[0] = measure q[0];"));
}

#[test]
fn preserves_structured_parse_failure_without_an_opaque_handle() {
    let error = parse_source("measure q[0], q[1]")
        .expect_err("malformed bounded source must remain a structured parse error");
    assert_eq!(error.status(), Status::Parse);
    assert!(error.diagnostic().contains("SYNQ-P008"));
}

#[test]
fn rejects_interior_nul_without_calling_the_native_abi() {
    let error = parse_source("quantum h q[0]\0")
        .expect_err("Rust wrapper must reject C-string-incompatible source locally");
    assert_eq!(error.status(), Status::InvalidArgument);
    assert!(error.diagnostic().contains("interior NUL"));
}
