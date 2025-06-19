// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "runtime_secrets.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> RuntimeSecrets::secrets;

void RuntimeSecrets::store(const std::string& key, const std::string& value) {
    secrets[key] = value;
    std::cout << "[Secrets] Stored key.\n";
}

std::string RuntimeSecrets::retrieve(const std::string& key) {
    return secrets.count(key) ? secrets[key] : "[not found]";
}

}