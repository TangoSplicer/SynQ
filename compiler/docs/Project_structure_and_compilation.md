// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
Project Structure and Compilation

A SynQ project typically has a structure similar to a C++ or Java project. Source code lives in a src/ directory (compiler and runtime), with a stdlib/ for standard libraries and an examples/ folder for example programs. The compiler (synqc) translates SynQ code into an intermediate representation or machine code, and the runtime can execute the compiled program.

To compile and run a SynQ program:

1. Write your code in a .synq file.


2. Run the SynQ compiler: for example, synqc my_program.synq (this produces an executable or bytecode).


3. Use the SynQ runtime to execute the output, or run the produced executable directly.



Currently, SynQ is in the prototype stage – the compiler and runtime are basic stubs, but they outline how the system works. As development continues, the compiler will be extended to fully support the SynQ grammar and the runtime will interface with real quantum backends and AI model APIs.
