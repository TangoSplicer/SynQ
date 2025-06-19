// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

Project Structure and Compilation

A SynQ project typically has a structure similar to a C++ or Java project. Source code lives in a src/ directory (compiler and runtime), with a stdlib/ for standard libraries and an examples/ folder for example programs. The compiler (synqc) translates SynQ code into an intermediate representation or machine code, and the runtime can execute the compiled program.

To compile and run a SynQ program:

1. Write your code in a .synq file.


2. Run the SynQ compiler: for example, synqc my_program.synq (this produces an executable or bytecode).


3. Use the SynQ runtime to execute the output, or run the produced executable directly.



Currently, SynQ is in the prototype stage – the compiler and runtime are basic stubs, but they outline how the system works. As development continues, the compiler will be extended to fully support the SynQ grammar and the runtime will interface with real quantum backends and AI model APIs.
