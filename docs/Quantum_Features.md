// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

# Quantum Features in SynQ

SynQ offers full-spectrum support for quantum computing:

## ✅ Gates
- Standard: H, X, Y, Z, CNOT
- Parametric: RX, RY, RZ, U3(theta, phi, lambda)
- Custom symbolic gates via meta_circuits

## ✅ Measurements
```synq
MEASURE(0)
@assert QUBIT_STATE(0) in {"|0>", "|1>"}
```

## ✅ Circuits
```synq
H(0)
CNOT(0,1)
```

## ✅ Variational Templates
Support for parametric circuits for QAOA, VQE, and Grover.

## ✅ Simulation
Powered by QSimLA engine with support for:
- Latent vector injection
- Snapshotting
- Amplitude drift detection

