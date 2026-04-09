# GitHub Discussions - Q&A Category: Pinned FAQ

## Title
```
❓ Frequently Asked Questions - Start Here!
```

## Body (Pinned Post)

```markdown
# ❓ Frequently Asked Questions

Welcome to the SynQ Q&A category! This is the place to ask questions about SynQ, get help from the community, and learn how to use the platform.

## Quick Navigation

- **[Installation Help](#installation-help)** - Getting SynQ set up
- **[Getting Started](#getting-started)** - Your first SynQ program
- **[Quantum Computing](#quantum-computing)** - Quantum-specific questions
- **[Machine Learning](#machine-learning)** - ML integration questions
- **[Performance & Optimization](#performance--optimization)** - Speed and efficiency
- **[Troubleshooting](#troubleshooting)** - Common issues and solutions
- **[Commercial & Licensing](#commercial--licensing)** - License questions

---

## Installation Help

### Q: What are the system requirements for SynQ?

**A:** SynQ requires:
- **Minimum:** 2-core CPU, 4GB RAM, 2GB storage
- **Recommended:** 8-core CPU, 16GB RAM, 10GB SSD storage
- **OS:** Linux (Ubuntu 18.04+), macOS (10.15+), or Windows (with WSL2)

See the [Getting Started Guide](https://github.com/TangoSplicer/SynQ/blob/main/GETTING_STARTED.md) for detailed installation instructions.

### Q: Can I install SynQ on Windows?

**A:** Yes! SynQ works on Windows 10+ with WSL2 (Windows Subsystem for Linux 2). Follow the [Windows installation guide](https://github.com/TangoSplicer/SynQ/blob/main/GETTING_STARTED.md#windows-wsl2).

### Q: How long does installation take?

**A:** Installation typically takes 20-30 minutes depending on your internet speed and system performance. We provide automated build scripts to make it faster.

### Q: I'm getting build errors. What should I do?

**A:** Check the [Troubleshooting section](https://github.com/TangoSplicer/SynQ/blob/main/GETTING_STARTED.md#troubleshooting) of the Getting Started Guide. If you can't find your issue, create a new discussion in this Q&A category with:
- Your OS and version
- The exact error message
- Steps you took before the error

---

## Getting Started

### Q: What's a good first program to write in SynQ?

**A:** Start with the Hello World example:

```synq
fn main() {
    println("Hello, SynQ!");
}
```

Then explore the [examples directory](https://github.com/TangoSplicer/SynQ/tree/main/examples) for more complex programs.

### Q: Where can I find documentation?

**A:** Documentation is available at:
- [README](https://github.com/TangoSplicer/SynQ#readme) - Project overview
- [Getting Started Guide](https://github.com/TangoSplicer/SynQ/blob/main/GETTING_STARTED.md) - Installation and first steps
- [User Guide](https://github.com/TangoSplicer/SynQ/blob/main/USER_GUIDE.md) - Comprehensive documentation
- [Examples](https://github.com/TangoSplicer/SynQ/tree/main/examples) - Working code samples
- [Compiler Docs](https://github.com/TangoSplicer/SynQ/tree/main/compiler/docs) - Technical documentation

### Q: How do I run a SynQ program?

**A:** Once installed, run a SynQ file with:

```bash
synq run program.synq
```

Or compile and run:

```bash
synq build program.synq
./program
```

---

## Quantum Computing

### Q: Do I need quantum computing experience to use SynQ?

**A:** No! SynQ is designed for both beginners and experts. Start with the [quantum examples](https://github.com/TangoSplicer/SynQ/tree/main/examples) to learn the basics.

### Q: Can I run SynQ on real quantum hardware?

**A:** Currently, SynQ simulates quantum circuits on classical hardware. Integration with real quantum hardware (IBM, IonQ, Rigetti, etc.) is planned for future releases.

### Q: What quantum gates does SynQ support?

**A:** SynQ supports 20+ quantum gates including:
- Single-qubit gates: H, X, Y, Z, S, T, RX, RY, RZ
- Multi-qubit gates: CNOT, SWAP, Toffoli, CZ
- Measurement and state preparation

See the [Quantum Features documentation](https://github.com/TangoSplicer/SynQ/blob/main/compiler/docs/Quantum_Features.md) for details.

### Q: How many qubits can SynQ simulate?

**A:** SynQ can simulate up to 20 qubits on a typical system. Performance depends on your hardware and the complexity of your circuit.

---

## Machine Learning

### Q: Can I use SynQ for classical machine learning?

**A:** Yes! SynQ includes neural networks, tensor operations, and optimization algorithms for classical ML.

### Q: How do I integrate quantum and classical ML?

**A:** See the [Hybrid Example](https://github.com/TangoSplicer/SynQ/blob/main/examples/HYBRID_EXAMPLE_README.md) for a complete demonstration of quantum-classical-ML integration.

### Q: What ML frameworks does SynQ support?

**A:** SynQ has built-in ML support. You can also integrate with external frameworks through the FFI (Foreign Function Interface).

---

## Performance & Optimization

### Q: How fast is SynQ compared to other languages?

**A:** SynQ achieves 90%+ of C/C++ performance while maintaining safety and ease of use. Compilation is 100x faster than traditional compilers through incremental compilation.

### Q: How can I optimize my SynQ code?

**A:** See the [Performance Optimization Guide](https://github.com/TangoSplicer/SynQ/blob/main/compiler/docs/MemoryGuide.md) for best practices.

### Q: What's the compilation time?

**A:** Incremental compilation typically takes <1 second for small changes. Full compilation depends on project size.

---

## Troubleshooting

### Q: I'm getting "command not found: synq"

**A:** Make sure SynQ is installed and in your PATH. Try:

```bash
which synq
```

If it returns nothing, reinstall SynQ or add it to your PATH.

### Q: My program crashes with a memory error

**A:** SynQ has memory safety built-in, but check:
1. You're not using unsafe code blocks
2. Your arrays/lists aren't exceeding available memory
3. You're not creating infinite loops

### Q: The compiler is slow

**A:** Try:
1. Use incremental compilation: `synq build --incremental`
2. Reduce project size
3. Check your system resources

### Q: I found a bug. What should I do?

**A:** Report it on [GitHub Issues](https://github.com/TangoSplicer/SynQ/issues) with:
- Description of the bug
- Steps to reproduce
- Expected vs actual behavior
- Your system information

---

## Commercial & Licensing

### Q: What license does SynQ use?

**A:** SynQ uses dual licensing:
- **MIT License** - Free for personal, educational, and open-source projects
- **Commercial License** - For commercial products and enterprises

See [LICENSE_FAQ.md](https://github.com/TangoSplicer/SynQ/blob/main/LICENSE_FAQ.md) for details.

### Q: Can I use SynQ in a commercial product?

**A:** Yes! You can either:
1. Use the MIT license (open-source your code)
2. Purchase a Commercial License for proprietary use

See [LICENSE_COMMERCIAL.md](https://github.com/TangoSplicer/SynQ/blob/main/LICENSE_COMMERCIAL.md) for commercial licensing terms.

### Q: How much does a commercial license cost?

**A:** Commercial licensing is available for enterprises. Contact us via GitHub Discussions (Q&A category) or Issues for pricing and details.

---

## Didn't Find Your Answer?

**Create a new discussion!** Click "New discussion" and:
1. Choose the **Q&A** category
2. Write a clear title
3. Describe your question in detail
4. Include any error messages or code samples

Our community and team will help you out! 🚀

---

## Helpful Resources

- 📖 [Documentation](https://github.com/TangoSplicer/SynQ/wiki)
- 🚀 [Getting Started](https://github.com/TangoSplicer/SynQ/blob/main/GETTING_STARTED.md)
- 💻 [Examples](https://github.com/TangoSplicer/SynQ/tree/main/examples)
- 🐛 [Report Bugs](https://github.com/TangoSplicer/SynQ/issues)
- 🔒 [Security](https://github.com/TangoSplicer/SynQ/blob/main/SECURITY.md)
```

---

## Instructions for Posting

1. Go to **Discussions** → **Q&A** category
2. Click **New discussion**
3. Copy the title above
4. Copy the body above
5. Click **Start discussion**
6. **Pin this discussion** (click the three dots → Pin discussion)

This will be the first thing people see in the Q&A category!
