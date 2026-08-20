# synqc Packaging Status and Specification Index

**Status:** Remotely verified non-publishing candidate-archive path plus the
public-release acceptance boundary. This is not a public binary-release
announcement.

## Current delivery position

The recovery CMake profile builds `synqc 0.1.0-experimental`, installs it with
`cmake --install`, and configures a local ZIP-oriented CPack path. The new
non-publishing candidate workflow creates CLI-only Linux, macOS ARM64, and
Windows X64 archives; writes JSON and SHA-256 manifests; enforces a contents
allowlist; extracts each exact archive; runs CLI smoke checks; generates GitHub
provenance attestations; and retains the results as CI artifacts. The workflow
passed as [synqc Release Candidate #32401780347](https://github.com/TangoSplicer/SynQ/actions/runs/32401780347). Its optional manual tag input checks out and resolves an existing tag before embedding it in a non-published manifest; this run used the untagged `not-a-release` path.

The exact source-build baseline is Compiler Core
[#32403435066](https://github.com/TangoSplicer/SynQ/actions/runs/32403435066),
revision `8125287`: 48/48 ordinary Linux CTests, 34/34 Windows MSVC and macOS
Clang CTests, three experimental static-SDK consumer jobs, and 33/33 Linux/Clang
sanitizer CTests. None of this publishes GitHub Release assets, user-downloadable
checksums, signed binaries, installers, or package-manager entries.

The CMake delivery surface can include an experimental static SDK when
`BUILD_RECOVERY_NATIVE_SDK=ON`; that is a source/CMake integration path, not a
distributed CLI bundle or stable ABI. The test-only shared ABI and consumer
fixtures are never a release-library product. See
[`EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md).

## Proposed path

[`SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md`](./SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md)
defines the proposed zero-cost path: CLI-only Linux/macOS archives and Windows
ZIPs, SHA-256 manifests, clean-extraction tests, immutable GitHub Release assets,
optional GitHub provenance attestations, explicit unsigned macOS/Windows limits,
and separate acceptance gates before publication.

> The candidate archive gates, including the optional validated-tag input, are
> implemented and passed for revision `8125287`.
> SynQ still has source-build/install paths and retained CI artifacts—not
> published public CLI packages. A separate confirmed publication step remains
> required.

## Prepared release-readiness records

The proposed tag, exact candidate-asset evidence, and tag-to-asset publication
boundary are recorded in
[`EXPERIMENTAL_RELEASE_CANDIDATE_RECORD_v0.1.0.md`](./EXPERIMENTAL_RELEASE_CANDIDATE_RECORD_v0.1.0.md).
That proposed tag has not been created and the retained candidate archives have
not been relabeled as public assets.

For future users, [`EXPERIMENTAL_CLI_ARCHIVE_VERIFICATION.md`](./EXPERIMENTAL_CLI_ARCHIVE_VERIFICATION.md)
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
