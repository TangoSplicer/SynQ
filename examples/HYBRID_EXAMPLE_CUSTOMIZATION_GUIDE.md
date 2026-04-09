# Hybrid Quantum-ML Example: Comprehensive Customization Guide

**Purpose:** Learn how to modify the hybrid example for your specific use case  
**Audience:** Intermediate to advanced SynQ developers  
**Time to Complete:** 30-60 minutes per customization  

---

## Table of Contents

1. [Quick Customizations](#quick-customizations)
2. [Algorithm-Specific Customizations](#algorithm-specific-customizations)
3. [Advanced Customizations](#advanced-customizations)
4. [Real-World Use Cases](#real-world-use-cases)
5. [Troubleshooting Customizations](#troubleshooting-customizations)

---

## Quick Customizations

These are simple parameter changes you can make without modifying the code structure.

### Customization 1: Increase Optimization Iterations

**What it does:** Run more iterations to find better solutions

**Original code (VQE):**
```synq
let iterations = 10
```

**Customized code:**
```synq
let iterations = 50  // More iterations = better solution (slower)
```

**Impact:**
- Longer execution time (5-10x slower)
- Better optimization results
- More accurate energy estimates

**When to use:**
- When you need high accuracy
- When computational time is not a constraint
- For production deployments

---

### Customization 2: Adjust Learning Rate

**What it does:** Controls how fast the optimizer learns

**Original code (QML):**
```synq
let learning_rate = 0.01
```

**Customized code - Fast Learning:**
```synq
let learning_rate = 0.1  // Faster convergence (may overshoot)
```

**Customized code - Slow Learning:**
```synq
let learning_rate = 0.001  // Slower but more stable convergence
```

**Impact:**
- Higher learning rate: Faster but less stable
- Lower learning rate: Slower but more stable
- Optimal value depends on your problem

**When to use:**
- Fast learning: Quick prototyping, small problems
- Slow learning: Production, large problems, sensitive systems

---

### Customization 3: Change Number of Training Epochs

**What it does:** Controls how many times the model sees training data

**Original code (QML):**
```synq
let epochs = 5
```

**Customized code:**
```synq
let epochs = 20  // More epochs = better training (slower)
```

**Impact:**
- More epochs: Better model accuracy
- Fewer epochs: Faster training
- Diminishing returns after certain point

**When to use:**
- More epochs: When accuracy is critical
- Fewer epochs: For quick testing and prototyping

---

### Customization 4: Increase Number of Qubits

**What it does:** Makes quantum circuits larger and more powerful

**Original code (QAOA):**
```synq
let num_qubits = 4
```

**Customized code:**
```synq
let num_qubits = 8  // Larger problem (exponentially slower)
```

**Important:** Quantum simulation is exponential in qubit count!

| Qubits | Simulation Time | Memory |
|--------|-----------------|--------|
| 2 | <1ms | 1 MB |
| 4 | 1ms | 10 MB |
| 8 | 100ms | 1 GB |
| 12 | 10s | 100 GB |
| 16 | 1000s | 10 TB |
| 20 | Infeasible | Infeasible |

**When to use:**
- 4-8 qubits: Testing and development
- 8-12 qubits: Small production problems
- 12+ qubits: Real quantum hardware only

---

### Customization 5: Modify Parameter Ranges

**What it does:** Changes the range of parameters being optimized

**Original code (VQE):**
```synq
let theta = i as float * learning_rate  // 0 to 1.0
let phi = (iterations as float - i as float) * learning_rate  // 1.0 to 0
```

**Customized code - Larger range:**
```synq
let theta = i as float * learning_rate * 3.14159  // 0 to π
let phi = (iterations as float - i as float) * learning_rate * 3.14159  // π to 0
```

**Customized code - Smaller range:**
```synq
let theta = i as float * learning_rate * 0.5  // 0 to 0.5
let phi = (iterations as float - i as float) * learning_rate * 0.5  // 0.5 to 0
```

**Impact:**
- Larger range: Explores more parameter space (slower)
- Smaller range: Focuses on specific region (faster)

**When to use:**
- Larger range: When you don't know optimal parameters
- Smaller range: When you have prior knowledge

---

## Algorithm-Specific Customizations

These customizations modify specific algorithms for different problems.

### VQE Customizations

#### Customization 6: Add More Quantum Layers

**What it does:** Makes the quantum circuit deeper and more expressive

**Original code:**
```synq
@fn vqe_circuit(theta: float, phi: float): -> Circuit
    let reg = Register(2)
    let circ = Circuit(reg)
    
    // Initial state
    circ.h(reg[0])
    circ.h(reg[1])
    
    // Layer 1
    circ.ry(reg[0], theta)
    circ.ry(reg[1], theta)
    circ.cnot(reg[0], reg[1])
    
    // Layer 2
    circ.rz(reg[0], phi)
    circ.rz(reg[1], phi)
    circ.cnot(reg[1], reg[0])
    
    return circ
```

**Customized code - 3 Layers:**
```synq
@fn vqe_circuit(theta: float, phi: float): -> Circuit
    let reg = Register(2)
    let circ = Circuit(reg)
    
    // Initial state
    circ.h(reg[0])
    circ.h(reg[1])
    
    // Layer 1
    circ.ry(reg[0], theta)
    circ.ry(reg[1], theta)
    circ.cnot(reg[0], reg[1])
    
    // Layer 2
    circ.rz(reg[0], phi)
    circ.rz(reg[1], phi)
    circ.cnot(reg[1], reg[0])
    
    // Layer 3 (NEW)
    circ.ry(reg[0], theta * 0.5)
    circ.ry(reg[1], phi * 0.5)
    circ.cnot(reg[0], reg[1])
    
    return circ
```

**Impact:**
- Deeper circuits: More expressive, better results
- Shallower circuits: Faster execution, less noise

**When to use:**
- More layers: When current solution is not accurate enough
- Fewer layers: For NISQ (Noisy Intermediate-Scale Quantum) devices

---

#### Customization 7: Change Quantum Gates

**What it does:** Uses different quantum gates for different effects

**Original code - RY and RZ gates:**
```synq
circ.ry(reg[0], theta)
circ.rz(reg[0], phi)
```

**Customized code - RX and RY gates:**
```synq
circ.rx(reg[0], theta)
circ.ry(reg[0], phi)
```

**Customized code - Pauli gates:**
```synq
circ.x(reg[0])  // Pauli-X (NOT gate)
circ.y(reg[1])  // Pauli-Y
circ.z(reg[0])  // Pauli-Z
```

**Available gates:**
- **Single-qubit:** H (Hadamard), X, Y, Z, S, T, RX, RY, RZ
- **Two-qubit:** CNOT, CZ, SWAP
- **Three-qubit:** Toffoli, Fredkin

**When to use:**
- RX/RY/RZ: General parameterized rotations
- Pauli gates: Specific problem requirements
- CNOT: Entanglement
- CZ: Alternative entanglement

---

#### Customization 8: Add Multiple Qubits to VQE

**What it does:** Solves larger quantum chemistry problems

**Original code - 2 qubits:**
```synq
let reg = Register(2)
```

**Customized code - 4 qubits:**
```synq
let reg = Register(4)
let circ = Circuit(reg)

// Apply gates to all qubits
for i in 0..4:
    circ.h(reg[i])

// Entangle all qubits
for i in 0..3:
    circ.cnot(reg[i], reg[i + 1])
```

**Impact:**
- More qubits: Larger molecular systems
- Exponential complexity increase

**When to use:**
- 2-4 qubits: Testing and development
- 4-8 qubits: Small molecules (H2, LiH)
- 8+ qubits: Real quantum hardware

---

### QML Customizations

#### Customization 9: Add More Training Data

**What it does:** Trains the model on more examples

**Original code - 4 samples:**
```synq
let training_data = [
    [0.1, 0.2],
    [0.3, 0.4],
    [0.5, 0.6],
    [0.7, 0.8]
]

let training_labels = [0, 0, 1, 1]
```

**Customized code - 8 samples:**
```synq
let training_data = [
    [0.1, 0.2],
    [0.15, 0.25],
    [0.3, 0.4],
    [0.35, 0.45],
    [0.5, 0.6],
    [0.55, 0.65],
    [0.7, 0.8],
    [0.75, 0.85]
]

let training_labels = [0, 0, 0, 0, 1, 1, 1, 1]
```

**Impact:**
- More data: Better generalization, longer training
- Less data: Faster training, risk of overfitting

**When to use:**
- More data: Production models
- Less data: Quick testing

---

#### Customization 10: Change Feature Encoding

**What it does:** Encodes classical data differently into quantum states

**Original code - RY encoding:**
```synq
@fn quantum_feature_map(data: List<float>): -> Circuit
    let reg = Register(2)
    let circ = Circuit(reg)
    
    circ.ry(reg[0], data[0])
    circ.ry(reg[1], data[1])
    circ.cnot(reg[0], reg[1])
    
    return circ
```

**Customized code - RX encoding:**
```synq
@fn quantum_feature_map(data: List<float>): -> Circuit
    let reg = Register(2)
    let circ = Circuit(reg)
    
    circ.rx(reg[0], data[0])
    circ.rx(reg[1], data[1])
    circ.cnot(reg[0], reg[1])
    
    return circ
```

**Customized code - Amplitude encoding:**
```synq
@fn quantum_feature_map(data: List<float>): -> Circuit
    let reg = Register(2)
    let circ = Circuit(reg)
    
    // Normalize data
    let norm = data[0] * data[0] + data[1] * data[1]
    let a = data[0] / norm
    let b = data[1] / norm
    
    // Amplitude encoding
    circ.ry(reg[0], 2.0 * a)
    circ.ry(reg[1], 2.0 * b)
    
    return circ
```

**Impact:**
- Different encodings: Different feature extraction
- RY/RX: Simple, fast
- Amplitude: More expressive, slower

**When to use:**
- RY/RX: Quick prototyping
- Amplitude: When you need better feature extraction

---

#### Customization 11: Implement Custom Loss Function

**What it does:** Uses different loss functions for training

**Original code - Mean Squared Error:**
```synq
let loss = (prediction - label) * (prediction - label)
```

**Customized code - Binary Cross-Entropy:**
```synq
let epsilon = 0.0001
let loss = -(label * log(prediction + epsilon) + 
             (1.0 - label) * log(1.0 - prediction + epsilon))
```

**Customized code - Hinge Loss:**
```synq
let margin = 1.0
let loss = max(0.0, margin - label * prediction)
```

**Impact:**
- Different loss functions: Different optimization behavior
- MSE: Regression problems
- Cross-entropy: Classification problems
- Hinge loss: SVM-style classification

**When to use:**
- MSE: Regression
- Cross-entropy: Binary classification
- Hinge loss: Margin-based classification

---

### QAOA Customizations

#### Customization 12: Add More QAOA Layers

**What it does:** Makes QAOA circuit deeper for better approximation

**Original code - 1 layer:**
```synq
@fn qaoa_circuit(gamma: float, beta: float, num_qubits: int): -> Circuit
    let reg = Register(num_qubits)
    let circ = Circuit(reg)
    
    // Initial superposition
    for i in 0..num_qubits:
        circ.h(reg[i])
    
    // Problem Hamiltonian
    for i in 0..(num_qubits - 1):
        circ.rzz(reg[i], reg[i + 1], gamma)
    
    // Mixer Hamiltonian
    for i in 0..num_qubits:
        circ.rx(reg[i], beta)
    
    return circ
```

**Customized code - 3 layers:**
```synq
@fn qaoa_circuit(gamma: float, beta: float, num_qubits: int): -> Circuit
    let reg = Register(num_qubits)
    let circ = Circuit(reg)
    
    // Initial superposition
    for i in 0..num_qubits:
        circ.h(reg[i])
    
    // Layer 1
    for i in 0..(num_qubits - 1):
        circ.rzz(reg[i], reg[i + 1], gamma)
    for i in 0..num_qubits:
        circ.rx(reg[i], beta)
    
    // Layer 2
    for i in 0..(num_qubits - 1):
        circ.rzz(reg[i], reg[i + 1], gamma * 0.7)
    for i in 0..num_qubits:
        circ.rx(reg[i], beta * 0.7)
    
    // Layer 3
    for i in 0..(num_qubits - 1):
        circ.rzz(reg[i], reg[i + 1], gamma * 0.5)
    for i in 0..num_qubits:
        circ.rx(reg[i], beta * 0.5)
    
    return circ
```

**Impact:**
- More layers: Better approximation ratio
- Fewer layers: Faster execution

**When to use:**
- 1-2 layers: Quick testing
- 3-5 layers: Production problems
- 5+ layers: Only on real quantum hardware

---

#### Customization 13: Change Problem Hamiltonian

**What it does:** Solves different optimization problems

**Original code - MaxCut (ZZ interactions):**
```synq
for i in 0..(num_qubits - 1):
    circ.rzz(reg[i], reg[i + 1], gamma)
```

**Customized code - Weighted MaxCut:**
```synq
let weights = [1.0, 2.0, 1.5, 0.8]  // Different edge weights

for i in 0..(num_qubits - 1):
    circ.rzz(reg[i], reg[i + 1], gamma * weights[i])
```

**Customized code - All-to-all interactions:**
```synq
for i in 0..num_qubits:
    for j in (i + 1)..num_qubits:
        circ.rzz(reg[i], reg[j], gamma)
```

**Customized code - Custom problem:**
```synq
// Traveling Salesman Problem encoding
for i in 0..(num_qubits - 1):
    circ.rzz(reg[i], reg[(i + 1) % num_qubits], gamma)
```

**Impact:**
- Different Hamiltonians: Different problems
- ZZ: MaxCut, graph partitioning
- All-to-all: Fully connected problems
- Custom: Domain-specific problems

**When to use:**
- MaxCut: Graph partitioning
- Weighted: Real-world graphs
- All-to-all: Dense problems
- Custom: Your specific problem

---

## Advanced Customizations

These customizations combine multiple changes for sophisticated use cases.

### Customization 14: Hybrid VQE-QML System

**What it does:** Uses VQE to initialize QML weights

**Code:**
```synq
// Run VQE to find good parameters
vqe_optimization()

// Use VQE parameters to initialize QML weights
let vqe_weights = [best_theta, best_phi]

// Train QML with VQE-initialized weights
let weights = vqe_weights
for epoch in 0..epochs:
    for i in 0..training_data.length():
        let prediction = quantum_classifier(training_data[i], weights)
        // Update weights...
```

**Impact:**
- Better initialization leads to faster convergence
- Combines strengths of both algorithms

**When to use:**
- When you have prior knowledge from VQE
- For transfer learning scenarios

---

### Customization 15: Adaptive Learning Rate

**What it does:** Changes learning rate during training

**Code:**
```synq
let initial_learning_rate = 0.1
let final_learning_rate = 0.001

for epoch in 0..epochs:
    // Decay learning rate
    let progress = epoch as float / epochs as float
    let learning_rate = initial_learning_rate * (1.0 - progress) + 
                        final_learning_rate * progress
    
    for i in 0..training_data.length():
        let prediction = quantum_classifier(training_data[i], weights)
        weights[0] = weights[0] - learning_rate * (prediction - label)
```

**Impact:**
- Faster initial convergence
- Finer tuning at the end
- Better final results

**When to use:**
- For production models
- When training is slow

---

### Customization 16: Ensemble of Circuits

**What it does:** Combines multiple quantum circuits for better results

**Code:**
```synq
@fn ensemble_prediction(data: List<float>, ensemble_weights: List<List<float>>): -> float
    let total_prediction = 0.0
    
    for weights in ensemble_weights:
        let prediction = quantum_classifier(data, weights)
        total_prediction = total_prediction + prediction
    
    let avg_prediction = total_prediction / ensemble_weights.length() as float
    return avg_prediction

// Train multiple classifiers
let ensemble_weights = []
for ensemble_idx in 0..5:
    let weights = [0.5, 0.5]
    // Train weights...
    ensemble_weights.append(weights)

// Use ensemble for prediction
let ensemble_pred = ensemble_prediction(test_data, ensemble_weights)
```

**Impact:**
- Better generalization
- Reduced variance
- Slower inference

**When to use:**
- For critical applications
- When accuracy is paramount

---

## Real-World Use Cases

### Use Case 1: Drug Discovery with Custom Molecules

**Goal:** Find ground state energy of a specific molecule

**Customizations needed:**
1. Increase VQE iterations to 100
2. Add more qubits (6-8 for small molecules)
3. Implement proper Hamiltonian for target molecule
4. Add error correction

**Code template:**
```synq
// Target molecule: LiH (Lithium Hydride)
let iterations = 100
let num_qubits = 6

@fn liH_hamiltonian(circuit: Circuit, theta: float, phi: float): -> void
    // Implement LiH-specific Hamiltonian
    // ...

@fn drug_discovery_vqe(): -> void
    for i in 0..iterations:
        let theta = i as float * 0.1
        let phi = (iterations as float - i as float) * 0.1
        
        let circuit = vqe_circuit(theta, phi)
        liH_hamiltonian(circuit, theta, phi)
        
        let energy = estimate_energy_vqe(circuit)
        // Track best energy...
```

---

### Use Case 2: Portfolio Optimization

**Goal:** Optimize investment portfolio using QAOA

**Customizations needed:**
1. Increase QAOA layers to 5
2. Implement financial cost function
3. Add constraints (budget, risk limits)
4. Use real market data

**Code template:**
```synq
let num_qubits = 10  // 10 assets
let num_layers = 5

@fn portfolio_cost_function(assets: List<float>, returns: List<float>): -> float
    // Calculate portfolio return and risk
    let total_return = 0.0
    let total_risk = 0.0
    
    for i in 0..assets.length():
        total_return = total_return + assets[i] * returns[i]
    
    // Risk calculation...
    return total_return - total_risk

@fn portfolio_optimization(): -> void
    // Use QAOA with portfolio cost function
    // ...
```

---

### Use Case 3: Quantum Machine Learning for Time Series

**Goal:** Predict stock prices using quantum ML

**Customizations needed:**
1. Increase training data to 1000+ samples
2. Implement sliding window feature extraction
3. Add validation set
4. Implement early stopping

**Code template:**
```synq
let training_data = load_stock_data(1000)
let validation_data = load_stock_data(200)
let epochs = 50

@fn time_series_qml(): -> void
    let weights = [0.5, 0.5]
    let best_val_loss = 999.0
    let patience = 5
    let no_improve_count = 0
    
    for epoch in 0..epochs:
        // Training loop...
        
        // Validation
        let val_loss = 0.0
        for i in 0..validation_data.length():
            let prediction = quantum_classifier(validation_data[i], weights)
            val_loss = val_loss + (prediction - labels[i]) * (prediction - labels[i])
        
        // Early stopping
        if val_loss < best_val_loss:
            best_val_loss = val_loss
            no_improve_count = 0
        else:
            no_improve_count = no_improve_count + 1
        
        if no_improve_count >= patience:
            break
```

---

## Troubleshooting Customizations

### Problem 1: Optimization Not Converging

**Symptoms:** Loss doesn't decrease, energy stays constant

**Solutions:**
1. Increase learning rate
2. Add more training data
3. Simplify the problem
4. Check parameter ranges

**Code fix:**
```synq
// Before
let learning_rate = 0.001

// After
let learning_rate = 0.1  // 100x increase
```

---

### Problem 2: Out of Memory

**Symptoms:** Program crashes with memory error

**Solutions:**
1. Reduce number of qubits
2. Reduce training data size
3. Use smaller batch size
4. Clear intermediate results

**Code fix:**
```synq
// Before
let num_qubits = 16

// After
let num_qubits = 8  // Half the qubits
```

---

### Problem 3: Too Slow

**Symptoms:** Program takes too long to run

**Solutions:**
1. Reduce iterations
2. Reduce training data
3. Use fewer QAOA layers
4. Optimize circuit depth

**Code fix:**
```synq
// Before
let iterations = 100
let epochs = 50

// After
let iterations = 20
let epochs = 10
```

---

### Problem 4: Poor Results

**Symptoms:** Final results are not accurate

**Solutions:**
1. Increase iterations
2. Add more training data
3. Increase circuit depth
4. Adjust learning rate

**Code fix:**
```synq
// Before
let iterations = 10
let learning_rate = 0.1

// After
let iterations = 50
let learning_rate = 0.01  // Slower, more stable
```

---

## Customization Checklist

Before deploying your customized example, verify:

- [ ] All parameters are reasonable for your problem
- [ ] Number of qubits doesn't exceed available memory
- [ ] Learning rates are in reasonable range (0.001 - 0.1)
- [ ] Training data is properly formatted
- [ ] Iterations/epochs are sufficient for convergence
- [ ] Code compiles without errors
- [ ] Results are reasonable for your problem
- [ ] Performance is acceptable
- [ ] Error handling is in place

---

## Summary

You can customize the hybrid example by:

1. **Quick changes:** Modify parameters (iterations, learning rate, qubits)
2. **Algorithm changes:** Modify gates, layers, encodings
3. **Advanced changes:** Combine algorithms, add ensembles, implement custom functions
4. **Real-world:** Apply to specific domains (drug discovery, finance, ML)

Start with quick changes, then progress to more advanced customizations as you become comfortable with the code.

---

**Ready to customize?** Choose a use case and start modifying! 🚀
