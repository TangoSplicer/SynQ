# Experimental `synqc` GitHub Release Notes Template

**Status:** Reusable template only. Completing this document does not create a
tag, draft, release, uploaded asset, or latest-release designation.

GitHub permits automatically generated release notes, but those notes summarize
merged pull requests and contributors rather than substituting for artifact
identity, verification, experimental limits, or operator review.[1] Use this
template as the human-authored factual core; append generated notes only after
reviewing them.

---

## SynQ CLI `{{TAG}}` — Experimental Pre-release

**Release status:** Experimental pre-release. Do **not** treat this as a stable
runtime, stable ABI, hardware service, provider integration, signed installer,
notarized macOS application, package-manager package, performance commitment, or
enterprise support offering.

### Exact source and evidence

| Field | Final value |
| --- | --- |
| Git tag | `{{TAG}}` |
| Target commit | `{{FULL_COMMIT_SHA}}` |
| Candidate workflow | `{{CANDIDATE_WORKFLOW_URL}}` |
| Exact-revision Compiler Core evidence | `{{COMPILER_CORE_RUN_URL}}` |
| Platform candidate outcome | `{{LINUX_RESULT}}`; `{{MACOS_ARM64_RESULT}}`; `{{WINDOWS_X64_RESULT}}` |
| Ordinary / platform-neutral / sanitizer counts | `{{LINUX_CTESTS}}`; `{{WINDOWS_MACOS_CTESTS}}`; `{{SANITIZER_CTESTS}}` |

### Downloadable CLI-only archives

| Platform / architecture | Archive | SHA-256 | Manifest | Provenance |
| --- | --- | --- | --- | --- |
| Linux / X64 | `{{LINUX_ARCHIVE}}` | `{{LINUX_SHA256}}` | `{{LINUX_MANIFEST}}` | `{{LINUX_ATTESTATION_RESULT}}` |
| macOS / ARM64 | `{{MACOS_ARCHIVE}}` | `{{MACOS_SHA256}}` | `{{MACOS_MANIFEST}}` | `{{MACOS_ATTESTATION_RESULT}}` |
| Windows / X64 | `{{WINDOWS_ARCHIVE}}` | `{{WINDOWS_SHA256}}` | `{{WINDOWS_MANIFEST}}` | `{{WINDOWS_ATTESTATION_RESULT}}` |

### Verify before use

1. Download the matching archive, JSON manifest, and `SHA256SUMS.txt` from this
   release.
2. Verify SHA-256 and the release asset using the instructions in
   [`EXPERIMENTAL_CLI_ARCHIVE_VERIFICATION.md`](./EXPERIMENTAL_CLI_ARCHIVE_VERIFICATION.md).
3. Verify the GitHub attestation with the explicit repository identity and signer
   workflow stated in this release.
4. Extract to an empty directory and check `--version` and `--help` before using
   the documented SynQ validation example.

### Explicit platform and trust limits

Linux X64, macOS ARM64, and Windows X64 describe only the exact candidate
archive builds and clean-extraction smoke checks recorded above. They do not
provide general package support. The macOS archive is unsigned and not notarized
unless this release explicitly replaces that statement. The Windows archive is
unsigned unless this release explicitly replaces that statement. A GitHub
attestation relates an artifact to its recorded build provenance; it is not a
security guarantee.[2]

### Current language boundaries

`synqc` remains version `0.1.0-experimental`. It validates, inspects, exports,
evaluates, and locally simulates only documented bounded subsets. It is not a
complete runtime or hardware service. Read the current
[`KNOWN_LIMITS_v0.1.0.md`](./KNOWN_LIMITS_v0.1.0.md) before use.

### Publication checklist

- [ ] `{{TAG}}` exists and resolves to `{{FULL_COMMIT_SHA}}`.
- [ ] Every uploaded archive matches its final manifest and `SHA256SUMS.txt`.
- [ ] Final candidate workflow passes archive creation, clean extraction, and
      provenance attestation for all uploaded platforms.
- [ ] Exact-revision Compiler Core matrix passes.
- [ ] This release is marked **pre-release** and **not latest**.
- [ ] No source-generated archive is described as the CLI archive.
- [ ] The explicit unsigned, not-notarized, non-package-manager, and
      experimental-runtime boundaries above remain accurate.
- [ ] The operator has obtained fresh confirmation immediately before publishing.

---

## References

[1]: https://docs.github.com/en/repositories/releasing-projects-on-github/automatically-generated-release-notes "GitHub Docs — Automatically generated release notes"
[2]: https://docs.github.com/en/actions/concepts/security/artifact-attestations "GitHub Docs — Artifact attestations"
