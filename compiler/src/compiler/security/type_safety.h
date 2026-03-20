// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 12: Security Hardening - Advanced Type Safety System

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <optional>
#include <cstdint>

namespace synq::compiler::security {

/**
 * @enum TypeKind
 * @brief Kind of type in the type system
 */
enum class TypeKind {
    PRIMITIVE,      // bool, i32, i64, f32, f64, etc.
    ALGEBRAIC,      // Sum type (enum-like)
    PRODUCT,        // Product type (struct-like)
    FUNCTION,       // Function type
    DEPENDENT,      // Dependent type (type depends on value)
    PHANTOM,        // Phantom type (compile-time only)
    GENERIC,        // Generic/polymorphic type
    REFINED,        // Refined type (subtype with constraints)
    INTERSECTION,   // Intersection type (multiple traits)
    UNION,          // Union type (multiple possible types)
};

/**
 * @struct TypeConstraint
 * @brief Constraint on a type
 */
struct TypeConstraint {
    std::string name;
    std::string predicate;  // Logical predicate
    std::string error_msg;
};

/**
 * @class Type
 * @brief Represents a type in the SynQ type system
 */
class Type {
public:
    explicit Type(TypeKind kind, const std::string& name);
    virtual ~Type() = default;

    TypeKind get_kind() const { return kind; }
    const std::string& get_name() const { return name; }

    /**
     * @brief Add a constraint to this type
     */
    void add_constraint(const TypeConstraint& constraint);

    /**
     * @brief Check if a value satisfies all constraints
     */
    bool satisfies_constraints(const std::string& value) const;

    /**
     * @brief Get all constraints
     */
    const std::vector<TypeConstraint>& get_constraints() const { return constraints; }

    /**
     * @brief Check if this type is assignable from another type
     */
    virtual bool is_assignable_from(const Type& other) const;

    /**
     * @brief Get string representation
     */
    virtual std::string to_string() const;

protected:
    TypeKind kind;
    std::string name;
    std::vector<TypeConstraint> constraints;
};

/**
 * @class AlgebraicType
 * @brief Sum type (enum-like) for type safety
 * 
 * Algebraic types allow expressing mutually exclusive possibilities.
 * Example: Result<T, E> = Ok(T) | Err(E)
 */
class AlgebraicType : public Type {
public:
    struct Variant {
        std::string name;
        std::vector<std::shared_ptr<Type>> fields;
    };

    explicit AlgebraicType(const std::string& name);

    /**
     * @brief Add a variant to this algebraic type
     */
    void add_variant(const Variant& variant);

    /**
     * @brief Get all variants
     */
    const std::vector<Variant>& get_variants() const { return variants; }

    /**
     * @brief Check if a variant exists
     */
    bool has_variant(const std::string& name) const;

    bool is_assignable_from(const Type& other) const override;
    std::string to_string() const override;

private:
    std::vector<Variant> variants;
};

/**
 * @class ProductType
 * @brief Product type (struct-like) for type safety
 * 
 * Product types combine multiple fields into a single type.
 * Example: Point { x: i32, y: i32 }
 */
class ProductType : public Type {
public:
    struct Field {
        std::string name;
        std::shared_ptr<Type> type;
        bool is_mutable;
    };

    explicit ProductType(const std::string& name);

    /**
     * @brief Add a field to this product type
     */
    void add_field(const Field& field);

    /**
     * @brief Get all fields
     */
    const std::vector<Field>& get_fields() const { return fields; }

    /**
     * @brief Get field by name
     */
    const Field* get_field(const std::string& name) const;

    bool is_assignable_from(const Type& other) const override;
    std::string to_string() const override;

private:
    std::vector<Field> fields;
};

/**
 * @class FunctionType
 * @brief Function type for type safety
 * 
 * Function types specify parameter and return types.
 * Example: (i32, i32) -> i32
 */
class FunctionType : public Type {
public:
    explicit FunctionType(const std::string& name);

    /**
     * @brief Set parameter types
     */
    void set_parameters(const std::vector<std::shared_ptr<Type>>& params);

    /**
     * @brief Set return type
     */
    void set_return_type(std::shared_ptr<Type> return_type);

