# SynQ Beginner Tutorial: Your First Hybrid Quantum Script

**Status:** Beginner-friendly getting-started guide for the SynQ experimental recovery profile.
**Last reviewed:** 16 August 2026

## Welcome to SynQ

SynQ is an experimental hybrid quantum–classical language kernel. If you are
new to quantum computing or compiler development, this tutorial will guide you
through building the compiler, running your first validation check, exporting
clean OpenQASM 3, and simulating a Bell state locally.

---

## Step 1: Prerequisites & Build

Make sure you are on a supported Ubuntu-like Linux environment (as documented in
[`TESTED_ENVIRONMENTS.md`](./TESTED_ENVIRONMENTS.md)). Install the required
build tools and clone or open the repository:

```bash
sudo apt-get install -y cmake g++ nlohmann-json3-dev libssl-dev
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
```

Verify your build by running the test suite:

```bash
ctest --test-dir compiler/build --output-on-failure
```

---

## Step 2: Write Your First SynQ File

Create a file named `bell.synq` in your working directory. We will use an
Alpha feature gate for qubit declarations (`qubit-declarations`):

```synq
#[experimental(feature = "qubit-declarations")]
qubit q[2]
quantum h q[0]
quantum cx q[0], q[1]
```

---

## Step 3: Validate and Export OpenQASM 3

Run the compiler CLI (`synqc`) to validate your source code:

```bash
./compiler/build/synqc bell.synq --validate
```

If the file is syntactically valid, the CLI returns exit code `0`.

Now, emit strict Hybrid OpenQASM 3 source code:

```bash
./compiler/build/synqc bell.synq --emit-openqasm-hybrid --out bell.qasm
cat bell.qasm
```

You will see clean OpenQASM 3 output ready for external simulators or parsers:

```qasm
OPENQASM 3.0;
qreg q[2];
h q[0];
cx q[0], q[1];
```

---

## Step 4: Simulate Locally

Compute the pure-state basis and marginal probabilities of your circuit using
SynQ's bounded local simulator:

```bash
./compiler/build/synqc bell.synq --simulate
```

The simulator computes the exact quantum state probabilities without sampling or
noise, showing that states `|00>` and `|11>` each occur with 50% probability.

---

## Understanding Compiler Errors

SynQ uses structured diagnostics. If you write an invalid statement—such as
referencing an undeclared qubit or omitting an experimental feature gate—the
compiler will print a clear diagnostic code (e.g., `SYNQ-S002` or `SYNQ-P001`)
to help you learn the language rules quickly.

For more details, explore [`docs/CLI.md`](./CLI.md) and
[`docs/PROJECT_STATUS.md`](./PROJECT_STATUS.md).
