# Alpha Identifier-`if` OpenQASM 3 Lowering

**Status:** Alpha strict-Hybrid source-lowering contract. This document defines
one target-side classical-storage mapping; it does not define SynQ execution,
assignment, mutable state, measurement feedback, loops, or a stable OpenQASM
compatibility guarantee.

## Purpose and scope

SynQ already parses and statically resolves bounded Boolean identifier and
expression conditions under the `classical-control-flow` Alpha opt-in. The
strict-Hybrid exporter now lowers one additional, deliberately smaller subset:
an `if` whose condition is an earlier top-level Boolean **literal declaration**
and whose body is exactly one supported typed quantum gate.

This is target-side source lowering, not local branch execution. OpenQASM 3
defines Boolean values and conditional statements, while allowing hardware
implementations to restrict unsupported runtime processing. SynQ therefore
emits only a Boolean value initialized from `true` or `false`; it does not infer
support for measurement feedback or arbitrary runtime computation.[1] [2]

## Accepted SynQ source

The source must opt into both existing Alpha features and must declare the
Boolean before the controlled gate.

```synq
#[experimental(feature = "qubit-declarations")]
#[experimental(feature = "classical-control-flow")]
let enabled = true
qubit q[1]
if enabled then quantum h q[0]
```

The exact strict-Hybrid output is:

```openqasm
OPENQASM 3.0;
include "stdgates.inc";
qubit[1] q;
bool synq_bool_enabled = true;
if (synq_bool_enabled) h q[0];
```

The emitted `synq_bool_<source-name>` identifier is generated target storage.
It is not a SynQ user binding, an ABI name, a stable textual-output promise, or
a declaration of mutable runtime memory. The exporter emits one initialization
only and emits no assignment operation.

| Aspect | Contract |
| --- | --- |
| Source condition | One earlier top-level `let name = true` or `let name = false` binding, referenced as `if name then ...`. |
| Target representation | `bool synq_bool_<name> = true|false;` declared in the generated OpenQASM source. |
| Controlled body | Exactly one existing supported typed quantum gate with earlier declared, in-range qubit operands. |
| Feature gate | The existing Alpha `classical-control-flow` gate remains mandatory; the feature is not stable. |
| Validation | Exact C++ exporter smoke coverage plus a generated fixture accepted by the OpenQASM 3 reference parser. |

## Deliberate rejections

The exporter rejects the following forms rather than inventing target-state or
execution semantics.

| Rejected form | Reason |
| --- | --- |
| `let alias = enabled` followed by `if alias` | Declaration aliases require a separate constant-propagation and target-storage contract. |
| `if not enabled`, `if enabled and fallback`, or `if enabled or fallback` | Boolean-expression evaluation and target precedence are not in this lowering subset. |
| `if observed` after `measure q[0] as observed` | Named measurement-result export and measurement-feedback storage are not implemented. |
| Forward, unresolved, or non-Boolean identifier conditions | No prior compatible target storage exists. |
| `while`, `else`, blocks, or measurement bodies | They require loop/branch lifetime, termination, and measurement semantics. |
| Assignment or mutation | SynQ has no assignment/runtime model in this recovery profile. |

The local simulator does not execute the resulting branch. The C ABI, static
SDK, and source-only Alpha Rust wrapper do not expose a control-flow runtime as
a result of this export capability.

The existing Qiskit OpenQASM 3 importer is retained for the established
uncontrolled reference fixture, but it currently rejects `bool` declarations
needed by this control form. That importer limitation is therefore not used as
an acceptance claim for identifier-`if` export; the OpenQASM 3 reference parser
is the independent syntax-validation boundary for this increment.

## Safety and evolution rule

Any broader control feature must first choose a single semantic path:

| Path | Prerequisite before acceptance |
| --- | --- |
| Target lowering | A written mapping for classical storage, writes, lifetime, types, target support, exact source output, parser/import proof, and negative fixtures. |
| Local execution | A written state model, bounded termination/resource policy, deterministic results, error behavior, and simulator coverage. |

This contract intentionally chooses neither path for Boolean expressions,
measurements, loops, or mutable values. A future increment must update the
contract and its tests before broadening the accepted source surface.

## References

[1]: https://openqasm.com/language/classical.html "OpenQASM Live Specification — Classical Instructions"

[2]: https://openqasm.com/versions/3.0/intro.html "OpenQASM 3.0 Specification — Introduction and Implementation Details"
