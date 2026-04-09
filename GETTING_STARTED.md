# Getting Started with SynQ: Comprehensive Installation Guide

**Version:** 1.0  
**Last Updated:** March 2026  
**Scope:** Complete installation and setup for all operating systems

Welcome to SynQ! This comprehensive guide will help you install SynQ, configure your development environment, and write your first programs. Whether you're on Linux, macOS, or Windows, we have you covered.

## Table of Contents

1. [System Requirements](#system-requirements)
2. [Quick Start (Automated Scripts)](#quick-start-automated-scripts)
3. [Manual Installation](#manual-installation)
4. [Development Environment Setup](#development-environment-setup)
5. [Verification and Testing](#verification-and-testing)
6. [Your First Programs](#your-first-programs)
7. [Troubleshooting](#troubleshooting)
8. [Next Steps](#next-steps)

---

## System Requirements

### Minimum Requirements

- **CPU:** 2 cores, 2 GHz
- **RAM:** 4 GB
- **Storage:** 5 GB (for build artifacts and dependencies)
- **Internet:** Required for downloading dependencies

### Recommended Requirements

- **CPU:** 8 cores, 3 GHz
- **RAM:** 16 GB
- **Storage:** 10 GB SSD
- **Internet:** High-speed connection for faster downloads

### Operating Systems

- **Linux:** Ubuntu 18.04+, Debian 10+, Fedora 30+, CentOS 8+
- **macOS:** 10.15 (Catalina) or later
- **Windows:** Windows 10/11 with WSL2 (Windows Subsystem for Linux 2)

---

## Quick Start (Automated Scripts)

The fastest way to get SynQ running is to use the automated installation scripts. These scripts handle all dependencies and configuration automatically.

### For Linux (Ubuntu/Debian)

**Step 1: Download the installation script**

```bash
curl -fsSL https://raw.githubusercontent.com/TangoSplicer/SynQ/main/scripts/install-linux.sh -o install-synq.sh
chmod +x install-synq.sh
```

**Step 2: Run the script**

```bash
./install-synq.sh
```

The script will:
- Update system packages
- Install C++ compiler (GCC or Clang)
- Install CMake and build tools
- Install Python 3.8+
- Install Node.js and npm
- Clone SynQ repository
- Build the compiler
- Install backend dependencies
- Install frontend dependencies
- Run verification tests

**Estimated time:** 15-20 minutes

### For macOS

**Step 1: Download the installation script**

```bash
curl -fsSL https://raw.githubusercontent.com/TangoSplicer/SynQ/main/scripts/install-macos.sh -o install-synq.sh
chmod +x install-synq.sh
```

**Step 2: Run the script**

```bash
./install-synq.sh
```

The script will:
- Install Xcode Command Line Tools (if needed)
- Install Homebrew (if needed)
- Install C++ compiler
- Install CMake and build tools
- Install Python 3.8+
- Install Node.js and npm
- Clone SynQ repository
- Build the compiler
- Install backend dependencies
- Install frontend dependencies
- Run verification tests

**Estimated time:** 20-30 minutes (first run may take longer due to Xcode tools)

### For Windows (WSL2)

**Step 1: Enable WSL2**

If you don't have WSL2 enabled, open PowerShell as Administrator and run:

```powershell
wsl --install
```

Restart your computer when prompted.

**Step 2: Open WSL2 terminal**

Open Windows Terminal and select "Ubuntu" (or your preferred Linux distribution).

**Step 3: Download and run the installation script**

```bash
curl -fsSL https://raw.githubusercontent.com/TangoSplicer/SynQ/main/scripts/install-linux.sh -o install-synq.sh
chmod +x install-synq.sh
./install-synq.sh
```

The script will follow the same steps as Linux.

**Estimated time:** 20-30 minutes

---

## Manual Installation

If you prefer to install components manually or the automated script doesn't work for your system, follow these detailed instructions.

### Linux (Ubuntu/Debian)

#### Step 1: Update System Packages

```bash
sudo apt-get update
sudo apt-get upgrade -y
```

#### Step 2: Install Build Tools

```bash
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    wget \
    pkg-config \
    libssl-dev
```

Verify installation:

```bash
gcc --version
cmake --version
git --version
```

#### Step 3: Install Python 3.8+

```bash
sudo apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    python3-dev
```

Verify installation:

```bash
python3 --version
pip3 --version
```

#### Step 4: Install Node.js and npm

```bash
curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
sudo apt-get install -y nodejs
```

Verify installation:

```bash
node --version
npm --version
```

#### Step 5: Clone SynQ Repository

```bash
git clone https://github.com/TangoSplicer/SynQ.git
cd SynQ
```

#### Step 6: Build the Compiler

```bash
cd compiler
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)
cd ../..
```

This will take 5-10 minutes depending on your system.

Verify the compiler was built:

```bash
ls -la compiler/build/synq
```

You should see the `synq` executable.

#### Step 7: Install Backend Dependencies

```bash
cd backend
pip3 install -r requirements.txt
cd ..
```

#### Step 8: Install Frontend Dependencies

```bash
cd frontend
npm install
cd ..
```

#### Step 9: Verify Installation

Run the compiler tests:

```bash
cd compiler/build
ctest --verbose
cd ../..
```

Expected output: All tests pass (87% coverage, 1000+ tests)

---

### macOS

#### Step 1: Install Xcode Command Line Tools

```bash
xcode-select --install
```

Follow the prompts to complete installation. This may take 10-15 minutes.

#### Step 2: Install Homebrew

If you don't have Homebrew installed:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### Step 3: Install Build Tools

```bash
brew install cmake git curl wget
```

Verify installation:

```bash
cmake --version
git --version
```

#### Step 4: Install Python 3.8+

```bash
brew install python3
```

Verify installation:

```bash
python3 --version
pip3 --version
```

#### Step 5: Install Node.js and npm

```bash
brew install node
```

Verify installation:

```bash
node --version
npm --version
```

#### Step 6: Clone SynQ Repository

```bash
git clone https://github.com/TangoSplicer/SynQ.git
cd SynQ
```

#### Step 7: Build the Compiler

```bash
cd compiler
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(sysctl -n hw.ncpu)
cd ../..
```

This will take 5-10 minutes depending on your system.

Verify the compiler was built:

```bash
ls -la compiler/build/synq
```

#### Step 8: Install Backend Dependencies

```bash
cd backend
pip3 install -r requirements.txt
cd ..
```

#### Step 9: Install Frontend Dependencies

```bash
cd frontend
npm install
cd ..
```

#### Step 10: Verify Installation

Run the compiler tests:

```bash
cd compiler/build
ctest --verbose
cd ../..
```

---

### Windows (WSL2)

Follow the **Linux (Ubuntu/Debian)** instructions above. WSL2 provides a full Linux environment, so the same steps apply.

---

## Development Environment Setup

### Option 1: Docker Compose (Recommended for Development)

Docker Compose allows you to run the entire SynQ stack with a single command.

#### Prerequisites

- Docker: https://docs.docker.com/get-docker/
- Docker Compose: https://docs.docker.com/compose/install/

#### Start Services

```bash
cd SynQ
docker-compose up -d
```

Services will be available at:
- **Backend API:** http://localhost:8000
- **Frontend:** http://localhost:3000
- **Prometheus:** http://localhost:9090
- **Grafana:** http://localhost:3000 (credentials: admin/admin)

#### Stop Services

```bash
docker-compose down
```

#### View Logs

```bash
docker-compose logs -f backend
docker-compose logs -f frontend
```

### Option 2: Local Development (Manual Services)

If you prefer to run services locally without Docker:

#### Terminal 1: Backend API

```bash
cd SynQ/backend
uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
```

Backend will be available at http://localhost:8000

#### Terminal 2: Frontend

```bash
cd SynQ/frontend
npm run dev
```

Frontend will be available at http://localhost:3000

#### Terminal 3: Compiler (if needed)

```bash
cd SynQ/compiler/build
./synq ../examples/hello.synq
```

---

## Verification and Testing

### Verify Compiler Installation

```bash
cd SynQ/compiler/build
./synq --version
./synq --help
```

### Run Compiler Tests

```bash
cd SynQ/compiler/build
ctest --verbose
```

Expected: All tests pass (87% coverage, 1000+ tests)

### Run Backend Tests

```bash
cd SynQ/backend
pytest tests/ -v --cov=app
```

Expected: All tests pass

### Run Frontend Tests

```bash
cd SynQ/frontend
npm run test
```

Expected: All tests pass

### Run All Tests

```bash
cd SynQ
./scripts/run-all-tests.sh
```

---

## Your First Programs

Now let's write and run your first SynQ programs!

### Program 1: Hello World

Create `hello.synq`:

```synq
// Your first SynQ program
print("Welcome to SynQ — Hybrid Quantum Language")
```

Run it:

```bash
cd SynQ
./compiler/build/synq hello.synq
```

Expected output:

```
Welcome to SynQ — Hybrid Quantum Language
```

### Program 2: Variables and Types

Create `variables.synq`:

```synq
@import core

// Variable declarations
let language = "SynQ"        // Immutable string
var year = 2025              // Mutable integer
const max_points = 100       // Constant integer
auto bonus = 17.5            // Auto-inferred float

// Print variables
print("Language:", language)
print("Year:", year)
print("Max Points:", max_points)
print("Bonus:", bonus)

// Type checking
let t1 = typeof(language)
let t2 = typeof(year)
let t3 = typeof(bonus)

print("Type of language:", t1)
print("Type of year:", t2)
print("Type of bonus:", t3)
```

Run it:

```bash
./compiler/build/synq variables.synq
```

Expected output:

```
Language: SynQ
Year: 2025
Max Points: 100
Bonus: 17.5
Type of language: string
Type of year: int
Type of bonus: float
```

### Program 3: Functions and Loops

Create `functions.synq`:

```synq
@import core

// Function to calculate factorial
@fn factorial(n: int): -> int
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

// Function to print multiplication table
@fn print_table(num: int, limit: int): -> void
    for i in 1..limit:
        let result = num * i
        print(num, "×", i, "=", result)

// Main program
print("Factorial of 5:", factorial(5))
print("")
print("Multiplication table for 7:")
print_table(7, 10)
```

Run it:

```bash
./compiler/build/synq functions.synq
```

Expected output:

```
Factorial of 5: 120

Multiplication table for 7:
7 × 1 = 7
7 × 2 = 14
7 × 3 = 21
7 × 4 = 28
7 × 5 = 35
7 × 6 = 42
7 × 7 = 49
7 × 8 = 56
7 × 9 = 63
7 × 10 = 70
```

### Program 4: Lists and Pattern Matching

Create `lists.synq`:

```synq
@import core

// List operations
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
let primes = [2, 3, 5, 7, 11, 13]

print("Numbers:", numbers)
print("Primes:", primes)
print("Length of numbers:", numbers.length())

// Iteration
print("\nEven numbers:")
for num in numbers:
    if num % 2 == 0:
        print(num)

// Pattern matching
let status = "running"

match status:
    "running" => print("\nStatus: System is running")
    "stopped" => print("\nStatus: System is stopped")
    "error" => print("\nStatus: System has an error")
    _ => print("\nStatus: Unknown")
```

Run it:

```bash
./compiler/build/synq lists.synq
```

Expected output:

```
Numbers: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
Primes: [2, 3, 5, 7, 11, 13]
Length of numbers: 10

Even numbers:
2
4
6
8
10

Status: System is running
```

### Program 5: Your First Quantum Program

Create `quantum.synq`:

```synq
import quantum.core
import quantum.circuits
import quantum.measurements

// Create quantum register with 2 qubits
let reg = Register(2)
let q0 = reg[0]
let q1 = reg[1]

// Create quantum circuit
let circ = Circuit(reg)

// Apply Hadamard gate to first qubit (creates superposition)
circ.h(q0)

// Apply CNOT gate to entangle qubits
circ.cnot(q0, q1)

// Run circuit and measure
let result = circ.run()

print("Quantum Circuit Result:")
print("Circuit: H(q0) -> CNOT(q0, q1)")
print("Result:", result)
print("")
print("Expected: 50% |00⟩ and 50% |11⟩")
print("(Run multiple times to see different results)")
```

Run it:

```bash
./compiler/build/synq quantum.synq
```

Expected output (varies each run):

```
Quantum Circuit Result:
Circuit: H(q0) -> CNOT(q0, q1)
Result: |00⟩

Expected: 50% |00⟩ and 50% |11⟩
(Run multiple times to see different results)
```

Run it multiple times to see different results!

### Program 6: Error Handling

Create `error_handling.synq`:

```synq
@import core

// Function with error handling
@fn safe_divide(a: int, b: int): -> float
    try:
        if b == 0:
            throw "Division by zero"
        return a as float / b as float
    catch error:
        print("Error caught:", error)
        return 0.0

// Test error handling
print("10 / 2 =", safe_divide(10, 2))
print("10 / 0 =", safe_divide(10, 0))
print("20 / 4 =", safe_divide(20, 4))
```

Run it:

```bash
./compiler/build/synq error_handling.synq
```

Expected output:

```
10 / 2 = 5.0
Error caught: Division by zero
10 / 0 = 0.0
20 / 4 = 5.0
```

---

## Troubleshooting

### Build Issues

#### CMake not found

**Linux:**
```bash
sudo apt-get install cmake
```

**macOS:**
```bash
brew install cmake
```

**Windows (WSL2):**
```bash
sudo apt-get install cmake
```

#### C++ compiler not found

**Linux (GCC):**
```bash
sudo apt-get install build-essential
```

**Linux (Clang):**
```bash
sudo apt-get install clang
```

**macOS:**
```bash
xcode-select --install
```

#### Build fails with "Permission denied"

```bash
chmod +x compiler/build/synq
```

#### Out of disk space during build

The build requires approximately 5-10 GB of space. Check available space:

```bash
df -h
```

If low on space, clean build artifacts:

```bash
cd compiler/build
make clean
cd ../..
```

### Python Issues

#### Python 3.8+ not found

**Linux:**
```bash
sudo apt-get install python3.8 python3.8-venv python3.8-dev
```

**macOS:**
```bash
brew install python3
```

#### pip install fails

Update pip first:

```bash
pip3 install --upgrade pip
pip3 install -r requirements.txt --no-cache-dir
```

#### Virtual environment issues

Create a fresh virtual environment:

```bash
cd backend
python3 -m venv venv
source venv/bin/activate  # Linux/macOS
# or
venv\Scripts\activate  # Windows
pip install -r requirements.txt
```

### Node.js Issues

#### Node.js not found

**Linux:**
```bash
curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
sudo apt-get install nodejs
```

**macOS:**
```bash
brew install node
```

#### npm install fails

Clear npm cache and try again:

```bash
npm cache clean --force
npm install
```

### Runtime Issues

#### Compiler not found

Verify the compiler was built:

```bash
ls -la compiler/build/synq
```

If missing, rebuild:

```bash
cd compiler/build
cmake ..
cmake --build .
cd ../..
```

#### Backend API won't start

Check if port 8000 is in use:

```bash
lsof -i :8000  # Linux/macOS
netstat -ano | findstr :8000  # Windows
```

If in use, kill the process or use a different port:

```bash
uvicorn app.main:app --port 8001
```

#### Frontend won't start

Check if port 3000 is in use:

```bash
lsof -i :3000  # Linux/macOS
netstat -ano | findstr :3000  # Windows
```

If in use, use a different port:

```bash
npm run dev -- --port 3001
```

#### Tests fail

Rebuild everything:

```bash
cd compiler/build
cmake ..
cmake --build .
ctest --verbose
```

If tests still fail, check for missing dependencies:

```bash
cd ../../backend
pip install -r requirements.txt

cd ../frontend
npm install
```

---

## Next Steps

### 1. Explore the Documentation

- **[USER_GUIDE.md](USER_GUIDE.md)** - Complete user guide with advanced topics
- **[README_COMPREHENSIVE.md](README_COMPREHENSIVE.md)** - Detailed architecture
- **[PHASE14_STANDARD_LIBRARY.md](PHASE14_STANDARD_LIBRARY.md)** - Standard library reference
- **[PHASE15_IDE_DEVELOPER_EXPERIENCE.md](PHASE15_IDE_DEVELOPER_EXPERIENCE.md)** - IDE guide

### 2. Explore Examples

The repository includes many examples in `compiler/examples/`:

```bash
ls compiler/examples/
```

Notable examples:
- `hello.synq` - Hello World
- `classical_basics.synq` - Classical programming
- `quantum_demo.synq` - Quantum circuits
- `qaoa_maxcut.synq` - Quantum optimization
- `variational_energy.synq` - VQE algorithm
- `teleportation.synq` - Quantum teleportation

### 3. Use the Interactive Frontend

Start the frontend and use the web interface:

```bash
cd frontend
npm run dev
```

Visit http://localhost:3000 to access:
- Real-time code editor
- Quantum circuit visualizer
- Interactive debugging
- Performance profiler

### 4. Join the Community

Connect with other SynQ developers:
- **GitHub:** https://github.com/TangoSplicer/SynQ
- **GitHub Discussions:** Ask questions and share your work
- **GitHub Issues:** Report bugs and suggest features

### 5. Build Your First Project

Try creating a program that combines classical and quantum code:

- Quantum random number generator
- Bell state preparation and measurement
- Quantum Fourier transform
- Hybrid quantum-classical optimization
- Machine learning with quantum features

---

## Getting Help

If you encounter issues:

1. **Check the FAQ** - See GitHub Discussions for common questions
2. **Search existing issues** - https://github.com/TangoSplicer/SynQ/issues
3. **Ask on GitHub Discussions** - https://github.com/TangoSplicer/SynQ/discussions
4. **Open a GitHub issue** - Include error message, steps to reproduce, and your environment

---

## System-Specific Notes

### Linux

- **Recommended distributions:** Ubuntu 20.04 LTS or later, Debian 11+
- **Package manager:** apt (Ubuntu/Debian), dnf (Fedora), yum (CentOS)
- **Build time:** 5-10 minutes on modern hardware
- **Disk space:** 5-10 GB required

### macOS

- **Minimum version:** macOS 10.15 (Catalina)
- **Recommended version:** macOS 12+ (Monterey or later)
- **Xcode tools:** Required (installed via `xcode-select --install`)
- **Build time:** 5-15 minutes (longer on first run due to Xcode tools)
- **Disk space:** 5-10 GB required
- **Note:** M1/M2 Macs are fully supported

### Windows (WSL2)

- **Windows version:** Windows 10 Build 19041+ or Windows 11
- **WSL2 distribution:** Ubuntu 18.04 LTS or later recommended
- **Build time:** 5-10 minutes
- **Disk space:** 5-10 GB required in WSL2 filesystem
- **Note:** File I/O may be slower between Windows and WSL2 filesystems

---

## Performance Tips

### Faster Builds

Use parallel compilation:

```bash
cd compiler/build
cmake --build . --parallel $(nproc)  # Linux/macOS
cmake --build . --parallel %NUMBER_OF_PROCESSORS%  # Windows
```

### Faster Tests

Run tests in parallel:

```bash
cd compiler/build
ctest --parallel $(nproc)
```

### Faster Development

Use incremental builds:

```bash
cd compiler/build
cmake --build . --target synq
```

---

## What's Next?

Congratulations! You now have SynQ installed and running. Here's your journey:

1. **Week 1:** Explore basic examples
2. **Week 2:** Write your first quantum program
3. **Week 3:** Build a hybrid quantum-classical application
4. **Week 4:** Contribute to the SynQ community

---

**Happy coding with SynQ! 🚀**

**Questions?** Join our community on [GitHub Discussions](https://github.com/TangoSplicer/SynQ/discussions)
