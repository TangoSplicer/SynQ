import { Button } from "@/components/ui/button";
import CodeEditor from "@/components/CodeEditor";

const examples = [
  {
    heading: "Variational Quantum Eigensolver (VQE)",
    title: "VQE - Find Ground State Energy",
    code: `// Illustrative recovery example — execution is simulated in this UI
from qml import VQE, H2_Hamiltonian

circuit Ansatz {
  qreg q[2]
  h(q[0])
  rz(q[0], theta)
  cnot(q[0], q[1])
  rz(q[1], phi)
}

energy = VQE(
  hamiltonian=H2_Hamiltonian,
  ansatz=Ansatz,
  optimizer="COBYLA",
  max_iterations=100
)

print(f"Ground state energy: {energy}")`,
  },
  {
    heading: "Circuit Synthesis Sketch",
    title: "Automatic Circuit Generation from Specification",
    code: `// Illustrative recovery example — no synthesis service is connected
from ai import synthesize
from qml import QAOA

problem = "MaxCut on 4-vertex graph"

circuit = synthesize(
  problem_spec=problem,
  optimization_level="aggressive",
  target_backend="ibmq_lima"
)

print(f"Circuit depth: {circuit.depth()}")
print(f"Gate count: {circuit.size()}")`,
  },
  {
    heading: "Hardware-Aware Transpilation Sketch",
    title: "Automatic Hardware Adaptation",
    code: `// Illustrative recovery example — no hardware backend is connected
from ai import transpile
from compiler import Circuit

circuit = Circuit()
circuit.h(0)
circuit.cnot(0, 1)
circuit.cnot(1, 2)
circuit.measure_all()

transpiled = transpile(
  circuit=circuit,
  target_backend="ibmq_lima",
  optimization_level=3
)

print(f"Original depth: {circuit.depth()}")
print(f"Optimized depth: {transpiled.depth()}")`,
  },
] as const;

export default function InteractiveExamples() {
  return (
    <section className="container py-20">
      <div className="text-center mb-16">
        <h2 className="text-4xl font-bold text-slate-900 dark:text-white mb-4">Interactive Source Sketches</h2>
        <p className="text-lg text-slate-600 dark:text-slate-300">
          The editor is a local UI demonstration; it does not execute SynQ programs or submit work to quantum hardware.
        </p>
      </div>

      <div className="space-y-8">
        {examples.map((example) => (
          <div key={example.title}>
            <h3 className="text-2xl font-bold text-slate-900 dark:text-white mb-4">{example.heading}</h3>
            <CodeEditor title={example.title} language="synq" defaultCode={example.code} />
          </div>
        ))}
      </div>

      <div className="mt-12 p-8 bg-blue-50 dark:bg-blue-900/20 rounded-lg border border-blue-200 dark:border-blue-800">
        <h3 className="text-lg font-semibold text-slate-900 dark:text-white mb-2">Recovery boundary</h3>
        <p className="text-slate-600 dark:text-slate-300 mb-4">
          The current backend exposes health and source-only example metadata. Authentication, job submission, real execution, and hardware integration are not implemented by this recovery profile.
        </p>
        <a href="https://github.com/TangoSplicer/SynQ" target="_blank" rel="noopener noreferrer">
          <Button>View current recovery status</Button>
        </a>
      </div>
    </section>
  );
}
