use std::ffi::{CStr, CString};
use std::fs;
use std::os::raw::{c_char, c_uint};
use std::path::PathBuf;
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
    fn synq_parse_file(
        utf8_path: *const c_char,
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

fn fixture_path(name: &str) -> PathBuf {
    std::env::temp_dir().join(format!("synq_rust_abi_{}_{}", std::process::id(), name))
}

fn run() -> Result<(), String> {
    unsafe {
        require(synq_abi_version() == 1, "Rust consumer observes C ABI version 1")?;
        let version = CStr::from_ptr(synq_version()).to_string_lossy();
        require(version.as_ref() == "synq-c-abi/1", "Rust consumer observes the v1 ABI identifier")?;

        let source_path = fixture_path("valid.synq");
        fs::write(
            &source_path,
            "#[experimental(feature = \"parameterized-quantum-gates\")]\nquantum h q[0]\nquantum rx(pi/2) q[1]\nmeasure q[1]\n",
        )
        .map_err(|error| format!("could not write Rust fixture: {error}"))?;
        let source_c = CString::new(source_path.to_string_lossy().as_bytes())
            .map_err(|_| "Rust fixture path contains an interior NUL byte".to_owned())?;
        let mut program: *mut SynqProgram = ptr::null_mut();
        let mut diagnostic: *mut c_char = ptr::null_mut();
        let parse_status = synq_parse_file(source_c.as_ptr(), &mut program, &mut diagnostic);
        if parse_status != SYNQ_STATUS_OK {
            let message = take_owned_string(diagnostic);
            let _ = fs::remove_file(&source_path);
            return Err(format!("Rust consumer could not parse supported source: {message}"));
        }
        require(!program.is_null(), "successful Rust parse returns an opaque program handle")?;
        if !diagnostic.is_null() {
            synq_string_free(diagnostic);
            synq_program_free(program);
            let _ = fs::remove_file(&source_path);
            return Err("successful Rust parse unexpectedly returned a diagnostic".to_owned());
        }

        let mut openqasm: *mut c_char = ptr::null_mut();
        diagnostic = ptr::null_mut();
        let export_status = synq_export_openqasm3(program, &mut openqasm, &mut diagnostic);
        if export_status != SYNQ_STATUS_OK {
            let message = take_owned_string(diagnostic);
            synq_program_free(program);
            let _ = fs::remove_file(&source_path);
            return Err(format!("Rust consumer could not export supported source: {message}"));
        }
        let exported = take_owned_string(openqasm);
        synq_program_free(program);
        let _ = fs::remove_file(&source_path);
        require(exported.contains("bit[2] c;") && exported.contains("c[1] = measure q[1];"),
                "Rust consumer receives bounded measurement OpenQASM output")?;
        if !diagnostic.is_null() {
            synq_string_free(diagnostic);
            return Err("successful Rust export unexpectedly returned a diagnostic".to_owned());
        }

        let missing_path = fixture_path("missing.synq");
        let missing_c = CString::new(missing_path.to_string_lossy().as_bytes())
            .map_err(|_| "Rust missing-fixture path contains an interior NUL byte".to_owned())?;
        program = ptr::null_mut();
        diagnostic = ptr::null_mut();
        let missing_status = synq_parse_file(missing_c.as_ptr(), &mut program, &mut diagnostic);
        require(missing_status == SYNQ_STATUS_PARSE_ERROR && program.is_null(),
                "Rust consumer receives a parse error without an opaque handle for a missing file")?;
        let missing_diagnostic = take_owned_string(diagnostic);
        require(missing_diagnostic.contains("SYNQ-P001"),
                "Rust consumer receives the structured missing-file diagnostic code")?;
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
