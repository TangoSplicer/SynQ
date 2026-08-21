# Verify an Experimental `synqc` CLI Archive

**Status:** User-facing instructions for the published
[`v0.1.0-experimental` pre-release](https://github.com/TangoSplicer/SynQ/releases/tag/v0.1.0-experimental).
**Scope:** Linux X64, macOS ARM64, and Windows X64 CLI-only archives from that
experimental pre-release.

## Purpose and trust boundary

Verification should establish that a downloaded file matches the published
archive and that its available GitHub Actions provenance is consistent with the
repository and workflow identity. It does **not** establish that SynQ is secure,
stable, signed, notarized, compatible with every system, or safe for production
use. GitHub states that attestations link an artifact to its source and build
instructions, while the consumer still defines the trust policy and evaluates the
artifact.[1]

> Do not run an archive merely because a hash or an attestation verifies. Read
> the experimental boundaries, inspect the release notes, and run the documented
> `--version`, `--help`, and `--validate` checks on a non-critical machine first.

## Before downloading

Open the repository’s **Releases** page and find the exact pre-release tag
listed in its release notes. Download only these three release assets plus their
corresponding JSON manifests and `SHA256SUMS.txt` files:

| Platform | Published archive filename | Local trust boundary |
| --- | --- | --- |
| Linux X64 | `synqc-0.1.0-experimental-linux-X64.tar.gz` | Unsigned experimental archive; no package-manager claim. |
| macOS ARM64 | `synqc-0.1.0-experimental-macos-ARM64.tar.gz` | Unsigned and not notarized unless release notes explicitly say otherwise. |
| Windows X64 | `synqc-0.1.0-experimental-windows-X64.zip` | Unsigned archive; Windows reputation prompts are not a defect claim or a bypass instruction. |

Do not use GitHub-generated source ZIP or source TAR links as substitutes for
the CLI archive: GitHub documents that release-asset verification does not apply
to those on-demand generated source bundles.[2]

## Step 1 — Verify SHA-256

Use the final release’s `SHA256SUMS.txt`, not the historical CI-candidate values.

### Linux

```bash
sha256sum --check SHA256SUMS.txt
```

### macOS

```bash
shasum -a 256 -c SHA256SUMS.txt
```

### Windows PowerShell

```powershell
$expected = (Get-Content .\SHA256SUMS.txt | Select-String 'synqc-.*windows-X64\.zip').ToString().Split()[0]
$actual = (Get-FileHash .\synqc-0.1.0-experimental-windows-X64.zip -Algorithm SHA256).Hash.ToLower()
if ($actual -eq $expected) { 'SHA-256 verified' } else { throw 'SHA-256 mismatch — do not extract or run this file.' }
```

A mismatch means the local file does not match the recorded asset hash. Delete
that download and retrieve it again from the release page; do not attempt to
repair, rename, or use it.

## Step 2 — Verify GitHub Release integrity, where available

GitHub documents `gh release verify RELEASE-TAG` for checking an immutable
release and `gh release verify-asset RELEASE-TAG ARTIFACT-PATH` for checking a
local artifact against a release asset.[2] These commands apply only if the
published release actually exposes that verification capability.

```bash
gh release verify v0.1.0-experimental --repo TangoSplicer/SynQ
gh release verify-asset v0.1.0-experimental synqc-0.1.0-experimental-linux-X64.tar.gz --repo TangoSplicer/SynQ
```

If GitHub does not report an immutable release, that is not proof of a broken
archive; it means this optional platform control is unavailable for that release.
Continue to treat the archive as experimental and use the hash plus provenance
steps below.

## Step 3 — Verify the final archive attestation, where published

The GitHub CLI manual requires an artifact path and a precise repository or owner
identity; using `--repo` narrows verification to the named repository.[3]

```bash
gh attestation verify synqc-0.1.0-experimental-linux-X64.tar.gz \
  --repo TangoSplicer/SynQ \
  --signer-workflow TangoSplicer/SynQ/.github/workflows/synqc-release-candidate.yml
```

The final release notes must name the signer workflow and source commit. Confirm
the verified result matches those values before using the archive. An attestation
does not replace independent review, security analysis, signature-based code
signing, macOS notarization, or tests.[1]

## Step 4 — Extract and perform non-executing identification

Extract the archive to a new empty directory. Before compiling any SynQ program,
run only the documented CLI identification commands:

```bash
./synqc --version
./synqc --help
```

On Windows PowerShell, use:

```powershell
.\synqc.exe --version
.\synqc.exe --help
```

The reported version should be `0.1.0-experimental`. Then follow the release
notes’ minimal documented SynQ `--validate` smoke example. The CLI is not a
hardware client and these checks do not submit a job or connect to a provider.

## References

[1]: https://docs.github.com/en/actions/concepts/security/artifact-attestations "GitHub Docs — Artifact attestations"
[2]: https://docs.github.com/en/code-security/how-tos/secure-your-supply-chain/secure-your-dependencies/verify-release-integrity "GitHub Docs — Verifying the integrity of a release"
[3]: https://cli.github.com/manual/gh_attestation_verify "GitHub CLI manual — gh attestation verify"
