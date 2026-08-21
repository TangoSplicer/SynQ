# synqc Packaging and Distribution Specification v0.1.0

**Status:** Experimental release specification with one published evidence-bound
pre-release. The design and acceptance contract does not establish signed
binaries, installers, operating-system packages, a stable runtime, or a stable
ABI.

**Applies to:** The recovery-profile `synqc` command-line executable on Linux,
macOS, and Windows. It does not package a stable C ABI, shared library,
language-specific wrapper, quantum provider connector, or hardware service.

**Design objective:** Create a zero-cost, inspectable, reproducible-enough
experimental CLI distribution path for a solo maintainer. The first release
channel shall prefer a small downloadable archive and explicit verification over
native installers or package registries that would imply a broader support,
trust, or maintenance commitment.

## 1. Current position and non-claims

The current recovery CMake profile builds `synqc 0.1.0-experimental` and
configures a local CPack ZIP path. The Compiler Core workflow uploads build
artifacts separately on Linux, Windows, and macOS; it does not create GitHub
Release assets, checksum manifests, signatures, attestations, installers, or
package-manager entries. [1] [2] The final tag-aware candidate workflow then
validated and produced Linux X64, macOS ARM64, and Windows X64 CLI-only archives
with manifests, checksums, extraction smoke checks, and provenance attestations;
the nine verification files were published in the
[`v0.1.0-experimental` pre-release](https://github.com/TangoSplicer/SynQ/releases/tag/v0.1.0-experimental).
The exact source evidence, [Compiler Core #32403435066][3], covers the tag
target with 48/48 ordinary Linux CTests, 34/34 Windows MSVC CTests, 34/34 macOS
Clang CTests, three static-SDK consumer jobs, and 33/33 Linux/Clang sanitizer
CTests. The release does not validate a consumer-machine installation beyond the
same-platform clean-extraction smoke checks.

> **A build artifact is not a release.** A release claim begins only after the
> exact downloadable archive, its verification material, and its clean
> extraction/install test have been built and retained by the designated release
> workflow for the same source revision.

| Delivery surface | Current status | This specification proposes | Explicit non-claim until gates pass |
| --- | --- | --- | --- |
| Source build | Documented and remotely exercised on the three CI operating-system families. | Keep as the canonical fallback and debugging route. | A source build does not create a supported binary package. |
| CPack archive | Configured and locally described as ZIP-only; prior local archive inspection is older evidence. | Replace as the public mechanism with per-platform, CLI-only release archives. | CPack output itself is not the published archive mechanism. |
| GitHub Actions artifact | Current CI uploads build outputs. | Use only as a workflow intermediate; promote verified files into immutable GitHub Release assets. | CI artifact retention is not public release retention or provenance. |
| GitHub Release asset | Published experimentally for Linux X64, macOS ARM64, and Windows X64 at `v0.1.0-experimental`. | Retain the same tag/manifest/checksum/extraction gates for later releases. | No stable ABI/runtime, signing, notarization, installer, or package-manager support claim. |
| Native installer/package registry | Not published. | Deferred. | No `.msi`, `.pkg`, `.deb`, `.rpm`, Homebrew, Chocolatey, winget, apt, or equivalent claim. |

## 2. Release-channel decision

The initial public channel shall be **GitHub Release assets containing
platform-specific, CLI-only archives**. This route is free to host for a public
repository, keeps the downloaded bytes adjacent to source and workflow evidence,
and lets users inspect a release with the GitHub CLI. GitHub documents
`gh release verify <tag>` for an immutable release and
`gh release verify-asset <tag> <local-file>` for an exact local-release-asset
comparison. [4]

The release channel deliberately does not use an installer. An installer makes
platform-specific privilege, uninstall, code-signing, and support expectations
more likely. A single executable in a user-controlled directory is sufficient
for the experimental CLI and lets every user remove it by deleting that directory.

## 3. Package identity and contents

### 3.1 Artifact naming

Each release workflow shall compute the package identity from a signed-off tag,
the exact source commit, and the actual build runner architecture. Do not use
`latest`, moving branch names, or an undocumented architecture label.

| Target family | Proposed file name | Packaging format | Architecture rule |
| --- | --- | --- | --- |
| Linux | `synqc-<cli-version>-linux-<arch>.tar.gz` | `tar.gz` | Publish only an architecture that has completed the release workflow; do not infer x86_64 or ARM support from another runner. |
| macOS | `synqc-<cli-version>-macos-<arch>.tar.gz` | `tar.gz` | One archive per tested build architecture; do not label an archive universal unless a universal binary and its verification are explicitly added. |
| Windows | `synqc-<cli-version>-windows-<arch>.zip` | `zip` | Publish only the MSVC-built `.exe` for the release workflow architecture. |
| Cross-platform metadata | `synqc-<cli-version>-SHA256SUMS.txt` | UTF-8 text | Lists exact archive filename, byte count, SHA-256 value, tag, commit, build workflow URL, and UTC build timestamp. |
| Release manifest | `synqc-<cli-version>-manifest.json` | JSON | Machine-readable version of identity, archive contents, test evidence, dependency mode, and verification instructions. |

`<cli-version>` is the exact `synqc --version` compatibility label, initially
`0.1.0-experimental`. Because that label alone does not identify a commit, the
manifest must include the full commit SHA and release tag. A future version
format may add build metadata only after the CLI compatibility and test policy
is revised.

### 3.2 Archive layout

Every archive must contain one top-level directory matching the archive stem.
The initial CLI-only surface is intentionally small.

```text
synqc-<cli-version>-<platform>-<arch>/
├── bin/
│   └── synqc[.exe]
├── LICENSE
├── LICENSE_COMMERCIAL.md
├── NOTICE.txt
├── README.md
├── VERSION.json
└── docs/
    ├── CLI.md
    ├── KNOWN_LIMITS_v0.1.0.md
    ├── SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md
    └── EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md
```

`VERSION.json` must contain the release tag, full commit SHA, CLI label, archive
name, SHA-256 value, build workflow URL, test-run URL, build compiler identity,
and build options. `NOTICE.txt` must state that the archive is experimental and
does not include a stable ABI, provider integration, or hardware access.

The initial public CLI archive must **exclude** `synq_lib`, CMake package files,
the public C header, `synq_ffi_shared`, test fixtures, object files, compiler
databases, source tree, API tokens, and any private build metadata. The
experimental static SDK remains a separately tested source/CMake integration
route, not a bundled side effect of CLI installation. [2] [5]

## 4. Platform-specific installation and user experience

### 4.1 Linux

The Linux archive is an unprivileged extraction route. It must not invoke `sudo`,
modify `/usr/local`, install an init service, run an installer script, or alter a
shell profile without the user choosing to do so.

```bash
tar -xzf synqc-<cli-version>-linux-<arch>.tar.gz
mkdir -p "$HOME/.local/bin"
cp "synqc-<cli-version>-linux-<arch>/bin/synqc" "$HOME/.local/bin/"
"$HOME/.local/bin/synqc" --version
```

The README must show a shell-appropriate method to add `$HOME/.local/bin` to
`PATH`, but must not ask users to pipe a remote script to a shell. Linux archive
publication is not an `.deb`, `.rpm`, distribution-repository, system-wide, or
cross-distribution support commitment.

### 4.2 macOS

The macOS archive uses the same unprivileged extraction model and must clearly
state its architecture. It must also disclose its trust state before download.

```bash
tar -xzf synqc-<cli-version>-macos-<arch>.tar.gz
mkdir -p "$HOME/.local/bin"
cp "synqc-<cli-version>-macos-<arch>/bin/synqc" "$HOME/.local/bin/"
"$HOME/.local/bin/synqc" --version
```

The zero-cost initial route is **unsigned and not notarized** unless a release
manifest proves otherwise. Apple’s documented notarization flow requires
Developer ID signing, Hardened Runtime, and a secure timestamp before submission.
[6] Therefore, the project must not claim Gatekeeper trust, a signed Developer
ID executable, a notarized archive, or universal binary support merely because a
macOS source build passed in CI.

### 4.3 Windows

The Windows archive must contain `bin\synqc.exe` and a PowerShell-first, no-admin
path. It must not install a service, write to `Program Files`, alter registry
state, or claim an MSI uninstall record.

```powershell
Expand-Archive .\synqc-<cli-version>-windows-<arch>.zip -DestinationPath "$HOME\synqc"
& "$HOME\synqc\synqc-<cli-version>-windows-<arch>\bin\synqc.exe" --version
```

The initial zero-cost Windows route is **unsigned** unless the release manifest
states otherwise. Microsoft documents that unsigned downloaded files can show a
SmartScreen warning, that enterprise policy may block continuation, and that
unsigned files build reputation independently for each version. [7] The package
page must instruct users to verify the release source and checksum before
running; it must never tell users to disable SmartScreen or bypass an
organization’s security policy. The route is not an `.msi`, MSIX, Microsoft
Store, winget, Chocolatey, trusted-publisher, or unattended-enterprise claim.

## 5. Integrity, provenance, and signing policy

### 5.1 Required for the first public archive

Every published archive requires the following no-cost verification materials:

| Requirement | Release behavior | User-facing check |
| --- | --- | --- |
| Immutable tag and release | Create the release only after all final assets are attached; publish it only when complete. | `gh release verify <tag>` where GitHub immutable releases are enabled. [4] |
| SHA-256 manifest | Generate `SHA256SUMS.txt` in the release workflow after packaging; include filename and byte count. | Linux/macOS: `shasum -a 256 -c ...`; Windows: `Get-FileHash -Algorithm SHA256`. |
| Machine-readable manifest | Attach one JSON manifest that ties each archive to its tag, commit, workflow, tests, options, and hash. | Inspectable without executing the archive. |
| Fresh-extraction smoke check | Extract the exact archive in a clean workflow directory and run `synqc --version`, `--help`, and one documented `--validate` fixture. | Release notes link the workflow evidence. |
| Archive-content allowlist | Fail the workflow if the archive contains anything outside Section 3.2. | Prevents accidental SDK/test/private artifact inclusion. |

The SHA-256 manifest is an integrity aid, not an identity signature. Checksums
must be retrieved from the same immutable release and verified against the
release manifest; a checksum displayed on a mutable page is not enough on its
own.

### 5.2 Implemented candidate provenance gate

The final tag-aware candidate workflow generated GitHub artifact attestations for
the exact Linux, macOS ARM64, and Windows X64 candidate archives in
[run #32482750179](https://github.com/TangoSplicer/SynQ/actions/runs/32482750179).
It verified the optional existing-tag input and its tag-to-checkout guard, then
embedded `v0.1.0-experimental` in each manifest. The matching three archives,
manifests, and SHA-256 files were published in the
[`v0.1.0-experimental` pre-release](https://github.com/TangoSplicer/SynQ/releases/tag/v0.1.0-experimental).
Any later public-release workflow must retain that gate and document
`gh attestation verify` against the published repository and release asset.
GitHub describes attestations as cryptographically signed
provenance claims containing workflow, repository, commit, and trigger context,
but explicitly states that an attestation is not a guarantee that an artifact is
secure. [8] Attestation therefore improves traceability; it does not replace
tests, review, checksums, or user judgment.

### 5.3 Deferred signing and notarization

No code-signing certificate, macOS Developer ID certificate, Apple
notarization, Microsoft Artifact Signing subscription, Microsoft Store account,
or commercial package-repository account is assumed by this specification. Any
such route is outside the zero-cost baseline and must be a separate decision
with a revised budget, identity, credential-protection, disclosure, and release
test plan. A later signed route must retain the unsigned archive verification
path until it has its own clean-install evidence.

## 6. Build and release workflow contract

Any later dedicated publishing workflow should be `synqc-release.yml`. It must trigger only
from a maintainer-approved version tag or explicit manual dispatch that supplies
an exact tag and commit. It must never publish from an unreviewed pull request or
from the moving `main` branch.

### 6.1 Build matrix

| Platform | Build environment | Required package output | Minimum release evidence |
| --- | --- | --- | --- |
| Linux | Ubuntu runner with the documented CMake, C++17, OpenSSL, and JSON dependencies. | CLI-only `tar.gz`. | Full Linux recovery suite plus archive extraction smoke. |
| macOS | macOS runner with the documented Homebrew CMake, nlohmann-json, and OpenSSL inputs. | Architecture-labeled CLI-only `tar.gz`. | Platform-neutral macOS suite plus archive extraction smoke. |
| Windows | Windows runner with MSVC and documented vcpkg OpenSSL/nlohmann-json inputs. | Architecture-labeled CLI-only `.zip`. | Platform-neutral Windows suite plus archive extraction smoke. |

The workflow must configure `BUILD_RECOVERY_CLI=ON` and
`BUILD_RECOVERY_NATIVE_SDK=OFF` for public CLI archive production. It must
record every non-default CMake option in `VERSION.json`. SDK production and CLI
archive production are separate deliverables and must not be conflated.

### 6.2 Release sequence

1. The maintainer creates a candidate tag that names the intended CLI label and
   records the full commit SHA.
2. The release workflow builds and tests the source profile for each platform.
   At minimum, it must retain all then-current Compiler Core platform tests;
   releases may not relax them.
3. It stages the CLI-only install surface, builds the platform archive, and
   creates `SHA256SUMS.txt`, `VERSION.json`, and the JSON manifest.
4. It extracts the exact generated archive into a clean directory on the same
   platform and runs the archive smoke checks.
5. It generates and verifies the proposed provenance attestation if that gate
   has been implemented.
6. It uploads all candidate assets for inspection, then creates or publishes the
   GitHub Release only after every required matrix job and archive smoke check
   succeeds.
7. It records release URLs, full commit SHA, platform architecture, test counts,
   archive hashes, exceptions, and known limits in the changelog and release
   notes.

A failed test, missing hash, inconsistent version/commit record, archive
allowlist failure, or extraction-smoke failure blocks publication for all
platforms. The project must not publish a partial “major-platform release” while
advertising universal availability.

## 7. Acceptance tests and release evidence

The following tests are new required work; no row is currently satisfied merely
because source CI succeeds.

| Identifier | Requirement | Pass condition |
| --- | --- | --- |
| PKG-001 | Identity | Archive name, manifest CLI version, `synqc --version`, tag, and commit SHA agree exactly. |
| PKG-002 | CLI-only contents | Archive allowlist passes; no SDK library/header, shared test ABI, tests, source, or secret appears. |
| PKG-003 | Linux extraction | Fresh archive extraction runs `--version`, `--help`, and one documented `--validate` command. |
| PKG-004 | macOS extraction | Same as PKG-003 on the built architecture; manifest truthfully labels unsigned/not-notarized state. |
| PKG-005 | Windows extraction | Same as PKG-003 from PowerShell; release notes explicitly disclose unsigned/SmartScreen behavior. |
| PKG-006 | Integrity | Independently generated SHA-256 values match every final archive; manifest byte counts match uploaded files. |
| PKG-007 | Release verification | Published release and each local asset pass documented GitHub CLI verification where immutable releases are enabled. |
| PKG-008 | Provenance | If attestations are enabled, the exact release asset verifies against the designated repository, tag, and workflow. |
| PKG-009 | Documentation | Platform README sections include install, verify, remove, support boundary, and known-limit links. |
| PKG-010 | Regression | The unchanged full Compiler Core matrix succeeds for the exact release commit. |

## 8. Versioning, compatibility, and removal

The CLI remains `0.1.0-experimental`. A platform archive must not imply that the
source language, CLI flags, exit codes, diagnostic prose, OpenQASM output, C ABI,
or behavior is stable across releases. Release notes must list breaking changes
and link the known-limit ledger.

The initial removal procedure is intentionally simple: remove the extracted
directory and any user-created `PATH` entry. No installer database, service,
system package, registry modification, or automatic updater is included. An
automatic updater is out of scope until a signed/provenance-backed update policy
and rollback procedure exist.

## 9. Support and security boundaries

This specification creates neither a support SLA nor a distribution warranty.
Users report reproducible issues through GitHub Issues with platform, runner or
host architecture, archive filename, checksum, release tag, `synqc --version`,
complete command, and first failure output. Do not request private credentials,
quantum-provider tokens, personal files, or a blanket security-policy bypass.

The first public archive can be described only as an **experimental CLI archive
with platform-specific workflow evidence** after every Section 7 gate passes.
It cannot be described as generally available, signed, notarized, package-manager
installable, cross-platform supported, production-ready, security-certified, or
hardware-capable without separate evidence.

## 10. Implementation order

| Priority | Zero-cost increment | Why it precedes the next step |
| --- | --- | --- |
| 1 | Add a release-only workflow that builds CLI-only archives, manifests, and SHA-256 files without publishing them. | Establishes deterministic contents and archive-smoke evidence before a public download claim. |
| 2 | Add PKG-001 through PKG-006 and retain candidate archives on all three platform jobs. | Validates installation and integrity mechanics independently of a release page. |
| 3 | Add immutable GitHub Release publication and GitHub CLI verification guidance. | Promotes only already-tested archives. |
| 4 | Add artifact attestations for final archives and PKG-008. | Adds traceable provenance without representing it as a security guarantee. |
| 5 | Re-evaluate signing, notarization, package managers, or installers only with a budget and platform-support decision. | These routes introduce external identity, cost, and maintenance commitments. |

## References

[1]: ../compiler/CMakeLists.txt "SynQ recovery CMake configuration"

[2]: ../compiler/cmake/packaging.cmake "SynQ current CPack and install configuration"

[3]: https://github.com/TangoSplicer/SynQ/actions/runs/32374149046 "Compiler Core #32374149046"

[4]: https://docs.github.com/en/code-security/how-tos/secure-your-supply-chain/secure-your-dependencies/verify-release-integrity "GitHub Docs — Verifying the integrity of a release"

[5]: ./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md "SynQ Experimental ABI and Distribution Policy"

[6]: https://developer.apple.com/documentation/security/notarizing-macos-software-before-distribution "Apple Developer Documentation — Notarizing macOS software before distribution"

[7]: https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/smartscreen-reputation "Microsoft Learn — SmartScreen reputation for Windows app developers"

[8]: https://docs.github.com/en/actions/concepts/security/artifact-attestations "GitHub Docs — Artifact attestations"
