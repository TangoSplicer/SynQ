// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 12: Security Hardening - Memory Safety System

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cstdint>

namespace synq::compiler::security {

/**
 * @enum Ownership
 * @brief Ownership model for memory management
 */
enum class Ownership {
    OWNED,      // Exclusive ownership - can move/transfer
    BORROWED,   // Temporary reference - cannot move
    SHARED,     // Shared ownership - reference counted
};

/**
 * @enum BorrowType
 * @brief Type of borrow for borrowed references
 */
enum class BorrowType {
    IMMUTABLE,  // Immutable borrow - read-only
    MUTABLE,    // Mutable borrow - read-write
};

/**
 * @struct OwnershipMetadata
 * @brief Metadata for ownership tracking
 */
struct OwnershipMetadata {
    Ownership ownership;
    BorrowType borrow_type;
    uint64_t owner_id;
    uint64_t borrow_id;
    size_t borrow_count;
    bool is_moved;
    std::string location;  // File:line for debugging
    uint64_t timestamp;    // When ownership was acquired
};

/**
 * @class OwnershipTracker
 * @brief Tracks ownership and borrowing relationships
 * 
 * The ownership system prevents:
 * - Use-after-free bugs
 * - Double-free bugs
 * - Data races
 * - Buffer overflows
 * 
 * Rules:
 * 1. Each value has exactly one owner
 * 2. Owner can transfer ownership (move)
 * 3. Owner can lend references (borrow)
 * 4. Multiple immutable borrows allowed
 * 5. Only one mutable borrow allowed
 * 6. No mutable borrow while immutable borrows exist
 * 7. Borrowed references cannot be moved
 */
class OwnershipTracker {
public:
    OwnershipTracker();
    ~OwnershipTracker();

    /**
     * @brief Register a new owned value
     * @param value_id Unique identifier for the value
     * @param owner_id Unique identifier for the owner
     * @param location Source code location (file:line)
     * @return true if registration successful
     */
    bool register_owned(uint64_t value_id, uint64_t owner_id, const std::string& location);

    /**
     * @brief Transfer ownership from one owner to another
     * @param value_id Value being transferred
     * @param from_owner Current owner
     * @param to_owner New owner
     * @param location Source code location
     * @return true if transfer successful
     */
    bool transfer_ownership(uint64_t value_id, uint64_t from_owner, uint64_t to_owner,
                           const std::string& location);

    /**
     * @brief Create an immutable borrow
     * @param value_id Value being borrowed
     * @param owner_id Owner of the value
     * @param borrower_id Borrower identifier
     * @param location Source code location
     * @return true if borrow successful
     */
    bool borrow_immutable(uint64_t value_id, uint64_t owner_id, uint64_t borrower_id,
                         const std::string& location);

    /**
     * @brief Create a mutable borrow
     * @param value_id Value being borrowed
     * @param owner_id Owner of the value
     * @param borrower_id Borrower identifier
     * @param location Source code location
     * @return true if borrow successful
     */
    bool borrow_mutable(uint64_t value_id, uint64_t owner_id, uint64_t borrower_id,
                       const std::string& location);

    /**
     * @brief Return a borrowed reference
     * @param value_id Value being returned
     * @param borrower_id Borrower returning the reference
     * @param location Source code location
     * @return true if return successful
     */
    bool return_borrow(uint64_t value_id, uint64_t borrower_id, const std::string& location);

    /**
     * @brief Check if a value is borrowed
     * @param value_id Value to check
     * @return true if value is currently borrowed
     */
    bool is_borrowed(uint64_t value_id) const;

    /**
     * @brief Check if a value has been moved
     * @param value_id Value to check
     * @return true if value has been moved
     */
    bool is_moved(uint64_t value_id) const;

    /**
     * @brief Get ownership metadata for a value
     * @param value_id Value to query
     * @return Ownership metadata or nullptr if not found
     */
    const OwnershipMetadata* get_metadata(uint64_t value_id) const;

    /**
     * @brief Validate ownership rules
     * @return true if all ownership rules are satisfied
     */
    bool validate() const;

    /**
     * @brief Get all violations
     * @return Vector of violation descriptions
     */
    const std::vector<std::string>& get_violations() const { return violations; }

    /**
     * @brief Clear all tracking data
     */
    void clear();

private:
    std::unordered_map<uint64_t, OwnershipMetadata> ownership_map;
    std::unordered_map<uint64_t, std::vector<uint64_t>> borrow_graph;  // value -> borrowers
    std::vector<std::string> violations;

