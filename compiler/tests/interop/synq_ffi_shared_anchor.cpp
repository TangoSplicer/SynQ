// Test-only shared-library anchor. Referencing one public C ABI symbol causes
// the static recovery library to contribute the implementation object without
// exposing an alternate interface or changing the C ABI contract.
#include "synq/synq_ffi.h"

extern "C" unsigned int synq_ffi_shared_anchor(void) {
    return synq_abi_version();
}
