# Competitive Leverage Review and Implementation Direction

**Review date:** 17 August 2026  
**Purpose:** Select defensible implementation priorities for SynQ’s experimental
language kernel. This review does not claim feature parity with established
frameworks or services.

## Market reality

The established tools reviewed here already offer much broader capability than
SynQ’s bounded recovery profile. IBM documents Qiskit as a modular framework for
research and development with circuit construction, optimization, execution,
error-mitigation resources, and access to IBM’s compute service.[1] PennyLane
positions itself as an open-source Python framework for differentiable quantum
programming and high-performance simulation workflows.[2] Q# documents a
high-level language with an integrated compiler/runtime model, qubit management,
classical–quantum integration, and hardware-agnostic mapping.[3] Cirq documents
a Python circuit library with circuit manipulation, optimization, simulation,
noise modeling, and hardware-oriented execution.[4]

SynQ must therefore not compete by implying equivalent hardware, cloud, noise,
optimization, differentiable-programming, or general-runtime support. Its viable
position is a small, inspectable language kernel whose claims are backed by
source-level contracts, focused rejection tests, reproducible CMake consumers,
and independently scoped CI evidence.

## Three leverage points

| Leverage point | Why it is defensible | Bounded implementation direction | Explicit non-goal |
| --- | --- | --- | --- |
| **1. Semantic transparency and quantum-resource safety** | General frameworks often hide language semantics behind host-language APIs. SynQ can make a small hybrid language’s values, bindings, resources, and failure modes explicit and testable. | Publish an alpha semantic-kernel contract, introduce a resolver-owned binding/type environment, then define named-register allocation and bounded multi-register simulation semantics. | Claiming a complete type system, general runtime, or hardware resource manager. |
| **2. Proof-carrying portable interoperability** | SynQ already has an opaque C ABI with independently scoped Linux, Windows, and static-SDK evidence. The next differentiation is to make one host-language adapter safe, small, and reproducible rather than merely reachable. | Deliver one Rust alpha wrapper with explicit ownership/error mapping, ABI version checks, native-language tests, and a no-registry, source/CMake consumption path. | Calling the ABI stable, publishing a package registry release, or asserting safe wrappers for every language. |
| **3. Auditable target lowering and reproducible learning** | Existing frameworks provide rich dynamic ecosystems. SynQ can emphasize exact source-to-target artifacts, rejection boundaries, and a beginner workflow that never pretends to execute in the browser. | Preserve strict OpenQASM export contracts, extend only after semantic decisions, add fixture-to-output traceability and explain every accepted/rejected construct in beginner materials. | Claiming provider integration, live cloud compilation, or universal OpenQASM/backend support. |

## Decision

The leverage points are dependency ordered. The first implementation must be the
semantic kernel because it supplies the vocabulary for resource safety and a safe
Rust API. The second is named-register resource/multi-register simulation
semantics, because named syntax already exists and must not remain a lowering-only
feature. The third is a narrow Rust wrapper, because the current C ABI already
has a remotely exercised Rust consumer and its packaging boundary can remain
source-based and zero-budget.

## References

[1] [IBM Quantum Documentation: Introduction to Qiskit](https://quantum.cloud.ibm.com/docs/en/guides)  
[2] [PennyLane: Quantum Programming Software](https://pennylane.ai/)  
[3] [Microsoft Learn: Introduction to Q#](https://learn.microsoft.com/en-us/azure/quantum/qsharp-overview)  
[4] [Google Quantum AI: Cirq Overview](https://quantumai.google/cirq)
