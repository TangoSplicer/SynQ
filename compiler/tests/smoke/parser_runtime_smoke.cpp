#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "compiler/ast.h"
#include "compiler/parser.h"
#include "compiler/runtime.h"

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

std::string write_fixture(const std::string& filename, const std::string& content) {
    const std::string path = "/tmp/" + filename;
    std::ofstream fixture(path);
    fixture << content;
    fixture.close();
    return path;
}

bool parser_accepts_supported_fixture() {
    const std::string path = write_fixture(
        "synq_parser_supported_fixture.synq",
        "// A recovery-parser fixture\n"
        "let theta = 0.5;\n"
        "print theta;\n"
        "delay 0\n"
        "quantum bell_pair\n"
        "ai concise_summary\n");

    Parser parser;
    std::unique_ptr<ASTNode> root(parser.parseFile(path));
    std::remove(path.c_str());

    if (!require(root != nullptr, "parser accepts supported fixture")) return false;
    auto* program = dynamic_cast<ProgramNode*>(root.get());
    if (!require(program != nullptr, "parser returns ProgramNode")) return false;
    if (!require(program->statements.size() == 5, "parser retains declarations and supported instructions")) return false;

    auto* declaration = dynamic_cast<DeclarationNode*>(program->statements[0]);
    if (!require(declaration != nullptr, "parser creates DeclarationNode")) return false;
    if (!require(declaration->name == "theta" && declaration->value == "0.5", "parser preserves declaration fields")) return false;
    if (!require(declaration->line == 2, "parser records declaration source line")) return false;

    const std::string expected_operations[] = {"print", "delay", "quantum", "ai"};
    const std::string expected_arguments[] = {"theta", "0", "bell_pair", "concise_summary"};
    for (std::size_t index = 0; index < 4; ++index) {
        auto* instruction = dynamic_cast<InstructionNode*>(program->statements[index + 1]);
        if (!require(instruction != nullptr, "parser creates InstructionNode")) return false;
        if (!require(instruction->op == expected_operations[index], "parser preserves instruction operation")) return false;
        if (!require(instruction->args.size() == 1 && instruction->args.front() == expected_arguments[index],
                     "parser preserves instruction argument")) return false;
        if (!require(instruction->line == index + 3, "parser records source line")) return false;
    }
    return true;
}

bool parser_rejects_invalid_fixture() {
    const std::string malformed_path = write_fixture("synq_parser_malformed_fixture.synq", "delay later\n");
    const std::string unsupported_path = write_fixture("synq_parser_unsupported_fixture.synq", "compile program\n");
    const std::string declaration_path = write_fixture("synq_parser_bad_declaration_fixture.synq", "let 9bad = value\n");
    Parser parser;

    std::unique_ptr<ASTNode> malformed(parser.parseFile(malformed_path));
    std::unique_ptr<ASTNode> unsupported(parser.parseFile(unsupported_path));
    std::unique_ptr<ASTNode> malformed_declaration(parser.parseFile(declaration_path));
    std::unique_ptr<ASTNode> missing(parser.parseFile("/tmp/synq_parser_missing_fixture.synq"));
    std::remove(malformed_path.c_str());
    std::remove(unsupported_path.c_str());
    std::remove(declaration_path.c_str());

    return require(malformed == nullptr, "parser rejects non-numeric delay") &&
           require(unsupported == nullptr, "parser rejects unsupported operation") &&
           require(malformed_declaration == nullptr, "parser rejects malformed declaration") &&
           require(missing == nullptr, "parser rejects missing input file");
}

bool runtime_executes_supported_and_fallback_paths() {
    synq::compiler::Program program{
        {
            {"print", {"fixture output"}},
            {"delay", {"0"}},
            {"quantum", {"bell_pair"}},
            {"ai", {"summarize"}},
            {"unknown", {}},
        },
    };

    std::ostringstream output;
    auto* original_buffer = std::cout.rdbuf(output.rdbuf());
    synq::compiler::Runtime runtime;
    runtime.initialize();
    runtime.execute(program);
    std::cout.rdbuf(original_buffer);

    const std::string text = output.str();
    return require(text.find("Runtime initialization complete") != std::string::npos,
                   "runtime initializes") &&
           require(text.find("Executing SynQ Program") != std::string::npos,
                   "runtime begins execution") &&
           require(text.find("[print] fixture output") != std::string::npos,
                   "runtime executes print instruction") &&
           require(text.find("[delay] 0ms") != std::string::npos,
                   "runtime executes zero-duration delay") &&
           require(text.find("[quantum] Invoking quantum kernel") != std::string::npos,
                   "runtime reaches quantum placeholder path") &&
           require(text.find("[ai] Querying AI agent") != std::string::npos,
                   "runtime reaches AI placeholder path") &&
           require(text.find("[warn] Unknown instruction: unknown") != std::string::npos,
                   "runtime reports unknown instruction") &&
           require(text.find("Execution complete") != std::string::npos,
                   "runtime completes execution");
}

}  // namespace

int main() {
    if (!parser_accepts_supported_fixture()) return 1;
    if (!parser_rejects_invalid_fixture()) return 1;
    if (!runtime_executes_supported_and_fallback_paths()) return 1;

    std::cout << "SynQ parser and runtime smoke test passed\n";
    return 0;
}
