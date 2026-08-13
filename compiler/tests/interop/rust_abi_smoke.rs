use std::ffi::{CStr, CString};
use std::os::raw::{c_char, c_uint};
use std::ptr;

#[repr(C)]
struct SynqProgram {
    _private: [u8; 0],
}

const SYNQ_STATUS_OK: i32 = 0;
const SYNQ_STATUS_PARSE_ERROR: i32 = 2;

extern "C" {
    fn synq_abi_version() -> c_uint;
    fn synq_version() -> *const c_char;
    fn synq_parse_source(
        utf8_source: *const c_char,
        out_program: *mut *mut SynqProgram,
        out_diagnostic: *mut *mut c_char,
    ) -> i32;
    fn synq_export_openqasm3(
        program: *const SynqProgram,
        out_openqasm3: *mut *mut c_char,
        out_diagnostic: *mut *mut c_char,
    ) -> i32;
    fn synq_string_free(value: *mut c_char);
    fn synq_program_free(program: *mut SynqProgram);
}

unsafe fn take_owned_string(value: *mut c_char) -> String {
    if value.is_null() {
        return String::new();
    }
    let copied = CStr::from_ptr(value).to_string_lossy().into_owned();
    synq_string_free(value);
    copied
}

fn require(condition: bool, message: &str) -> Result<(), String> {
    if condition {
        Ok(())
    } else {
        Err(message.to_owned())
    }
}

fn run() -> Result<(), String> {
    unsafe {
        require(synq_abi_version() == 1, "Rust consumer observes C ABI version 1")?;
        let version = CStr::from_ptr(synq_version()).to_string_lossy();
        require(version.as_ref() == "synq-c-abi/1", "Rust consumer observes the v1 ABI identifier")?;

        let source_c = CString::new(
            "#[experimental(feature = \"parameterized-quantum-gates\")]\nquantum h q[0]\nquantum rx(pi/2) q[1]\nmeasure q[1]\n",
        )
        .map_err(|_| "Rust source fixture contains an interior NUL byte".to_owned())?;
        let mut program: *mut SynqProgram = ptr::null_mut();
        let mut diagnostic: *mut c_char = ptr::null_mut();
        let parse_status = synq_parse_source(source_c.as_ptr(), &mut program, &mut diagnostic);
        if parse_status != SYNQ_STATUS_OK {
            let message = take_owned_string(diagnostic);
            return Err(format!("Rust consumer could not parse supported in-memory source: {message}"));
        }
        require(!program.is_null(), "successful Rust in-memory parse returns an opaque program handle")?;
        if !diagnostic.is_null() {
            synq_string_free(diagnostic);
            synq_program_free(program);
            return Err("successful Rust parse unexpectedly returned a diagnostic".to_owned());
        }

        let mut openqasm: *mut c_char = ptr::null_mut();
        diagnostic = ptr::null_mut();
        let export_status = synq_export_openqasm3(program, &mut openqasm, &mut diagnostic);
        if export_status != SYNQ_STATUS_OK {
            let message = take_owned_string(diagnostic);
            synq_program_free(program);
            return Err(format!("Rust consumer could not export supported source: {message}"));
        }
        let exported = take_owned_string(openqasm);
        synq_program_free(program);
        require(exported.contains("bit[2] c;") && exported.contains("c[1] = measure q[1];"),
                "Rust consumer receives bounded measurement OpenQASM output from memory source")?;
        if !diagnostic.is_null() {
            synq_string_free(diagnostic);
            return Err("successful Rust export unexpectedly returned a diagnostic".to_owned());
        }

        let malformed_c = CString::new("measure q[0], q[1]")
            .map_err(|_| "Rust malformed source fixture contains an interior NUL byte".to_owned())?;
        program = ptr::null_mut();
        diagnostic = ptr::null_mut();
        let malformed_status = synq_parse_source(malformed_c.as_ptr(), &mut program, &mut diagnostic);
        require(malformed_status == SYNQ_STATUS_PARSE_ERROR && program.is_null(),
                "Rust consumer receives a parse error without an opaque handle for malformed in-memory source")?;
        let malformed_diagnostic = take_owned_string(diagnostic);
        require(malformed_diagnostic.contains("<memory>:1") && malformed_diagnostic.contains("SYNQ-P008"),
                "Rust consumer receives the structured in-memory measurement diagnostic code")?;
    }
    Ok(())
}

fn main() {
    if let Err(message) = run() {
        eprintln!("FAIL: {message}");
        std::process::exit(1);
    }
    println!("SynQ Rust C ABI smoke test passed");
}
