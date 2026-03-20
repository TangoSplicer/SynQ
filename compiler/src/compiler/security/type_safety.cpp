// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 12: Security Hardening - Type Safety Implementation

#include "type_safety.h"
#include <algorithm>
#include <sstream>

namespace synq::compiler::security {

// Type implementation

Type::Type(TypeKind kind, const std::string& name)
    : kind(kind), name(name) {
}

void Type::add_constraint(const TypeConstraint& constraint) {
    constraints.push_back(constraint);
}

bool Type::satisfies_constraints(const std::string& value) const {
    for (const auto& constraint : constraints) {
        // Simplified constraint checking - in reality would evaluate predicate
        if (value.empty() && constraint.predicate.find("non-empty") != std::string::npos) {
            return false;
        }
    }
    return true;
}

bool Type::is_assignable_from(const Type& other) const {
    return kind == other.kind && name == other.name;
}

std::string Type::to_string() const {
    return name;
}

// AlgebraicType implementation

AlgebraicType::AlgebraicType(const std::string& name)
    : Type(TypeKind::ALGEBRAIC, name) {
}

void AlgebraicType::add_variant(const Variant& variant) {
    variants.push_back(variant);
}

bool AlgebraicType::has_variant(const std::string& name) const {
    return std::any_of(variants.begin(), variants.end(),
                      [&name](const Variant& v) { return v.name == name; });
}

bool AlgebraicType::is_assignable_from(const Type& other) const {
    if (other.get_kind() != TypeKind::ALGEBRAIC) {
        return false;
    }
    const auto* other_algebraic = dynamic_cast<const AlgebraicType*>(&other);
    if (!other_algebraic) {
        return false;
    }
    return name == other.get_name();
}

std::string AlgebraicType::to_string() const {
    std::ostringstream oss;
    oss << name << " = ";
    for (size_t i = 0; i < variants.size(); ++i) {
        if (i > 0) oss << " | ";
        oss << variants[i].name;
        if (!variants[i].fields.empty()) {
            oss << "(";
            for (size_t j = 0; j < variants[i].fields.size(); ++j) {
                if (j > 0) oss << ", ";
                oss << variants[i].fields[j]->to_string();
            }
            oss << ")";
        }
    }
    return oss.str();
}

// ProductType implementation

ProductType::ProductType(const std::string& name)
    : Type(TypeKind::PRODUCT, name) {
}

void ProductType::add_field(const Field& field) {
    fields.push_back(field);
}

const ProductType::Field* ProductType::get_field(const std::string& name) const {
    auto it = std::find_if(fields.begin(), fields.end(),
                          [&name](const Field& f) { return f.name == name; });
    if (it != fields.end()) {
        return &(*it);
    }
    return nullptr;
}

bool ProductType::is_assignable_from(const Type& other) const {
    if (other.get_kind() != TypeKind::PRODUCT) {
        return false;
    }
    const auto* other_product = dynamic_cast<const ProductType*>(&other);
    if (!other_product) {
        return false;
    }
    return name == other.get_name();
}

std::string ProductType::to_string() const {
    std::ostringstream oss;
    oss << name << " { ";
    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << fields[i].name << ": " << fields[i].type->to_string();
        if (fields[i].is_mutable) oss << " (mut)";
    }
    oss << " }";
    return oss.str();
}

// FunctionType implementation

FunctionType::FunctionType(const std::string& name)
    : Type(TypeKind::FUNCTION, name) {
}

void FunctionType::set_parameters(const std::vector<std::shared_ptr<Type>>& params) {
    parameters = params;
}

void FunctionType::set_return_type(std::shared_ptr<Type> return_type) {
    this->return_type = return_type;
}

bool FunctionType::is_assignable_from(const Type& other) const {
    if (other.get_kind() != TypeKind::FUNCTION) {
        return false;
    }
    const auto* other_func = dynamic_cast<const FunctionType*>(&other);
    if (!other_func) {
        return false;
    }
    if (parameters.size() != other_func->parameters.size()) {
        return false;
    }
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (!parameters[i]->is_assignable_from(*other_func->parameters[i])) {
            return false;
        }
    }
    return return_type->is_assignable_from(*other_func->return_type);
}

