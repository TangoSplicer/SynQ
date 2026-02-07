# Phase 11 Stage 1: Modular Compilation Pipeline - Implementation Plan

## Current Architecture Analysis

### Existing Compiler Structure

The SynQ compiler currently has:

1. **Main Entry Point** (`main.cpp`) - CLI interface with basic flags
2. **Runtime** (`runtime.h/cpp`) - Instruction execution engine
3. **Exporter** - Converts code to various formats
4. **Plugin System** - Dynamic plugin loading
5. **Multiple Backends** - x86, WASM, Qiskit, SynQVM
6. **Parser** - Basic parsing logic
7. **Codegen** - Code generation backend

### Current Compilation Flow

```
Source Code (.synq)
    ↓
Parser (parser.cpp)
    ↓
AST (ast.h/cpp)
    ↓
Codegen (codegen.cpp)
    ↓
Backend-specific code generation
    ↓
Output (x86, WASM, etc.)
```

### Issues with Current Architecture

1. **Monolithic Pipeline** - All stages run sequentially without intermediate caching
2. **No Dependency Tracking** - Cannot determine which files changed
3. **No Serialization** - Cannot cache intermediate representations
4. **Single-threaded** - No parallelization support
5. **No Incremental Compilation** - Must recompile everything on each change

---

## Phase 11 Stage 1: Modular Pipeline Architecture

### Target Architecture

```
Source Code (.synq)
    ↓
┌─────────────────────────────────────────────────┐
│ Stage 1: Lexical Analysis                       │
│ - Tokenization                                  │
│ - Syntax validation                             │
│ Output: TokenStream (serializable)              │
└─────────────────────────────────────────────────┘
    ↓ [Cache: tokens.cache]
┌─────────────────────────────────────────────────┐
│ Stage 2: Parsing                                │
│ - Build Abstract Syntax Tree (AST)              │
│ - Syntax error detection                        │
│ Output: AST (serializable)                      │
└─────────────────────────────────────────────────┘
    ↓ [Cache: ast.cache]
┌─────────────────────────────────────────────────┐
│ Stage 3: Semantic Analysis                      │
│ - Type checking                                 │
│ - Symbol resolution                             │
│ - Scope analysis                                │
│ Output: Typed AST (serializable)                │
└─────────────────────────────────────────────────┘
    ↓ [Cache: typed_ast.cache]
┌─────────────────────────────────────────────────┐
│ Stage 4: Optimization                           │
│ - Dead code elimination                         │
│ - Constant folding                              │
│ - Loop optimization                             │
│ Output: Optimized IR (serializable)             │
└─────────────────────────────────────────────────┘
    ↓ [Cache: optimized_ir.cache]
┌─────────────────────────────────────────────────┐
│ Stage 5: Code Generation                        │
│ - Convert to LLVM IR                            │
│ - Backend-specific generation                   │
│ Output: LLVM IR (serializable)                  │
└─────────────────────────────────────────────────┘
    ↓ [Cache: llvm_ir.cache]
┌─────────────────────────────────────────────────┐
│ Stage 6: Backend Compilation                    │
│ - LLVM optimization passes                      │
│ - Machine code generation                       │
│ Output: Native code / WASM / etc.               │
└─────────────────────────────────────────────────┘
    ↓
Final Output
```

### Key Benefits

1. **Caching** - Each stage output can be cached
2. **Dependency Tracking** - Know which files depend on which
3. **Incremental Compilation** - Only recompile changed files + dependents
4. **Parallelization** - Independent files can compile in parallel
5. **Debugging** - Inspect intermediate representations
6. **Extensibility** - Easy to add new stages or backends

---

## Implementation Tasks

### Task 1: Create Serializable Intermediate Representations

**Files to Create:**
- `compiler/ir/token_stream.h/cpp` - Tokenized source
- `compiler/ir/ast_ir.h/cpp` - Abstract Syntax Tree
- `compiler/ir/typed_ast_ir.h/cpp` - Type-checked AST
- `compiler/ir/optimized_ir.h/cpp` - Optimized intermediate representation
- `compiler/ir/llvm_ir_wrapper.h/cpp` - LLVM IR wrapper

**Key Features:**
- All IR types inherit from `SerializableIR` base class
- Implement `serialize()` and `deserialize()` methods
- Use JSON for serialization format
- Include metadata (timestamps, file hashes, dependencies)

**Example Structure:**

```cpp
// compiler/ir/serializable_ir.h
class SerializableIR {
public:
    virtual ~SerializableIR() = default;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
    
    std::string file_path;
    std::string file_hash;
    std::vector<std::string> dependencies;
    uint64_t timestamp;
};

// compiler/ir/token_stream.h
class TokenStream : public SerializableIR {
public:
    struct Token {
        std::string type;
        std::string value;
        int line, column;
    };
    
    std::vector<Token> tokens;
    
    std::string serialize() const override;
    void deserialize(const std::string& data) override;
};
```

### Task 2: Create Compilation Pipeline Framework

