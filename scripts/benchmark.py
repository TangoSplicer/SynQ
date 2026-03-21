#!/usr/bin/env python3
"""Performance benchmarking script for SynQ platform."""

import time
import statistics
import json
from datetime import datetime
from typing import List, Dict, Any
import requests
import subprocess


class Benchmark:
    """Benchmark runner for SynQ components."""
    
    def __init__(self, name: str, iterations: int = 10):
        self.name = name
        self.iterations = iterations
        self.results: List[float] = []
    
    def run(self, func, *args, **kwargs) -> Dict[str, Any]:
        """Run benchmark and collect metrics."""
        self.results = []
        
        # Warmup
        for _ in range(2):
            func(*args, **kwargs)
        
        # Actual runs
        for _ in range(self.iterations):
            start = time.perf_counter()
            func(*args, **kwargs)
            elapsed = time.perf_counter() - start
            self.results.append(elapsed * 1000)  # Convert to ms
        
        return self.get_stats()
    
    def get_stats(self) -> Dict[str, Any]:
        """Calculate statistics."""
        return {
            'name': self.name,
            'iterations': self.iterations,
            'min': min(self.results),
            'max': max(self.results),
            'mean': statistics.mean(self.results),
            'median': statistics.median(self.results),
            'stdev': statistics.stdev(self.results) if len(self.results) > 1 else 0,
            'p95': sorted(self.results)[int(len(self.results) * 0.95)],
            'p99': sorted(self.results)[int(len(self.results) * 0.99)],
        }


class BackendBenchmark:
    """Backend API benchmarking."""
    
    def __init__(self, base_url: str = 'http://localhost:8000'):
        self.base_url = base_url
        self.results = {}
    
    def benchmark_list_circuits(self):
        """Benchmark listing circuits."""
        def request():
            response = requests.get(f'{self.base_url}/api/circuits')
            assert response.status_code == 200
        
        bench = Benchmark('List Circuits', iterations=20)
        self.results['list_circuits'] = bench.run(request)
    
    def benchmark_create_circuit(self):
        """Benchmark creating a circuit."""
        def request():
            response = requests.post(
                f'{self.base_url}/api/circuits',
                json={'name': 'bench_circuit', 'qubits': 5}
            )
            assert response.status_code in [200, 201]
        
        bench = Benchmark('Create Circuit', iterations=10)
        self.results['create_circuit'] = bench.run(request)
    
    def benchmark_simulate_circuit(self):
        """Benchmark circuit simulation."""
        def request():
            response = requests.post(
                f'{self.base_url}/api/circuits/1/simulate',
                json={'shots': 100}
            )
            assert response.status_code in [200, 404]
        
        bench = Benchmark('Simulate Circuit', iterations=10)
        self.results['simulate_circuit'] = bench.run(request)
    
    def run_all(self):
        """Run all backend benchmarks."""
        print("🔧 Backend Benchmarks")
        print("=" * 50)
        
        try:
            self.benchmark_list_circuits()
            print(f"✓ List Circuits: {self.results['list_circuits']['mean']:.2f}ms")
        except Exception as e:
            print(f"✗ List Circuits: {e}")
        
        try:
            self.benchmark_create_circuit()
            print(f"✓ Create Circuit: {self.results['create_circuit']['mean']:.2f}ms")
        except Exception as e:
            print(f"✗ Create Circuit: {e}")
        
        try:
            self.benchmark_simulate_circuit()
            print(f"✓ Simulate Circuit: {self.results['simulate_circuit']['mean']:.2f}ms")
        except Exception as e:
            print(f"✗ Simulate Circuit: {e}")
        
        print()
        return self.results


class CompilerBenchmark:
    """Compiler benchmarking."""
    
    def __init__(self, compiler_path: str = './compiler/build/synq'):
        self.compiler_path = compiler_path
        self.results = {}
    
    def benchmark_full_build(self, project_path: str):
        """Benchmark full build."""
        def build():
            subprocess.run(
                [self.compiler_path, 'compile', project_path],
                check=True,
                capture_output=True
            )
        
        bench = Benchmark('Full Build', iterations=3)
        self.results['full_build'] = bench.run(build)
    
    def benchmark_incremental_build(self, project_path: str):
        """Benchmark incremental build."""
        def build():
            subprocess.run(
                [self.compiler_path, 'compile', project_path, '--incremental'],
                check=True,
                capture_output=True
            )
        
        bench = Benchmark('Incremental Build', iterations=5)
        self.results['incremental_build'] = bench.run(build)
    
    def run_all(self, project_path: str = 'examples/quantum_circuit.synq'):
        """Run all compiler benchmarks."""
        print("🔧 Compiler Benchmarks")
        print("=" * 50)
        
        try:
            self.benchmark_full_build(project_path)
            print(f"✓ Full Build: {self.results['full_build']['mean']:.2f}ms")
        except Exception as e:
            print(f"✗ Full Build: {e}")
        
        try:
            self.benchmark_incremental_build(project_path)
            print(f"✓ Incremental Build: {self.results['incremental_build']['mean']:.2f}ms")
        except Exception as e:
            print(f"✗ Incremental Build: {e}")
        
        print()
        return self.results


class FrontendBenchmark:
    """Frontend benchmarking."""
    
    def __init__(self, base_url: str = 'http://localhost:3000'):
        self.base_url = base_url
        self.results = {}
    
    def benchmark_page_load(self):
        """Benchmark page load time."""
        def request():
            response = requests.get(self.base_url)
            assert response.status_code == 200
        
        bench = Benchmark('Page Load', iterations=10)
        self.results['page_load'] = bench.run(request)
    
    def run_all(self):
        """Run all frontend benchmarks."""
        print("🔧 Frontend Benchmarks")
        print("=" * 50)
        
        try:
            self.benchmark_page_load()
            print(f"✓ Page Load: {self.results['page_load']['mean']:.2f}ms")
        except Exception as e:
            print(f"✗ Page Load: {e}")
        
        print()
        return self.results


def main():
    """Run all benchmarks."""
    print("\n" + "=" * 50)
    print("SynQ Performance Benchmarks")
    print("=" * 50 + "\n")
    
    all_results = {
        'timestamp': datetime.now().isoformat(),
        'benchmarks': {}
    }
    
    # Backend benchmarks
    backend = BackendBenchmark()
    all_results['benchmarks']['backend'] = backend.run_all()
    
    # Compiler benchmarks
    compiler = CompilerBenchmark()
    all_results['benchmarks']['compiler'] = compiler.run_all()
    
    # Frontend benchmarks
    frontend = FrontendBenchmark()
    all_results['benchmarks']['frontend'] = frontend.run_all()
    
    # Save results
    with open('benchmark_results.json', 'w') as f:
        json.dump(all_results, f, indent=2)
    
    print("✓ Results saved to benchmark_results.json")
    print("\n" + "=" * 50)


if __name__ == '__main__':
    main()