std::string FunctionType::to_string() const {
    std::ostringstream oss;
    oss << "(";
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << parameters[i]->to_string();
    }
    oss << ") -> " << (return_type ? return_type->to_string() : "void");
    return oss.str();
}

// DependentType implementation

DependentType::DependentType(const std::string& name, std::shared_ptr<Type> base_type)
    : Type(TypeKind::DEPENDENT, name), base_type(base_type) {
}

void DependentType::set_dependent_param(const std::string& param_name,
                                       const std::string& param_type) {
    dependent_param = {param_name, param_type};
}

bool DependentType::is_assignable_from(const Type& other) const {
    if (other.get_kind() != TypeKind::DEPENDENT) {
        return false;
    }
    const auto* other_dependent = dynamic_cast<const DependentType*>(&other);
    if (!other_dependent) {
        return false;
    }
    return base_type->is_assignable_from(*other_dependent->base_type);
}

std::string DependentType::to_string() const {
    std::ostringstream oss;
    oss << name << "[" << dependent_param.first << ": " << dependent_param.second << "]";
    return oss.str();
}

// PhantomType implementation

PhantomType::PhantomType(const std::string& name, std::shared_ptr<Type> wrapped_type)
    : Type(TypeKind::PHANTOM, name), wrapped_type(wrapped_type) {
}

void PhantomType::add_phantom_constraint(const std::string& constraint) {
    phantom_constraints.push_back(constraint);
}

bool PhantomType::is_assignable_from(const Type& other) const {
    if (other.get_kind() != TypeKind::PHANTOM) {
        return false;
    }
    const auto* other_phantom = dynamic_cast<const PhantomType*>(&other);
    if (!other_phantom) {
        return false;
    }
    return wrapped_type->is_assignable_from(*other_phantom->wrapped_type);
}

std::string PhantomType::to_string() const {
    std::ostringstream oss;
    oss << name << "<" << wrapped_type->to_string() << ">";
    return oss.str();
}

// RefinedType implementation

RefinedType::RefinedType(const std::string& name, std::shared_ptr<Type> base_type)
    : Type(TypeKind::REFINED, name), base_type(base_type) {
}

void RefinedType::add_refinement(const TypeConstraint& constraint) {
    add_constraint(constraint);
}

bool RefinedType::is_assignable_from(const Type& other) const {
    if (other.get_kind() != TypeKind::REFINED) {
        return false;
    }
    const auto* other_refined = dynamic_cast<const RefinedType*>(&other);
    if (!other_refined) {
        return false;
    }
    return base_type->is_assignable_from(*other_refined->base_type);
}

std::string RefinedType::to_string() const {
    std::ostringstream oss;
    oss << name << " = {x: " << base_type->to_string() << " | ";
    for (size_t i = 0; i < constraints.size(); ++i) {
        if (i > 0) oss << " && ";
        oss << constraints[i].predicate;
    }
    oss << "}";
    return oss.str();
}

// TypeChecker implementation

TypeChecker::TypeChecker() {
}

void TypeChecker::register_type(std::shared_ptr<Type> type) {
    if (type) {
        type_registry[type->get_name()] = type;
    }
}

std::shared_ptr<Type> TypeChecker::get_type(const std::string& name) const {
    auto it = type_registry.find(name);
    if (it != type_registry.end()) {
        return it->second;
    }
    return nullptr;
}

bool TypeChecker::is_compatible(const Type& from, const Type& to) const {
    return to.is_assignable_from(from);
}

std::shared_ptr<Type> TypeChecker::infer_type(const std::string& expression) {
    // Simplified type inference - in reality would parse and analyze expression
    if (expression.find("true") != std::string::npos ||
        expression.find("false") != std::string::npos) {
        return get_type("bool");
    }
    if (expression.find("\"") != std::string::npos) {
        return get_type("string");
    }
    if (expression.find(".") != std::string::npos) {
        return get_type("f64");
    }
    return get_type("i32");
}

bool TypeChecker::satisfies_constraints(const Type& type, const std::string& value) const {
    return type.satisfies_constraints(value);
}

void TypeChecker::record_error(const std::string& error) {
    errors.push_back(error);
}

} // namespace synq::compiler::security
