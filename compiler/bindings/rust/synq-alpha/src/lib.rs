//! Experimental, source-based Rust ownership wrapper for SynQ C ABI v1.
//!
//! This crate intentionally exposes only parse-from-source and bounded OpenQASM
//! export. It is not a registry release, stable Rust API, language runtime, or
//! provider integration layer.

use std::ffi::{CStr, CString};
use std::fmt;
use std::os::raw::{c_char, c_uint};
use std::ptr;

pub const ABI_VERSION: u32 = 1;

#[repr(C)]
struct RawProgram {
    _private: [u8; 0],
}

extern "C" {
    fn synq_abi_version() -> c_uint;
    fn synq_version() -> *const c_char;
    fn synq_parse_source(
        utf8_source: *const c_char,
        out_program: *mut *mut RawProgram,
        out_diagnostic: *mut *mut c_char,
    ) -> i32;
    fn synq_export_openqasm3(
        program: *const RawProgram,
        out_openqasm3: *mut *mut c_char,
        out_diagnostic: *mut *mut c_char,
    ) -> i32;
    fn synq_string_free(value: *mut c_char);
    fn synq_program_free(program: *mut RawProgram);
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Status {
    InvalidArgument,
    Parse,
    Export,
    Internal,
    Unexpected(i32),
}

impl Status {
    fn from_raw(value: i32) -> Self {
        match value {
            1 => Self::InvalidArgument,
            2 => Self::Parse,
            3 => Self::Export,
            4 => Self::Internal,
            other => Self::Unexpected(other),
        }
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Error {
    status: Status,
    diagnostic: String,
}

impl Error {
    pub fn status(&self) -> Status {
        self.status
    }

    pub fn diagnostic(&self) -> &str {
        &self.diagnostic
    }
}

impl fmt::Display for Error {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(formatter, "SynQ {:?}: {}", self.status, self.diagnostic)
    }
}

impl std::error::Error for Error {}

#[derive(Debug)]
pub struct Program {
    raw: *mut RawProgram,
}

impl Drop for Program {
    fn drop(&mut self) {
        unsafe { synq_program_free(self.raw) };
    }
}

unsafe fn take_owned_string(value: *mut c_char) -> String {
    if value.is_null() {
        return String::new();
    }
    let copied = CStr::from_ptr(value).to_string_lossy().into_owned();
    synq_string_free(value);
    copied
}

fn abi_error() -> Option<Error> {
    let version = unsafe { synq_abi_version() };
    if version == ABI_VERSION {
        None
    } else {
        Some(Error {
            status: Status::Unexpected(version as i32),
            diagnostic: format!("requires SynQ C ABI v{ABI_VERSION}, found v{version}"),
        })
    }
}

pub fn abi_identifier() -> &'static str {
    unsafe {
        let value = synq_version();
        if value.is_null() {
            ""
        } else {
            // The ABI specifies a static NUL-terminated identifier.
            CStr::from_ptr(value).to_str().unwrap_or("")
        }
    }
}

pub fn parse_source(source: &str) -> Result<Program, Error> {
    if let Some(error) = abi_error() {
        return Err(error);
    }
    let source = CString::new(source).map_err(|_| Error {
        status: Status::InvalidArgument,
        diagnostic: "source contains an interior NUL byte".to_owned(),
    })?;
    let mut program = ptr::null_mut();
    let mut diagnostic = ptr::null_mut();
    let status = unsafe { synq_parse_source(source.as_ptr(), &mut program, &mut diagnostic) };
    let message = unsafe { take_owned_string(diagnostic) };
    if status == 0 && !program.is_null() {
        return Ok(Program { raw: program });
    }
    if !program.is_null() {
        unsafe { synq_program_free(program) };
    }
    Err(Error {
        status: Status::from_raw(status),
        diagnostic: if message.is_empty() { "native parse failed without a diagnostic".to_owned() } else { message },
    })
}

impl Program {
    pub fn export_openqasm3(&self) -> Result<String, Error> {
        let mut output = ptr::null_mut();
        let mut diagnostic = ptr::null_mut();
        let status = unsafe { synq_export_openqasm3(self.raw, &mut output, &mut diagnostic) };
        let message = unsafe { take_owned_string(diagnostic) };
        let qasm = unsafe { take_owned_string(output) };
        if status == 0 {
            return Ok(qasm);
        }
        Err(Error {
            status: Status::from_raw(status),
            diagnostic: if message.is_empty() { "native export failed without a diagnostic".to_owned() } else { message },
        })
    }
}
