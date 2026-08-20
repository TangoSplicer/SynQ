# Experimental `synqc` Release-Candidate Record v0.1.0

**Status:** **Prepared record only.** No GitHub Release, public downloadable
asset, or Git tag has been created by this document.  
**Proposed tag:** `v0.1.0-experimental` (**available on the remote at the time
of this record**).  
**Candidate source revision:** `fdbbbec43b75ec7cbc2efef1c62ec951f5800230`.  
**Candidate workflow:** [synqc Release Candidate #32393670587][1].  
**Required source evidence:** [Compiler Core #32394772652][2].

## Decision record

The proposed tag deliberately matches the existing `synqc 0.1.0-experimental`
CLI version. It is a **pre-release identifier**, not a stable-language, stable
ABI, hardware-provider, package-manager, installer, signing, or notarization
claim. The candidate source revision is the exact commit built by the completed
three-platform candidate workflow; the required Compiler Core run independently
validated that same revision with 48/48 ordinary Linux CTests, 34/34 Windows
MSVC and macOS Clang CTests, three SDK-consumer jobs, and 33/33 isolated
Linux/Clang sanitizer CTests.[2]

| Proposed public identity | Prepared value | Current state |
| --- | --- | --- |
| Tag | `v0.1.0-experimental` | **Not created** |
| Release title | `SynQ CLI v0.1.0-experimental (pre-release)` | Template only |
| GitHub Release | One pre-release, not latest | **Not created** |
| Target commit | `fdbbbec43b75ec7cbc2efef1c62ec951f5800230` | Remotely verified |
| Archive provenance | GitHub Actions artifact attestations | Candidate evidence only |

## Retained candidate evidence

The following files were generated, extracted, validated, attested, and retained
by run #32393670587. They are **CI artifacts**, not GitHub Release assets. Their
hashes are recorded here only to make the final tagged candidate reproducible and
auditable; they are not a user-download announcement.

| Candidate platform / architecture | Retained candidate archive | SHA-256 | Candidate manifest |
| --- | --- | --- | --- |
| Linux / X64 | `synqc-0.1.0-experimental-linux-X64.tar.gz` | `e1ab54fbeab761039b4597ab39d22c8b1fb35ce5ec03917f65f2ba082e80ad92` | `synqc-0.1.0-experimental-linux-X64-manifest.json` |
| macOS / ARM64 | `synqc-0.1.0-experimental-macos-ARM64.tar.gz` | `ad35809d9936a5d09b6c205e1d2cc3329e055b466e971521108f16029be2bbf3` | `synqc-0.1.0-experimental-macos-ARM64-manifest.json` |
| Windows / X64 | `synqc-0.1.0-experimental-windows-X64.zip` | `104bc02451de637aef08c89582d647fdffad5fae6d196d113748d7ed077631a3` | `synqc-0.1.0-experimental-windows-X64-manifest.json` |

> **Important:** Each retained candidate manifest has `release_tag:
> "not-a-release"`. The files above must not be relabeled or uploaded as
> `v0.1.0-experimental` assets. Before publication, the final candidate must be
> regenerated from the confirmed tag with the exact tag embedded in its manifest,
> followed by its own clean-extraction, hash, and provenance checks.

## Public-release acceptance record

The operator must record the following final values in the published release
notes before publishing:

| Required release-note field | Source of truth | Must match |
| --- | --- | --- |
| Tag and target commit | Confirmed annotated or lightweight Git tag | Candidate manifest `release_tag` and `commit` |
| Archive names and SHA-256 values | Final `SHA256SUMS.txt` and JSON manifests | Uploaded release assets |
| Clean-extraction result | Final candidate workflow logs | Each uploaded platform archive |
| Provenance result | GitHub attestation verification against the final asset | Exact uploaded file and `TangoSplicer/SynQ` identity |
| Required compiler evidence | Exact-revision Compiler Core run | Tag target commit |

Creating a draft or published GitHub Release, creating the tag, uploading any
asset, or editing release notes is an external public action. It requires a
fresh, explicit user confirmation of the exact tag, commit, asset list, and note
text immediately before that action.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32393670587 "synqc Release Candidate #32393670587"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/32394772652 "Compiler Core #32394772652"
