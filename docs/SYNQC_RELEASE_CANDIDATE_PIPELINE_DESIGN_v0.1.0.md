# synqc Non-Publishing Release-Candidate Pipeline Design v0.1.0

**Status:** Remotely verified non-publishing candidate workflow.
**Evidence:** [synqc Release Candidate #32393670587](https://github.com/TangoSplicer/SynQ/actions/runs/32393670587), revision `fdbbbec`, 20 August 2026.

This document authorizes candidate archive creation and retention for test
evidence only; it does not authorize creating, editing, or publishing a GitHub
Release.

## Purpose

The pipeline turns an already tested source revision into three candidate
CLI-only archives, then proves that each exact archive extracts and executes the
bounded `synqc` command. It separates candidate evidence from public delivery so
that an experimental project can validate package mechanics without implying that
a release exists.

## Trigger and source identity

The candidate workflow runs on a narrow `main`-branch path set and by manual
dispatch. It checks out the triggering commit and embeds its full SHA in each
manifest. The workflow neither creates a tag nor calls a release-publishing API.
Candidate artifact retention is a CI convenience, not a public download channel.

| Input | Rule |
| --- | --- |
| CLI label | Read from the configured executable via `synqc --version`; currently `0.1.0-experimental`. |
| Commit | Exact checked-out `GITHUB_SHA`; no branch alias or `latest` label. |
| Platform | The runner OS plus the runner architecture supplied by Actions. |
| Trust state | `unsigned` for macOS and Windows candidates unless a future approved signing job changes both the artifact and manifest. |

## Candidate contents and allowlist

Each candidate archive has one top-level directory and only the CLI, licenses,
release notice, current CLI/limit/policy documents, and JSON version metadata.
The packager rejects SDK libraries and headers, test-only shared ABI outputs,
tests, source, object files, build files, and unexpected files. The static C ABI
SDK therefore remains an independently validated source/CMake path.

## Validation contract

The validation script extracts the exact archive to a clean directory, checks its
top-level path and allowlist, reads `VERSION.json`, runs `synqc --version` and
`synqc --help`, and validates a small checked-in SynQ source. It also checks that
the archive SHA-256 and byte count agree with the generated manifest.

| Platform | Candidate format | Required test profile before packaging | Archive smoke |
| --- | --- | --- | --- |
| Linux | `tar.gz` | Full ordinary Linux recovery profile. | Extraction, version, help, validation, manifest/hash/allowlist checks. |
| macOS | `tar.gz` | Existing platform-neutral macOS profile. | Extraction, version, help, validation, manifest/hash/allowlist checks. |
| Windows | `zip` | Existing platform-neutral Windows profile. | Extraction, version, help, validation, manifest/hash/allowlist checks. |

The Linux sanitizer profile remains an independently scoped reliability job. It
does not package an archive and its absence from candidate packaging must not be
represented as artifact sanitization evidence.

## Provenance preparation

Candidate archives are eligible for GitHub artifact attestations after exact
archive creation. The workflow receives only `contents: read`, `id-token: write`,
and `attestations: write` permissions. It uses the documented `actions/attest@v4`
interface with the candidate archive as `subject-path`. GitHub describes an
attestation as a provenance claim and warns that it is not, by itself, a security
guarantee.[1]

The implemented workflow retained candidate archive attestations for Linux,
macOS ARM64, and Windows X64 in run #32393670587. It does not attach an asset to
a GitHub Release or assert user-facing provenance. A future release candidate
must still pass the documented publication gates before any asset is published.

## Verified run result

The candidate workflow completed all three platform jobs. Each job built the
configured platform source profile, created its candidate archive, validated the
exact extracted archive, generated a GitHub provenance attestation, and retained
the candidate materials. Compiler Core
[#32394772652](https://github.com/TangoSplicer/SynQ/actions/runs/32394772652)
also passed all seven jobs for the exact revision: 48/48 ordinary Linux CTests,
34/34 Windows and macOS CTests, three SDK-consumer jobs, and 33/33 sanitizer
CTests. This evidence is limited to the named CI platform/architecture candidates
and does not make them public packages or general platform-support commitments.

## Publication separation

The workflow has no `contents: write` permission and contains no `gh release`,
GitHub release action, package-registry upload, installer, Homebrew, Chocolatey,
winget, Microsoft Store, or notarization command. A distinct release-publication
workflow may be proposed only after a candidate run passes and the user confirms
the exact tag, assets, hashes, and release text.

## References

[1]: https://docs.github.com/en/actions/how-tos/secure-your-work/use-artifact-attestations/use-artifact-attestations "GitHub Docs — Using artifact attestations to establish provenance for builds"