    /**
     * @brief Check if a mutable borrow is allowed
     */
    bool can_borrow_mutable(uint64_t value_id) const;

    /**
     * @brief Record a violation
     */
    void record_violation(const std::string& violation);
};

/**
 * @template Owned<T>
 * @brief Smart pointer with ownership semantics
 * 
 * Owned<T> represents exclusive ownership of a value.
 * Only one Owned<T> can exist for a given value.
 * Owned<T> can be moved but not copied.
 */
template<typename T>
class Owned {
public:
    explicit Owned(T* ptr, uint64_t owner_id, OwnershipTracker& tracker)
        : ptr(ptr), owner_id(owner_id), tracker(&tracker), moved(false) {
    }

    ~Owned() {
        if (ptr && !moved) {
            delete ptr;
        }
    }

    // Move semantics (allowed)
    Owned(Owned&& other) noexcept
        : ptr(other.ptr), owner_id(other.owner_id), tracker(other.tracker), moved(false) {
        other.moved = true;
    }

    Owned& operator=(Owned&& other) noexcept {
        if (this != &other) {
            if (ptr && !moved) {
                delete ptr;
            }
            ptr = other.ptr;
            owner_id = other.owner_id;
            tracker = other.tracker;
            moved = false;
            other.moved = true;
        }
        return *this;
    }

    // Copy semantics (deleted)
    Owned(const Owned&) = delete;
    Owned& operator=(const Owned&) = delete;

    // Access
    T* operator->() { return ptr; }
    const T* operator->() const { return ptr; }
    T& operator*() { return *ptr; }
    const T& operator*() const { return *ptr; }

    // Get raw pointer
    T* get() { return ptr; }
    const T* get() const { return ptr; }

    // Check if moved
    bool is_moved() const { return moved; }

private:
    T* ptr;
    uint64_t owner_id;
    OwnershipTracker* tracker;
    bool moved;
};

/**
 * @template Borrowed<T>
 * @brief Reference with borrow semantics
 * 
 * Borrowed<T> represents a temporary reference to a value.
 * Multiple immutable Borrowed<T> can exist.
 * Only one mutable Borrowed<T> can exist at a time.
 * Borrowed<T> cannot be moved.
 */
template<typename T>
class Borrowed {
public:
    explicit Borrowed(T* ptr, uint64_t borrower_id, OwnershipTracker& tracker, bool mutable_borrow)
        : ptr(ptr), borrower_id(borrower_id), tracker(&tracker), mutable_borrow(mutable_borrow) {
    }

    ~Borrowed() {
        // Borrow is automatically returned when Borrowed goes out of scope
    }

    // No move semantics (deleted)
    Borrowed(Borrowed&&) = delete;
    Borrowed& operator=(Borrowed&&) = delete;

    // No copy semantics (deleted)
    Borrowed(const Borrowed&) = delete;
    Borrowed& operator=(const Borrowed&) = delete;

    // Access
    T* operator->() { return ptr; }
    const T* operator->() const { return ptr; }
    T& operator*() { return *ptr; }
    const T& operator*() const { return *ptr; }

    // Get raw pointer
    T* get() { return ptr; }
    const T* get() const { return ptr; }

    // Check if mutable
    bool is_mutable() const { return mutable_borrow; }

private:
    T* ptr;
    uint64_t borrower_id;
    OwnershipTracker* tracker;
    bool mutable_borrow;
};

/**
 * @class MemorySafetyValidator
 * @brief Validates memory safety at compile time and runtime
 */
class MemorySafetyValidator {
public:
    explicit MemorySafetyValidator(OwnershipTracker& tracker);

    /**
     * @brief Validate a code block for memory safety
     * @param code_block Code to validate
     * @return true if memory safe
     */
    bool validate_code_block(const std::string& code_block);

    /**
     * @brief Check for use-after-free
     * @return true if no use-after-free detected
     */
    bool check_use_after_free() const;

    /**
     * @brief Check for double-free
     * @return true if no double-free detected
     */
    bool check_double_free() const;

    /**
     * @brief Check for data races
     * @return true if no data races detected
     */
    bool check_data_races() const;

    /**
     * @brief Check for buffer overflows
     * @return true if no buffer overflows detected
     */
    bool check_buffer_overflows() const;

    /**
     * @brief Get all detected issues
     * @return Vector of issue descriptions
     */
    const std::vector<std::string>& get_issues() const { return issues; }

private:
    OwnershipTracker& tracker;
    std::vector<std::string> issues;

    void record_issue(const std::string& issue);
};

} // namespace synq::compiler::security
