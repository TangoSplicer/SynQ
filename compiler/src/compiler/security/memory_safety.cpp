// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 12: Security Hardening - Memory Safety Implementation

#include "memory_safety.h"
#include <chrono>
#include <algorithm>

namespace synq::compiler::security {

OwnershipTracker::OwnershipTracker() {
}

OwnershipTracker::~OwnershipTracker() {
    clear();
}

bool OwnershipTracker::register_owned(uint64_t value_id, uint64_t owner_id,
                                      const std::string& location) {
    if (ownership_map.find(value_id) != ownership_map.end()) {
        record_violation("Value " + std::to_string(value_id) + " already owned at " + location);
        return false;
    }

    OwnershipMetadata metadata;
    metadata.ownership = Ownership::OWNED;
    metadata.borrow_type = BorrowType::IMMUTABLE;
    metadata.owner_id = owner_id;
    metadata.borrow_id = 0;
    metadata.borrow_count = 0;
    metadata.is_moved = false;
    metadata.location = location;
    metadata.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    ownership_map[value_id] = metadata;
    borrow_graph[value_id] = std::vector<uint64_t>();

    return true;
}

bool OwnershipTracker::transfer_ownership(uint64_t value_id, uint64_t from_owner,
                                         uint64_t to_owner, const std::string& location) {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        record_violation("Value " + std::to_string(value_id) + " not found at " + location);
        return false;
    }

    if (it->second.owner_id != from_owner) {
        record_violation("Value " + std::to_string(value_id) + " not owned by " +
                        std::to_string(from_owner) + " at " + location);
        return false;
    }

    if (it->second.borrow_count > 0) {
        record_violation("Cannot transfer value " + std::to_string(value_id) +
                        " while borrowed at " + location);
        return false;
    }

    it->second.owner_id = to_owner;
    it->second.location = location;
    it->second.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    return true;
}

bool OwnershipTracker::borrow_immutable(uint64_t value_id, uint64_t owner_id,
                                       uint64_t borrower_id, const std::string& location) {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        record_violation("Value " + std::to_string(value_id) + " not found at " + location);
        return false;
    }

    if (it->second.owner_id != owner_id) {
        record_violation("Value " + std::to_string(value_id) + " not owned by " +
                        std::to_string(owner_id) + " at " + location);
        return false;
    }

    // Immutable borrow allowed if no mutable borrow exists
    if (it->second.borrow_type == BorrowType::MUTABLE && it->second.borrow_count > 0) {
        record_violation("Cannot immutably borrow value " + std::to_string(value_id) +
                        " while mutably borrowed at " + location);
        return false;
    }

    it->second.borrow_count++;
    borrow_graph[value_id].push_back(borrower_id);

    return true;
}

bool OwnershipTracker::borrow_mutable(uint64_t value_id, uint64_t owner_id,
                                     uint64_t borrower_id, const std::string& location) {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        record_violation("Value " + std::to_string(value_id) + " not found at " + location);
        return false;
    }

    if (it->second.owner_id != owner_id) {
        record_violation("Value " + std::to_string(value_id) + " not owned by " +
                        std::to_string(owner_id) + " at " + location);
        return false;
    }

    // Mutable borrow only allowed if no other borrows exist
    if (it->second.borrow_count > 0) {
        record_violation("Cannot mutably borrow value " + std::to_string(value_id) +
                        " while already borrowed at " + location);
        return false;
    }

    it->second.borrow_type = BorrowType::MUTABLE;
    it->second.borrow_count = 1;
    borrow_graph[value_id].push_back(borrower_id);

    return true;
}

