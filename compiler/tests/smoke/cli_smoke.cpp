#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

std::string quote(const std::filesystem::path& path) {
    return "\"" + path.string() + "\"";
}

std::string read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

bool write_file(const std::filesystem::path& path, const std::string& contents) {
    std::ofstream output(path, std::ios::binary);
    output << contents;
    return static_cast<bool>(output);
}

#ifdef _WIN32
bool write_windows_launcher(const std::filesystem::path& path, const std::filesystem::path& executable) {
    std::ofstream output(path, std::ios::binary);
    output << "@echo off\r\n\"" << executable.string() << "\" %*\r\n";
    return static_cast<bool>(output);
}
#endif

}  // namespace

int main(int argc, char** argv) {
    if (!require(argc == 2, "CLI executable path is supplied by CTest")) return 1;
    const std::filesystem::path executable = argv[1];
    const auto base = std::filesystem::temp_directory_path() / "synq_cli_smoke";
    const auto quantum = base.string() + "_quantum.synq";
    const auto constants = base.string() + "_constants.synq";
    const auto semantics = base.string() + "_semantics.synq";
    const auto simulation = base.string() + "_simulation.synq";
    const auto multi_register_simulation = base.string() + "_multi_register_simulation.synq";
    const auto named_registers = base.string() + "_named_registers.synq";
    const auto literal_if = base.string() + "_literal_if.synq";
    const auto invalid = base.string() + "_invalid.synq";
    const auto qasm = base.string() + "_output.qasm";
    const auto hybrid_qasm = base.string() + "_hybrid_output.qasm";
    const auto named_hybrid_qasm = base.string() + "_named_hybrid_output.qasm";
    const auto literal_if_qasm = base.string() + "_literal_if_output.qasm";
    const auto stdout_path = base.string() + "_stdout.txt";
    const auto stderr_path = base.string() + "_stderr.txt";

#ifdef _WIN32
    const auto launcher = base.string() + "_launcher.cmd";
    if (!require(write_windows_launcher(launcher, executable), "writes a Windows CLI launcher")) return 1;
    const std::string invoke = "call " + quote(launcher);
#else
    const std::string invoke = quote(executable);
#endif

    if (!require(write_file(quantum, "quantum h q[0]\nmeasure q[0]\n"), "writes quantum CLI fixture") ||
        !require(write_file(constants,
                            "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
                            "let seed = 5\nlet total = seed + 4\nlet ready = true\n"),
                 "writes constant-evaluation CLI fixture") ||
        !require(write_file(semantics,
                            "let seed = 5\nlet selected = seed\nmeasure q[0] as observed\n"),
                 "writes semantic-inspection CLI fixture") ||
        !require(write_file(simulation,
                            "#[experimental(feature = \"qubit-declarations\")]\n"
                            "qubit q[2]\nquantum bell_pair q[0], q[1]\nmeasure q[0]\nmeasure q[1]\n"),
                 "writes bounded-simulation CLI fixture") ||
        !require(write_file(multi_register_simulation,
                            "#[experimental(feature = \"qubit-declarations\")]\n"
                            "#[experimental(feature = \"named-qubit-register-operands\")]\n"
                            "qubit data[1]\nqubit ancilla[1]\n"
                            "quantum bell_pair data[0], ancilla[0]\n"
                            "measure data[0]\nmeasure ancilla[0]\n"),
                 "writes multi-register simulation CLI fixture") ||
        !require(write_file(named_registers,
                            "#[experimental(feature = \"qubit-declarations\")]\n"
                            "#[experimental(feature = \"named-qubit-register-operands\")]\n"
                            "qubit data[1]\nqubit ancilla[1]\n"
                            "quantum cx data[0], ancilla[0]\nmeasure data[0]\n"),
                 "writes named-register strict-export CLI fixture") ||
        !require(write_file(literal_if,
                            "#[experimental(feature = \"qubit-declarations\")]\n"
                            "#[experimental(feature = \"classical-control-flow\")]\n"
                            "qubit q[1]\nif true then quantum h q[0]\n"),
                 "writes literal-if strict-export CLI fixture") ||
        !require(write_file(invalid, "quantum cx q[0]\n"), "writes invalid CLI fixture")) return 1;

    if (!require(std::system((invoke + " --version > " + quote(stdout_path) + " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(stdout_path).find("synqc 0.1.0-experimental") != std::string::npos,
                 "version mode reports the documented experimental recovery CLI version")) return 1;

    if (!require(std::system((invoke + " " + quote(quantum) + " --validate > " + quote(stdout_path) +
                              " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(stdout_path).find("valid bounded recovery-profile") != std::string::npos,
                 "validate mode reports a successful typed parse/lower/resolve workflow")) return 1;

    if (!require(std::system((invoke + " " + quote(quantum) + " --emit-openqasm --out " + quote(qasm) +
                              " > " + quote(stdout_path) + " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(qasm).find("OPENQASM 3.0;") != std::string::npos &&
                     read_file(qasm).find("c[0] = measure q[0];") != std::string::npos,
                 "OpenQASM mode writes the supported bounded source output")) return 1;

    if (!require(std::system((invoke + " " + quote(simulation) + " --emit-openqasm-hybrid --out " + quote(hybrid_qasm) +
                              " > " + quote(stdout_path) + " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(hybrid_qasm).find("qubit[2] q;") != std::string::npos &&
                     read_file(hybrid_qasm).find("h q[0];") != std::string::npos &&
                     read_file(hybrid_qasm).find("cx q[0], q[1];") != std::string::npos,
                 "strict Hybrid OpenQASM mode preserves the explicit declaration and typed Bell lowering")) return 1;

    if (!require(std::system((invoke + " " + quote(named_registers) + " --emit-openqasm-hybrid --out " + quote(named_hybrid_qasm) +
                              " > " + quote(stdout_path) + " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(named_hybrid_qasm).find("qubit[1] data;") != std::string::npos &&
                     read_file(named_hybrid_qasm).find("qubit[1] ancilla;") != std::string::npos &&
                     read_file(named_hybrid_qasm).find("cx data[0], ancilla[0];") != std::string::npos,
                 "strict Hybrid OpenQASM mode lowers the Alpha named-register subset")) return 1;

    if (!require(std::system((invoke + " " + quote(literal_if) + " --emit-openqasm-hybrid --out " + quote(literal_if_qasm) +
                              " > " + quote(stdout_path) + " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(literal_if_qasm).find("if (true) h q[0];") != std::string::npos,
                 "strict Hybrid OpenQASM mode lowers the Alpha literal-if gate subset")) return 1;

    if (!require(std::system((invoke + " " + quote(constants) + " --eval-constants > " + quote(stdout_path) +
                              " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(stdout_path).find("total = Integer:9") != std::string::npos &&
                     read_file(stdout_path).find("ready = Boolean:true") != std::string::npos,
                 "experimental constant-evaluation mode prints deterministic evaluated bindings")) return 1;

    if (!require(std::system((invoke + " " + quote(semantics) + " --inspect-semantics > " + quote(stdout_path) +
                              " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(stdout_path).find("binding selected | Value | Integer | line 2 | depends-on seed") != std::string::npos &&
                     read_file(stdout_path).find("binding observed | MeasurementResult | Boolean | line 3") != std::string::npos,
                 "semantic inspection mode prints resolved binding metadata without evaluation")) return 1;

    if (!require(std::system((invoke + " " + quote(simulation) + " --simulate > " + quote(stdout_path) +
                              " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(stdout_path).find("basis |00> probability = 0.5") != std::string::npos &&
                     read_file(stdout_path).find("basis |11> probability = 0.5") != std::string::npos &&
                     read_file(stdout_path).find("measurement q[0] probability_one = 0.5") != std::string::npos,
                 "simulation mode prints deterministic bounded probabilities")) return 1;

    if (!require(std::system((invoke + " " + quote(multi_register_simulation) + " --simulate > " + quote(stdout_path) +
                              " 2> " + quote(stderr_path)).c_str()) == 0 &&
                     read_file(stdout_path).find("register data[1] physical_offset = 0") != std::string::npos &&
                     read_file(stdout_path).find("register ancilla[1] physical_offset = 1") != std::string::npos &&
                     read_file(stdout_path).find("measurement data[0] probability_one = 0.5") != std::string::npos &&
                     read_file(stdout_path).find("measurement ancilla[0] probability_one = 0.5") != std::string::npos,
                 "simulation mode reports source register identity and deterministic cross-register probabilities")) return 1;

    const int invalid_status = std::system((invoke + " " + quote(invalid) + " --validate > " + quote(stdout_path) +
                                            " 2> " + quote(stderr_path)).c_str());
    if (!require(invalid_status != 0 && read_file(stderr_path).find("SYNQ-S002") != std::string::npos,
                 "validate mode preserves structured diagnostics and nonzero failure")) return 1;

    std::filesystem::remove(quantum);
    std::filesystem::remove(constants);
    std::filesystem::remove(semantics);
    std::filesystem::remove(simulation);
    std::filesystem::remove(multi_register_simulation);
    std::filesystem::remove(named_registers);
    std::filesystem::remove(literal_if);
    std::filesystem::remove(invalid);
    std::filesystem::remove(qasm);
    std::filesystem::remove(hybrid_qasm);
    std::filesystem::remove(named_hybrid_qasm);
    std::filesystem::remove(literal_if_qasm);
    std::filesystem::remove(stdout_path);
    std::filesystem::remove(stderr_path);
#ifdef _WIN32
    std::filesystem::remove(launcher);
#endif
    std::cout << "SynQ CLI smoke test passed\n";
    return 0;
}
