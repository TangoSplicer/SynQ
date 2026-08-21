# Experimental `synqc` Release-Candidate Record v0.1.0

**Status:** **Published experimental pre-release record.** The annotated Git
tag and GitHub pre-release named below exist.
**Public release:** [SynQ CLI `v0.1.0-experimental`][3] (pre-release; not
latest).
**Tag:** `v0.1.0-experimental`.
**Candidate source revision:** `812528711dc9189ba7d1af27657821b9d54a39ab`.
**Final candidate workflow:** [synqc Release Candidate #32482750179][1].
**Required source evidence:** [Compiler Core #32403435066][2].

## Decision record

The tag deliberately matches the existing `synqc 0.1.0-experimental`
CLI version. It is a **pre-release identifier**, not a stable-language, stable
ABI, hardware-provider, package-manager, installer, signing, or notarization
claim. The candidate source revision is the exact commit built by the completed
three-platform tag-aware candidate workflow; the required Compiler Core run independently
validated that same revision with 48/48 ordinary Linux CTests, 34/34 Windows
MSVC and macOS Clang CTests, three SDK-consumer jobs, and 33/33 isolated
Linux/Clang sanitizer CTests.[2]

| Public identity | Published value | Current state |
| --- | --- | --- |
| Tag | `v0.1.0-experimental` | **Created**; annotated tag at the target commit |
| Release title | `SynQ CLI v0.1.0-experimental` | **Published** pre-release, not latest |
| GitHub Release | [One pre-release, not latest][3] | **Published** |
| Target commit | `812528711dc9189ba7d1af27657821b9d54a39ab` | Remotely verified |
| Archive provenance | GitHub Actions artifact attestations | Final tagged candidate evidence |

## Published archive evidence

The following files were generated, extracted, validated, attested, and retained
by final tag-aware run #32482750179, then uploaded to the [published
pre-release][3]. Their SHA-256 values were checked against their matching
platform `SHA256SUMS.txt` and JSON manifests before upload.

| Platform / architecture | Published CLI archive | SHA-256 | Published manifest |
| --- | --- | --- | --- |
| Linux / X64 | `synqc-0.1.0-experimental-linux-X64.tar.gz` | `fd5e6999ff513c0dcd9230caaa4e127c67b56c2a73720f16d8897d55aca05e80` | `synqc-0.1.0-experimental-linux-X64-manifest.json` |
| macOS / ARM64 | `synqc-0.1.0-experimental-macos-ARM64.tar.gz` | `c927d4cf103a949dc14e34c7147f63fc27560367a89a05dbc9899209e2880384` | `synqc-0.1.0-experimental-macos-ARM64-manifest.json` |
| Windows / X64 | `synqc-0.1.0-experimental-windows-X64.zip` | `caf6c3e78a12a303b560b998748944361470b369dfc496797094a4db3f2b1a75` | `synqc-0.1.0-experimental-windows-X64-manifest.json` |

> **Important:** Each published manifest embeds `release_tag:
> "v0.1.0-experimental"` and commit
> `812528711dc9189ba7d1af27657821b9d54a39ab`. These archives are unsigned;
> the macOS archive is not notarized. The provenance attestations improve
> traceability and are not a security guarantee.

## Publication acceptance record

The following release gates were satisfied before publication:

| Release gate | Source of truth | Verified result |
| --- | --- | --- |
| Tag and target commit | Confirmed annotated Git tag | Matches every final candidate manifest |
| Archive names and SHA-256 values | Final `SHA256SUMS.txt` and JSON manifests | Matches the nine uploaded release assets |
| Clean extraction and CLI smoke | Final candidate workflow logs | Passed for Linux X64, macOS ARM64, and Windows X64 |
| Provenance generation | GitHub Actions candidate workflow | Attestation generated for each final candidate archive |
| Required compiler evidence | Exact-revision Compiler Core run | Passed all seven jobs for the tag target commit |
| Fresh approval | User confirmation immediately before external actions | Received before tag, candidate dispatch, and pre-release publication |

The published assets remain experimental CLI-only archives. They do not create a
stable runtime or ABI, hardware service, provider integration, installer,
package-manager package, signing identity, notarization, or general platform
support guarantee.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32482750179 "synqc Release Candidate #32482750179"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/32403435066 "Compiler Core #32403435066"
[3]: https://github.com/TangoSplicer/SynQ/releases/tag/v0.1.0-experimental "SynQ CLI v0.1.0-experimental pre-release"
