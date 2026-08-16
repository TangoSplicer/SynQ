# Security Policy

## Scope and current boundary

SynQ is experimental software. The verified compiler recovery profile, its
experimental C ABI, and the static native SDK path are in scope for responsible
reports. Historical frontend and backend prototype directories may contain
incomplete or unverified code; they should be treated cautiously, not as hosted
services.

This policy does not provide a security-service-level agreement, bounty program,
or promise of a fixed response time. It establishes a responsible reporting path
for a solo-maintained, zero-budget project.

## Reporting a vulnerability

Use GitHub’s **private vulnerability reporting** for this repository when that
repository feature is available. Include a minimal reproduction, affected commit
or tag, environment details, expected impact, and suggested mitigation if known.
Do not publish credentials, private keys, exploit payloads, or complete attack
steps in a public issue.

If private reporting is unavailable, open a minimal public issue that states a
potential security concern without disclosing sensitive details, then wait for a
maintainer-provided private channel. Do not use a public issue to request or
share access to third-party systems.

## What to expect

Reports are triaged as capacity allows. A maintainer may request clarification,
reproduction steps, or a minimal patch. Confirmed fixes will be documented in
`CHANGELOG.md` when disclosure would not increase user risk. No unsupported
platform, provider, hardware, stable-ABI, or distribution guarantee is implied
by accepting a report.

## Safe research rules

Do not target external systems, quantum providers, other users, or third-party
accounts. Use local fixtures and the repository’s documented bounded compiler
profile. Never commit secrets or report output that contains credentials.
