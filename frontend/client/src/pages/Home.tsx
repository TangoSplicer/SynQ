import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { ArrowRight, Zap, Network, Server, Code2, Cpu, GitBranch, Check, X, Microscope, Lightbulb, TrendingUp, Brain } from "lucide-react";
import CodeEditor from "@/components/CodeEditor";

export default function Home() {
  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-50 to-slate-100 dark:from-slate-950 dark:to-slate-900">
      {/* Navigation */}
      <nav className="sticky top-0 z-50 bg-white/80 dark:bg-slate-900/80 backdrop-blur-md border-b border-slate-200 dark:border-slate-800">
        <div className="container flex items-center justify-between py-4">
          <div className="flex items-center gap-2">
            <Cpu className="w-6 h-6 text-blue-600" />
            <span className="text-xl font-bold text-slate-900 dark:text-white">SynQ</span>
          </div>
          <div className="flex items-center gap-6">
            <a href="#pillars" className="text-sm font-medium text-slate-600 dark:text-slate-300 hover:text-blue-600 transition">
              Features
            </a>
            <a href="#roadmap" className="text-sm font-medium text-slate-600 dark:text-slate-300 hover:text-blue-600 transition">
              Roadmap
            </a>
            <a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer">
              <Button variant="default" size="sm">Get Started</Button>
            </a>
          </div>
        </div>
      </nav>

      {/* Hero Section */}
      <section className="container py-20 md:py-32">
        <div className="max-w-3xl mx-auto text-center">
          <h1 className="text-5xl md:text-6xl font-bold text-slate-900 dark:text-white mb-6 leading-tight">
            The Unified Platform for <span className="text-transparent bg-clip-text bg-gradient-to-r from-blue-600 to-purple-600">Hybrid Quantum Computing</span>
          </h1>
          <p className="text-xl text-slate-600 dark:text-slate-300 mb-8 leading-relaxed">
            SynQ combines quantum algorithms, classical AI, and intelligent compilation into a single, elegant programming language. Designed for the future of hybrid computation.
          </p>
          <div className="flex flex-col sm:flex-row gap-4 justify-center">
            <a href="#pillars">
              <Button size="lg" className="gap-2">
                Explore the Expansion <ArrowRight className="w-4 h-4" />
              </Button>
            </a>
            <a href="/collaboration">
              <Button size="lg" variant="outline">
                Try Collaboration Demo
              </Button>
            </a>
            <a href="https://github.com/TangoSplicer/SynQ/blob/main/README.md" target="_blank" rel="noopener noreferrer">
              <Button size="lg" variant="outline">
                View Documentation
              </Button>
            </a>
          </div>
        </div>
      </section>

      {/* Three Pillars Section */}
      <section id="pillars" className="container py-20">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Three Strategic Pillars</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            Comprehensive expansions designed to maintain SynQ's competitive edge
          </p>
        </div>

        <div className="grid md:grid-cols-3 gap-8">
          {/* Pillar 1: AI-Quantum Synergy */}
          <Card className="border-2 border-blue-200 dark:border-blue-900 hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-blue-100 dark:bg-blue-900 rounded-lg flex items-center justify-center mb-4">
                <Zap className="w-6 h-6 text-blue-600" />
              </div>
              <CardTitle>AI-Quantum Synergy</CardTitle>
              <CardDescription>Making AI an active component of quantum workflows</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">Native QML Module</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  First-class support for quantum machine learning algorithms like VQE and QAOA, directly integrated into the SynQ standard library.
                </p>
              </div>
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">AI-Driven Synthesis</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  Translate high-level specifications into optimized quantum circuits using intelligent AI agents.
                </p>
              </div>
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">Hardware-Aware Transpilation</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  AI intelligently adapts circuits to physical hardware constraints for optimal execution.
                </p>
              </div>
            </CardContent>
          </Card>

          {/* Pillar 2: Ecosystem Growth */}
          <Card className="border-2 border-purple-200 dark:border-purple-900 hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-purple-100 dark:bg-purple-900 rounded-lg flex items-center justify-center mb-4">
                <Network className="w-6 h-6 text-purple-600" />
              </div>
              <CardTitle>Ecosystem Growth</CardTitle>
              <CardDescription>Building a self-sustaining community around SynQHub</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">SynQHub Registry</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  Zero-cost, GitHub-backed plugin marketplace for community-driven extensions.
                </p>
              </div>
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">CLI Tools</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  <code className="text-xs bg-slate-100 dark:bg-slate-800 px-2 py-1 rounded">synq install</code> and <code className="text-xs bg-slate-100 dark:bg-slate-800 px-2 py-1 rounded">synq publish</code> for seamless plugin management.
                </p>
              </div>
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">Quantum-Safe Security</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  Local signature verification ensures plugin integrity and security.
                </p>
              </div>
            </CardContent>
          </Card>

          {/* Pillar 3: Enterprise Readiness */}
          <Card className="border-2 border-green-200 dark:border-green-900 hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-green-100 dark:bg-green-900 rounded-lg flex items-center justify-center mb-4">
                <Server className="w-6 h-6 text-green-600" />
              </div>
              <CardTitle>Enterprise Readiness</CardTitle>
              <CardDescription>Production-grade hardware abstraction and reliability</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">Standardized HAL</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  Hardware Abstraction Layer for seamless integration with IBM, IonQ, Rigetti, and custom backends.
                </p>
              </div>
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">Job Management</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  Real-time job submission, monitoring, and result retrieval across multiple providers.
                </p>
              </div>
              <div>
                <h4 className="font-semibold text-slate-900 dark:text-white mb-2">Hardware-Agnostic Code</h4>
                <p className="text-sm text-slate-600 dark:text-slate-400">
                  Write once, run on any quantum platform with minimal code changes.
                </p>
              </div>
            </CardContent>
          </Card>
        </div>
      </section>

      {/* Features Grid */}
      <section className="container py-20 bg-white dark:bg-slate-900 rounded-2xl my-12">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Comprehensive Feature Set</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            All the tools needed for modern quantum-classical-AI development
          </p>
        </div>

        <div className="grid md:grid-cols-2 gap-6">
          <div className="flex gap-4">
            <div className="flex-shrink-0">
              <div className="w-10 h-10 bg-blue-100 dark:bg-blue-900 rounded-lg flex items-center justify-center">
                <Code2 className="w-5 h-5 text-blue-600" />
              </div>
            </div>
            <div>
              <h3 className="font-semibold text-slate-900 dark:text-white mb-2">Target-Specific Code Generation</h3>
              <p className="text-sm text-slate-600 dark:text-slate-400">
                Compile SynQ code to x86, WASM, Qiskit, and SynQ VM with a single command.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex-shrink-0">
              <div className="w-10 h-10 bg-purple-100 dark:bg-purple-900 rounded-lg flex items-center justify-center">
                <Zap className="w-5 h-5 text-purple-600" />
              </div>
            </div>
            <div>
              <h3 className="font-semibold text-slate-900 dark:text-white mb-2">AI-Powered Optimization</h3>
              <p className="text-sm text-slate-600 dark:text-slate-400">
                Automatic circuit optimization and CNOT cancellation via intelligent AI agents.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex-shrink-0">
              <div className="w-10 h-10 bg-green-100 dark:bg-green-900 rounded-lg flex items-center justify-center">
                <GitBranch className="w-5 h-5 text-green-600" />
              </div>
            </div>
            <div>
              <h3 className="font-semibold text-slate-900 dark:text-white mb-2">Live Plugin Mutation</h3>
              <p className="text-sm text-slate-600 dark:text-slate-400">
                Dynamically load and modify plugins at runtime without recompilation.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex-shrink-0">
              <div className="w-10 h-10 bg-orange-100 dark:bg-orange-900 rounded-lg flex items-center justify-center">
                <Server className="w-5 h-5 text-orange-600" />
              </div>
            </div>
            <div>
              <h3 className="font-semibold text-slate-900 dark:text-white mb-2">Multi-Backend Support</h3>
              <p className="text-sm text-slate-600 dark:text-slate-400">
                Seamlessly switch between simulators and real quantum hardware providers.
              </p>
            </div>
          </div>
        </div>
      </section>

      {/* Feature Comparison Section */}
      <section className="container py-20">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">SynQ vs. The Competition</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            See how SynQ compares to leading quantum platforms
          </p>
        </div>

        <div className="overflow-x-auto">
          <table className="w-full border-collapse">
            <thead>
              <tr className="bg-slate-100 dark:bg-slate-800">
                <th className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-left font-semibold text-slate-900 dark:text-white">Feature</th>
                <th className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center font-semibold text-blue-600">SynQ</th>
                <th className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center font-semibold text-slate-600 dark:text-slate-300">PennyLane</th>
                <th className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center font-semibold text-slate-600 dark:text-slate-300">Classiq</th>
                <th className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center font-semibold text-slate-600 dark:text-slate-300">Qiskit</th>
              </tr>
            </thead>
            <tbody>
              <tr className="hover:bg-slate-50 dark:hover:bg-slate-800/50">
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-slate-900 dark:text-white font-medium">Unified Hybrid Language</td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
              </tr>
              <tr className="hover:bg-slate-50 dark:hover:bg-slate-800/50">
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-slate-900 dark:text-white font-medium">AI-Driven Circuit Synthesis</td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
              </tr>
              <tr className="hover:bg-slate-50 dark:hover:bg-slate-800/50">
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-slate-900 dark:text-white font-medium">Hardware-Aware Transpilation</td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
              </tr>
              <tr className="hover:bg-slate-50 dark:hover:bg-slate-800/50">
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-slate-900 dark:text-white font-medium">Native QML Support</td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
              </tr>
              <tr className="hover:bg-slate-50 dark:hover:bg-slate-800/50">
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-slate-900 dark:text-white font-medium">Live Plugin Mutation</td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><X className="w-5 h-5 text-red-600 mx-auto" /></td>
              </tr>
              <tr className="hover:bg-slate-50 dark:hover:bg-slate-800/50">
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-slate-900 dark:text-white font-medium">Multi-Backend Support</td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
                <td className="border border-slate-200 dark:border-slate-700 px-6 py-4 text-center"><Check className="w-5 h-5 text-green-600 mx-auto" /></td>
              </tr>
            </tbody>
          </table>
        </div>
      </section>

      {/* Use Cases Section */}
      <section className="container py-20">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Real-World Use Cases</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            How each pillar solves critical quantum-AI problems
          </p>
        </div>

        <div className="grid md:grid-cols-3 gap-8">
          {/* Use Case 1: Drug Discovery */}
          <Card className="hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-blue-100 dark:bg-blue-900 rounded-lg flex items-center justify-center mb-4">
                <Microscope className="w-6 h-6 text-blue-600" />
              </div>
              <CardTitle>Drug Discovery</CardTitle>
              <CardDescription>Pillar: AI-Quantum Synergy</CardDescription>
            </CardHeader>
            <CardContent>
              <p className="text-sm text-slate-600 dark:text-slate-400 mb-4">
                Use VQE and QML to simulate molecular structures and predict drug efficacy. SynQ's native QML module enables researchers to train quantum-classical hybrid models for protein folding and molecular docking.
              </p>
              <p className="text-xs font-medium text-blue-600 dark:text-blue-400">→ 10x faster molecular simulations</p>
            </CardContent>
          </Card>

          {/* Use Case 2: Portfolio Optimization */}
          <Card className="hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-purple-100 dark:bg-purple-900 rounded-lg flex items-center justify-center mb-4">
                <TrendingUp className="w-6 h-6 text-purple-600" />
              </div>
              <CardTitle>Portfolio Optimization</CardTitle>
              <CardDescription>Pillar: Enterprise Readiness</CardDescription>
            </CardHeader>
            <CardContent>
              <p className="text-sm text-slate-600 dark:text-slate-400 mb-4">
                Deploy QAOA circuits across multiple quantum backends via SynQ's Hardware Abstraction Layer. Switch between simulators and real QPUs without code changes for risk analysis and portfolio rebalancing.
              </p>
              <p className="text-xs font-medium text-purple-600 dark:text-purple-400">→ Hardware-agnostic optimization</p>
            </CardContent>
          </Card>

          {/* Use Case 3: Supply Chain Logistics */}
          <Card className="hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-green-100 dark:bg-green-900 rounded-lg flex items-center justify-center mb-4">
                <Lightbulb className="w-6 h-6 text-green-600" />
              </div>
              <CardTitle>Supply Chain Logistics</CardTitle>
              <CardDescription>Pillar: Ecosystem Growth</CardDescription>
            </CardHeader>
            <CardContent>
              <p className="text-sm text-slate-600 dark:text-slate-400 mb-4">
                Extend SynQ with domain-specific plugins via SynQHub for constraint satisfaction and vehicle routing. Community-driven optimization algorithms tailored to logistics challenges.
              </p>
              <p className="text-xs font-medium text-green-600 dark:text-green-400">→ Community-powered solutions</p>
            </CardContent>
          </Card>
        </div>
      </section>

      {/* Code Snippet Showcase Section */}
      <section className="container py-20">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Code Examples</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            See SynQ in action with real quantum-AI code
          </p>
        </div>

        <div className="grid md:grid-cols-3 gap-8">
          {/* VQE Example */}
          <Card className="border-2 border-blue-200 dark:border-blue-900">
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <Brain className="w-5 h-5 text-blue-600" />
                VQE Example
              </CardTitle>
              <CardDescription>Variational Quantum Eigensolver</CardDescription>
            </CardHeader>
            <CardContent>
              <pre className="bg-slate-900 text-slate-100 p-4 rounded-lg overflow-x-auto text-xs font-mono leading-relaxed">
{`// Native SynQ QML
from qml import VQE

circuit Ansatz {
  qreg q[2]
  h(q[0])
  rz(q[0], theta)
  cnot(q[0], q[1])
}

energy = VQE(
  Hamiltonian="H2",
  AnsatzCircuit=Ansatz,
  Optimizer="COBYLA"
)

print(energy)`}
              </pre>
            </CardContent>
          </Card>

          {/* Circuit Synthesis Example */}
          <Card className="border-2 border-purple-200 dark:border-purple-900">
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <Zap className="w-5 h-5 text-purple-600" />
                AI Synthesis
              </CardTitle>
              <CardDescription>High-level to optimized circuit</CardDescription>
            </CardHeader>
            <CardContent>
              <pre className="bg-slate-900 text-slate-100 p-4 rounded-lg overflow-x-auto text-xs font-mono leading-relaxed">
{`// AI-driven synthesis
from ai import synthesize

spec = "Grover search on 4 qubits"

circuit = synthesize(
  specification=spec,
  optimization="aggressive"
)

print(circuit.depth())
// Output: 12 gates (optimized)`}
              </pre>
            </CardContent>
          </Card>

          {/* Transpilation Example */}
          <Card className="border-2 border-green-200 dark:border-green-900">
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <Code2 className="w-5 h-5 text-green-600" />
                Transpilation
              </CardTitle>
              <CardDescription>Hardware-aware compilation</CardDescription>
            </CardHeader>
            <CardContent>
              <pre className="bg-slate-900 text-slate-100 p-4 rounded-lg overflow-x-auto text-xs font-mono leading-relaxed">
{`// Hardware-aware transpilation
from ai import transpile

transpiled = transpile(
  circuit=my_circuit,
  target_backend="ibmq_lima"
)

// Automatically decomposes
// CNOTs for limited connectivity
// Optimizes gate sequences`}
              </pre>
            </CardContent>
          </Card>
        </div>
      </section>

      {/* Try It Out Section */}
      <section className="container py-20">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Try It Out</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            Experience SynQ's interactive code editor with live execution
          </p>
        </div>

        <div className="space-y-8">
          {/* VQE Interactive Example */}
          <div>
            <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-4">Variational Quantum Eigensolver (VQE)</h3>
            <CodeEditor
              title="VQE - Find Ground State Energy"
              language="synq"
              defaultCode={`// Variational Quantum Eigensolver
from qml import VQE, H2_Hamiltonian

circuit Ansatz {
  qreg q[2]
  h(q[0])
  rz(q[0], theta)
  cnot(q[0], q[1])
  rz(q[1], phi)
}

// Optimize to find ground state
energy = VQE(
  hamiltonian=H2_Hamiltonian,
  ansatz=Ansatz,
  optimizer="COBYLA",
  max_iterations=100
)

print(f"Ground state energy: {energy}")`}
            />
          </div>

          {/* Circuit Synthesis Example */}
          <div>
            <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-4">AI-Driven Circuit Synthesis</h3>
            <CodeEditor
              title="Automatic Circuit Generation from Specification"
              language="synq"
              defaultCode={`// AI-Driven Circuit Synthesis
from ai import synthesize
from qml import QAOA

// High-level problem specification
problem = "MaxCut on 4-vertex graph"

// AI automatically generates optimized circuit
circuit = synthesize(
  problem_spec=problem,
  optimization_level="aggressive",
  target_backend="ibmq_lima"
)

print(f"Circuit depth: {circuit.depth()}")
print(f"Gate count: {circuit.size()}")
print(f"Estimated fidelity: {circuit.fidelity()}")`}
            />
          </div>

          {/* Transpilation Example */}
          <div>
            <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-4">Hardware-Aware Transpilation</h3>
            <CodeEditor
              title="Automatic Hardware Adaptation"
              language="synq"
              defaultCode={`// Hardware-Aware Transpilation
from ai import transpile
from compiler import Circuit

// Define quantum circuit
circuit = Circuit()
circuit.h(0)
circuit.cnot(0, 1)
circuit.cnot(1, 2)
circuit.measure_all()

// Transpile for specific hardware
transpiled = transpile(
  circuit=circuit,
  target_backend="ibmq_lima",
  optimization_level=3
)

print(f"Original depth: {circuit.depth()}")
print(f"Optimized depth: {transpiled.depth()}")
print(f"Improvement: {100 * (1 - transpiled.depth()/circuit.depth())}%")`}
            />
          </div>
        </div>

        <div className="mt-12 p-8 bg-blue-50 dark:bg-blue-900/20 rounded-lg border border-blue-200 dark:border-blue-800">
          <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">🚀 Ready to Deploy?</h3>
          <p className="text-slate-600 dark:text-slate-300 mb-4">
            For production use, connect this editor to a real SynQ backend by setting up API authentication and job submission endpoints.
          </p>
          <a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer">
            <Button>View Backend Integration Guide</Button>
          </a>
        </div>
      </section>

      {/* Roadmap Section */}
      <section id="roadmap" className="container py-20">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Strategic Roadmap</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            The path forward for SynQ's continued evolution
          </p>
        </div>

        <div className="space-y-6 max-w-2xl mx-auto">
          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-blue-600 rounded-full flex items-center justify-center text-white font-bold text-sm">1</div>
              <div className="w-1 h-16 bg-gradient-to-b from-blue-600 to-purple-600 mt-2"></div>
            </div>
            <div className="pb-6">
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 1: AI-Quantum Integration</h3>
              <p className="text-slate-600 dark:text-slate-400">
                Native QML module, AI-driven synthesis, and hardware-aware transpilation implemented.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-purple-600 rounded-full flex items-center justify-center text-white font-bold text-sm">2</div>
              <div className="w-1 h-16 bg-gradient-to-b from-purple-600 to-green-600 mt-2"></div>
            </div>
            <div className="pb-6">
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 2: SynQHub Ecosystem</h3>
              <p className="text-slate-600 dark:text-slate-400">
                Zero-cost GitHub-backed plugin registry with CLI tools and quantum-safe signatures.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-green-600 rounded-full flex items-center justify-center text-white font-bold text-sm">3</div>
              <div className="w-1 h-16 bg-gradient-to-b from-green-600 to-cyan-600 mt-2"></div>
            </div>
            <div className="pb-6">
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 3: Enterprise Deployment</h3>
              <p className="text-slate-600 dark:text-slate-400">
                Standardized HAL for multi-cloud quantum computing and production-grade reliability.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-cyan-600 rounded-full flex items-center justify-center text-white font-bold text-sm">4</div>
              <div className="w-1 h-16 bg-gradient-to-b from-cyan-600 to-amber-600 mt-2"></div>
            </div>
            <div className="pb-6">
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 4: Advanced Features & Analytics</h3>
              <p className="text-slate-600 dark:text-slate-400">
                Real-time analytics engine, ML-based anomaly detection, advanced threat detection, multi-region compliance (SOC2, HIPAA, GDPR), and distributed tracing.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-amber-600 rounded-full flex items-center justify-center text-white font-bold text-sm">5</div>
              <div className="w-1 h-16 bg-gradient-to-b from-amber-600 to-emerald-600 mt-2"></div>
            </div>
            <div className="pb-6">
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 5: Classical Language Evolution</h3>
              <p className="text-slate-600 dark:text-slate-400">
                Modern language features: pattern matching, generics, functional programming, error handling, async/await, and macros with meta-programming.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-emerald-600 rounded-full flex items-center justify-center text-white font-bold text-sm">6</div>
              <div className="w-1 h-16 bg-gradient-to-b from-emerald-600 to-cyan-600 mt-2"></div>
            </div>
            <div className="pb-6">
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 6: Collaboration & Intelligence</h3>
              <p className="text-slate-600 dark:text-slate-400">
                WebSocket real-time collaboration, ML intelligence for circuit optimization, and enhanced API gateway with GraphQL support.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-cyan-600 rounded-full flex items-center justify-center text-white font-bold text-sm">7</div>
              <div className="w-1 h-16 bg-gradient-to-b from-cyan-600 to-indigo-600 mt-2"></div>
            </div>
            <div className="pb-6">
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 7: Backend WebSocket & ML Integration</h3>
              <p className="text-slate-600 dark:text-slate-400">
                Production-grade WebSocket server with 10,000+ concurrent connections, ML training pipeline infrastructure, and full GraphQL API implementation.
              </p>
            </div>
          </div>

          <div className="flex gap-4">
            <div className="flex flex-col items-center">
              <div className="w-8 h-8 bg-indigo-600 rounded-full flex items-center justify-center text-white font-bold text-sm">8</div>
            </div>
            <div>
              <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Phase 8: Frontend Integration & Real-Time Features</h3>
              <p className="text-slate-600 dark:text-slate-400">
                Real-time editor with WebSocket sync, ML suggestion panel with predictions, quantum circuit visualizer, and end-to-end collaboration workflows.
              </p>
            </div>
          </div>
        </div>
      </section>

      {/* Phase 5 & Classical Language Enhancements Section */}
      <section className="container py-20">
        <div className="text-center mb-16">
          <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Phase 5: Classical Language Evolution</h2>
          <p className="text-lg text-slate-600 dark:text-slate-300">
            Modern programming language features for powerful classical computation
          </p>
        </div>

        <div className="mb-12 bg-gradient-to-r from-green-50 to-emerald-50 dark:from-green-900/20 dark:to-emerald-900/20 rounded-xl p-8 border border-green-200 dark:border-green-800">
          <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-6">What the Classical Side Can Do</h3>
          <div className="grid md:grid-cols-2 gap-8">
            <div>
              <h4 className="font-semibold text-slate-900 dark:text-white mb-3 flex items-center gap-2">
                <span className="text-xl">🎯</span> Advanced Type System
              </h4>
              <p className="text-sm text-slate-600 dark:text-slate-400 mb-3">
                SynQ's classical side features a sophisticated type system with algebraic data types, generics, and dependent types. This enables compile-time guarantees and eliminates entire classes of runtime errors.
              </p>
              <pre className="bg-slate-900 text-slate-100 p-3 rounded text-xs font-mono overflow-x-auto">
{`type Result<T, E> = Ok(T) | Err(E)
type Option<T> = Some(T) | None

fn safe_divide(a: f64, b: f64) -> Result<f64, str> {
  if b == 0.0 { Err("Division by zero") }
  else { Ok(a / b) }
}`}
              </pre>
            </div>
            <div>
              <h4 className="font-semibold text-slate-900 dark:text-white mb-3 flex items-center gap-2">
                <span className="text-xl">⚡</span> Functional Programming
              </h4>
              <p className="text-sm text-slate-600 dark:text-slate-400 mb-3">
                First-class functions, higher-order operations, and composition enable elegant data processing. Immutable data structures by default prevent entire categories of bugs.
              </p>
              <pre className="bg-slate-900 text-slate-100 p-3 rounded text-xs font-mono overflow-x-auto">
{`let process = compose(
  filter(|x| x > 0),
  map(|x| x * 2),
  fold(0, |acc, x| acc + x)
)

let result = process([1, -2, 3, 4])
// result = 16`}
              </pre>
            </div>
            <div>
              <h4 className="font-semibold text-slate-900 dark:text-white mb-3 flex items-center gap-2">
                <span className="text-xl">🔄</span> Pattern Matching
              </h4>
              <p className="text-sm text-slate-600 dark:text-slate-400 mb-3">
                Powerful pattern matching with exhaustiveness checking ensures all cases are handled. Guard clauses enable complex conditional logic in a clear, declarative style.
              </p>
              <pre className="bg-slate-900 text-slate-100 p-3 rounded text-xs font-mono overflow-x-auto">
{`match result {
  | Ok(value) if value > 100 => handle_large(value)
  | Ok(value) => handle_small(value)
  | Err(msg) => log_error(msg)
}`}
              </pre>
            </div>
            <div>
              <h4 className="font-semibold text-slate-900 dark:text-white mb-3 flex items-center gap-2">
                <span className="text-xl">⏱️</span> Async/Await
              </h4>
              <p className="text-sm text-slate-600 dark:text-slate-400 mb-3">
                Native async/await support for concurrent programming. Seamlessly handle I/O-bound operations, network requests, and parallel quantum job submissions.
              </p>
              <pre className="bg-slate-900 text-slate-100 p-3 rounded text-xs font-mono overflow-x-auto">
{`async fn execute_circuits(circuits: List<Circuit>) -> List<Result> {
  let futures = circuits.map(|c| execute_async(c))
  await join_all(futures)
}`}
              </pre>
            </div>
          </div>
        </div>

        <div className="grid md:grid-cols-2 gap-8 mb-12">
          <Card className="border-2 border-green-200 dark:border-green-900 hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-green-100 dark:bg-green-900 rounded-lg flex items-center justify-center mb-4">
                <Code2 className="w-6 h-6 text-green-600" />
              </div>
              <CardTitle>Core Language Features</CardTitle>
              <CardDescription>Modern programming constructs</CardDescription>
            </CardHeader>
            <CardContent className="space-y-3">
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-green-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">Pattern Matching</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Destructuring and algebraic data types</p>
                </div>
              </div>
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-green-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">Generics & Polymorphism</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Type-safe reusable code</p>
                </div>
              </div>
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-green-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">Functional Programming</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">First-class functions and composition</p>
                </div>
              </div>
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-green-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">Result Types</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Improved error handling</p>
                </div>
              </div>
            </CardContent>
          </Card>

          <Card className="border-2 border-amber-200 dark:border-amber-900 hover:shadow-lg transition">
            <CardHeader>
              <div className="w-12 h-12 bg-amber-100 dark:bg-amber-900 rounded-lg flex items-center justify-center mb-4">
                <TrendingUp className="w-6 h-6 text-amber-600" />
              </div>
              <CardTitle>Performance & Ecosystem</CardTitle>
              <CardDescription>Production-grade capabilities</CardDescription>
            </CardHeader>
            <CardContent className="space-y-3">
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-amber-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">Advanced Optimizations</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">SIMD vectorization and loop unrolling</p>
                </div>
              </div>
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-amber-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">Parallel Execution</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Async/await and thread pools</p>
                </div>
              </div>
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-amber-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">Package Manager</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">SynQPkg for library distribution</p>
                </div>
              </div>
              <div className="flex items-start gap-3">
                <Check className="w-5 h-5 text-amber-600 mt-0.5 flex-shrink-0" />
                <div>
                  <p className="font-semibold text-slate-900 dark:text-white">C/C++ FFI</p>
                  <p className="text-sm text-slate-600 dark:text-slate-400">Native library integration</p>
                </div>
              </div>
            </CardContent>
          </Card>
        </div>

        <div className="bg-slate-100 dark:bg-slate-800 rounded-xl p-8 mb-12">
          <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-6">Complete Implementation Summary</h3>
          <div className="grid md:grid-cols-2 gap-6">
            <div>
              <div className="text-sm font-semibold text-blue-600 dark:text-blue-400 mb-2">Phase 1-4 Complete</div>
              <p className="text-sm text-slate-600 dark:text-slate-400">43+ endpoints, 15+ database models, 15,000+ lines of code. Includes QML, synthesis, transpilation, analytics, compliance, and threat detection.</p>
            </div>
            <div>
              <div className="text-sm font-semibold text-amber-600 dark:text-amber-400 mb-2">Phase 5 Complete</div>
              <p className="text-sm text-slate-600 dark:text-slate-400">15+ endpoints, 8+ models, 4,000+ lines. Pattern matching, generics, functional programming, error handling, async/await, macros.</p>
            </div>
            <div>
              <div className="text-sm font-semibold text-emerald-600 dark:text-emerald-400 mb-2">Phase 6 Complete</div>
              <p className="text-sm text-slate-600 dark:text-slate-400">30+ endpoints, 13+ models, 7,500+ lines. WebSocket collaboration, ML intelligence, API gateway, real-time editing, presence tracking.</p>
            </div>
            <div>
              <div className="text-sm font-semibold text-cyan-600 dark:text-cyan-400 mb-2">Phase 7-8 Complete</div>
              <p className="text-sm text-slate-600 dark:text-slate-400">40+ endpoints, 5+ components, 7,500+ lines. WebSocket server, ML training, GraphQL API, real-time editor, circuit visualizer, ML suggestions.</p>
            </div>
          </div>
        </div>
      </section>

      {/* Phase 6 Section */}
      <section className="container py-20">
        <div className="mb-12">
          <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-8 text-center">🚀 Phase 6: Advanced Collaboration & Intelligence (Q1-Q2 2025)</h3>
          <p className="text-center text-slate-600 dark:text-slate-400 mb-12 max-w-3xl mx-auto">
            The final phase brings real-time collaboration, advanced ML intelligence, and an enhanced API gateway to create the most sophisticated quantum development platform.
          </p>
          
          <div className="grid md:grid-cols-3 gap-6 mb-12">
            {/* Collaboration */}
            <Card className="border-2 border-cyan-500 dark:border-cyan-600 bg-cyan-50 dark:bg-cyan-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">👥</span> Real-Time Collaboration
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-2 text-sm">
                <p><strong>WebSocket Sessions:</strong> Live multi-user editing with operational transformation</p>
                <p><strong>Presence Tracking:</strong> See cursor positions and selections of all participants</p>
                <p><strong>Shared Debugging:</strong> Step through quantum algorithms together</p>
                <p><strong>Comment Threads:</strong> Asynchronous feedback on code and circuits</p>
                <p><strong>Version History:</strong> Full edit history with branching and merging</p>
              </CardContent>
            </Card>

            {/* ML Intelligence */}
            <Card className="border-2 border-emerald-500 dark:border-emerald-600 bg-emerald-50 dark:bg-emerald-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">🧠</span> ML Intelligence
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-2 text-sm">
                <p><strong>Circuit Optimization:</strong> ML-driven gate reduction and depth minimization</p>
                <p><strong>Resource Estimation:</strong> Predict qubit and classical bit requirements</p>
                <p><strong>Pattern Recognition:</strong> Identify quantum algorithm patterns automatically</p>
                <p><strong>Performance Prediction:</strong> Estimate execution time before running</p>
                <p><strong>Smart Recommendations:</strong> Suggest optimizations based on hardware</p>
              </CardContent>
            </Card>

            {/* API Gateway */}
            <Card className="border-2 border-violet-500 dark:border-violet-600 bg-violet-50 dark:bg-violet-900/20">
              <CardHeader>
                <CardTitle className="flex items-center gap-2">
                  <span className="text-2xl">⚡</span> Enhanced API Gateway
                </CardTitle>
              </CardHeader>
              <CardContent className="space-y-2 text-sm">
                <p><strong>GraphQL Support:</strong> Flexible queries alongside REST API</p>
                <p><strong>Advanced Routing:</strong> Intelligent request routing and load balancing</p>
                <p><strong>Rate Limiting:</strong> Sophisticated quota management and burst allowance</p>
                <p><strong>API Analytics:</strong> Detailed metrics on usage and performance</p>
                <p><strong>Developer Portal:</strong> Self-service documentation and testing</p>
              </CardContent>
            </Card>
          </div>

          <div className="bg-gradient-to-r from-cyan-100 to-emerald-100 dark:from-cyan-900/30 dark:to-emerald-900/30 rounded-xl p-8">
            <h3 className="text-xl font-bold text-slate-900 dark:text-white mb-6">Phase 6 Implementation Highlights</h3>
            <div className="grid md:grid-cols-2 gap-6 text-sm">
              <div>
                <p className="font-semibold text-slate-900 dark:text-white mb-2">🔄 Collaboration Engine</p>
                <p className="text-slate-600 dark:text-slate-400">WebSocket-based real-time editing with operational transformation (OT) for conflict-free collaboration. Support for 50+ concurrent users per session with full audit logging.</p>
              </div>
              <div>
                <p className="font-semibold text-slate-900 dark:text-white mb-2">🤖 ML Models</p>
                <p className="text-slate-600 dark:text-slate-400">TensorFlow/PyTorch models for circuit analysis. Continuous improvement from user feedback. Model versioning and A/B testing framework for production deployment.</p>
              </div>
              <div>
                <p className="font-semibold text-slate-900 dark:text-white mb-2">📊 Analytics Dashboard</p>
                <p className="text-slate-600 dark:text-slate-400">Real-time API metrics, usage patterns, and performance monitoring. Detailed insights into collaboration session activity and ML prediction accuracy.</p>
              </div>
              <div>
                <p className="font-semibold text-slate-900 dark:text-white mb-2">🔐 Enterprise Features</p>
                <p className="text-slate-600 dark:text-slate-400">API key management, advanced rate limiting, multi-tenancy support, and comprehensive audit trails for compliance and security.</p>
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* CTA Section */}
      <section className="container py-20">
        <div className="bg-gradient-to-r from-blue-600 to-purple-600 rounded-2xl p-12 md:p-16 text-center">
          <h2 className="text-3xl md:text-4xl font-bold text-white mb-6">
            Ready to Build the Future of Quantum Computing?
          </h2>
          <p className="text-lg text-blue-100 mb-8 max-w-2xl mx-auto">
            Join the SynQ community and be part of the hybrid quantum-classical-AI revolution.
          </p>
          <div className="flex flex-col sm:flex-row gap-4 justify-center">
            <a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer">
              <Button size="lg" variant="secondary" className="gap-2">
                Get Started <ArrowRight className="w-4 h-4" />
              </Button>
            </a>
            <a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer">
              <Button size="lg" variant="outline" className="text-white border-white hover:bg-white/10">
                View on GitHub
              </Button>
            </a>
          </div>
        </div>
      </section>

      {/* Footer */}
      <footer className="border-t border-slate-200 dark:border-slate-800 mt-20">
        <div className="container py-12">
          <div className="grid md:grid-cols-4 gap-8 mb-8">
            <div>
              <div className="flex items-center gap-2 mb-4">
                <Cpu className="w-5 h-5 text-blue-600" />
                <span className="font-bold text-slate-900 dark:text-white">SynQ</span>
              </div>
              <p className="text-sm text-slate-600 dark:text-slate-400">
                The unified platform for hybrid quantum-classical-AI computing.
              </p>
            </div>
            <div>
              <h4 className="font-semibold text-slate-900 dark:text-white mb-4">Product</h4>
              <ul className="space-y-2 text-sm text-slate-600 dark:text-slate-400">
                <li><a href="#pillars" className="hover:text-blue-600 transition">Features</a></li>
                <li><a href="https://github.com/TangoSplicer/SynQ/blob/main/README.md" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">Documentation</a></li>
                <li><a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">GitHub</a></li>
              </ul>
            </div>
            <div>
              <h4 className="font-semibold text-slate-900 dark:text-white mb-4">Community</h4>
              <ul className="space-y-2 text-sm text-slate-600 dark:text-slate-400">
                <li><a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">GitHub</a></li>
                <li><a href="https://discord.gg" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">Discord</a></li>
                <li><a href="https://github.com/TangoSplicer/SynQ/discussions" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">Discussions</a></li>
              </ul>
            </div>
            <div>
              <h4 className="font-semibold text-slate-900 dark:text-white mb-4">Legal</h4>
              <ul className="space-y-2 text-sm text-slate-600 dark:text-slate-400">
                <li><a href="https://github.com/TangoSplicer/SynQ/blob/main/LICENSE" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">License</a></li>
                <li><a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">Contributing</a></li>
                <li><a href="https://github.com/TangoSplicer/SynQ/issues" target="_blank" rel="noopener noreferrer" className="hover:text-blue-600 transition">Issues</a></li>
              </ul>
            </div>
          </div>
          <div className="border-t border-slate-200 dark:border-slate-800 pt-8 flex flex-col md:flex-row justify-between items-center">
            <p className="text-sm text-slate-600 dark:text-slate-400">
              © 2025 SynQ Contributors. All rights reserved.
            </p>
            <div className="flex gap-6 mt-4 md:mt-0">
              <a href="https://twitter.com" target="_blank" rel="noopener noreferrer" className="text-slate-600 dark:text-slate-400 hover:text-blue-600 transition">Twitter</a>
              <a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer" className="text-slate-600 dark:text-slate-400 hover:text-blue-600 transition">GitHub</a>
              <a href="https://linkedin.com" target="_blank" rel="noopener noreferrer" className="text-slate-600 dark:text-slate-400 hover:text-blue-600 transition">LinkedIn</a>
            </div>
          </div>
        </div>
      </footer>
    </div>
  );
}
