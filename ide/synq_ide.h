// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 15: IDE & Developer Experience - Core Architecture

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace synq::ide {

// ============================================================================
// IDE Core Architecture
// ============================================================================

/**
 * @class SynQIDE
 * @brief Main IDE application
 */
class SynQIDE {
public:
    SynQIDE();
    ~SynQIDE();

    void run();
    void open_project(const std::string& path);
    void create_project(const std::string& name, const std::string& template_type);
    void close_project();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

// ============================================================================
// Code Editor
// ============================================================================

/**
 * @class CodeEditor
 * @brief Advanced code editor with syntax highlighting
 */
class CodeEditor {
public:
    CodeEditor();

    void open_file(const std::string& path);
    void save_file();
    void save_as(const std::string& path);
    void close_file();

    std::string get_content() const;
    void set_content(const std::string& content);

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();

    void goto_line(int line);
    void find(const std::string& text);
    void replace(const std::string& find, const std::string& replace);
    void replace_all(const std::string& find, const std::string& replace);

    void set_theme(const std::string& theme);
    void set_font_size(int size);
    void set_tab_size(int size);

private:
    std::string current_file;
    std::string content;
    std::vector<std::string> undo_stack;
    std::vector<std::string> redo_stack;
};

// ============================================================================
// Syntax Highlighting & Language Support
// ============================================================================

/**
 * @class SyntaxHighlighter
 * @brief Provides syntax highlighting for SynQ and other languages
 */
class SyntaxHighlighter {
public:
    enum class TokenType {
        KEYWORD,
        IDENTIFIER,
        NUMBER,
        STRING,
        COMMENT,
        OPERATOR,
        PUNCTUATION,
        WHITESPACE,
        ERROR,
    };

    struct Token {
        TokenType type;
        std::string text;
        int line;
        int column;
    };

    std::vector<Token> tokenize(const std::string& code);
    std::string highlight_html(const std::string& code);
};

/**
 * @class CodeCompletion
 * @brief Intelligent code completion
 */
class CodeCompletion {
public:
    struct Suggestion {
        std::string text;
        std::string description;
        std::string type;  // function, variable, class, etc.
    };

    std::vector<Suggestion> get_completions(const std::string& code, int line, int column);
    std::vector<Suggestion> get_signatures(const std::string& function_name);
};

// ============================================================================
// Quantum Circuit Visualizer
// ============================================================================

/**
 * @class QuantumVisualizer
 * @brief Visualizes quantum circuits
 */
class QuantumVisualizer {
public:
    struct CircuitDiagram {
        std::string svg;
        int width;
        int height;
    };

    CircuitDiagram visualize_circuit(const std::string& circuit_code);
    CircuitDiagram visualize_state_vector(const std::vector<double>& amplitudes);
    CircuitDiagram visualize_measurement_results(const std::map<std::string, int>& results);

    void set_theme(const std::string& theme);
    void set_scale(double scale);
};

// ============================================================================
// Debugger
// ============================================================================

/**
 * @class Debugger
 * @brief Real-time debugger with breakpoints
 */
class Debugger {
public:
    struct Breakpoint {
        std::string file;
        int line;
        std::string condition;
        bool enabled;
    };

    struct StackFrame {
        std::string function;
        std::string file;
        int line;
        std::map<std::string, std::string> local_variables;
    };

    void start_debugging(const std::string& program);
    void stop_debugging();
    void pause();
    void resume();
    void step_over();
    void step_into();
    void step_out();

    void add_breakpoint(const std::string& file, int line);
    void remove_breakpoint(const std::string& file, int line);
    void toggle_breakpoint(const std::string& file, int line);

    std::vector<StackFrame> get_stack_trace();
    std::string get_variable_value(const std::string& name);
    void set_variable_value(const std::string& name, const std::string& value);

    std::vector<Breakpoint> get_breakpoints() const;
};

// ============================================================================
// Performance Profiler
// ============================================================================

/**
 * @class Profiler
 * @brief Performance profiling and optimization
 */
class Profiler {
public:
    struct FunctionProfile {
        std::string name;
        long long call_count;
        double total_time_ms;
        double avg_time_ms;
        double max_time_ms;
        double min_time_ms;
    };

    struct MemoryProfile {
        long long total_allocated;
        long long total_freed;
        long long peak_usage;
        std::vector<std::pair<std::string, long long>> allocations;
    };

    void start_profiling();
    void stop_profiling();

    std::vector<FunctionProfile> get_function_profiles();
    MemoryProfile get_memory_profile();

    void generate_report(const std::string& output_file);
    void generate_flame_graph(const std::string& output_file);
};

// ============================================================================
// Project Manager
// ============================================================================

/**
 * @class ProjectManager
 * @brief Manages projects and files
 */
class ProjectManager {
public:
    struct Project {
        std::string name;
        std::string path;
        std::string template_type;
        std::vector<std::string> source_files;
        std::map<std::string, std::string> config;
    };

    void create_project(const std::string& name, const std::string& path,
                       const std::string& template_type);
    void open_project(const std::string& path);
    void close_project();

    Project get_current_project() const;
    std::vector<std::string> get_source_files() const;
    void add_file(const std::string& path);
    void remove_file(const std::string& path);

    void build();
    void rebuild();
    void clean();

    void run();
    void run_with_args(const std::vector<std::string>& args);
};

// ============================================================================
// Build System
// ============================================================================

/**
 * @class BuildSystem
 * @brief Manages compilation and linking
 */
class BuildSystem {
public:
    enum class BuildType {
        DEBUG,
        RELEASE,
        PROFILE,
    };

    struct BuildResult {
        bool success;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
        long long build_time_ms;
    };

    void set_build_type(BuildType type);
    BuildResult build();
    BuildResult rebuild();
    void clean();

    void set_compiler_flags(const std::vector<std::string>& flags);
    void set_linker_flags(const std::vector<std::string>& flags);
};

// ============================================================================
// Testing Framework
// ============================================================================

/**
 * @class TestFramework
 * @brief Integrated testing framework
 */
class TestFramework {
public:
    struct TestResult {
        std::string test_name;
        bool passed;
        std::string error_message;
        long long execution_time_ms;
    };

    void discover_tests();
    std::vector<TestResult> run_all_tests();
    std::vector<TestResult> run_test(const std::string& test_name);
    std::vector<TestResult> run_tests_matching(const std::string& pattern);

    void generate_coverage_report(const std::string& output_file);
};

// ============================================================================
// Language Server Protocol (LSP)
// ============================================================================

/**
 * @class LanguageServer
 * @brief Language server for IDE integration
 */
class LanguageServer {
public:
    struct Diagnostic {
        int line;
        int column;
        std::string message;
        std::string severity;  // error, warning, info
    };

    struct Location {
        std::string file;
        int line;
        int column;
    };

    void initialize();
    void shutdown();

    std::vector<Diagnostic> get_diagnostics(const std::string& file);
    Location goto_definition(const std::string& file, int line, int column);
    std::vector<Location> find_references(const std::string& file, int line, int column);
    std::string get_hover_info(const std::string& file, int line, int column);
};

// ============================================================================
// UI Framework
// ============================================================================

/**
 * @class UIFramework
 * @brief Cross-platform UI framework
 */
class UIFramework {
public:
    enum class Platform {
        WINDOWS,
        MACOS,
        LINUX,
    };

    static Platform get_platform();

    class Window {
    public:
        void create(const std::string& title, int width, int height);
        void show();
        void hide();
        void close();
    };

    class Menu {
    public:
        void add_item(const std::string& label, std::function<void()> callback);
        void add_separator();
    };

    class Toolbar {
    public:
        void add_button(const std::string& label, std::function<void()> callback);
        void add_separator();
    };

    class StatusBar {
    public:
        void set_message(const std::string& message);
        void set_progress(int value, int max);
    };
};

// ============================================================================
// Settings & Preferences
// ============================================================================

/**
 * @class Settings
 * @brief IDE settings and preferences
 */
class Settings {
public:
    void load_settings();
    void save_settings();

    std::string get_setting(const std::string& key, const std::string& default_value = "");
    void set_setting(const std::string& key, const std::string& value);

    void reset_to_defaults();
};

// ============================================================================
// Plugin System
// ============================================================================

/**
 * @class PluginManager
 * @brief Manages IDE plugins
 */
class PluginManager {
public:
    struct Plugin {
        std::string name;
        std::string version;
        std::string description;
        std::string author;
    };

    void load_plugins(const std::string& plugin_dir);
    void load_plugin(const std::string& plugin_path);
    void unload_plugin(const std::string& plugin_name);

    std::vector<Plugin> get_loaded_plugins() const;
    bool is_plugin_loaded(const std::string& plugin_name) const;
};

// ============================================================================
// Output & Logging
// ============================================================================

/**
 * @class OutputPanel
 * @brief Displays compilation, execution, and debug output
 */
class OutputPanel {
public:
    void append_output(const std::string& text);
    void append_error(const std::string& text);
    void append_warning(const std::string& text);
    void clear();

    std::string get_output() const;
};

} // namespace synq::ide