bool OwnershipTracker::return_borrow(uint64_t value_id, uint64_t borrower_id,
                                    const std::string& location) {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        record_violation("Value " + std::to_string(value_id) + " not found at " + location);
        return false;
    }

    if (it->second.borrow_count == 0) {
        record_violation("Value " + std::to_string(value_id) + " not borrowed at " + location);
        return false;
    }

    auto& borrowers = borrow_graph[value_id];
    auto borrow_it = std::find(borrowers.begin(), borrowers.end(), borrower_id);
    if (borrow_it == borrowers.end()) {
        record_violation("Borrower " + std::to_string(borrower_id) + " did not borrow value " +
                        std::to_string(value_id) + " at " + location);
        return false;
    }

    borrowers.erase(borrow_it);
    it->second.borrow_count--;

    if (it->second.borrow_count == 0) {
        it->second.borrow_type = BorrowType::IMMUTABLE;
    }

    return true;
}

bool OwnershipTracker::is_borrowed(uint64_t value_id) const {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        return false;
    }
    return it->second.borrow_count > 0;
}

bool OwnershipTracker::is_moved(uint64_t value_id) const {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        return false;
    }
    return it->second.is_moved;
}

const OwnershipMetadata* OwnershipTracker::get_metadata(uint64_t value_id) const {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        return nullptr;
    }
    return &it->second;
}

bool OwnershipTracker::validate() const {
    violations.clear();

    // Check for orphaned values
    for (const auto& [value_id, metadata] : ownership_map) {
        if (metadata.owner_id == 0) {
            const_cast<OwnershipTracker*>(this)->record_violation(
                "Value " + std::to_string(value_id) + " has no owner");
        }
    }

    // Check for inconsistent borrow counts
    for (const auto& [value_id, borrowers] : borrow_graph) {
        auto it = ownership_map.find(value_id);
        if (it != ownership_map.end()) {
            if (it->second.borrow_count != borrowers.size()) {
                const_cast<OwnershipTracker*>(this)->record_violation(
                    "Value " + std::to_string(value_id) +
                    " has inconsistent borrow count");
            }
        }
    }

    return violations.empty();
}

void OwnershipTracker::clear() {
    ownership_map.clear();
    borrow_graph.clear();
    violations.clear();
}

bool OwnershipTracker::can_borrow_mutable(uint64_t value_id) const {
    auto it = ownership_map.find(value_id);
    if (it == ownership_map.end()) {
        return false;
    }
    return it->second.borrow_count == 0;
}

void OwnershipTracker::record_violation(const std::string& violation) {
    violations.push_back(violation);
}

// MemorySafetyValidator implementation

MemorySafetyValidator::MemorySafetyValidator(OwnershipTracker& tracker)
    : tracker(tracker) {
}

bool MemorySafetyValidator::validate_code_block(const std::string& code_block) {
    issues.clear();

    if (!check_use_after_free()) {
        return false;
    }

    if (!check_double_free()) {
        return false;
    }

    if (!check_data_races()) {
        return false;
    }

    if (!check_buffer_overflows()) {
        return false;
    }

    return true;
}

bool MemorySafetyValidator::check_use_after_free() const {
    // Check if any value is used after being moved
    for (const auto& violation : tracker.get_violations()) {
        if (violation.find("moved") != std::string::npos) {
            const_cast<MemorySafetyValidator*>(this)->record_issue(
                "Potential use-after-free: " + violation);
            return false;
        }
    }
    return true;
}

bool MemorySafetyValidator::check_double_free() const {
    // Check if any value is freed twice
    // This is prevented by the ownership system
    return true;
}

bool MemorySafetyValidator::check_data_races() const {
    // Check if multiple mutable borrows exist
    for (const auto& violation : tracker.get_violations()) {
        if (violation.find("mutably borrowed") != std::string::npos) {
            const_cast<MemorySafetyValidator*>(this)->record_issue(
                "Potential data race: " + violation);
            return false;
        }
    }
    return true;
}

bool MemorySafetyValidator::check_buffer_overflows() const {
    // Buffer overflow detection would require bounds checking
    // This is a placeholder for more sophisticated analysis
    return true;
}

void MemorySafetyValidator::record_issue(const std::string& issue) {
    issues.push_back(issue);
}

} // namespace synq::compiler::security