**Files to Create:**
- `compiler/pipeline/compilation_stage.h` - Base stage interface
- `compiler/pipeline/pipeline.h/cpp` - Pipeline orchestrator
- `compiler/pipeline/stage_lexer.h/cpp` - Lexical analysis stage
- `compiler/pipeline/stage_parser.h/cpp` - Parsing stage
- `compiler/pipeline/stage_semantic.h/cpp` - Semantic analysis stage
- `compiler/pipeline/stage_optimizer.h/cpp` - Optimization stage
- `compiler/pipeline/stage_codegen.h/cpp` - Code generation stage

**Pipeline Interface:**

```cpp
// compiler/pipeline/compilation_stage.h
class CompilationStage {
public:
    virtual ~CompilationStage() = default;
    
    virtual std::string name() const = 0;
    virtual std::shared_ptr<SerializableIR> execute(
        const std::shared_ptr<SerializableIR>& input
    ) = 0;
};

// compiler/pipeline/pipeline.h
class CompilationPipeline {
public:
    void add_stage(std::shared_ptr<CompilationStage> stage);
    std::shared_ptr<SerializableIR> compile(
        const std::string& source_code
    );
    
private:
    std::vector<std::shared_ptr<CompilationStage>> stages;
};
```

### Task 3: Implement Dependency Tracking

**Files to Create:**
- `compiler/dependency/dependency_graph.h/cpp` - Dependency tracking
- `compiler/dependency/file_hasher.h/cpp` - File change detection
- `compiler/cache/compilation_cache.h/cpp` - IR caching

**Key Features:**

```cpp
// compiler/dependency/dependency_graph.h
class DependencyGraph {
public:
    void add_dependency(const std::string& file, 
                       const std::string& depends_on);
    std::vector<std::string> get_dependents(const std::string& file);
    std::vector<std::string> get_changed_files(
        const std::vector<std::string>& all_files
    );
};

// compiler/cache/compilation_cache.h
class CompilationCache {
public:
    bool has_cached(const std::string& file);
    std::shared_ptr<SerializableIR> get_cached(const std::string& file);
    void cache(const std::string& file, 
              const std::shared_ptr<SerializableIR>& ir);
    
private:
    std::unordered_map<std::string, std::shared_ptr<SerializableIR>> cache;
    std::string cache_dir = ".synq_cache";
};
```

### Task 4: Implement Incremental Compilation

**Files to Create:**
- `compiler/incremental/incremental_compiler.h/cpp` - Incremental compilation logic

**Key Features:**

```cpp
// compiler/incremental/incremental_compiler.h
class IncrementalCompiler {
public:
    void compile(const std::vector<std::string>& files);
    
private:
    DependencyGraph dependency_graph;
    CompilationCache cache;
    CompilationPipeline pipeline;
    
    std::vector<std::string> get_files_to_recompile(
        const std::vector<std::string>& files
    );
};
```

**Algorithm:**

```
1. Load dependency graph from cache
2. For each file:
   a. Compute file hash
   b. Compare with cached hash
   c. If changed, mark for recompilation
3. For each changed file:
   a. Get all dependents
   b. Mark dependents for recompilation
4. Compile only marked files
5. Update cache
```

### Task 5: Integrate with Existing Compiler

**Files to Modify:**
- `src/main.cpp` - Update CLI to use new pipeline
- `src/compiler/runtime.h/cpp` - Update to use modular pipeline
- `src/compiler/exporter.h/cpp` - Update to use new IR

**Integration Points:**

```cpp
// Updated main.cpp
int main(int argc, char** argv) {
    // ... argument parsing ...
    
    // Create incremental compiler
    IncrementalCompiler compiler;
    
    // Compile files
    compiler.compile({filepath});
    
    // Get final IR
    auto final_ir = compiler.get_result();
    
    // Execute or export as before
    if (do_run) {
        Runtime runtime;
        runtime.execute(final_ir);
    }
    
    return 0;
}
```

---

## Implementation Timeline

| Task | Duration | Dependencies |
|------|----------|--------------|
| Task 1: Serializable IRs | 1 week | None |
| Task 2: Pipeline Framework | 1 week | Task 1 |
| Task 3: Dependency Tracking | 1 week | Task 2 |
| Task 4: Incremental Compilation | 1 week | Task 3 |
| Task 5: Integration | 1 week | Task 4 |
| Testing & Optimization | 1 week | All tasks |

**Total: 6 weeks**

---

## Success Criteria

Phase 11 Stage 1 is complete when:

1. ✅ All IR types are serializable and deserializable
2. ✅ Compilation pipeline executes all 6 stages
3. ✅ Dependency graph accurately tracks file dependencies
4. ✅ Incremental compilation works correctly
5. ✅ Cache reduces rebuild time by 10x for single-file changes
6. ✅ All existing tests pass
7. ✅ Comprehensive test suite for new features

---

## Next Stages (After Stage 1)

**Stage 2: Parallel Compilation** - Use modular pipeline to compile independent files in parallel
**Stage 3: LLVM Backend Integration** - Replace current codegen with LLVM IR
**Stage 4: Advanced Optimizations** - Implement LLVM optimization passes
**Stage 5: JIT Compilation** - Profile and compile hot paths at runtime

---

## Conclusion

Phase 11 Stage 1 establishes the modular compilation pipeline foundation that enables incremental compilation, parallel compilation, and advanced optimizations in subsequent stages. This is the critical first step for achieving the Phase 11 performance goals.
