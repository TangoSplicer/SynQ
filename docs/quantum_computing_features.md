// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

Quantum Computing Features

SynQ has built-in quantum computing support. You can create quantum registers and apply quantum gates using the quantum module:

QubitRegister: Represents a collection of qubits you can manipulate. For example, q = quantum.QubitRegister(4) creates 4 qubits initialized to |0000⟩.

Quantum Gates: Methods like hadamard(i) and cnot(ctrl, tgt) apply standard quantum gates (Hadamard, CNOT) to the qubits.


The standard library provides advanced quantum algorithms:

QEBET (Quantum Entanglement-Based Encoding Technique): A novel routine that encodes classical data into an entangled quantum state. This could be used for quantum cryptography or dense data storage in qubits (in SynQ, see qebet.encode function).

QPE-LA (Quantum Phase Estimation for Linear Algebra): An algorithm that uses Quantum Phase Estimation to solve linear systems of equations, based on the HHL algorithm. This demonstrates how quantum computers can find solutions exponentially faster for certain problems by estimating matrix eigenvalues and using them to invert the matrix.

Graph Algorithms: SynQ includes quantum routines for graph problems. For instance, a function graph.find_marked_node uses a Szegedy quantum walk combined with phase estimation to locate a special "marked" node in a graph, illustrating the quantum advantage in network analysis.


Using these features, you can write programs that prepare quantum states, run quantum subroutines, and bring the results back into classical variables. SynQ handles the details of interfacing with quantum hardware or simulators in the background, allowing you to focus on algorithm logic.

AI Integration

One of SynQ’s unique capabilities is native integration of AI models:

The ai module provides classes like ChatGPT and LLaMA which allow your program to send queries to AI models and obtain generated responses.

This means your SynQ program can, for example, use a large language model to generate text, code, or decisions dynamically at runtime.


Usage Example: You can create an AI assistant in code:

import ai

assistant = ai.ChatGPT(model="gpt-4")
reply = assistant.query("Explain the quantum superposition principle.")
print(reply)

In this example, the program queries a GPT-4 model for an explanation. In practice, the SynQ runtime would handle connecting to the model (via API or local inference). Similarly, you can use the ai.LLaMA class for open-source LLMs.

Integrating AI at the language level opens up new possibilities, such as programs that can explain their own code, adapt based on natural language instructions, or perform complex decision-making by leveraging pretrained knowledge.