    /**
     * @brief Get parameter types
     */
    const std::vector<std::shared_ptr<Type>>& get_parameters() const { return parameters; }

    /**
     * @brief Get return type
     */
    std::shared_ptr<Type> get_return_type() const { return return_type; }

    bool is_assignable_from(const Type& other) const override;
    std::string to_string() const override;

private:
    std::vector<std::shared_ptr<Type>> parameters;
    std::shared_ptr<Type> return_type;
};

/**
 * @class DependentType
 * @brief Dependent type where type depends on value
 * 
 * Dependent types allow expressing type-level constraints.
 * Example: Vector<T, N> where N is a compile-time constant
 */
class DependentType : public Type {
public:
    explicit DependentType(const std::string& name, std::shared_ptr<Type> base_type);

    /**
     * @brief Set the dependent parameter
     */
    void set_dependent_param(const std::string& param_name, const std::string& param_type);

    /**
     * @brief Get the base type
     */
    std::shared_ptr<Type> get_base_type() const { return base_type; }

    /**
     * @brief Get the dependent parameter
     */
    const std::pair<std::string, std::string>& get_dependent_param() const {
        return dependent_param;
    }

    bool is_assignable_from(const Type& other) const override;
    std::string to_string() const override;

private:
    std::shared_ptr<Type> base_type;
    std::pair<std::string, std::string> dependent_param;  // name, type
};

/**
 * @class PhantomType
 * @brief Phantom type for compile-time tracking
 * 
 * Phantom types exist only at compile-time and have no runtime representation.
 * Used for tracking invariants and preventing misuse.
 * Example: Sealed<T> prevents certain operations
 */
class PhantomType : public Type {
public:
    explicit PhantomType(const std::string& name, std::shared_ptr<Type> wrapped_type);

    /**
     * @brief Get the wrapped type
     */
    std::shared_ptr<Type> get_wrapped_type() const { return wrapped_type; }

    /**
     * @brief Add a phantom constraint
     */
    void add_phantom_constraint(const std::string& constraint);

    /**
     * @brief Get all phantom constraints
     */
    const std::vector<std::string>& get_phantom_constraints() const {
        return phantom_constraints;
    }

    bool is_assignable_from(const Type& other) const override;
    std::string to_string() const override;

private:
    std::shared_ptr<Type> wrapped_type;
    std::vector<std::string> phantom_constraints;
};

/**
 * @class RefinedType
 * @brief Refined type with constraints
 * 
 * Refined types are subtypes with additional constraints.
 * Example: NonNegativeInt = {x: i32 | x >= 0}
 */
class RefinedType : public Type {
public:
    explicit RefinedType(const std::string& name, std::shared_ptr<Type> base_type);

    /**
     * @brief Add a refinement constraint
     */
    void add_refinement(const TypeConstraint& constraint);

    /**
     * @brief Get the base type
     */
    std::shared_ptr<Type> get_base_type() const { return base_type; }

    bool is_assignable_from(const Type& other) const override;
    std::string to_string() const override;

private:
    std::shared_ptr<Type> base_type;
};

/**
 * @class TypeChecker
 * @brief Type checker for compile-time type safety
 */
class TypeChecker {
public:
    TypeChecker();

    /**
     * @brief Register a type
     */
    void register_type(std::shared_ptr<Type> type);

    /**
     * @brief Get a registered type
     */
    std::shared_ptr<Type> get_type(const std::string& name) const;

    /**
     * @brief Check type compatibility
     */
    bool is_compatible(const Type& from, const Type& to) const;

    /**
     * @brief Infer type from expression
     */
    std::shared_ptr<Type> infer_type(const std::string& expression);

    /**
     * @brief Check if type satisfies constraints
     */
    bool satisfies_constraints(const Type& type, const std::string& value) const;

    /**
     * @brief Get all type errors
     */
    const std::vector<std::string>& get_errors() const { return errors; }

    /**
     * @brief Clear errors
     */
    void clear_errors() { errors.clear(); }

private:
    std::unordered_map<std::string, std::shared_ptr<Type>> type_registry;
    std::vector<std::string> errors;

    void record_error(const std::string& error);
};

} // namespace synq::compiler::security
