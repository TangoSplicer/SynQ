# synqc Packaging Status and Specification Index

**Status:** Published `v0.1.0-experimental` GitHub pre-release with verified
CLI-only archives. It remains an unsigned, experimental delivery path—not a
stable runtime or ABI release, installer, package-manager package, signing, or
notarization announcement.

## Current delivery position

The recovery CMake profile builds `synqc 0.1.0-experimental`, installs it with
`cmake --install`, and configures a local ZIP-oriented CPack path. The new
tag-aware candidate workflow creates CLI-only Linux, macOS ARM64, and
Windows X64 archives; writes JSON and SHA-256 manifests; enforces a contents
allowlist; extracts each exact archive; runs CLI smoke checks; generates GitHub
provenance attestations; and retains the results as CI artifacts. The workflow
passed as [synqc Release Candidate #32482750179](https://github.com/TangoSplicer/SynQ/actions/runs/32482750179). Its optional manual tag input checks out and resolves an existing tag before embedding it in the manifest; the final run embedded `v0.1.0-experimental`.

The exact source-build baseline is Compiler Core
[#32403435066](https://github.com/TangoSplicer/SynQ/actions/runs/32403435066),
revision `8125287`: 48/48 ordinary Linux CTests, 34/34 Windows MSVC and macOS
Clang CTests, three experimental static-SDK consumer jobs, and 33/33 Linux/Clang
sanitizer CTests. The approved final candidate files are published in the
[`v0.1.0-experimental` pre-release](https://github.com/TangoSplicer/SynQ/releases/tag/v0.1.0-experimental): three archives, three manifests, and three
platform checksum files. They are not signed binaries, installers, or
package-manager entries.

The CMake delivery surface can include an experimental static SDK when
`BUILD_RECOVERY_NATIVE_SDK=ON`; that is a source/CMake integration path, not a
distributed CLI bundle or stable ABI. The test-only shared ABI and consumer
fixtures are never a release-library product. See
[`EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md).

## Implemented experimental path

[`SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md`](./SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md)
defines the zero-cost path: CLI-only Linux/macOS archives and Windows
ZIPs, SHA-256 manifests, clean-extraction tests, immutable GitHub Release assets,
optional GitHub provenance attestations, explicit unsigned macOS/Windows limits,
and acceptance gates before publication.

> The candidate archive gates, including the optional validated-tag input, are
> implemented and passed for the `v0.1.0-experimental` tag target. The three
> corresponding CLI archives are now published as an experimental pre-release;
> source-build/install paths, static SDK boundaries, and all non-runtime limits
> remain unchanged.

## Published release record and verification

The published tag, exact candidate-asset evidence, and tag-to-asset publication
record are recorded in
[`EXPERIMENTAL_RELEASE_CANDIDATE_RECORD_v0.1.0.md`](./EXPERIMENTAL_RELEASE_CANDIDATE_RECORD_v0.1.0.md).
The public [GitHub pre-release](https://github.com/TangoSplicer/SynQ/releases/tag/v0.1.0-experimental) uploads only the three CLI archives, their JSON manifests,
and their platform checksum files.

For users of the published pre-release, [`EXPERIMENTAL_CLI_ARCHIVE_VERIFICATION.md`](./EXPERIMENTAL_CLI_ARCHIVE_VERIFICATION.md)
explains checksum, optional release-integrity, GitHub-attestation, and
non-executing identification checks. The reusable
[`EXPERIMENTAL_RELEASE_NOTES_TEMPLATE.md`](./EXPERIMENTAL_RELEASE_NOTES_TEMPLATE.md)
requires exact tag, commit, archive, hash, extraction, provenance, and
experimental-boundary values before public publication.

## Current local commands

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
cmake --install compiler/build --prefix "$PWD/compiler/stage"
```

These commands are useful for local experimental evaluation. They do not create
a signed, notarized, attested, or publicly released package.
