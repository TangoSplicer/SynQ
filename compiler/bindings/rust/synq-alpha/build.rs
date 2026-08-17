use std::env;

fn main() {
    println!("cargo:rerun-if-env-changed=SYNQ_FFI_DIR");
    let native_dir = env::var("SYNQ_FFI_DIR")
        .expect("SYNQ_FFI_DIR must point to the SynQ C ABI shared-library directory");
    println!("cargo:rustc-link-search=native={native_dir}");
    println!("cargo:rustc-link-lib=dylib=synq_ffi");
}
