# Contributing to SynQ

SynQ is an **experimental language kernel** maintained around a small, tested
compiler recovery profile. Contributions are welcome when they make the verified
subset easier to build, inspect, test, document, or extend safely. This document
does not promise review times, paid support, or acceptance of every proposal.

## Start with the verified boundary

Before changing compiler behavior, read the [project status](docs/PROJECT_STATUS.md),
[operational-readiness assessment](docs/OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md),
and [tested-environment guide](docs/TESTED_ENVIRONMENTS.md). The supported compiler
evidence is the bounded CMake recovery profile; it is distinct from historical
frontend and backend prototype directories.

| Change type | Begin here | Minimum evidence expected |
| --- | --- | --- |
| Parser, IR, resolver, exporter, or simulator | Relevant design record and nearby smoke test | Positive and negative CTest coverage; local full recovery-suite pass. |
| Public C ABI or native SDK path | `docs/C_ABI.md` and the experimental ABI policy | Ownership/error-contract review; C consumer and installed-SDK conformance stay passing. |
| Rust, Common Lisp, Clojure, or Mercury proof fixture | `docs/INTEROPERABILITY_CONTRACT.md` | Keep the opaque C ABI boundary; do not label a fixture a distributed wrapper or package. |
| Documentation or examples | Project status and the closest design record | Reproducible commands and precise experimental/non-claim language. |
| Frontend/backend prototype repair | Directory-specific source and existing tests | Do not claim a service is runnable until its build/run path is independently verified. |

## Local compiler validation

On the verified Ubuntu-like recovery path, follow the full prerequisite and
toolchain guidance in [`docs/TESTED_ENVIRONMENTS.md`](docs/TESTED_ENVIRONMENTS.md).
After configuring, run:

```bash
cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
```

Run the smallest relevant CTest while iterating, then run the complete suite
before opening a pull request. The current CI workflow is also the authoritative
record of what is independently exercised on GitHub.

## Language-change discipline

Every language feature must have a bounded contract. Add or update a design
record before expanding syntax or semantics; specify the required feature gate,
accepted forms, diagnostics, export behavior, simulator behavior, and explicit
non-goals. Keep Alpha behavior disabled without its exact source annotation.

Avoid silently widening execution, allocation, hardware, provider, package, or
compatibility behavior. A parser accepting source is not evidence that a runtime,
simulator, or backend executes it. If a target cannot preserve the construct,
reject it with a focused diagnostic rather than discarding or rewriting it.

## Pull requests and documentation

Use small, single-purpose pull requests. Describe the user-visible contract, the
tests run, and every non-goal that remains. Update `CHANGELOG.md` for a verified
user-visible increment, and update the related design/status evidence only after
the claimed test result exists.

Do not add secrets, private keys, access tokens, credentials, proprietary
datasets, or unreviewed generated binaries. Report a potential security issue
under [`SECURITY.md`](SECURITY.md), not in a public issue with exploit details.

## Community channels

Use [GitHub Discussions](https://github.com/TangoSplicer/SynQ/discussions) for
questions, early design feedback, and proposals. Use [GitHub Issues](https://github.com/TangoSplicer/SynQ/issues)
for reproducible defects, scoped documentation corrections, and well-bounded
feature requests. For significant semantic work, open a discussion or issue
first so the contract can be agreed before a large implementation starts.
