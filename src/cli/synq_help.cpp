// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_help.cpp

#include <iostream>

int main() {
    std::cout << "🧭 SynQ CLI - Command Overview\n";
    std::cout << "Usage: synq <command> [args]\n\n";

    std::cout << "Core Commands:\n";
    std::cout << "  create       Scaffold new project or plugin\n";
    std::cout << "  build        Compile project\n";
    std::cout << "  run          Run a SynQ source file\n";
    std::cout << "  test         Execute tests and show coverage\n";
    std::cout << "  plugin       Manage plugins (build, publish, etc.)\n";
    std::cout << "  debug        Launch debugger\n";
    std::cout << "  ai           Run an AI prompt\n";
    std::cout << "  config       View/set configuration\n\n";

    std::cout << "Advanced Tools:\n";
    std::cout << "  mutate       AI-guided plugin mutation\n";
    std::cout << "  fuzz         Fuzz AI models for robustness\n";
    std::cout << "  qtrace       Visualize quantum trace\n";
    std::cout << "  inspect      Inspect project internals\n";
    std::cout << "  skillmap     Suggest skills based on usage\n";
    std::cout << "  chain        Run distributed job chains\n";
    std::cout << "  sign         Quantum-safe file signing\n\n";

    std::cout << "System/Meta:\n";
    std::cout << "  version      Show compiler version\n";
    std::cout << "  doctor       Run system diagnostics\n";
    std::cout << "  init         Start a minimal SynQ project\n";
    std::cout << "  man          View manual/reference by command\n";

    std::cout << "\n";

    std::cout << "Automation Scripts (via synq/scripts):\n";
    std::cout << "  build.sh           Build the full SynQ platform\n";
    std::cout << "  clean.sh           Clean build artifacts\n";
    std::cout << "  build_all_targets.sh  Compile QASM/LLVM/all targets\n";
    std::cout << "  test.sh            Run unit + integration tests\n";
    std::cout << "  coverage.sh        Dump coverage report\n";
    std::cout << "  fuzz_test.sh       AI-driven plugin fuzzing\n";
    std::cout << "  sign.sh            Sign compiled outputs\n";
    std::cout << "  mutate.sh          Mutate plugins with AI\n";
    std::cout << "  plugin_bench.sh    Benchmark plugin performance\n";
    std::cout << "  plugin_docgen.sh   Generate plugin documentation\n";
    std::cout << "  deploy.sh          Deploy signed builds\n";
    std::cout << "  push_synqhub.sh    Upload plugins to SynQHub\n";
    std::cout << "  package_bundle.sh  Package builds/plugins\n";
    std::cout << "  format.sh          Auto-format entire codebase\n";
    std::cout << "  lint.sh            Run linter on core/compiler\n";
    std::cout << "  inspect_project.sh Deep scan project structure\n";
    std::cout << "  ai_review.sh       AI assistant code reviewer\n";
    std::cout << "  auto_commit.sh     Git commit + push automation\n";
    std::cout << "  ci-pipeline.yml    GitHub Actions CI config\n";
    std::cout << "  entropy_rebalance.sh Normalize entropy for reproducibility\n";
    std::cout << "\n";

    return 0;
}